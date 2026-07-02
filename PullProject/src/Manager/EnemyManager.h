/*
 * @brief 敵を管理をするクラス
 */
#pragma once
#include "ManagerBase.h"
#include "Definition/Enum/EnemyType.h"
#include "Generator/EnemyGenerator.h"
#include <memory>
#include <vector>
#include <string>
#include "Dxlib.h"

#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

class EnemyBase;
class EnemyGenerator;
class EffectManager;
class StageManager;

using EnemyPtr = std::unique_ptr<EnemyBase>;

class EnemyManager : public ManagerBase {

private:
	EnemyGenerator generator;	// 生成管理

	std::vector<EnemyPtr> useEnemyArray;	// 使用中の敵
	std::vector<std::vector<EnemyPtr>> unuseEnemyArray;	// 未使用の敵

	EffectManager& effectManager;

public:
	EnemyManager(EffectManager& _effectManager);
	~EnemyManager();
	
private:
	void Start();

public:
	/*
	 * @brief 使用中の敵の更新
	 */
	void Update();

	/*
	 * @brief 使用中の敵描画
	 */
	void Render();

	/*
	 * @brief 敵の使用
	 */
	void UseEnemy(EnemyType _type,VECTOR _pos);

	/*
	 * @brief 使用中の敵全てを未使用化
	 */
	void UnuseAllEnemy();

	void SpawnStageFramePoint(EnemyType _type ,StageManager& _stageManager);

private:
	/*
	 * @brief　敵の未使用化
	 */
	void UnuseEnemy(EnemyPtr _unuseEnemy);
};

#endif