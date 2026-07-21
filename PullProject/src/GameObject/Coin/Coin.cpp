#include "Coin.h"
#include "Component/Collider/Collider.h"
#include "Game/GameData.h"
#include "Application.h"

Coin::Coin(int _modelHandle, VECTOR _pos)
	:GameObject(_modelHandle,_pos,Tag::Coin)
	,addScore(100)
{
	Start();
}

void Coin::Start() {
	pCollider = std::make_unique<SphereCollider>(this, VZero, 50);
	GetTransform()->AddPosition(GetBottomFramePos());
}

void Coin::Update() {
	GetTransform()->AddRotation(VUp, 5);
}

void Coin::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {
	if (_pOther->GetGameObject()->GetTag() == Tag::Player) {

		// スコアを加算
		GameData::AddScore(addScore);
		GameData::AddCoin();
		Application::GetInstance().GetAudioManager().Play("GetCoin", 255.0f, false, GetPosition(), 1000.0f);
		isActive = false;
	}
}
