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
	, pCatchObject(nullptr)
	, extendSpeed(20.0f)
	, returnSpeedRatio(0.3f)

	, RETURN_THRESHOLD(1.0f)
	, ARM_LENGTH_MAX(1500.0f)
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
		// ウデ伸ばし(ウデ戻し中じゃなければ)
		if (action.button[static_cast<int>(PlayerAction::ArmExtend)] &&
			handsState != HandsState::ArmsReturning)
			handsState = HandsState::ArmsExtending;
		// ウデ戻し
		if (action.buttonUp[static_cast<int>(PlayerAction::ArmExtend)])
			handsState = HandsState::ArmsReturning;
	}

	// 手の移動
	HandsMove();

	// 掴み移動
	if (catchState == CatchState::PillerCatch) {
		CatchMoving();
	}

	if (!pCatchObject)
		return;
	ImGui::Begin("CatchObjectPosition&Rotation");
	ImGui::Text("%f, %f, %f", pCatchObject->GetPosition().x, pCatchObject->GetPosition().y, pCatchObject->GetPosition().z);
	ImGui::Text("%f, %f, %f", pCatchObject->GetRotation().x, pCatchObject->GetRotation().y, pCatchObject->GetRotation().z);
	ImGui::End();
	ImGui::Begin("HandsPosition&Rotation");
	ImGui::Text("%f, %f, %f", GetPosition().x, GetPosition().y, GetPosition().z);
	ImGui::Text("%f, %f, %f", GetRotation().x, GetRotation().y, GetRotation().z);
	ImGui::End();
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

	// 当たったオブジェクトを保存
	pCatchObject = other;

	// 当たったのが敵の場合
	auto enemy = dynamic_cast<EnemyBase*>(other);
	if (enemy) {
		// ステート変更
		catchState = CatchState::EnemyCatch;
		handsState = HandsState::Catch;
		// 敵を一時的に子にする
		enemy->GetTransform()->AttachParent(GetTransform());
		// 敵の掴まった時処理
		enemy->CaughtAction(GetRotation(), VZero);
		StartJoypadVibration(DX_INPUT_PAD1, 300, 180, -1);
	}

	/*
	 * @author Sekino
	 */
	auto missile = dynamic_cast<Missile*>(other);
	if (missile) {
		// ステート変更
		catchState = CatchState::EnemyCatch;
		handsState = HandsState::Catch;
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
		//else {
		//	// 最大距離まで伸ばしたら戻る
		//	handsState = HandsState::ArmsReturning;
		//}

	}
	// ウデ戻し中なら戻ってくる
	else if (handsState == HandsState::ArmsReturning){
		// 戻ってきたとみなす
		if (distSq < RETURN_THRESHOLD * RETURN_THRESHOLD) {
			pTransform->SetPosition(VZero);
			handsState = HandsState::Idle;
		}
		// 戻ってくる
		else {
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
	if (!pCatchObject)
		return;
	if (pCatchObject->GetTag() == Player)
		return;
	// ウデを伸ばしていない状態なら無視(キャッチなら無視しない)
	if (handsState != HandsState::ArmsExtending &&
		handsState != HandsState::Catch)
		return;

	// 手を離したか
	bool release = action.buttonUp[static_cast<int>(PlayerAction::ArmExtend)];

	// 掴んだのが敵の場合
	auto enemy = dynamic_cast<EnemyBase*>(pCatchObject);
	if (enemy) {
		// 敵を離す
		if (release) {
			enemy->ThrownAction(GetTransform()->GetForward());
			catchState = CatchState::None;
			handsState = HandsState::ArmsReturning;
			// 子じゃなくする
			enemy->GetTransform()->DetachParent();
		}
	}

	/*
	 * @author Sekino
	 */
	auto missile = dynamic_cast<Missile*>(pCatchObject);
	if (missile) {
		// ミサイルを離す
		if (release) {
			missile->ThrownAction(GetTransform()->GetForward());
			catchState = CatchState::None;
			handsState = HandsState::ArmsReturning;
		}
	}

	// 掴んだのがレバーの場合
	auto lever = dynamic_cast<Lever*>(pCatchObject);
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
	if (pCatchObject->GetTag() == Hook) {
		if (release) {
			catchState = CatchState::PillerCatch;
			pOwner->SetIsGravity(false);
		}
	}
}
