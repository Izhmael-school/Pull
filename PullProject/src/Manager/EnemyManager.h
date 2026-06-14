/*
 * @brief 敵を管理をするクラス
 */
#pragma once
#include "ManagerBase.h"
#include "../Component/Singleton.h"
#include "../Generator/EnemyGenerator.h"
#include <memory>

#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

class EnemyManager : public ManagerBase, Singleton<EnemyManager> {

private:
	//std::unique_ptr<EnemyGenerator> generator;

public:
	EnemyManager();
	~EnemyManager();

private:
	void Start();
};

#endif