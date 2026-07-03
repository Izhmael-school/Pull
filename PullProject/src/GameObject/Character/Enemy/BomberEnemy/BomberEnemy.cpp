#include "BomberEnemy.h"

BomberEnemy::BomberEnemy(int _modelHandle, VECTOR _pos)
	:EnemyBase(_modelHandle, _pos) {
	Start();
}

BomberEnemy::~BomberEnemy(){}

void BomberEnemy::Start(){
	EnemyBase::Start();
	type = Bomber;
}

void BomberEnemy::Setup(){
	EnemyBase::Setup();
	// アニメーションの終了に合わせて爆発させる
	SetAnimEvent("Attack",-1, [this]() {Exprosion(); });
}

void BomberEnemy::HitObject(){
	Exprosion();
}

void BomberEnemy::Exprosion(){
	// 爆発を作る
	sphereEvent(GetPosition(), 1000);
	effectEvent("Explosion", GetPosition(), 200.0f, VZero);
	// 自分を消すように要請する
	wantUnuse = true;
}
