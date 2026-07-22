#include "CoinGenerator.h"
#include "Definition/CommonModule/MyJson.h"
#include "Manager/GameObjectManager.h"
#include <string>
#include "GameObject/Coin/Coin.h"
#include "DxLib.h"

void CoinGenerator::GenerateCoin(int _stageID, int _stageModelHandle) {
#if _DEBUG
	auto data = MyJson::LoadJsonFile(std::format(COINDATA_FILEPATH, _stageID));
#else
	auto data = MyJson::LoadBinary(std::format(RELEASE_COINDATA_FILEPATH, _stageID));
#endif
	// データが無ければ帰る
	if (data.empty()) return;
	int index = 0;
	for (auto d : data["data"]) {
		// 文字列に明示的に変換
		std::string frameName = d["frameName"];
		// フレーム番号の取得
		int frameIndex = MV1SearchFrame(_stageModelHandle, frameName.c_str());
		// 座標の取得
		VECTOR pos = MV1GetFramePosition(_stageModelHandle, frameIndex);
		// 生成
		class Coin* coin = GameObjectManager::GetInstance().CreateGameObject<class Coin>("Coin", pos);
	}
}
