/*
 * @brief 敵の生成をするクラス
 * @author Sekino
 */
#pragma once
#ifndef _ENEMYGENERATOR_H_
#define _ENEMYGENERATOR_H_

#include <memory>
#include <vector>
#include <functional>
#include "Definition/Enum/EnemyType.h"
#include "DxLib.h"
#include "GameObject/Character/Enemy/EnemyBase.h"

class EffectManager;

using EnemyPtr = std::unique_ptr<EnemyBase>;


const char const ENEMYDATA_FILEPATH[] = "src/Data/EnemyData.json";
const char const RELEASE_ENEMYDATA_FILEPATH[] = "res/ExternalFile/Resource/EnemyData.msgpack";

class EnemyGenerator{
private:
	std::vector<int> originEnemyModelHandle;	// モデルの配列
	std::vector<int> originEnemyAnimHandle;	// アニメーションの配列
	std::vector<std::function<EnemyPtr(int,VECTOR)>> CreateEnemyEvent;// 敵を生成する関数の配列

public:
	EnemyGenerator();
	~EnemyGenerator();

private:
	/*
	 * @brief Jsonファイルからのモデル読み込み
	 */
	void LoadModel();
	/*
	 * @brief 敵を生成する関数をセットする
	 */
	void SetCreateEvent();

	/*
	 * @brief 歩く敵の生成
	 */
	EnemyPtr CreateWalker(int _modelHandle,VECTOR _pos);

	/*
	 * @brief 弾を吐く敵の生成
	 */
	EnemyPtr CreateShooter(int _modelHandle, VECTOR _pos);

	/*
	 * @brief 自爆する敵の生成
	 */
	EnemyPtr CreateBomber(int _modelHandle, VECTOR _pos);

	/*
	 * @brief 尻尾を掴める敵の生成
	 */
	EnemyPtr CreateTail(int _modelHandle, VECTOR _pos);

	/*
	 * @brief 鎧を着ている敵の生成
	 */
	EnemyPtr CreateArmor(int _modelHandle, VECTOR _pos);
public:
	/*
	 * @brief 敵の生成
	 * @param _type:敵の種類
	 * @param _pos:初期位置
	 * @param _effectManager:エフェクトマネージャー
	 */
	EnemyPtr CreateEnemy(EnemyType _type,VECTOR _pos);
};
#endif // !_ENEMYGENERATOR_H_