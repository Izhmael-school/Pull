#include "CoinGenerator.h"
#include "Definition/CommonModule/MyJson.h"
#include "Manager/GameObjectManager.h"
#include <string>
#include "GameObject/Coin/Coin.h"
#include "DxLib.h"

void CoinGenerator::GenerateCoin(int _stageID, int _stageModelHandle) {
	auto data = MyJson::LoadJsonFile(std::format(COINDATA_FILEPATH, _stageID));
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
		Coin* coin = GameObjectManager::GetInstance().CreateGameObject<Coin>("Coin", pos,Tag::None);
	}
}
