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

PlayerCharacter::PlayerCharacter()
	: speed(10.0f)
{}

void PlayerCharacter::Start() {

}

void PlayerCharacter::Update() {
	auto camera = CameraManager::GetInstance().GetCamera()->GetTransform();
}
