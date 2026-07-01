/*
 *	PlayerHands.cpp
 *	@author Riku
 */

#include "PlayerHands.h"
#include "../Enemy/EnemyBase.h"
#include "../../../Manager/InputManager.h"
#include "../../../Definition/Const/VECTORConst.h"
#include "../../../Definition/CommonModule/MyMath.h"
#include "../../../Manager/CameraManager.h"
#include "../../Stage/Gimmick/Lever.h"
#include "../../../Component/Collider/Collider.h"
#include "PlayerCharacter.h"

PlayerHands::PlayerHands(std::shared_ptr<PlayerCharacter> _owner, int _modelHandle, VECTOR _pos, Tag _tag)
	: Character(_modelHandle, _pos, _tag)
	, handsState(HandsState::Idle)
	, catchState(CatchState::None)
	, owner(_owner)
	, extendSpeed(20.0f)
	, returnSpeedRatio(0.7f)

	, RETURN_THRESHOLD(1.0f)
{
}

void PlayerHands::Start() {
	pCollider = std::make_unique<CapsuleCollider>(this, VScale(VUp, 70), VZero, 100, VZero);
	pCollider->SetResolve(false);
	pCollider->SetLayer(ColliderLayer::PlayerArm);
}

void PlayerHands::Update() {
	Character::Update();
	pTransform->Update();
	if (!pCollider) return;
	pCollider->Update();

	// 掴み状態
	if (handsState == HandsState::Catch) {
		// 掴み解除
		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_F)) {
			handsState = HandsState::ArmsReturning;
			catchState = CatchState::None;
		}
	}
	// 何も掴んでいなければ
	else {
		// ウデ伸ばし
		if (InputManager::GetInstance().IsKey(KEY_INPUT_E))
			handsState = HandsState::ArmsExtending;
		// ウデ戻し
		if (InputManager::GetInstance().IsKeyUp(KEY_INPUT_E))
			handsState = HandsState::ArmsReturning;
	}

	// 手の移動
	HandsMove();

	// 掴み移動
	if (catchState == CatchState::PillerCatch) {
		CatchMoving();
	}

}

void PlayerHands::Render() {
	Character::Render();
	if (pCollider)
		pCollider->Render();
}

void PlayerHands::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {
	auto other = _pOther->GetGameObject();
	// ウデを伸ばしていない状態なら無視
	if (handsState != HandsState::ArmsExtending)
		return;

	// 当たったのが敵の場合
	auto enemy = dynamic_cast<EnemyBase*>(other);
	if (enemy) {
		// ステート変更
		catchState = CatchState::EnemyCatch;
		handsState = HandsState::Catch;
		// 敵の掴まった時処理
		enemy->CaughtAction();
	}

	// 当たったのがレバーの場合
	auto lever = dynamic_cast<Lever*>(other);
	if (lever && InputManager::GetInstance().IsKey(KEY_INPUT_Q)) {
		handsState = HandsState::Catch;
	}
}

void PlayerHands::OnTriggerStay(Collider* _pSelf, Collider* _pOther) {
	auto other = _pOther->GetGameObject();
	if (other->GetTag() == Player)
		return;
	// ウデを伸ばしていない状態なら無視(キャッチなら無視しない)
	if (handsState != HandsState::ArmsExtending &&
		handsState != HandsState::Catch)
		return;

	// 当たったのが敵の場合
	auto enemy = dynamic_cast<EnemyBase*>(other);
	if (enemy) {
		// 敵を離す
		if (InputManager::GetInstance().IsKeyUp(KEY_INPUT_E)) {
			enemy->ThrownAction();
			catchState = CatchState::None;
			handsState = HandsState::ArmsReturning;
		}
	}

	// 当たったのがレバーの場合
	auto lever = dynamic_cast<Lever*>(other);
	// デバッグ用でQを押しながらだと掴み移動になる
	if (lever && InputManager::GetInstance().IsKey(KEY_INPUT_Q)) {
		if (InputManager::GetInstance().IsKeyUp(KEY_INPUT_E))
			catchState = CatchState::PillerCatch;
	}
	else if (lever) {
		// ステート変更
		catchState = CatchState::LeverCatch;
		handsState = HandsState::Catch;
		// 引っこ抜き
		bool pull = owner->Pull();
		// ギミック発動
		if (pull) {
			lever->SetLeverTrigger(pull);
			catchState = CatchState::None;
			handsState = HandsState::ArmsReturning;
		}
	}
}

void PlayerHands::OnTriggerExit(Collider* _pSelf, Collider* _pOther) {
}

/*
 *	手の移動処理
 */
void PlayerHands::HandsMove() {
	// ウデ伸ばし中なら前に進む
	if (handsState == HandsState::ArmsExtending) {
		pTransform->AddPosition(VForward, -extendSpeed);

		// ウデ伸ばし中は当たり判定の押し出しあり
		pCollider->SetResolve(true);
	}
	// ウデ戻し中なら戻ってくる
	else if (handsState == HandsState::ArmsReturning){
		float posZ = pTransform->GetLocalPosition().z;
		// 戻ってきたとみなす
		if (posZ > -RETURN_THRESHOLD) {
			pTransform->SetPosition(VZero);
			handsState = HandsState::Idle;
		}
		// 戻ってくる
		else {
			pTransform->SetPosition(VGet(0, 0, posZ * returnSpeedRatio));
		}

		// ウデ戻し中は当たり判定の押し出しなし
		pCollider->SetResolve(false);
	}
}

/*
 *	掴み移動処理
 */
void PlayerHands::CatchMoving() {
	// プレイヤーが手の位置に戻るまで移動
	if (pTransform->GetLocalPosition().z > 0) {
		catchState = CatchState::None;
		handsState = HandsState::Idle;
		pTransform->SetPosition(VZero);
	}
	else {
		owner->CatchMovingMove(extendSpeed);
		pTransform->AddPosition(VForward, extendSpeed);
	}
}