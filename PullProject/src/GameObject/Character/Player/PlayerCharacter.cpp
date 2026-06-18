/*
 *	@file	PlayerCharacter.cpp
 *	@author	Riku
 */

#include "PlayerCharacter.h"
#include "../../../Manager/InputManager.h"
#include "../../../Definition/Const/VECTORConst.h"
#include "../../../Definition/CommonModule/MyMath.h"
#include "../../../Manager/CameraManager.h"
#include "../../../Definition/CommonModule/MyMath.h"
#include <DxLib.h>

PlayerCharacter::PlayerCharacter(int _modelHandle, VECTOR _pos, Tag _tag)
	: Character(_modelHandle, _pos, _tag)
	, playerState(PlayerState::Normal)
	, speed(10.0f)
	, pullValue(0.0f)

	, PULL_VALUE_MAX(100.0f) 
	, PULL_CAMERA_SHAKE_POWER(20.0f)
	, PULL_CAMERA_SHAKE_TIME(5.0f){
}

void PlayerCharacter::Start() {
	
}

void PlayerCharacter::Update() {
	pTransform->Update();

	// 移動
	if (playerState == PlayerState::Normal)
		Move();

	// 引っこ抜き
	if (playerState == PlayerState::Catch)
		Pull();

	// 掴み用ステートチェンジ
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_F))
		playerState = PlayerState::Catch;
	if (InputManager::GetInstance().IsKeyUp(KEY_INPUT_F))
		playerState = PlayerState::Normal;
}

void PlayerCharacter::Render() {
	Character::Render();
}

/*
 *	移動
 */
void PlayerCharacter::Move() {
	auto camera = CameraManager::GetInstance().GetCamera()->GetTransform();
	if (!camera) return;
	playerState = PlayerState::Normal;

	// カメラのヨー(ラジアン)
	float yaw = MyMath::Deg2Rad(camera->GetLocalRotation().y);

	// カメラの角度のsin,cos
	float cameraSin = sinf(yaw);
	float cameraCos = cosf(yaw);
	if (InputManager::GetInstance().IsKey(KEY_INPUT_W))
		pTransform->AddPosition(VGet(speed * cameraSin, 0, speed * cameraCos));
	if (InputManager::GetInstance().IsKey(KEY_INPUT_S))
		pTransform->AddPosition(VGet(-speed * cameraSin, 0, -speed * cameraCos));
	if (InputManager::GetInstance().IsKey(KEY_INPUT_D))
		pTransform->AddPosition(VGet(speed * cameraCos, 0, speed * -cameraSin));
	if (InputManager::GetInstance().IsKey(KEY_INPUT_A))
		pTransform->AddPosition(VGet(-speed * cameraCos, 0, -speed * -cameraSin));

}
/*
 *	引っこ抜き
 */
void PlayerCharacter::Pull() {
	// 後ろに引き続けないと引っこ抜けない
	if (InputManager::GetInstance().IsKey(KEY_INPUT_S)) {
		pullValue++;
		// 微量のシェイク
		CameraManager::GetInstance().CameraShake(1,1);

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
	}
}

/*
 *	引き抜きラインに対する引っ張り値の割合取得
 *	@return	float
 */
float PlayerCharacter::GetPullValueRatio() {
	return pullValue / PULL_VALUE_MAX;
}