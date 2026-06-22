#include "Missile.h"
#include "Manager/TimeManager.h"
#include "Manager/ColliderObjectManager.h"

Missile::Missile(int _modelHandle, VECTOR _pos)
	:GameObject(_modelHandle,_pos)
	,moveSpeed(1000)
	,lifeLimitTime(5.0f)
	,lifeElapsedTime(0.0f)
{
	Start();
}

Missile::~Missile()
{
}

void Missile::Start(){
}

void Missile::Update(){
	GameObject::Update();
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

	if (lifeElapsedTime >= lifeLimitTime) {
		// 爆発
		Exprosion();
		lifeElapsedTime = 0;
	}
	else {
		lifeElapsedTime += d;
	}
}

void Missile::Exprosion(){
	ColliderObjectManager::GetInstance().CreateSphere(GetPosition(), 300);
}

void Missile::CatchStart(){
	// 掴まれたら時間をリセット
	lifeElapsedTime = 0.0f;
}

void Missile::Catching(){
}

void Missile::ThrowStart(){
	ChangeCaughtState(NoneCaughtState);
}

void Missile::Throwing(){
}

void Missile::HitObject()
{
}
