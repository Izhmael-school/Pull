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

#include "EventCameraMovement.h"

CameraObject::CameraObject()
	: mode(CameraMode::Debug)
	, speed(10.0f)
	, shakePower(0.0f)
	, shakeTime(0.0f)
	, shakeElapsedTime(0.0f)
	, isShaking(false)

	, isEvent(false)

	, PLAYER_DISTANCE(1000)
	, PULL_ZOOM_RATIO_MAX(0.95f)
	, PULL_ZOOM_RATIO_MIN(0.7f) {
}

void CameraObject::Start() {
}

void CameraObject::Update() {
	pTransform->Update();

#if _DEBUG
	// 引っ張りモード
	if (mode == CameraMode::Player || mode == CameraMode::Pull) {
		if (PlayerManager::GetInstance().GetPlayer()->IsCatch()) {
			mode = CameraMode::Pull;
		}
		else {
			mode = CameraMode::Player;
		}
	}
#endif

	// 各モード毎の更新処理
	switch (mode) {
	case CameraObject::CameraMode::Debug:
		DebugUpdate();
		break;
	case CameraObject::CameraMode::Player:
		PlayerUpdate();
		break;
	case CameraObject::CameraMode::Pull:
		PullUpdate();
		break;
	case CameraObject::CameraMode::Event:
		EventUpdate();
		break;
	}

	// カメラのシェイク(ステートに限定されない)
	if (isShaking)
		CameraShake();

	// カメラに反映
	VECTOR pos = GetPosition();
	VECTOR rot = GetRotation();
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
	auto player = PlayerManager::GetInstance().GetPlayer();
	if (!player) return;

	// カメラの回転
	//if (InputManager::GetInstance().IsKey(KEY_INPUT_UP))
	//	pTransform->AddRotation(VLeft, 2);
	//if (InputManager::GetInstance().IsKey(KEY_INPUT_DOWN))
	//	pTransform->AddRotation(VRight, 2);
	//if (InputManager::GetInstance().IsKey(KEY_INPUT_RIGHT))
	//	pTransform->AddRotation(VUp, 2);
	//if (InputManager::GetInstance().IsKey(KEY_INPUT_LEFT))
	//	pTransform->AddRotation(VDown, 2);
	//
	//// プレイヤーから離れた位置に配置
	//VECTOR distance = VScale(pTransform->GetForward(), -PLAYER_DISTANCE);
	//pTransform->SetPosition(VAdd(player->GetPosition(), distance));

	// 入力方向を保持
	VECTOR moveVec = VZero;
	if (InputManager::GetInstance().IsKey(KEY_INPUT_UP))
		moveVec = VAdd(moveVec, VScale(pTransform->GetUp(), -1));
	if (InputManager::GetInstance().IsKey(KEY_INPUT_DOWN))
		moveVec = VAdd(moveVec, pTransform->GetUp());
	if (InputManager::GetInstance().IsKey(KEY_INPUT_RIGHT))
		moveVec = VAdd(moveVec, VScale(pTransform->GetRight(), -1));
	if (InputManager::GetInstance().IsKey(KEY_INPUT_LEFT))
		moveVec = VAdd(moveVec, pTransform->GetRight());

	// 入力があれば回転
	if (moveVec.x != 0 ||
		moveVec.y != 0 ||
		moveVec.z != 0) {
		// カメラの移動
		pTransform->AddPosition(moveVec, speed);
	}

	// プレイヤーの方を向く
	pTransform->LookAt(player->GetPosition());

	// プレイヤーとカメラの距離が規定距離を超えていたら近づける
	VECTOR dir = VSub(player->GetPosition(), GetPosition());
	float dist = VDot(dir, dir) - PLAYER_DISTANCE * PLAYER_DISTANCE;
	if (dist > 0) {
		dir.y = 0;
		dir = VNorm(dir);
		dist = sqrtf(dist);
		pTransform->AddPosition(dir, speed);
	}
}

void CameraObject::PullUpdate() {
	auto player = PlayerManager::GetInstance().GetPlayer();
	if (!player) return;

	// プレイヤーの引き具合によってカメラの位置調整
	// ズーム割合の差
	float zoomRatioDiff = PULL_ZOOM_RATIO_MAX - PULL_ZOOM_RATIO_MIN;
	// イーズアウト補間
	// 最初は速く、最後は遅く引くように演出
	float pullRatio = player->GetPullValueRatio();
	pullRatio = 1.0f - powf(1.0f - pullRatio, 2.0f);
	// ズーム割合の差に引き抜き割合を適応
	float pullRatioDiff = pullRatio * zoomRatioDiff;
	// 実際のズーム割合
	float zoomRatio = PULL_ZOOM_RATIO_MIN + pullRatioDiff;
	// ズーム割合をプレイヤーとの距離に適応
	float distanceValue = PLAYER_DISTANCE * zoomRatio;

	// プレイヤーから離れた位置に配置
	VECTOR distance = VScale(pTransform->GetForward(), -distanceValue);
	VECTOR targetPos = VAdd(player->GetPosition(), distance);
	pTransform->SetPosition(MyMath::Lerp(GetPosition(), targetPos, 0.2f));
}

void CameraObject::EventUpdate() {
	if (!isEvent) {
		isEvent = true;
		EventCameraMovement::StartEventCamera(this,"startStage");
	}

	EventCameraMovement::Update(this);

	if (EventCameraMovement::IsEventEnd())
		isEvent = false;
}

/*
 *	カメラのシェイク
 *	@param	float shakeTime		シェイク時間
 *	@param	float shakePower	シェイクの大きさ
 */
void CameraObject::CameraShake() {
	// シェイク中のみ処理
	if (isShaking) {
		// シェイク
		float shakeValue = GetRand(shakePower * 2) - shakePower;
		pTransform->AddPosition(VScale(VOne, shakeValue));

		// 時間が経過しきったら終了
		shakeElapsedTime++;
		if (shakeElapsedTime >= shakeTime) {
			isShaking = false;
		}
	}
}

/*
 *	カメラのシェイクを作動
 *	@param	float setShakePowar	シェイクの大さ
 *	@param	float setShakeTime	シェイクの時間
 */
void CameraObject::CameraShakeActivate(float setShakePower, float setShakeTime) {
	isShaking = true;
	shakeElapsedTime = 0;
	shakePower = setShakePower;
	shakeTime = setShakeTime;
}
