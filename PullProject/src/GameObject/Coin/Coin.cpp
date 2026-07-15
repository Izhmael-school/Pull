#include "Coin.h"
#include "Component/Collider/Collider.h"
#include "Game/GameData.h"

Coin::Coin(int _modelHandle, VECTOR _pos, Tag _tag)
	:GameObject(_modelHandle,_pos,_tag)
	,addScore(100)
{
	Start();
}

void Coin::Start() {
	pCollider = std::make_unique<SphereCollider>(this, VZero, 50);
	GetTransform()->AddPosition(GetBottomFramePos());
}

void Coin::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {
	if (_pOther->GetGameObject()->GetTag() == Tag::Player) {

		// スコアを加算
		GameData::AddScore(addScore);
		GameData::AddCoin();

		isActive = false;
	}
}
