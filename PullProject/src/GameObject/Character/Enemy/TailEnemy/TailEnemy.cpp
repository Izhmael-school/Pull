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
	Character::Start();

	isGravity = false;

	// モデルの最小点と最大点を取得
	VECTOR maxBodyPos = VScale(MV1GetMeshMaxPosition(modelHandle, 1), 10);
	VECTOR minBodyPos = VScale(MV1GetMeshMinPosition(modelHandle, 1), 10);
	VECTOR maxLeg1 = VScale(MV1GetMeshMaxPosition(modelHandle, 3), 10);
	VECTOR minLeg1 = VScale(MV1GetMeshMinPosition(modelHandle, 3), 10);
	VECTOR maxLeg2 = VScale(MV1GetMeshMaxPosition(modelHandle, 4), 10);
	VECTOR minLeg2 = VScale(MV1GetMeshMinPosition(modelHandle, 4), 10);

	VECTOR min;
	min.x = std::min(minBodyPos.x, std::min(minLeg1.x, minLeg2.x));
	min.y = std::min(minBodyPos.y, std::min(minLeg1.y, minLeg2.y));
	min.z = std::min(minBodyPos.z, std::min(minLeg1.z, minLeg2.z));

	VECTOR max;
	max.x = std::max(maxBodyPos.x, std::max(maxLeg1.x, maxLeg2.x));
	max.y = std::max(maxBodyPos.y, std::max(maxLeg1.y, maxLeg2.y));
	max.z = std::max(maxBodyPos.z, std::max(maxLeg1.z, maxLeg2.z));

	// 当たり判定
	pCollider = std::make_unique<AABBCollider>(this, min, max);
	pCollider->SetResolve(false);

	tailFrameIndex = MV1SearchFrame(modelHandle, "TailPoint");
	VECTOR tailPos = MV1GetFramePosition(modelHandle, tailFrameIndex);

	shotFrameIndex = MV1SearchFrame(modelHandle, "ShotPoint");

	pTailCollider = std::make_unique<ColliderObject>(tailPos, 50, Enemy, 0.0f);
	auto sphere = static_cast<SphereCollider*>(pTailCollider->GetCollider());
	sphere->SetResolve(false);
	sphere->SetGameObject(this);
	sphere->SetLocalCenter(VSub(GetPosition(), MV1GetFramePosition(modelHandle, tailFrameIndex)));

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

	if (pCollider)
		pCollider->SetEnable(false);

	// 攻撃終了時処理を持たせる
	std::string taunt = "Taunt";
	auto tauntAnim = pAnimator->GetAnimation(taunt);
	if (tauntAnim != nullptr)
		tauntAnim->SetEvent([this]() {EndAttack();}, pAnimator->GetTotalTime(taunt));
	std::string bigShot = "BigShot";
	auto shotAnim = pAnimator->GetAnimation(bigShot);
	if (shotAnim != nullptr)
		shotAnim->SetEvent([this]() {EndAttack();}, pAnimator->GetTotalTime(bigShot));

	// アニメーションに合わせてミサイルを出す
	shotAnim->SetEvent([this]() {GameObjectManager::GetInstance().CreateGameObject<Missile>("BossMissile", GetTransform()->GetForward(), MV1GetFramePosition(modelHandle, shotFrameIndex));}, 20);
	shotAnim->SetEvent([this]() {GameObjectManager::GetInstance().CreateGameObject<Missile>("BossMissile", GetTransform()->GetForward(), MV1GetFramePosition(modelHandle, shotFrameIndex));}, 30);
	shotAnim->SetEvent([this]() {GameObjectManager::GetInstance().CreateGameObject<Missile>("BossMissile", GetTransform()->GetForward(), MV1GetFramePosition(modelHandle, shotFrameIndex));}, 40);

	tauntAnim->SetEvent([this]() {ColliderObjectManager::GetInstance().CreateAABB(GetPosition(), VScale(VAdd(VLeft, VBack), 500), VAdd(VScale(VAdd(VRight, VForward), 500), VScale(VUp, 100)), None, 0.1f);}, 40);
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

void TailEnemy::ThrownAction() {
	EnemyBase::ThrownAction();
	pCollider->SetEnable(true);
}

void TailEnemy::Catching() {
	EnemyBase::Catching();

	pAnimator->Play("Shot", 2.0f);
}
