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

class EnemyBase;

class EnemyGenerator{
private:
	std::vector<int> originEnemyModelHandle;	// モデルの配列
	std::vector<int> originEnemyAnimHandle;	// アニメーションの配列
	std::vector<std::function<std::unique_ptr<EnemyBase>(int,VECTOR)>> CreateEnemyEvent;// 敵を生成する関数の配列

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
	std::unique_ptr<EnemyBase> CreateWalker(int _modelHandle,VECTOR _pos);
public:
	/*
	 * @brief 敵の生成
	 * @param _type:敵の種類
	 * @param _pos:初期位置
	 */
	std::unique_ptr<EnemyBase> CreateEnemy(EnemyType _type,VECTOR _pos);
};
#endif // !_ENEMYGENERATOR_H_