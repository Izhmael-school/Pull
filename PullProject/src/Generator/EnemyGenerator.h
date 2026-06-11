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
	static int originWalkEnemyModel;

private:
	static void LoadModel();

public:
	static std::unique_ptr<EnemyBase> CreateWalkEnemy();
};
#endif // !_ENEMYGENERATOR_H_