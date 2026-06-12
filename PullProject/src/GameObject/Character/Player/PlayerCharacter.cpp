/*
 *	@file	PlayerCharacter.cpp
 *	@author	Riku
 */

#include "PlayerCharacter.h"
#include "../../../Manager/InputManager.h"
#include "../../../Definition/Const/VECTORConst.h"
#include "../../../Definition/CommonModule/MyMath.h"
#include "../../../Manager/CameraManager.h"
#include <DxLib.h>

PlayerCharacter::PlayerCharacter(int _modelHandle, VECTOR _pos, Tag _tag)
	: Character(_modelHandle, _pos, _tag)
	, speed(10.0f){
}

void PlayerCharacter::Start() {

}

void PlayerCharacter::Update() {
	auto camera = CameraManager::GetInstance().GetCamera()->GetTransform();

	// 移動
	// カメラの角度のsin,cos
	float cameraSin = sinf(camera->GetLocalRotation().y);
	float cameraCos = cosf(camera->GetLocalRotation().y);
	if (InputManager::GetInstance().IsKey(KEY_INPUT_W))
		pTransform->AddPosition(VGet(-speed * cameraSin, 0, -speed * cameraCos));
	if (InputManager::GetInstance().IsKey(KEY_INPUT_S))
		pTransform->AddPosition(VGet(speed * cameraSin, 0, speed * cameraCos));
	if (InputManager::GetInstance().IsKey(KEY_INPUT_D))
		pTransform->AddPosition(VGet(-speed * cameraCos, 0, -speed * -cameraSin));
	if (InputManager::GetInstance().IsKey(KEY_INPUT_A))
		pTransform->AddPosition(VGet(speed * cameraCos, 0, speed * -cameraSin));

}
