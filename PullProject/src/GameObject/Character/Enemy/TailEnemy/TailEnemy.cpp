#include "TailEnemy.h"
#include "Definition/CommonModule/MyMath.h"
#include "GameObject/Collider/ColliderObject.h"
#include "Component/Collider/Collider.h"
#include "Manager/GameObjectManager.h"
#include "Manager/ColliderObjectManager.h"
#include "GameObject/Missile/Missile.h"

TailEnemy::TailEnemy(int _modelHandle, VECTOR _pos)
	:EnemyBase(_modelHandle, _pos) {
	Start();
}

void TailEnemy::Start() {
	EnemyBase::Start();

	// 当たり判定
	auto sphere = dynamic_cast<SphereCollider*>(pCollider.get());
	if (sphere)
		sphere->SetRadius(sphere->GetRadius() / 2);

	pCollider->SetResolve(false);
	pCollider->SetLayer(ColliderLayer::Default);

	tailFrameIndex = MV1SearchFrame(modelHandle, "TailPoint");
	VECTOR tailPos = MV1GetFramePosition(modelHandle, tailFrameIndex);

	shotFrameIndex = MV1SearchFrame(modelHandle, "ShotPoint");

	pTailCollider = std::make_unique<ColliderObject>(tailPos, 50, Enemy, 0.0f);
	auto tail = static_cast<SphereCollider*>(pTailCollider->GetCollider());
	tail->SetResolve(false);
	tail->SetGameObject(this);
	tail->SetLocalCenter(VSub(GetPosition(), MV1GetFramePosition(modelHandle, tailFrameIndex)));
	tail->SetLayer(ColliderLayer::Tail);
	type = Tail;
}

void TailEnemy::Update() {
	EnemyBase::Update();
	
	if (pTailCollider) {
		// 尻尾の位置に当たり判定を移動させる
		VECTOR pos = MV1GetFramePosition(modelHandle, tailFrameIndex);
		DrawSphere3D(pos, 16, 16, 0xff0000, 0xff0000,true);
		auto sphere = static_cast<SphereCollider*>(pTailCollider->GetCollider());
		sphere->SetLocalCenter(VSub(pos, GetPosition()));
		pTailCollider->Update();
	}
}

void TailEnemy::Render() {
	EnemyBase::Render();

	if (pTailCollider)
		pTailCollider->Render();
}

void TailEnemy::Setup() {
	EnemyBase::Setup();

	// 攻撃終了時処理を持たせる
	SetAnimEvent("Taunt", -1, [this]() {audioEvent("Taunt",255.0f,false,GetPosition(),1000.0f);EndAttack();});
	SetAnimEvent("BigShot", -1,[this]() {EndAttack();});

	// アニメーションに合わせてミサイルを出す
	SetAnimEvent("BigShot", 20, [this]() {CreateMissile();});
	SetAnimEvent("BigShot", 30, [this]() {CreateMissile();});
	SetAnimEvent("BigShot", 40, [this]() {CreateMissile();});

	SetAnimEvent("Taunt", 0, [this]() {audioEvent("Tail_Jump", 255.0f, false, GetPosition(), 1000.0f);});

	SetAnimEvent("Taunt", 40, [this]() {
		VECTOR min = VScale(VAdd(VLeft, VBack), 500);
		VECTOR max = VAdd(VScale(VAdd(VRight, VForward), 500), VScale(VUp, 100));
		VECTOR pos = GetPosition();
		aabbEvent(pos,min , max);
		effectEvent("Earthquake", pos, 0.7f, VZero);
		audioEvent("Taunt", 255.0f, false, pos, 1000.0f);
		});

	SetAnimEvent("Die", 0, [this]() {audioEvent("Tail_Down", 255.0f, false, GetPosition(), 1000.0f);});
}

void TailEnemy::AttackAction() {
	if (!isAttacking) {
		// 攻撃中にする
		isAttacking = true;
		canAttack = false;
		// アニメーション再生
		if (MyMath::Random(0, 1))
			pAnimator->Play("BigShot");
		else
			pAnimator->Play("Taunt");
	}

	int currentAnimIndex = pAnimator->GetCurrentAnimation();
	if (pAnimator->GetAnimation(currentAnimIndex)->name == "BigShot")
		GetTransform()->LookAtY(point.position);

	if (isAttacking)
		ChangeNextState(Attack);
}

void TailEnemy::WanderingAction() {
}

void TailEnemy::TracingAction() {
	ChangeNextState(Attack);
}

void TailEnemy::ThrownAction(VECTOR _dir) {
	EnemyBase::ThrownAction(_dir);
	pCollider->SetEnable(true);
}

void TailEnemy::Catching() {
	EnemyBase::Catching();

	pAnimator->Play("Shot", 2.0f);
	auto sphere = static_cast<SphereCollider*>(pTailCollider->GetCollider());
	sphere->SetRadius(100);
}

void TailEnemy::CreateMissile() {
	// 作る
	createEvent("BossMissile", this, GetTransform()->GetForward(), MV1GetFramePosition(modelHandle, shotFrameIndex));
}
