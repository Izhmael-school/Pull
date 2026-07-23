/*
 * @brief ステージID指定でステージ上にコインを生成する
 */
#pragma once
#ifndef _COINGENERATOR_H_
#define _COINGENERATOR_H_

constexpr char const COINDATA_FILEPATH[] = "src/Data/Stage/Coin/Stage_{}_CoinData.json";
constexpr char const RELEASE_COINDATA_FILEPATH[] = "res/ExternalFile/Stage/Coin/Stage_{}_CoinData.msgpack";
class CoinGenerator {
public:
	/*
	 * コインの生成
	 */
	static void GenerateCoin(int _stageID, int _stageModelHandle);
};
#endif
