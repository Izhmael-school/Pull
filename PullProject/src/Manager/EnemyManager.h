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
class AudioManager;
class StageManager;
class GameObjectManager;
class ColliderObjectManager;

using EnemyPtr = std::unique_ptr<EnemyBase>;

struct EnemyNeedManager {
	EffectManager& effectManager;
	AudioManager& audioManager;
};

class EnemyManager : public ManagerBase {

private:
	EnemyGenerator generator;	// 生成管理

	std::vector<EnemyPtr> useEnemyArray;	// 使用中の敵
	std::vector<std::vector<EnemyPtr>> unuseEnemyArray;	// 未使用の敵

	EffectManager& effectManager;

	GameObjectManager& gameObjectManager;

	ColliderObjectManager& colliderObjectManager;

	AudioManager& audioManager;
public:
	EnemyManager(EnemyNeedManager _need);
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

	/*
	 * @brief ステージの敵生成ポイントから敵を生成する
	 */
	void SpawnStageFramePoint(EnemyType _type ,StageManager& _stageManager);

	/*
	 * @brief アニメーションイベント用のコライダー生成
	 */
	void PlayAnimEvent_Sphere(VECTOR _pos, float _radius);

	/*
	 * @brief アニメーションイベント用のコライダー生成
	 */
	void PlayAnimEvent_AABB(VECTOR _pos, VECTOR _min, VECTOR _max);

	/*
	 * @brief アニメーションイベント用のエフェクト再生
	 */
	void PlayAnimEvent_Effect(const std::string& _effectName, VECTOR _pos, float _scale = 1.0f, VECTOR rot = VZero);

	/*
	 * @brief アニメーションイベント用のオーディオ再生
	 */
	void PlayAnimEvent_Audio(const std::string& _audioName, float _volume = 255.0f, bool _isLoop = false,VECTOR _pos = VZero,float distance = 100.0f);

	/*
	 * @brief アニメーションイベント用のオブジェクト生成
	 */
	void PlayAnimEvent_Missile(std::string _modelName,GameObject* _pOwner, VECTOR _dir,VECTOR _pos);

private:
	/*
	 * @brief　敵の未使用化
	 */
	void UnuseEnemy(EnemyPtr _unuseEnemy);
};

#endif