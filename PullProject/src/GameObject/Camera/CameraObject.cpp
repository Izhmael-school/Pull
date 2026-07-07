/*
 *	@file	CameraObject.cpp
 *	@author Riku
 */

#include "CameraObject.h"
#include "../../Manager/InputManager.h"
#include "../../Definition/Const/VECTORConst.h"
#include "../../Definition/CommonModule/MyMath.h"
#include "../../Manager/PlayerManager.h"
#include "../../Component/Collider/Collider.h"
#include "../../Pad/PadBase.h"
#include "../../Definition/Enum/PlayerActionEnum.h"
#include <DxLib.h>
#include <ImGui/imgui.h>

 /*
  * @author Sekino
  */
#include "EffekseerForDXLib.h"
#include "EventCameraMovement.h"

CameraObject::CameraObject()
	: mode(CameraMode::Player)
	, target(VZero)
	, speed(30.0f)
	, shakePower(0.0f)
	, shakeTime(0.0f)
	, shakeElapsedTime(0.0f)
	, chasePlayerPosY(0.0f)
	, isShaking(false)
	, isChaseXZ(false)
	, isChaseY(false)

	, isEvent(false)

	, PLAYER_DISTANCE(1000)
	, PULL_ZOOM_RATIO_MAX(0.95f)
	, PULL_ZOOM_RATIO_MIN(0.7f) 
	, TARGET_MOVE_RATIO(0.15f)
	, TARGET_DISTANCE_MAX(30.0f)
	, TARGET_THRESHOLD(0.5f)
	, POSITION_Y_LIMIT_UP(-900.0f)
	, POSITION_Y_LIMIT_DOWN(0.0f)
{}

void CameraObject::Start() {
	pCollider = std::make_unique<SphereCollider>(this, VZero, 100);
	pCollider->SetLayer(ColliderLayer::Camera);
}

void CameraObject::Update() {
	GameObject::Update();

#if _DEBUG
	// 引っ張りモード
	if (mode == CameraMode::Player || mode == CameraMode::Pull) {
		bool catchGimmick = PlayerManager::GetInstance().GetPlayer()->GetHands()->IsLeverCatch();
		if (catchGimmick) {
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
		pCollider->SetResolve(false);
		DebugUpdate();
		break;
	case CameraObject::CameraMode::Player:
		pCollider->SetResolve(true);
		PlayerUpdate();
		break;
	case CameraObject::CameraMode::Pull:
		pCollider->SetResolve(true);
		PullUpdate();
		break;
	case CameraObject::CameraMode::Event:
		pCollider->SetResolve(true);
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

	/*
	 * @brief Effekseerのカメラ設定を同期する
	 * @author Sekino
	 */
	Effekseer_Sync3DSetting();

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

void CameraObject::Render() {
	GameObject::Render();
}

void CameraObject::OnTriggerExit(Collider* _pSelf, Collider* _pOther) {
	int a = 1;
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

	// プレイヤーの入力
	ActionState action = player->GetInputAction();

	// 入力方向を保持
	VECTOR moveVec = VZero;
	// コントローラーの入力(反転しておく)
	Axis2D cameraMove = action.axis[static_cast<int>(PlayerAction::CameraMove)];
	cameraMove = { -cameraMove.x, -cameraMove.y };
	// コントローラー入力
	if (cameraMove.x != 0) {
		moveVec = VAdd(moveVec, VScale(pTransform->GetRight(), cameraMove.x));
	}
	// 上下移動には制限を掛ける
	VECTOR move = VAdd(GetPosition(), VScale(pTransform->GetUp(), -speed));
	float moveY = target.y - move.y;
	if (moveY < POSITION_Y_LIMIT_DOWN) {
		// コントローラー入力
		if (cameraMove.y < 0) {
			moveVec = VAdd(moveVec, VScale(pTransform->GetUp(), cameraMove.y));
		}
	}
	move = VAdd(GetPosition(), VScale(pTransform->GetUp(), speed));
	moveY = target.y - move.y;
	if (moveY > POSITION_Y_LIMIT_UP) {
		// コントローラー入力
		if (cameraMove.y > 0) {
			moveVec = VAdd(moveVec, VScale(pTransform->GetUp(), cameraMove.y));
		}
	}

	// 入力があれば回転
	if (moveVec.x != 0 ||
		moveVec.y != 0 ||
		moveVec.z != 0) {	
		// カメラの移動
		pTransform->AddPosition(moveVec, speed);
	}
	float y = target.y - GetPosition().y;
	ImGui::Begin("CameraPositionY");
	ImGui::Text("%f", y);
	ImGui::End();

	// ターゲットの方を向く
	pTransform->LookAt(target);

	// ターゲットとプレイヤーの距離が規定距離を超えたら近づく
	// XZ平面上
	VECTOR playerPos = player->GetPosition();
	playerPos.y = 0;
	VECTOR targetPos = target;
	targetPos.y = 0;
	VECTOR dist = VSub(targetPos, playerPos);
	float distSq = VDot(dist, dist);
	if (distSq > TARGET_DISTANCE_MAX * TARGET_DISTANCE_MAX) {
		isChaseXZ = true;
	}
	// Y軸上
	// 下方向
	float playerY = player->GetPosition().y;
	float targetY = target.y;
	if (targetY - playerY > TARGET_DISTANCE_MAX) {
		// 上方向の制限の関係でこの位置で更新
		chasePlayerPosY = player->GetPosition().y;
		isChaseY = true;
	}
	// 上方向
	if (playerY - targetY > TARGET_DISTANCE_MAX) {
		isChaseY = true;
		// プレイヤージャンプ中か地に足がついていなければ更新はしない
		if (!player->IsJump() || player->GetHitGroundingFrag())
			chasePlayerPosY = player->GetPosition().y;
	}

	// ターゲットの追いかけ
	if (isChaseXZ)
		TargetMoveXZ();
	if (isChaseY)
		TargetMoveY();
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
	/*
	 * @author Sekino
	 */

	if (!isEvent) {
		isEvent = true;
		EventCameraMovement::StartEventCamera(this,"startStage");
	}

	EventCameraMovement::Update(this);

	if (EventCameraMovement::IsEventEnd())
		isEvent = false;

	/**/
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
 *	ターゲットのXZ平面上の移動
 */
void CameraObject::TargetMoveXZ() {
	VECTOR playerPos = PlayerManager::GetInstance().GetPlayer()->GetPosition();
	// XZ平面上なのでYは合わせる
	playerPos.y = target.y;
	// ターゲットがプレイヤーと重なるまで移動
	VECTOR dist = VSub(target, playerPos);
	float distSq = VDot(dist, dist);
	if (distSq > TARGET_THRESHOLD) {
		target = MyMath::Lerp(target, playerPos, TARGET_MOVE_RATIO);
		// ターゲットの移動に合わせてカメラも移動
		pTransform->SetPosition(VAdd(target, VScale(pTransform->GetForward(), -PLAYER_DISTANCE)));
	}
	else {
		isChaseXZ = false;
	}
}

/*
 *	ターゲットのY軸移動
 */
void CameraObject::TargetMoveY() {
	auto player = PlayerManager::GetInstance().GetPlayer();
	// Y軸移動なのでXZは合わせる
	VECTOR newTargetPos = VGet(target.x, chasePlayerPosY, target.z);
	// ターゲットがプレイヤーと重なるまで移動
	VECTOR dist = VSub(target, newTargetPos);
	float distSq = VDot(dist, dist);
	if (distSq > TARGET_THRESHOLD) {
		target = MyMath::Lerp(target, newTargetPos, TARGET_MOVE_RATIO);
		// ターゲットの移動に合わせてカメラも移動
		pTransform->SetPosition(VAdd(target, VScale(pTransform->GetForward(), -PLAYER_DISTANCE)));
	}
	else {
		isChaseY = false;
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
