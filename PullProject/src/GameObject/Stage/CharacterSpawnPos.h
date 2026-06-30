/*
 *	@file	CharacterSpawnPos.h
 *  @author oorui
 */

#ifndef _CHARACTERSPAWNPOS_H_
#define _CHARACTERSPAWNPOS_H_

#include "../../Definition/CommonModule/MyMath.h"
#include <vector>
#include <string>

/*
 *	ステージからキャラクターの生成位置を取得するクラス
 */
class CharacterSpawnPos {
private:
	// 敵生成位置
	std::vector<VECTOR> enemyPositions;
	// プレイヤー生成位置
	VECTOR playerPosition;
	
public:

	/*
	 *	まとめてロードする
	 */
	void Load(int modelHandle);

	/*
	 * 敵の生成位置を探す
	 */
	void LoadEnemyPosition(int modelHandle);

	/*
	 * 敵生成位置一覧取得
	 */
	const std::vector<VECTOR>& GetEnemyPositions() const;

	/*
	 *	プレイヤーの生成位置を探す
	 */
	void LoadPlayerPosition(int modelHandle);

	/*
	 *	プレイヤーの生成位置を取得
	 */
	const VECTOR& GetPlayerPositions()const;


};

#endif // !_CHARACTERSPAWNPOS_H_

