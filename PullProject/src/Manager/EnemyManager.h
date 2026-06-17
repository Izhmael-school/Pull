/*
 * @brief 敵を管理をするクラス
 */
#pragma once
#include "ManagerBase.h"
#include "Component/Singleton.h"
#include "Definition/Enum/EnemyType.h"
#include <memory>
#include <vector>
#include "Dxlib.h"

#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

class EnemyBase;
class EnemyGenerator;

class EnemyManager : public ManagerBase, public Singleton<EnemyManager> {

private:
	std::unique_ptr<EnemyGenerator> generator;	// 生成管理

	std::vector<std::unique_ptr<EnemyBase>> useEnemyArray;	// 使用中の敵
	std::vector<std::vector<std::unique_ptr<EnemyBase>>> unuseEnemyArray;	// 未使用の敵

public:
	EnemyManager();
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

private:
	/*
	 * @brief　敵の未使用化
	 */
	void UnuseEnemy(std::unique_ptr<EnemyBase> _unuseEnemy);

};

#endif