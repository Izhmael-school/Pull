/*
 *	@file	PlayerCharacter.cpp
 *	@author	Riku
 */

#include "PlayerCharacter.h"
#include "../../../Manager/InputManager.h"
#include "../../../Definition/Const/VECTORConst.h"
#include "../../../Definition/CommonModule/MyMath.h"
#include "../../../Manager/CameraManager.h"
#include "../../../Component/Collider/Collider.h"
#include "../../Stage/Gimmick/Lever.h"
#include "../Enemy/EnemyBase.h"
#include <memory>
#include <DxLib.h>

PlayerCharacter::PlayerCharacter(int _modelHandle, VECTOR _pos, Tag _tag)
	: Character(_modelHandle, _pos, _tag)
	, playerState(PlayerState::Normal)
	, speed(10.0f)
	, pullValue(0.0f)

	, PULL_VALUE_MAX(100.0f)
	, PULL_CAMERA_SHAKE_POWER(20.0f)
	, PULL_CAMERA_SHAKE_TIME(5.0f) {
}

void PlayerCharacter::Start() {
	pCollider = std::make_unique<CapsuleCollider>(this, VScale(VUp, 70), VZero, 100, VZero);
}

void PlayerCharacter::Update() {
	pTransform->Update();
	if (!pCollider) return;
	pCollider->Update();

	// 移動
	if (playerState == PlayerState::Normal || playerState == PlayerState::EnemyCatch)
		Move();

	// ウデ伸ばし
	if (playerState != PlayerState::EnemyCatch)
		ArmsExtended();
	//if (InputManager::GetInstance().IsKeyUp(KEY_INPUT_E))

}

void PlayerCharacter::Render() {
	Character::Render();
	if (pCollider)
		pCollider->Render();
	if (pArmsCollider)
		pArmsCollider->Render();
}

void PlayerCharacter::OnTriggerEnter(Collider* _pOther) {
	auto other = _pOther->GetGameObject();

	// 当たったのが敵の場合
	auto enemy = dynamic_cast<EnemyBase*>(other);
	if (enemy) {
		// 敵を掴む
		EnemyCatch();
	}
}

void PlayerCharacter::OnTriggerStay(Collider* _pOther) {
	auto other = _pOther->GetGameObject();

	// 当たったのがレバーの場合
	auto lever = dynamic_cast<Lever*>(other);
	if (lever) {
		// 掴み用ステートチェンジ
		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_F))
			playerState = PlayerState::GimmickCatch;
		if (InputManager::GetInstance().IsKeyUp(KEY_INPUT_F))
			playerState = PlayerState::Normal;

		// 引っこ抜き
		if (playerState == PlayerState::GimmickCatch)
			// ギミック作動
			lever->SetLeverTrigger(Pull());
	}

	// 当たったのが敵の場合
	auto enemy = dynamic_cast<EnemyBase*>(other);
	if (enemy) {
		// 敵を離す
		if (InputManager::GetInstance().IsKeyUp(KEY_INPUT_E))
			EnemyRelease();
	}

}

void PlayerCharacter::OnTriggerExit(Collider* _pOther) {
	playerState = PlayerState::Normal;
}

/*
 *	移動
 */
void PlayerCharacter::Move() {
	auto camera = CameraManager::GetInstance().GetCamera();
	if (!camera) return;
	playerState = PlayerState::Normal;

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
		playerState = PlayerState::Normal;
		pullValue = 0;
		// カメラシェイク
		CameraManager::GetInstance().CameraShake(PULL_CAMERA_SHAKE_POWER, PULL_CAMERA_SHAKE_TIME);

		return true;
	}
	return false;
}

/*
 *	ウデ伸ばし
 */
void PlayerCharacter::ArmsExtended() {
	if (!InputManager::GetInstance().IsKey(KEY_INPUT_E))
		return;
	if (!pArmsCollider)
		pArmsCollider = std::make_unique<CapsuleCollider>(this, VZero, VZero, 100, VZero);

	// まっすぐ伸ばしていく
	VECTOR moveVec = VScale(pTransform->GetForward(), -20);
	pArmsCollider->Move(moveVec);
}

/*
 *	引き抜きラインに対する引っ張り値の割合取得
 *	@return	float
 */
float PlayerCharacter::GetPullValueRatio() {
	return pullValue / PULL_VALUE_MAX;
}

/*
 *	敵を掴む
 */
void PlayerCharacter::EnemyCatch() {
	playerState = PlayerState::EnemyCatch;
	// デバッグ用
	CameraManager::GetInstance().CameraShake(PULL_CAMERA_SHAKE_POWER, PULL_CAMERA_SHAKE_TIME);

}

/*
 *	敵を離す
 */
void PlayerCharacter::EnemyRelease() {
	playerState = PlayerState::Normal;
	// デバッグ用
	CameraManager::GetInstance().CameraShake(PULL_CAMERA_SHAKE_POWER, PULL_CAMERA_SHAKE_TIME);

}
