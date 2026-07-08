/*
 * @brief 自爆する敵クラス
 */
#pragma once
#ifndef _BOMBERENEMY_H_
#define _BOMBERENEMY_H_

#include "Definition/Const/ColorConst.h"

constexpr int EXPLOSION_LEVEL = 3;		// 爆発の段階
constexpr float COLOR_CHANGE_INTERVAL[EXPLOSION_LEVEL] = { 0.5f,0.25f,0.1f };

#include "../EnemyBase.h"
class BomberEnemy : public EnemyBase {
private:
	float explosionTime;		// 爆発するまでの時間
	float explosionElapsedTime;	// 爆発までの経過時間

	float texChangeTime;		// テクスチャ変更するまでの時間
	float texChangeElapsedTime;	// 変更までの経過時間
	bool isNoTexture;

	int currentExplosionLevel;
	COLOR_F BLINKING_COLOR[EXPLOSION_LEVEL] = { white_f,yellow_f,red_f };	// 爆発表現用の色

public:
	BomberEnemy(int _modelHandle, VECTOR _pos);
	~BomberEnemy();

private:
	void Start() override;

public:
	void Setup() override;

	void Catching() override;

	void Throwing() override;

	void HitObject() override;
private:
	/*
	 * @brief 自爆
	 */
	void Explosion();

	/*
	 * @brief 点滅
	 */
	void Blinking();	
};


#endif // !_BOMBERENEMY_H