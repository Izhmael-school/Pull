#include "TailEnemy.h"
#include "Definition/CommonModule/MyMath.h"
#include "GameObject/Collider/ColliderObject.h"
#include "Component/Collider/Collider.h"

TailEnemy::TailEnemy(int _modelHandle, VECTOR _pos)
	:EnemyBase(_modelHandle, _pos) {
	Start();
}

void TailEnemy::Start() {
	EnemyBase::Start();

	tailFrameIndex = MV1SearchFrame(modelHandle, "TailPoint");
	VECTOR tailPos = MV1GetFramePosition(modelHandle, tailFrameIndex);

	pTailCollider = std::make_unique<ColliderObject>(tailPos,50,Enemy,0.0f);

	auto sphere = static_cast<SphereCollider*>(pCollider.get());

	sphere->SetRadius(sphere->GetRadius() / 2);

	type = Tail;
}

void TailEnemy::Update() {
	EnemyBase::Update();

	if (pTailCollider) {
		pTailCollider->GetTransform()->SetPosition(MV1GetFramePosition(modelHandle, tailFrameIndex));
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
	std::string taunt = "Taunt";
	auto attackAnim = pAnimator->GetAnimation(taunt);
	if (attackAnim != nullptr)
		attackAnim->SetEvent([this]() {EndAttack();}, pAnimator->GetTotalTime(taunt));
	std::string bigShot = "BigShot";
	attackAnim = pAnimator->GetAnimation(bigShot);
	if (attackAnim != nullptr)
		attackAnim->SetEvent([this]() {EndAttack();}, pAnimator->GetTotalTime(bigShot));

	// アニメーションに合わせて当たり判定を出す
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

	if (isAttacking)
		ChangeNextState(Attack);
}

void TailEnemy::WanderingAction() {
}

void TailEnemy::TracingAction() {
	ChangeNextState(Attack);
}
