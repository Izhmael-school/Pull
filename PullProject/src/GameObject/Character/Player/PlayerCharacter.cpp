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

PlayerCharacter::PlayerCharacter(int _modelHandle, VECTOR _pos, Tag _tag)
	: Character(_modelHandle, _pos, _tag)
	, playerState(PlayerState::Idle)
	, speed(10.0f)
	, pullValue(0.0f)

	, PULL_VALUE_MAX(100.0f)
	, PULL_CAMERA_SHAKE_POWER(20.0f)
	, PULL_CAMERA_SHAKE_TIME(5.0f) {
}

void PlayerCharacter::Start() {
	pCollider = std::make_unique<CapsuleCollider>(this, VScale(VUp, 70), VZero, 100, VZero);
	pGroundingCollider = std::make_unique<SphereCollider>(this, VScale(VUp, -100), 10);
	isGravity = true;
}

void PlayerCharacter::Update() {
	Character::Update();
	pTransform->Update();

	// 移動
	if (!pHands->IsCatch())
		Move();

	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_SPACE)) {
		AddFallSpeed(-30);
		hitGroundingFrag = false;
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
	auto camera = CameraManager::GetInstance().GetCamera();
	if (!camera) return;
	playerState = PlayerState::Move;

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

	// ある程度引くと引っこ抜き
	if (pullValue >= PULL_VALUE_MAX) {
		pullValue = 0;
		// カメラシェイク
		CameraManager::GetInstance().CameraShake(PULL_CAMERA_SHAKE_POWER, PULL_CAMERA_SHAKE_TIME);

		return true;
	}
	return false;
}

/*
 *	手生成
 *	@param	int モデルハンドル
 *	@param	std::shared_ptr<PlayerCharacter> 所有者(プレイヤー)
 */
void PlayerCharacter::CreateHands(std::shared_ptr<PlayerCharacter> owner, int modelHandle) {
	pHands = std::make_shared<PlayerHands>(owner, modelHandle, VZero);
	pHands->GetTransform()->AttachParent(GetTransform());
	pHands->Start();
}
