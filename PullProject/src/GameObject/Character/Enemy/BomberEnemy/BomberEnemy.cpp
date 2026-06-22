#include "BomberEnemy.h"
#include "Manager/ColliderObjectManager.h"
#include "Component/Collider/Collider.h"

BomberEnemy::BomberEnemy(int _modelHandle, VECTOR _pos)
	:EnemyBase(_modelHandle, _pos) {
	Start();
}

BomberEnemy::~BomberEnemy(){}

void BomberEnemy::Start(){
	// ラディウスの計算
	VECTOR size = VSub(MV1GetMeshMaxPosition(modelHandle, 0), MV1GetMeshMinPosition(modelHandle, 0));
	float r = (VSize(size) * 100) / 2;

	pCollider = std::make_unique<SphereCollider>(this, VZero, r);

	type = Walker;
}

void BomberEnemy::Setup(){
	// アニメーションの終了に合わせて爆発させる
	auto anim = pAnimator->GetAnimation("Attack");
	anim->SetEvent([this]() {Exprosion();}, pAnimator->GetTotalTime("Attack"));
}

void BomberEnemy::HitObject(){
	Exprosion();
}

void BomberEnemy::Exprosion(){
	// 爆発を作る
	ColliderObjectManager::GetInstance().CreateSphere(GetPosition(), 1000);
	// 自分を消すように要請する
	wantUnuse = true;
}
