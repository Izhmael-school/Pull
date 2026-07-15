/*
 * @brief 保存すべきデータを一時的に保持する
 * @author Sekino
 */
#pragma once
#ifndef _GAMEDATA_H_
#define _GAMEDATA_H_

class GameData {
private:
	static int score;
	static int coin;

public:
	/*
	 * @brief スコアの取得
	 */
	static int GetScore() { return score; };

	/*
	 * @brief スコアの加算
	 */
	static void AddScore(int _add) { score += _add; }

	/*
	 * @brief スコアのリセット
	 */
	static void ResetScore() { score = 0; }

	/*
	 * @brief コインの取得
	 */
	static int GetCoin() { return coin; };

	/*
	 * @brief コインの加算
	 */
	static void AddCoin(int _add = 1) { coin += _add; }

	/*
	 * @brief コインのリセット
	 */
	static void ResetCoin() { coin = 0; }
};
#endif
