/*
 *	@file	PlayerCharacter.cpp
 *	@author	Riku
 */

#include "PlayerCharacter.h"
#include "../../../Manager/InputManager.h"
#include "../../../Definition/Const/VECTORConst.h"
#include "../../../Definition/CommonModule/MyMath.h"
#include "../../../Manager/CameraManager.h"
#include "../../Stage/Gimmick/Lever.h"
#include "../Enemy/EnemyBase.h"
#include "../../../Component/Collider/Collider.h"
#include <DxLib.h>
#include <ImGui/imgui.h>

PlayerCharacter::PlayerCharacter(int _modelHandle, VECTOR _pos, Tag _tag)
	: Character(_modelHandle, _pos, _tag)
	, playerState(PlayerState::Idle)
	, speed(10.0f)
	, pullValue(0.0f)

	, PULL_VALUE_MAX(100.0f)
	, PULL_CAMERA_SHAKE_POWER(20.0f)
	, PULL_CAMERA_SHAKE_TIME(5.0f) 
	, JUMP_POWER(20)
{}

void PlayerCharacter::Start() {
	pCollider = std::make_unique<CapsuleCollider>(this, VScale(VUp, 50), VScale(VUp, 10), 40, VZero);
	pGroundingCollider = std::make_unique<SphereCollider>(this, VScale(VUp, -30), 5);
	isGravity = true;

	// ジャンプアニメーションにイベントを仕込む
	auto jumpAnim = pAnimator->GetAnimation("Jump");
	float jumpAnimTime = pAnimator->GetTotalTime("Jump");
	// 跳ね上がり
	jumpAnim->SetEvent([this]() {
		pAnimator->ChangeSpeed("Jump", 1.0f);
		pHands->GetAnimator()->ChangeSpeed("Jump", 1.0f);
		}, jumpAnimTime * 0.4f);
	// ループしないように停止
	jumpAnim->SetEvent([this]() {
		pAnimator->ChangeSpeed("Jump", 0.0f);
		pHands->GetAnimator()->ChangeSpeed("Jump", 0.0f);
		}, jumpAnimTime);

	// 着地アニメーションにイベントを仕込む
	auto landAnim = pAnimator->GetAnimation("Land");
	// 再生終了時待機状態に
	landAnim->SetEvent([this]() {
		playerState = PlayerState::Idle;
		}, pAnimator->GetTotalTime("Land"));
}

void PlayerCharacter::Update() {
	Character::Update();
	
	// ジャンプ後着地なら着地アニメーション再生
	if (hitGroundingFrag && playerState == PlayerState::Jump) {
		pAnimator->Play("Land", 1.0f);
		pHands->GetAnimator()->Play("Land", 1.0f);
	}
	// 地面についているなら待機状態へ
	else if (hitGroundingFrag) {
		playerState = PlayerState::Idle;
	}

	// 移動
	if (!pHands->IsCatch())
		Move();

	// 待機アニメーション
	if (playerState == PlayerState::Idle) {
		pAnimator->Play("Idle");
		pHands->GetAnimator()->Play("Idle");
	}
}

void PlayerCharacter::Render() {
	Character::Render();
}

void PlayerCharacter::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {
	Character::OnTriggerEnter(_pSelf, _pOther);
}

void PlayerCharacter::OnTriggerStay(Collider* _pSelf, Collider* _pOther) {
}

void PlayerCharacter::OnTriggerExit(Collider* _pSelf, Collider* _pOther) {
	Character::OnTriggerExit(_pSelf, _pOther);
}

/*
 *	移動
 */
void PlayerCharacter::Move() {
	// ジャンプ
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_SPACE)) {
		AddFallSpeed(-JUMP_POWER);
		hitGroundingFrag = false;
		playerState = PlayerState::Jump;
		// ジャンプアニメーション(最初は高速再生)
		pAnimator->Play("Jump", 10.0f);
		pHands->GetAnimator()->Play("Jump", 10.0f);
	}

	auto camera = CameraManager::GetInstance().GetCamera();
	if (!camera) return;

	// カメラのヨー(ラジアン)
	float cameraYaw = MyMath::Deg2Rad(camera->GetRotation().y);

	// カメラの角度のsin,cos
	float cameraSin = sinf(cameraYaw);
	float cameraCos = cosf(cameraYaw);

	// 入力
	VECTOR moveDir = VZero;
	if (InputManager::GetInstance().IsKey(KEY_INPUT_W))
		moveDir = VAdd(moveDir, VGet(cameraSin, 0, cameraCos));
	if (InputManager::GetInstance().IsKey(KEY_INPUT_S))
		moveDir = VAdd(moveDir, VGet(-cameraSin, 0, -cameraCos));
	if (InputManager::GetInstance().IsKey(KEY_INPUT_D))
		moveDir = VAdd(moveDir, VGet(cameraCos, 0, -cameraSin));
	if (InputManager::GetInstance().IsKey(KEY_INPUT_A))
		moveDir = VAdd(moveDir, VGet(-cameraCos, 0, cameraSin));

	// 入力があれば移動
	if (moveDir.x != 0 ||
		moveDir.y != 0 ||
		moveDir.z != 0) {
		// 移動
		pTransform->AddPosition(moveDir, speed);
		// 角度を移動方向へ(現在モデルが逆向きなので反対向きにするようにしている)
		pTransform->SetRotation(VGet(0, MyMath::Rad2Deg(atan2f(-moveDir.x, -moveDir.z)), 0));
		
		if (playerState != PlayerState::Jump) {
			playerState = PlayerState::Move;
			// アニメーション再生
			pAnimator->Play("Run", 0.5f);
			pHands->GetAnimator()->Play("Run", 0.5f);
		}
	}
}
/*
 *	引っこ抜き
 */
bool PlayerCharacter::Pull() {
	// 後ろに引き続けないと引っこ抜けない
	if (InputManager::GetInstance().IsKey(KEY_INPUT_S)) {
		pullValue++;
		// 微量のシェイク
		CameraManager::GetInstance().CameraShake(1, 1);

	}
	else {
		pullValue = 0;
	}

	// 引っこ抜き具合によって赤く変色
	float pullRatio = 1 - pullValue / PULL_VALUE_MAX;
	MV1SetDifColorScale(modelHandle, GetColorF(1, pullRatio, pullRatio, 1));
	MV1SetDifColorScale(pHands->GetModelHandle(), GetColorF(1, pullRatio, pullRatio, 1));

	ImGui::Begin("PullValue");
	ImGui::Text("%f", pullValue);
	ImGui::End();
	// ある程度引くと引っこ抜き
	if (pullValue >= PULL_VALUE_MAX) {
		// カメラシェイク
		CameraManager::GetInstance().CameraShake(PULL_CAMERA_SHAKE_POWER, PULL_CAMERA_SHAKE_TIME);
		// 解除時処理を呼ぶ
		PullReset();
		
		return true;
	}
	return false;
}

/*
 *	引っこ抜き解除時処理
 */
void PlayerCharacter::PullReset() {
	pullValue = 0;
	// 色を戻す
	MV1SetDifColorScale(modelHandle, GetColorF(1, 1, 1, 1));
	MV1SetDifColorScale(pHands->GetModelHandle(), GetColorF(1, 1, 1, 1));
}

/*
 *	掴み移動の移動処理
 *	@param	float 移動速度
 */
void PlayerCharacter::CatchMovingMove(float moveSpeed) {
	pTransform->AddPosition(pTransform->GetForward(), -moveSpeed);
}

/*
 *	手生成
 *	@param	int モデルハンドル
 *	@param	std::shared_ptr<PlayerCharacter> 所有者(プレイヤー)
 */
void PlayerCharacter::CreateHands(std::shared_ptr<PlayerCharacter> owner, int modelHandle) {
	pHands = std::make_shared<PlayerHands>(owner, modelHandle, VZero);
	pHands->GetAnimator()->Load(modelHandle, false);
	pHands->GetTransform()->AttachParent(GetTransform());
	pHands->Start();
}
