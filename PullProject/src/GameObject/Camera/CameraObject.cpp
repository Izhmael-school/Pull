/*
 *	@file	CameraObject.cpp
 *	@author Riku
 */

#include "CameraObject.h"
#include "../../Manager/InputManager.h"
#include "../../Definition/Const/VECTORConst.h"
#include "../../Definition/CommonModule/MyMath.h"
#include "../../Manager/PlayerManager.h"
#include <DxLib.h>

CameraObject::CameraObject()
	: mode(CameraMode::Debug)
	, speed(10.0f)
{
}

void CameraObject::Start() {
}

void CameraObject::Update() {
	pTransform->Update();
	// 各モード毎の更新処理
	switch (mode) {
	case CameraObject::CameraMode::Debug:
		DebugUpdate();
		break;
	case CameraObject::CameraMode::Player:
		PlayerUpdate();
		break;
	case CameraObject::CameraMode::Event:
		EventUpdate();
		break;
	}

	// カメラに反映
	VECTOR pos = pTransform->GetPosition();
	VECTOR rot = pTransform->GetLocalRotation();
	SetCameraPositionAndAngle(
		pos,
		MyMath::Deg2Rad(rot.x),
		MyMath::Deg2Rad(rot.y),
		MyMath::Deg2Rad(rot.z));

#if _DEBUG
	// カメラモード変更
	if (InputManager::GetInstance().IsKey(KEY_INPUT_1))
		mode = CameraMode::Debug;
	if (InputManager::GetInstance().IsKey(KEY_INPUT_2))
		mode = CameraMode::Player;
	if (InputManager::GetInstance().IsKey(KEY_INPUT_3))
		mode = CameraMode::Event;
#endif

}

void CameraObject::DebugUpdate() {
	// 回転
	if (InputManager::GetInstance().IsKey(KEY_INPUT_UP)) 
		pTransform->AddRotation(VLeft, 2);
	if (InputManager::GetInstance().IsKey(KEY_INPUT_DOWN)) 
		pTransform->AddRotation(VRight, 2);
	if (InputManager::GetInstance().IsKey(KEY_INPUT_RIGHT)) 
		pTransform->AddRotation(VUp, 2);
	if (InputManager::GetInstance().IsKey(KEY_INPUT_LEFT)) 
		pTransform->AddRotation(VDown, 2);

	// 移動
	if (InputManager::GetInstance().IsKey(KEY_INPUT_W))
		pTransform->AddPosition(pTransform->GetForward(), speed);
	if (InputManager::GetInstance().IsKey(KEY_INPUT_S))
		pTransform->AddPosition(pTransform->GetForward(), -speed);
	if (InputManager::GetInstance().IsKey(KEY_INPUT_D))
		pTransform->AddPosition(pTransform->GetRight(), speed);
	if (InputManager::GetInstance().IsKey(KEY_INPUT_A))
		pTransform->AddPosition(pTransform->GetRight(), -speed);
	if (InputManager::GetInstance().IsKey(KEY_INPUT_Q))
		pTransform->AddPosition(pTransform->GetUp(), -speed);
	if (InputManager::GetInstance().IsKey(KEY_INPUT_E))
		pTransform->AddPosition(pTransform->GetUp(), speed);
}

void CameraObject::PlayerUpdate() {
	auto player = PlayerManager::GetInstance().GetPlayer()->GetTransform();
	
	// カメラの回転
	if (InputManager::GetInstance().IsKey(KEY_INPUT_UP))
		pTransform->AddRotation(VLeft, 2);
	if (InputManager::GetInstance().IsKey(KEY_INPUT_DOWN))
		pTransform->AddRotation(VRight, 2);
	if (InputManager::GetInstance().IsKey(KEY_INPUT_RIGHT))
		pTransform->AddRotation(VUp, 2);
	if (InputManager::GetInstance().IsKey(KEY_INPUT_LEFT))
		pTransform->AddRotation(VDown, 2);

	// プレイヤーから離れた位置に配置
	pTransform->SetPosition(VAdd(player->GetPosition(), VScale(pTransform->GetForward(), -100)));

}

void CameraObject::EventUpdate() {
}