/*
 *	PlayerHands.cpp
 *	@author Riku
 */

#include "PlayerHands.h"
#include "../Enemy/EnemyBase.h"
#include "../../../Definition/Const/VECTORConst.h"
#include "../../../Definition/CommonModule/MyMath.h"
#include "../../../Manager/CameraManager.h"
#include "../../Stage/Gimmick/Lever.h"
#include "../../../Component/Collider/Collider.h"
#include "PlayerCharacter.h"
#include "../../../Pad/PadBase.h"
#include "../../../Definition/Enum/PlayerActionEnum.h"
#include <ImGui/imgui.h>

/*
 * author Sekino
 */
#include "GameObject/Missile/Missile.h"

PlayerHands::PlayerHands(std::shared_ptr<PlayerCharacter> _owner, int _modelHandle, VECTOR _pos, Tag _tag)
	: Character(_modelHandle, _pos, _tag)
	, handsState(HandsState::Idle)
	, catchState(CatchState::None)
	, pOwner(_owner)
	, pCatchCollider(nullptr)
	, extendSpeed(20.0f)
	, returnSpeedRatio(0.3f)

	, RETURN_THRESHOLD(1.0f)
	, ARM_LENGTH_MAX(1500.0f)
	, ENEMY_CATCH_RETURN_THRESHOLD(100.0f)
{}

void PlayerHands::Start() {
	pCollider = std::make_unique<CapsuleCollider>(this, VScale(VUp, 50), VScale(VUp, 10), 40, VZero);
	pCollider->SetResolve(false);
	pCollider->SetLayer(ColliderLayer::PlayerArm);
}

void PlayerHands::Update() {
	Character::Update();

	// プレイヤーの入力アクションの取得
	action = pOwner->GetInputAction();

	// 掴み状態
	if (handsState == HandsState::Catch) {
		// 掴み更新処理
		CatchUpdate();
		
		// 掴み解除
		if (action.buttonDown[static_cast<int>(PlayerAction::CatchCancel)]) {
			handsState = HandsState::ArmsReturning;
			catchState = CatchState::None;
			// 引っこ抜き解除時処理を呼ぶ
			pOwner->PullReset();
		}
	}
	// 何も掴んでいなければ(かつ地に足ついていれば)
	else if (pOwner->GetHitGroundingFrag()) {
		// ウデ伸ばし(ウデ戻し中&&投げアニメーション中じゃなければ)
		if (action.button[static_cast<int>(PlayerAction::ArmExtend)] &&
			handsState != HandsState::ArmsReturning && 
			!pOwner->IsThrowAnimation()) {
			handsState = HandsState::ArmsExtending;
			pAnimator->Play("Stance", 0.5f);
			pOwner->GetAnimator()->Play("Stance", 0.5f);
		}
		// ウデ戻し
		if (action.buttonUp[static_cast<int>(PlayerAction::ArmExtend)]) {
			handsState = HandsState::ArmsReturning;
			pAnimator->Play("StanceCancel", 0.5f);
			pOwner->GetAnimator()->Play("StanceCancel", 0.5f);
		}
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
}

void PlayerHands::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {
	auto other = _pOther->GetGameObject();
	if (!other)
		return;
	// ウデを伸ばしていない状態なら無視
	if (handsState != HandsState::ArmsExtending)
		return;

	// 当たったコライダーを保存
	pCatchCollider = _pOther;

	auto enemy = dynamic_cast<EnemyBase*>(other);
	// 当たったのが敵の場合
	if (enemy) {
		auto enemyLayer = enemy->GetCollider()->GetLayer();
		// 尻尾の場合はレバーと同じように処理
		if (_pOther->GetLayer() == ColliderLayer::Tail) {
			// ステート変更
			catchState = CatchState::LeverCatch;
			handsState = HandsState::Catch;
			// 敵の掴まった時処理;
			enemy->CaughtAction();
		}
		// デフォルトレイヤーは処理しない
		else if (enemyLayer != ColliderLayer::Default) {
			// ステート変更
			catchState = CatchState::EnemyCatch;
			handsState = HandsState::Catch;
			// 敵を一時的に子にする
			enemy->GetTransform()->AttachParent(GetTransform(), false);
			enemy->SetIsGravity(false);
			// 敵の掴まった時処理
			VECTOR catchPos = VSub(enemy->GetPosition(), GetPosition());
			enemy->CaughtAction(GetRotation(), catchPos);
			// 振動
			StartJoypadVibration(DX_INPUT_PAD1, 300, 180, -1);
			// アニメーション
			pAnimator->Play("Carry");
			pOwner->GetAnimator()->Play("Carry");
		}
	}

	/*
	 * @author Sekino
	 */
	auto missile = dynamic_cast<Missile*>(other);
	if (missile) {
		// ステート変更
		catchState = CatchState::EnemyCatch;
		handsState = HandsState::Catch;
		// 敵を一時的に子にする
		missile->GetTransform()->AttachParent(GetTransform(), false);
		// 敵の掴まった時処理
		VECTOR catchPos = VSub(missile->GetPosition(), GetPosition());
		missile->GetTransform()->SetRotation(GetRotation());
		missile->GetTransform()->SetPosition(catchPos);
		// 敵の掴まった時処理
		missile->CaughtAction();
		StartJoypadVibration(DX_INPUT_PAD1, 300, 180, -1);
	}

	// 当たったのがフックの場合
	if (other->GetTag() == Hook) {
		handsState = HandsState::Catch;

		StartJoypadVibration(DX_INPUT_PAD1, 300, 180, -1);
	}

	// 当たったのがレバーの場合
	if (other->GetTag() == LeverTag) {
		// ステート変更
		catchState = CatchState::LeverCatch;
		handsState = HandsState::Catch;

		StartJoypadVibration(DX_INPUT_PAD1, 300, 180, -1);
	}
}

void PlayerHands::OnTriggerStay(Collider* _pSelf, Collider* _pOther) {
	
}

void PlayerHands::OnTriggerExit(Collider* _pSelf, Collider* _pOther) {
}

/*
 *	手の移動処理
 */
void PlayerHands::HandsMove() {
	VECTOR dist = VSub(pOwner->GetPosition(), pTransform->GetPosition());
	// 手とプレイヤーの距離の2乗
	float distSq = VDot(dist, dist);

	// ウデ伸ばし中なら前に進む
	if (handsState == HandsState::ArmsExtending) {
		// ウデ伸ばしの距離制限
		if (distSq < ARM_LENGTH_MAX * ARM_LENGTH_MAX) {
			// 移動
			pTransform->AddPosition(pTransform->GetForward(), -extendSpeed);
			// ウデ伸ばし中は当たり判定の押し出しあり
			pCollider->SetResolve(true);
		}
	}
	// ウデ戻し中か敵掴み中なら戻ってくる
	else if (handsState == HandsState::ArmsReturning ||
		catchState == CatchState::EnemyCatch){
		// 戻ってきたとみなす
		if (distSq < RETURN_THRESHOLD * RETURN_THRESHOLD) {
			pTransform->SetPosition(VZero);
			handsState = HandsState::Idle;
		}
		// 戻ってくる
		// (敵掴み中の場合は閾値を超えるまで戻ってくる)
		else if (distSq > ENEMY_CATCH_RETURN_THRESHOLD * ENEMY_CATCH_RETURN_THRESHOLD ||
			catchState != CatchState::EnemyCatch) {
			pTransform->SetPosition(
				MyMath::Lerp(
					pTransform->GetLocalPosition(), 
					VZero,
					returnSpeedRatio)
			);
		}

		// ウデ戻し中は当たり判定の押し出しなし
		pCollider->SetResolve(false);
	}
}

/*
 *	掴み移動処理
 */
void PlayerHands::CatchMoving() {
	VECTOR dist = VSub(pOwner->GetPosition(), pTransform->GetPosition());
	// 手とプレイヤーの距離の2乗
	float distSq = VDot(dist, dist);
	// 戻ってきたとみなす
	if (distSq < RETURN_THRESHOLD) {
		catchState = CatchState::None;
		handsState = HandsState::Idle;
		pTransform->SetPosition(VZero);
		pOwner->SetIsGravity(true);
	}
	// プレイヤーが手の位置に戻るまで移動
	else {
		pOwner->CatchMovingMove(extendSpeed);
		pTransform->AddPosition(pTransform->GetForward(), extendSpeed);
	}
}

/*
 *	掴み中の更新処理
 */
void PlayerHands::CatchUpdate() {
	if (!pCatchCollider)
		return;
	auto catchObject = pCatchCollider->GetGameObject();
	if (!catchObject)
		return;
	if (catchObject->GetTag() == Player)
		return;
	// ウデを伸ばしていない状態なら無視(キャッチなら無視しない)
	if (handsState != HandsState::ArmsExtending &&
		handsState != HandsState::Catch)
		return;

	// 手を離したか
	bool release = action.buttonUp[static_cast<int>(PlayerAction::ArmExtend)];

	// 掴んだのが敵の場合
	auto enemy = dynamic_cast<EnemyBase*>(catchObject);
	if (enemy) {
		// 尻尾の場合はレバーと同じように処理
		if (pCatchCollider->GetLayer() == ColliderLayer::Tail) {
			// 引っこ抜き
			bool pull = pOwner->Pull();
			// 敵を倒す
			if (pull) {
				enemy->ThrownAction(GetTransform()->GetForward());
				catchState = CatchState::None;
				handsState = HandsState::ArmsReturning;
			}
		}
		// 敵を離す
		else if (release) {
			enemy->ThrownAction(GetTransform()->GetForward());
			catchState = CatchState::None;
			handsState = HandsState::ArmsReturning;
			// 子じゃなくする
			enemy->GetTransform()->DetachParent();
			enemy->SetIsGravity(true);
			// 位置をワールド座標へ
			enemy->GetTransform()->AddPosition(GetPosition());
			// アニメーション再生
			pAnimator->Play("Throw", 1.0f);
			pOwner->GetAnimator()->Play("Throw", 1.0f);
		}
	}

	/*
	 * @author Sekino
	 */
	auto missile = dynamic_cast<Missile*>(catchObject);
	if (missile) {
		// ミサイルを離す
		if (release) {
			missile->ThrownAction(GetTransform()->GetForward());
			catchState = CatchState::None;
			handsState = HandsState::ArmsReturning;
			// 子じゃなくする
			missile->GetTransform()->DetachParent();
			// 位置をワールド座標へ
			missile->GetTransform()->AddPosition(GetPosition());
			// アニメーション再生
			pAnimator->Play("Throw");
			pOwner->GetAnimator()->Play("Throw");
		}
	}

	// 掴んだのがレバーの場合
	auto lever = dynamic_cast<Lever*>(catchObject);
	if (lever) {
		// 引っこ抜き
		bool pull = pOwner->Pull();
		// ギミック発動
		if (pull) {
			lever->SetLeverTrigger(pull);
			catchState = CatchState::None;
			handsState = HandsState::ArmsReturning;
		}
	}

	// 掴んだのがフックの場合
	if (catchObject->GetTag() == Hook) {
		if (release) {
			catchState = CatchState::PillerCatch;
			pOwner->SetIsGravity(false);
		}
	}
}
