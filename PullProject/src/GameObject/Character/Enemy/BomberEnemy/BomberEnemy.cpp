#include "BomberEnemy.h"
#include "Manager/ColliderObjectManager.h"
#include "Component/Collider/Collider.h"

BomberEnemy::BomberEnemy(int _modelHandle, VECTOR _pos)
	:EnemyBase(_modelHandle, _pos) {
	Start();
}

BomberEnemy::~BomberEnemy(){}

void BomberEnemy::Start(){
	type = Bomber;
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
