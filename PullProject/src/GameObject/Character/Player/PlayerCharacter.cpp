/*
 *	@file	PlayerCharacter.cpp
 *	@author	Riku
 */

#include "PlayerCharacter.h"
#include "../../../Definition/Const/VECTORConst.h"
#include "../../../Definition/CommonModule/MyMath.h"
#include "../../../Manager/CameraManager.h"
#include "../../Stage/Gimmick/Lever.h"
#include "../Enemy/EnemyBase.h"
#include "../../../Pad/PadBase.h"
#include "../../../Manager/InputSystemManager.h"
#include "../../../Definition/Enum/PlayerActionEnum.h"
#include "Application.h"
#include <DxLib.h>
#include <ImGui/imgui.h>

PlayerCharacter::PlayerCharacter(int _modelHandle, VECTOR _pos, Tag _tag)
	: Character(_modelHandle, _pos, _tag)
	, playerState(PlayerState::Idle)
	, speed(10.0f)
	, pullValue(0.0f)
	, lurchBackwardTime(0.0f)
	, returnColor(false)
	, throwAnimation(false)
	, dieAnimation(false)
	, isDead(false)
	, lockOn(false)
	, lockOnTarget(VZero)
	, lurchBackwardPos(VZero)

	, PULL_VALUE_MAX(100.0f)
	, PULL_CAMERA_SHAKE_POWER(20.0f)
	, PULL_CAMERA_SHAKE_TIME(5.0f)
	, JUMP_POWER(20)
	, RETURN_COLOR_RATIO(0.95f)
	, RETURN_PULL_VALUE_RATIO(0.95f)
	, TURN_RATIO(0.3f)
	, LURCH_BACKWARD_LENGTH(200.0f)
	, LURCH_BACKWARD_RATIO(0.95f)
	, LURCH_BACKWARD_THRESHOLD(30.0f)
	, LURCH_BACKWARD_TIME_MAX(70.0f)
	, VISION_LENGTH(1500.0f)
	, VISION_HEIGHT(100.0f)
	, VISION_ANGLE(30.0f)
	, CHANGE_LENGTH_TO_POWER_RATIO(0.0135f)
	, CATCH_MOVING_JAMP_LENGTH(200.0f)
{}

void PlayerCharacter::Start() {
	pCollider = std::make_unique<CapsuleCollider>(this, VScale(VUp, 50), VScale(VUp, 10), 40, VZero);
	pCollider->SetLayer(ColliderLayer::Player);
	pGroundingCollider = std::make_unique<SphereCollider>(this, VScale(VUp, -30), 5);
	isGravity = true;

	// ロックオン用の視界
	pLockOnVision = std::make_unique<RayCollider>(
		this, VZero,
		pTransform->GetForward(),
		VISION_LENGTH, VISION_HEIGHT, VISION_ANGLE, 0);
	pLockOnVision->SetLayer(ColliderLayer::PlayerRay);

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
	landAnim->SetEvent([this]() {
		throwAnimation = false;
		}, pAnimator->GetTotalTime("Land"));
	// 再生終了時待機状態に
	landAnim->SetEvent([this]() {
		playerState = PlayerState::Idle;
		}, pAnimator->GetTotalTime("Land"));

	// 持ち上げアニメーションにイベントを仕込む
	auto carryAnim = pAnimator->GetAnimation("Carry");
	float carryAnimTime = pAnimator->GetTotalTime("Carry");
	// ループしないように停止
	carryAnim->SetEvent([this]() {
		pAnimator->ChangeSpeed("Carry", 0.0f);
		pHands->GetAnimator()->ChangeSpeed("Carry", 0.0f);
		}, carryAnimTime);

	// ウデ伸ばしアニメーションにイベントを仕込む
	auto stanceAnim = pAnimator->GetAnimation("Stance");
	float stanceAnimTime = pAnimator->GetTotalTime("Stance");
	stanceAnim->SetEvent([this]() {
		throwAnimation = false;
		// SE
		Application::GetInstance().GetAudioManager().Play("PlayerShot");
		// 手使用状態に変更
		playerState = PlayerState::UseHands;
		}, 0.0f);
	// ループしないように停止
	// (なぜか再生終了時間で0にするとバグるので-0.2fしている)
	stanceAnim->SetEvent([this]() {
		pAnimator->ChangeSpeed("Stance", 0.0f);
		pHands->GetAnimator()->ChangeSpeed("Stance", 0.0f);
		}, stanceAnimTime - 0.2f);
	// ウデ戻しアニメーションにイベントを仕込む
	// (なぜか再生終了時間で0にするとバグるので-0.2fしている)
	auto stanceCancelAnim = pAnimator->GetAnimation("StanceCancel");
	float stanceCancelAnimTime = pAnimator->GetTotalTime("StanceCancel");
	stanceCancelAnim->SetEvent([this]() {
		throwAnimation = false;
		}, 0.0f);
	// ループしないように停止
	stanceCancelAnim->SetEvent([this]() {
		pAnimator->ChangeSpeed("StanceCancel", 0.0f);
		pHands->GetAnimator()->ChangeSpeed("StanceCancel", 0.0f);
		// 待機状態に変更
		playerState = PlayerState::Idle;
		}, stanceCancelAnimTime - 0.2f);

	// 投げアニメーションにイベントを仕込む
	auto throwAnim = pAnimator->GetAnimation("Throw");
	float throwAnimTime = pAnimator->GetTotalTime("Throw");
	// 再生中フラグ変更
	throwAnim->SetEvent([this]() {
		throwAnimation = true;
		}, 0.0f);
	// 再生中フラグ変更
	throwAnim->SetEvent([this]() {
		throwAnimation = false;
		// ループしないように停止
		pAnimator->ChangeSpeed("Throw", 0.0f);
		pHands->GetAnimator()->ChangeSpeed("Throw", 0.0f);
		}, throwAnimTime);

	// 死亡アニメーションにイベントを仕込む
	auto dieAnim = pAnimator->GetAnimation("Die");
	float dieAnimTime = pAnimator->GetTotalTime("Die");
	// アニメーションの最初にSE
	dieAnim->SetEvent([this]() {
		// SE
		Application::GetInstance().GetAudioManager().Play("PlayerDead");
		dieAnimation = true;
		}, 0.0f);
	// アニメーション終了時に死亡
	dieAnim->SetEvent([this]() {
		dieAnimation = false;
		isDead = true;
		}, dieAnimTime);
}

void PlayerCharacter::Update() {
	Character::Update();
	if (dieAnimation || isDead)
		return;

#if _DEBUG
	ImGui::Begin("PlayerPosition");
	ImGui::Text("%f, %f, %f", GetPosition().x, GetPosition().y, GetPosition().z);
	ImGui::End();
#endif
	
	// 入力アクションの更新
	action = InputSystemManager::GetInstance().GetInputState(ActionMap::PlayerAction);

	// ジャンプ後着地なら着地アニメーション再生(敵を持っていたらNG)
	if (hitGroundingFrag && 
		playerState == PlayerState::Jump &&
		!pHands->IsEnemyCatch()) {
		pAnimator->Play("Land", 1.0f);
		pHands->GetAnimator()->Play("Land", 1.0f);
	}
	// 地面についているなら待機状態へ
	// のけぞり中と手使用中はNG
	else if (hitGroundingFrag && 
		playerState != PlayerState::LurchBackward &&
		playerState != PlayerState::UseHands) {
		playerState = PlayerState::Idle;
	}

	// のけぞり
	if (playerState == PlayerState::LurchBackward) {
		LurchBackward();
	}
	// 移動
	else if ((!pHands->IsArmExtending() && !pHands->IsCatch()) || pHands->IsEnemyCatch()) {
		Move();
	}

	// ロックオン
	lockOn = GetVisionObject(lockOnTarget);
	// ロックオン解除
	if (action.buttonUp[static_cast<int>(PlayerAction::LockOn)] ||
		GetHands()->IsCatch()) {
		lockOn = false;
	}

	// ロックオン中はターゲットの方を向く
	if (lockOn) {
		VECTOR dir = VSub(lockOnTarget, GetPosition());
		VECTOR dirNorm = VNorm(dir);
		float yaw = MyMath::Rad2Deg(atan2f(-dirNorm.x, -dirNorm.z));
		pTransform->SetRotation(VGet(0.0f, yaw, 0.0f));
	}

	// 待機アニメーション(投げアニメーション中じゃなければ)
	if (playerState == PlayerState::Idle &&
		!throwAnimation) {
		if (pHands->IsEnemyCatch()) {
			// 敵を掴んでるならそれ用の待機アニメーション
			pAnimator->Play("CarryIdle");
			pHands->GetAnimator()->Play("CarryIdle");
		}
		else if (playerState != PlayerState::UseHands){
			// 手使用中でなければ待機アニメーション
			pAnimator->Play("Idle");
			pHands->GetAnimator()->Play("Idle");
		}
	}

	// 落下速度に応じて微振動
	if (fallSpeed > 0) {
		StartJoypadVibration(DX_INPUT_PAD1, fallSpeed * 0.1f, 180, -1);
	}

	// 色戻し
	if (returnColor) {
		ReturnColor();
	}


#if _DEBUG
	ImGui::Begin("PlayerAnimation");
	ImGui::Text("%d", pAnimator->GetCurrentAnimation());
	ImGui::Text("%d", pHands->GetAnimator()->GetCurrentAnimation());
	ImGui::End();

	ImGui::Begin("PlayerHitGrounding");
	if (hitGroundingFrag)
		ImGui::Text("true");
	else
		ImGui::Text("false");
	ImGui::End();
#endif
}

void PlayerCharacter::Render() {
	Character::Render();
}

void PlayerCharacter::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {
	if (_pSelf == pLockOnVision.get())
		return;
	Character::OnTriggerEnter(_pSelf, _pOther);

	// 死亡判定
	auto enemy = dynamic_cast<EnemyBase*>(_pOther->GetGameObject());
	if (enemy) {
		// 敵がつかまったり投げられていたら無視
		if (enemy->GetCurrentCaughtState() != 0)
			return;
	}
	auto otherTag = _pOther->GetGameObject()->GetTag();
	if (_pOther->GetLayer() == ColliderLayer::Enemy ||
		_pOther->GetLayer() == ColliderLayer::Retry ||
		otherTag == Explosion ||
		otherTag == EnemyAttack
		) {
		// 死亡アニメーション
		pAnimator->Play("Die", 1.0f);
		pHands->GetAnimator()->Play("Die", 1.0f);
	}
}

void PlayerCharacter::OnTriggerStay(Collider* _pSelf, Collider* _pOther) {
	if (_pSelf == pLockOnVision.get())
		return;
	Character::OnTriggerStay(_pSelf, _pOther);
}

void PlayerCharacter::OnTriggerExit(Collider* _pSelf, Collider* _pOther) {
	if (_pSelf == pLockOnVision.get())
		return;
	Character::OnTriggerExit(_pSelf, _pOther);
}

/*
 *	移動
 */
void PlayerCharacter::Move() {
	// 投げアニメーションは動けない
	if (throwAnimation)
		return;

	// ジャンプ
	if (action.buttonDown[static_cast<int>(PlayerAction::Jump)] &&
		hitGroundingFrag) {
		AddFallSpeed(-JUMP_POWER);
		groundCount = 0;
		hitGroundingFrag = false;
		playerState = PlayerState::Jump;
		// SE
		Application::GetInstance().GetAudioManager().Play("PlayerJump");
		// 敵を持ち上げている時はアニメーションしない
		if (!pHands->IsEnemyCatch()) {
			// ジャンプアニメーション(最初は高速再生)
			pAnimator->Play("Jump", 10.0f);
			pHands->GetAnimator()->Play("Jump", 10.0f);
		}
	}
	
	auto camera = CameraManager::GetInstance().GetCamera();
	if (!camera) return;

	// カメラのヨー(ラジアン)
	float cameraYaw = MyMath::Deg2Rad(camera->GetRotation().y);

	// カメラの角度のsin,cos
	float cameraSin = sinf(cameraYaw);
	float cameraCos = cosf(cameraYaw);
	// 前方向
	VECTOR forward = VGet(cameraSin, 0, cameraCos);
	// 右方向
	VECTOR right = VGet(cameraCos, 0, -cameraSin);

	// 入力
	VECTOR moveDir = VZero;
	Vector2 move = action.axis[static_cast<int>(PlayerAction::Move)];
	if (move.y != 0) {
		moveDir = VAdd(moveDir, VScale(forward, move.y));
	}
	if (move.x != 0) {
		moveDir = VAdd(moveDir, VScale(right, move.x));
	}

	// 入力があれば移動
	if (moveDir.x != 0 ||
		moveDir.y != 0 ||
		moveDir.z != 0) {
		// 移動
		pTransform->AddPosition(moveDir, speed);
		// 角度を移動方向へ(現在モデルが逆向きなので反対向きにするようにしている)
		float targetY = MyMath::Rad2Deg(atan2f(-moveDir.x, -moveDir.z));
		float currentY = GetRotation().y;
		float diff = targetY - currentY;
		// 振り向く方向が最適になるように
		while (diff > 180)
			diff -= 360;
		while (diff < -180)
			diff += 360;
		float nextY = currentY + diff * TURN_RATIO;
		// 角度が180～-180に収まるように正規化
		while (nextY > 180)
			nextY -= 360;
		while (nextY < -180)
			nextY += 360;
		// 角度適応
		pTransform->SetRotation(VGet(0, nextY, 0));
		
		if (playerState != PlayerState::Jump) {
			playerState = PlayerState::Move;
			// アニメーション再生
			if (pHands->IsEnemyCatch()) {
				pAnimator->Play("CarryRun", 0.5f);
				pHands->GetAnimator()->Play("CarryRun", 0.5f);
			}
			else {
				pAnimator->Play("Run", 0.5f);
				pHands->GetAnimator()->Play("Run", 0.5f);
			}
		}
	}
#if _DEBUG
	ImGui::Begin("PlayerRotation");
	VECTOR rot = GetRotation();
	ImGui::Text("%f, %f, %f", rot.x, rot.y, rot.z);
	ImGui::End();
#endif
}
/*
 *	色を戻す
 */
void PlayerCharacter::ReturnColor() {
	// 今の色を取得
	auto color = MV1GetDifColorScale(modelHandle);
	// 線形補間で色を抜いていく
	color.g = MyMath::Lerp(1, color.g, RETURN_COLOR_RATIO);
	color.b = color.g;
	// 1は超えないように
	if (color.g > 1) {
		color.g = 1;
		color.b = 1;
		returnColor = false;
	}
	// 適応
	MV1SetDifColorScale(modelHandle, color);
	MV1SetDifColorScale(pHands->GetModelHandle(), color);
}

/*
 *  のけぞり
 */
void PlayerCharacter::LurchBackward() {
	VECTOR dist = VSub(GetPosition(), lurchBackwardPos);
	// Y成分は抜く
	dist.y = 0;

	groundCount = 0;

	// のけぞり位置までの距離の2乗
	float distSq = VDot(dist, dist);
	// のけぞり
	if (distSq > LURCH_BACKWARD_THRESHOLD * LURCH_BACKWARD_THRESHOLD) {
		pTransform->SetPosition(MyMath::Lerp(lurchBackwardPos, GetPosition(), LURCH_BACKWARD_RATIO));
	}
	else {
		playerState = PlayerState::Idle;
	}

	// 一定時間以上続いていたら強制解除
	lurchBackwardTime++;
	if (lurchBackwardTime > LURCH_BACKWARD_TIME_MAX) {
		playerState = PlayerState::Idle;
		lurchBackwardTime = 0;
	}

#if _DEBUG
	ImGui::Begin("LurchBackwardPosition");
	ImGui::Text("%f, %f, %f", lurchBackwardPos.x, lurchBackwardPos.y, lurchBackwardPos.z);
	ImGui::End();
#endif
}

/*
 *	引っこ抜き
 */
bool PlayerCharacter::Pull() {
	lockOn = false;
	// 後ろに引き続けないと引っこ抜けない
	float back = action.axis[static_cast<int>(PlayerAction::Move)].y;
	if (back < 0) {
		pullValue -= back;
		// 微量のシェイク
		CameraManager::GetInstance().CameraShake(1, 1);
		// 振動
		StartJoypadVibration(DX_INPUT_PAD1, pullValue * 0.5f, 10, -1);

	}
	else {
		pullValue = MyMath::Lerp(0, pullValue, RETURN_PULL_VALUE_RATIO);
	}

	// 引っこ抜き具合によって赤く変色
	float pullRatio = 1 - pullValue / PULL_VALUE_MAX;
	MV1SetDifColorScale(modelHandle, GetColorF(1, pullRatio, pullRatio, 1));
	MV1SetDifColorScale(pHands->GetModelHandle(), GetColorF(1, pullRatio, pullRatio, 1));

#if _DEBUG
	ImGui::Begin("PullValue");
	ImGui::Text("%f", pullValue);
	ImGui::End();
#endif
	// ある程度引くと引っこ抜き
	if (pullValue >= PULL_VALUE_MAX) {
		// カメラシェイク
		CameraManager::GetInstance().CameraShake(PULL_CAMERA_SHAKE_POWER, PULL_CAMERA_SHAKE_TIME);
		// 振動
		StartJoypadVibration(DX_INPUT_PAD1, 1000, 180, -1);
		// のけぞり
		playerState = PlayerState::LurchBackward;
		lurchBackwardPos = VAdd(GetPosition(), VScale(pTransform->GetForward(), LURCH_BACKWARD_LENGTH));
		// ジャンプアニメーションで代用
		pAnimator->Play("Jump", 10.0f);
		pHands->GetAnimator()->Play("Jump", 10.0f);

		// 解除時処理を呼ぶ
		CatchReset();
		
		return true;
	}
	return false;
}

/*
 *	掴み解除時処理
 */
void PlayerCharacter::CatchReset() {
	pullValue = 0;
	returnColor = true;
	isGravity = true;
}

/*
 *	掴み移動の移動処理
 *	@param	float 移動速度
 */
void PlayerCharacter::CatchMovingMove(float moveSpeed) {
	pTransform->AddPosition(pTransform->GetForward(), -moveSpeed);
}

/*
 *	視界内にオブジェクトがあるかどうか
 *	@param[out]	GameObject*	視界内の一番近いオブジェクト
 */
bool PlayerCharacter::GetVisionObject(VECTOR& targetObject) {
	// プレイヤーと一番近いオブジェクトの位置を保存
	VECTOR lockOnPos = VZero;
	float length = FLT_MAX;
	auto hitObjects = pLockOnVision->GetHitObjects();
	for (auto object : hitObjects) {
		if (!object)
			continue;
		if (object->GetTag() != Enemy &&
			object->GetTag() != Hook &&
			object->GetTag() != LeverTag&&
			object->GetTag() != MissileObject)
			continue;

		auto enemy = dynamic_cast<EnemyBase*>(object);
		if (enemy) {
			// 敵がつかまったり投げられていたら無視
			if (enemy->GetCurrentCaughtState() != 0)
				continue;
		}

		VECTOR dir = VSub(object->GetPosition(), GetPosition());
		float newLength = VDot(dir, dir);
		if (length > newLength) {
			length = newLength;
			lockOnPos = object->GetPosition();
		}
	}
	if (lockOnPos.x != 0.0f ||
		lockOnPos.y != 0.0f ||
		lockOnPos.z != 0.0f) {
		targetObject = lockOnPos;
		// 入力があればロックオンする
		if (action.button[static_cast<int>(PlayerAction::LockOn)]) {
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

/*
 *	掴み移動時のジャンプ
 */
void PlayerCharacter::CatchMovingJamp() {
	// 手とプレイヤーの距離を出す
	VECTOR dir = VSub(pHands->GetPosition(), GetPosition());
	float length = VDot(dir, dir);
	length = sqrt(length);
	
	// 一定距離以上なら距離に応じてジャンプ
	if (length > CATCH_MOVING_JAMP_LENGTH) {
		AddFallSpeed(-length * CHANGE_LENGTH_TO_POWER_RATIO);
		groundCount = 0;
		hitGroundingFrag = false;
	}
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
