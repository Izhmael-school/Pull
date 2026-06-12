/*
 * @brief 敵の生成をするクラス
 * @author Sekino
 */
#pragma once
#ifndef _ENEMYGENERATOR_H_
#define _ENEMYGENERATOR_H_

#include <memory>
class EnemyBase;

class EnemyGenerator{
private:
	int originWalkEnemyModel;

public:
	EnemyGenerator();
	~EnemyGenerator();

private:
	void LoadModel();

public:
	std::unique_ptr<EnemyBase> CreateWalkEnemy();
};
#endif // !_ENEMYGENERATOR_H_