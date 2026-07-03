#include "Missile.h"
#include "Manager/TimeManager.h"
#include "Manager/ColliderObjectManager.h"
#include "Manager/EffectManager.h"


Missile::Missile(int _modelHandle,EffectManager* _effect, VECTOR _dir, VECTOR _pos)
	:GameObject(_modelHandle, _pos)
	, moveSpeed(1000)
	, lifeLimitTime(5.0f)
	, lifeElapsedTime(0.0f) 
	, pEffectManager(*_effect)
{
	GetTransform()->LookAtDir(_dir);
	if (_effect)
		pEffect = _effect->Play("MissileBoost", _pos, 10.0f, _dir);
}

Missile::~Missile() {
}

void Missile::Start() {
}

void Missile::Update() {
	GameObject::Update();
	if (!isActive) return;

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

	if(pEffect)
		pEffect->GetTransform()->SetPosition(GetBoostEffectPoint());


	if (lifeElapsedTime >= lifeLimitTime) {
		// 爆発
		Exprosion();
		pEffectManager.Play("Explosion", GetPosition(), 50.0f, VZero);
		lifeElapsedTime = 0;
		pEffect->Stop();
	}
	else {
		lifeElapsedTime += d;
	}
}

void Missile::Exprosion() {
	ColliderObjectManager::GetInstance().CreateSphere(GetPosition(), 300);
	isActive = false;
}

VECTOR Missile::GetBoostEffectPoint() {
	return MV1GetFramePosition(modelHandle, MV1SearchFrame(modelHandle, "BoostEffectPoint"));
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
