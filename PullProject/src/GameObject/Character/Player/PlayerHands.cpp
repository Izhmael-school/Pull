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

PlayerHands::PlayerHands(int _modelHandle, VECTOR _pos, Tag _tag)
	: Character(_modelHandle, _pos, _tag)
	, speed(10.0f)
{
}

void PlayerHands::Start() {
	pCollider = std::make_unique<CapsuleCollider>(this, VScale(VUp, 70), VZero, 100, VZero);

}

void PlayerHands::Update() {
	pTransform->Update();
	if (!pCollider) return;
	pCollider->Update();

	// 入力
	if (InputManager::GetInstance().IsKey(KEY_INPUT_E))
		pTransform->AddPosition(VForward, -speed);
	if (InputManager::GetInstance().IsKey(KEY_INPUT_Q))
		pTransform->AddPosition(VForward, speed);
}

void PlayerHands::Render() {
	Character::Render();
	if (pCollider)
		pCollider->Render();
}

void PlayerHands::OnTriggerEnter(Collider* _pOther) {
	auto other = _pOther->GetGameObject();

	// 当たったのが敵の場合
	auto enemy = dynamic_cast<EnemyBase*>(other);
	if (enemy) {
		// 敵を掴む
		enemy->CaughtAction();
	}
}

void PlayerHands::OnTriggerStay(Collider* _pOther) {
	auto other = _pOther->GetGameObject();

	// 当たったのが敵の場合
	auto enemy = dynamic_cast<EnemyBase*>(other);
	if (enemy) {
		// 敵を離す
		if (InputManager::GetInstance().IsKeyUp(KEY_INPUT_E)) {
			enemy->ThrownAction();
		}
	}
}

void PlayerHands::OnTriggerExit(Collider* _pOther)
{
}




