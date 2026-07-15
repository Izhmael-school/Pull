#include "BomberEnemy.h"
#include "Manager/TimeManager.h"

BomberEnemy::BomberEnemy(int _modelHandle, VECTOR _pos)
	:EnemyBase(_modelHandle, _pos)
	, explosionElapsedTime(0.0f)
	, explosionTime(2.0f * EXPLOSION_LEVEL)
	, currentExplosionLevel(0)
	, texChangeElapsedTime(0.5f)
	, texChangeTime(0.5f)
	, isNoTexture(false) {
	Start();
}

BomberEnemy::~BomberEnemy() {}

void BomberEnemy::Start() {
	EnemyBase::Start();
	type = Bomber;
	addScore = 500;
}

void BomberEnemy::Setup() {
	EnemyBase::Setup();
	// アニメーションの終了に合わせて爆発させる
	SetAnimEvent("Attack", -1, [this]() {Explosion(); });
}

void BomberEnemy::Catching() {
	EnemyBase::Catching();

	Blinking();
}

void BomberEnemy::Throwing() {
	EnemyBase::Throwing();
	Blinking();
}

void BomberEnemy::HitObject() {
	Explosion();
}

void BomberEnemy::Explosion() {
	// 爆発を作る
	sphereEvent(GetPosition(), 1000,Tag::Explosion,0.1f);
	effectEvent("Explosion", GetPosition(), 200.0f, VZero);
	audioEvent("Explosion", 255.0f, false, GetPosition(), 1000.0f);
	// 自分を消すように要請する
	wantUnuse = true;
}

void BomberEnemy::Blinking() {
	if (explosionElapsedTime >= explosionTime / EXPLOSION_LEVEL) {
		// 最終段階が終了したら爆発させる
		if (currentExplosionLevel == EXPLOSION_LEVEL - 1) {
			Explosion();
		}
		// 段階の上昇
		currentExplosionLevel++;
		explosionElapsedTime = 0.0f;
	}
	else {
		// 時間経過
		explosionElapsedTime += TimeManager::GetInstance().GetDeltaTime();
	}

	int currentLevel = min(currentExplosionLevel, EXPLOSION_LEVEL - 1);
	if (texChangeElapsedTime >= COLOR_CHANGE_INTERVAL[currentLevel]) {
		if (!isNoTexture) {
			// 段階に応じて色の変化
			MV1SetMaterialDifColor(modelHandle, 0, BLINKING_COLOR[currentLevel]);
			isNoTexture = true;
		}
		else {
			// 点滅させるために元に戻す
			MV1SetMaterialDifColor(modelHandle, 0, GetColorF(1, 1, 1, 1));
			isNoTexture = false;
		}
		texChangeElapsedTime = 0.0f;
	}
	else {
		texChangeElapsedTime += TimeManager::GetInstance().GetDeltaTime();
	}
}
