#include "Missile.h"
#include "Manager/TimeManager.h"
#include "Manager/ColliderObjectManager.h"
#include "Manager/EffectManager.h"
#include "Component/Collider/Collider.h"
#include "GameObject/Character/Player/PlayerHands.h"
#include "GameObject/Stage/Gimmick/BomBreakWall.h"

Missile::Missile(int _modelHandle, GameObject* _owner, EffectManager* _effect, VECTOR _dir, VECTOR _pos)
	:GameObject(_modelHandle, _pos)
	, moveSpeed(1000)
	, lifeLimitTime(5.0f)
	, lifeElapsedTime(0.0f)
	, pEffectManager(*_effect)
	, pOwner(_owner) {
	GetTransform()->LookAtDir(_dir);
	if (_effect)
		pEffect = _effect->Play("MissileBoost", _pos, 10.0f, _dir);
	Start();
}

Missile::~Missile() {
}

void Missile::Start() {
	VECTOR min = MV1GetMeshMinPosition(modelHandle, 0);
	VECTOR max = MV1GetMeshMaxPosition(modelHandle, 0);

	pCollider = std::make_unique<AABBCollider>(this, VScale(min, 100.0f), VScale(max, 100.0f));
}

void Missile::Update() {
	GameObject::Update();
	if (!isActive) return;

	CaughtUpdate();

	// 掴まれたら更新しない
	if (GetCurrentCaughtState() != NoneCaughtState) return;

	// 前方を取得
	VECTOR forward = GetTransform()->GetForward();
	// 正規化
	VECTOR dir = VNorm(forward);
	float d = TimeManager::GetInstance().GetDeltaTime();
	VECTOR pos = VScale(dir, moveSpeed * d);
	// 加算
	GetTransform()->AddPosition(pos);

	if (pEffect)
		pEffect->GetTransform()->SetPosition(GetBoostEffectPoint());


	if (lifeElapsedTime >= lifeLimitTime) {
		// 爆発
		Exprosion();
	}
	else {
		lifeElapsedTime += d;
	}
}

void Missile::Exprosion() {
	ColliderObjectManager::GetInstance().CreateSphere(GetPosition(), 300,None,0.5f,[](Collider* _pOther){
		auto breakWall = dynamic_cast<BomBreakWall*>(_pOther->GetGameObject());
		if(breakWall != nullptr) {
			breakWall->ActivGimmick(true);
		}
	});
	isActive = false;
	pEffectManager.Play("Explosion", GetPosition(), 50.0f, VZero);
	lifeElapsedTime = 0;
	pEffect->Stop();
}

VECTOR Missile::GetBoostEffectPoint() {
	return MV1GetFramePosition(modelHandle, MV1SearchFrame(modelHandle, "BoostEffectPoint"));
}

void Missile::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {

	auto hands = dynamic_cast<PlayerHands*>(_pOther->GetGameObject());
	// ウデの処理はここでは行わない
	if (hands != nullptr) {
		return;
	}

	if (pOwner != _pOther->GetGameObject())
		Exprosion();
}

void Missile::CaughtAction() {
	ChangeCaughtState(Catch);
}

void Missile::ThrownAction(VECTOR _dir) {
	GetTransform()->LookAtDir(VScale(_dir, -1));
	ChangeCaughtState(Throw);
}

void Missile::CatchStart() {
	// 掴まれたら時間をリセット
	lifeElapsedTime = 0.0f;
}

void Missile::Catching() {
}

void Missile::ThrowStart() {
	ChangeCaughtState(NoneCaughtState);
}

void Missile::Throwing() {
}

void Missile::HitObject() {
}
