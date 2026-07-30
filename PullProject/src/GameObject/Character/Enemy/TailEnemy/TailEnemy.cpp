#include "TailEnemy.h"
#include "Definition/CommonModule/MyMath.h"
#include "GameObject/Collider/ColliderObject.h"
#include "Component/Collider/Collider.h"
#include "Manager/GameObjectManager.h"
#include "Manager/ColliderObjectManager.h"
#include "Manager/Stage/StageManager.h"
#include "Manager/Stage/GimmickManager.h"
#include "GameObject/Missile/Missile.h"

TailEnemy::TailEnemy(int _modelHandle, VECTOR _pos)
	:EnemyBase(_modelHandle, _pos)
	, tailRadius(50) {
	Start();
}

void TailEnemy::Start() {
	EnemyBase::Start();

	// 当たり判定
	auto sphere = dynamic_cast<SphereCollider*>(pCollider.get());
	if (sphere)
		sphere->SetRadius(sphere->GetRadius() / 2);

	pCollider->SetResolve(true);
	pCollider->SetLayer(ColliderLayer::Default);

	tailFrameIndex = MV1SearchFrame(modelHandle, "TailPoint");
	VECTOR tailPos = MV1GetFramePosition(modelHandle, tailFrameIndex);

	shotFrameIndex = MV1SearchFrame(modelHandle, "ShotPoint");

	pTailCollider = std::make_unique<ColliderObject>(tailPos, tailRadius, Enemy, 0.0f);
	auto tail = static_cast<SphereCollider*>(pTailCollider->GetCollider());
	tail->SetResolve(false);
	tail->SetGameObject(this);
	tail->SetLocalCenter(VSub(GetPosition(), MV1GetFramePosition(modelHandle, tailFrameIndex)));
	tail->SetLayer(ColliderLayer::Tail);
	type = Tail;
	tag = Tag::TailEnemy;
	addScore = 5000;

	vision.rayAngle = 270;
	vision.rayLenght = 1500;
}

void TailEnemy::Update() {
	EnemyBase::Update();

	if (pTailCollider) {
		// 尻尾の位置に当たり判定を移動させる
		VECTOR pos = MV1GetFramePosition(modelHandle, tailFrameIndex);
		DrawSphere3D(pos, 16, 16, 0xff0000, 0xff0000, true);
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
	SetAnimEvent("Taunt", -1, [this]() {audioEvent("Taunt", 255.0f, false, GetPosition(), 1000.0f);EndAttack();});
	SetAnimEvent("BigShot", -1, [this]() {EndAttack();});

	// アニメーションに合わせてミサイルを出す
	SetAnimEvent("BigShot", 20, [this]() {CreateMissile();});
	SetAnimEvent("BigShot", 30, [this]() {CreateMissile();});
	SetAnimEvent("BigShot", 40, [this]() {CreateMissile();});

	SetAnimEvent("Taunt", 0, [this]() {audioEvent("Tail_Jump", 255.0f, false, GetPosition(), 1000.0f);});

	// 衝撃波
	SetAnimEvent("Taunt", 40, [this]() {
		VECTOR min = VAdd(VScale(VAdd(VLeft, VBack), 500), VScale(VUp, -100));
		VECTOR max = VAdd(VScale(VAdd(VRight, VForward), 500), VScale(VUp, -50));
		VECTOR pos = GetPosition();
		aabbEvent(pos, min, max,EnemyAttack,0.1f);
		pos.y += -50;
		effectEvent("Earthquake", pos, 0.7f, VZero);
		audioEvent("Taunt", 255.0f, false, pos, 1000.0f);
		});

	SetAnimEvent("Die", 0, [this]() {audioEvent("Tail_Down", 255.0f, false, GetPosition(), 1000.0f);});

	auto col = pTailCollider->GetCollider();
	if (col) {
		col->SetEnable(true);
		auto sphere = dynamic_cast<SphereCollider*>(col);
		sphere->SetRadius(tailRadius);
	}
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
		GetTransform()->GraduallyLookAtY(point.position);

	if (isAttacking)
		ChangeNextState(Attack);
}

void TailEnemy::WanderingAction() {
}

void TailEnemy::TracingAction() {
	ChangeNextState(Attack);
}

void TailEnemy::ThrownAction(VECTOR _dir) {
	pCollider->SetEnable(true);
	ChangeCaughtState(CaughtState::Throw);
}

void TailEnemy::CatchStart() {
	CaughtObject::CatchStart();
	ChangeNextState(OutofControl);
}

void TailEnemy::Catching() {
	ChangeNextState(OutofControl);
	pAnimator->Play("Shot", 2.0f);
	auto col = static_cast<SphereCollider*>(pTailCollider->GetCollider());
	col->SetRadius(tailRadius * 2);
}

void TailEnemy::ThrowStart() {
	HitObject();
}

void TailEnemy::Throwing() {
	ChangeNextState(OutofControl);
}

void TailEnemy::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {

}

void TailEnemy::Dead() {
	EnemyBase::Dead();

	// 尻尾消す
	auto col = pTailCollider->GetCollider();
	if (col) {
		col->SetEnable(false);
	}
	int stageID = StageManager::GetInstance().GetStageID();
	if (stageID == 6 || stageID == 7) {
		GimmickManager::GetInstance().ActivateLever(10000);
	}
}

void TailEnemy::CreateMissile() {
	// 作る
	createEvent("BossMissile", this, GetTransform()->GetForward(), MV1GetFramePosition(modelHandle, shotFrameIndex));
}
