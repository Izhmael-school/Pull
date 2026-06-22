/*
 * @brief 自爆する敵クラス
 */
#pragma once
#ifndef _BOMBERENEMY_H_
#define _BOMBERENEMY_H_

#include "../EnemyBase.h"
class BomberEnemy : public EnemyBase{
public:
	BomberEnemy(int _modelHandle, VECTOR _pos);
	~BomberEnemy();

private:
	void Start() override;

public: 
	void Setup();

	void HitObject() override;
private:
	/*
	 * @brief 自爆
	 */
	void Exprosion();
};

#endif // !_BOMBERENEMY_H