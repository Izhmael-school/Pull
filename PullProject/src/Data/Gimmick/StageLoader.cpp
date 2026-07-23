/*
 *	@file StageLoader.cpp
 *  @author oorui
 */

#include "StageLoader.h"
#include "Data/json.hpp"
#include "../../Manager/ModelManager.h"
#include "../../GameObject/Stage/Gimmick/GimmickFactory.h"
#include "../../Manager/Stage/GimmickObjectManager.h"
#include "../../GameObject/Stage/Gimmick/TriggerInterface.h"

#include "GameObject/Stage/Gimmick/Lever.h"
#include "GameObject/Stage/Gimmick/BreakWall.h"

#include <fstream>
#include <unordered_map>

#include "Definition/CommonModule/MyJson.h"
using json = nlohmann::json;

namespace {
	constexpr const char* _DATENAME_GIMMICK = "Gimmicks";	// ギミック項目
	constexpr const char* _DATENAME_LeverID = "LeverID";	// レバー対応ID
	constexpr const char* _DATENAME_POSITION = "Position";	// 座標
	constexpr const char* _DATENAME_MODEL = "Model";		// モデル
	constexpr const char* _DATENAME_TYPE = "Type";			// ギミックの種類
	constexpr const char* _DATENAME_LEVER = "Levers";		// レバー
	constexpr const char* _DATENAME_ROTATION = "Rotation";	// 回転
	constexpr const char* _LEVER_POS = "Lever_SP{}";			// レバー生成位置
}

void StageLoader::Load(const std::string& fileName, int stageHandle) {
#if _DEBUG
	auto data = MyJson::LoadJsonFile(fileName);
#else
	auto data = MyJson::LoadJsonFile(fileName);
	// auto data = MyJson::LoadBinary(fileName);
#endif
	// レバー対応オブジェクト
	std::unordered_map<int, TriggerInterface*> triggerMap;

	// ギミック生成
	for (auto& gimmick : data[_DATENAME_GIMMICK]) {
		// ギミックの種類を取得
		std::string type = gimmick[_DATENAME_TYPE];
		// レバーオブジェクトなら、IDを設定 ,なければ-1に指定
		int leverID = gimmick.value(_DATENAME_LeverID, -1);
		// ギミックの生成位置の名前を取得
		std::string point = gimmick[_DATENAME_POSITION];
		// ステージフレームから位置を取得
		int frame = MV1SearchFrame(stageHandle, point.c_str());

		// 位置を設定
		VECTOR pos = MV1GetFramePosition(stageHandle, frame);

		// 回転数を取得
		float baseRota = gimmick[_DATENAME_ROTATION];
		VECTOR vRota = { 0.0f,baseRota,0.0f };

		// 生成
		GimmickObject* object = GimmickFactory::Create(
			type,
			leverID,
			pos,
			vRota
		);

		// TriggerInterface取得
		TriggerInterface* trigger = dynamic_cast<TriggerInterface*>(object);

		if (trigger) {
			triggerMap[trigger->GetTriggerID()] = trigger;
		}

		// オブジェクトを登録
		GimmickObjectManager::GetInstance().Register(object);

	}

	// レバー生成
	for (auto& lever : data[_DATENAME_LEVER]) {
		// レバーIDを取得
		int id = lever[_DATENAME_LeverID];

		// ベースモデル取得
		int baseModel = ModelManager::GetInstance().Load("res/Model/Gimmick/Lever.mv1");
		// 実体モデル生成
		int model = MV1DuplicateModel(baseModel);
		// 座標を仮設定
		VECTOR pos = VGet(0, 0, 0);

		// 回転数を取得
		float baseRota = lever[_DATENAME_ROTATION];
		VECTOR vRota = { 0.0f,baseRota,0.0f };

		// 同じIDのギミック検索
		auto triggerObj = triggerMap.find(id);

		// 同じIDを持つギミックが存在するかどうか
		if (triggerObj != triggerMap.end() &&
			triggerObj->second->GetLeverSpawnPosition(pos)) {
			// ギミックから取得成功
		}
		else {
			// 保険としてStageHandleから取得
			// レバーの位置をIDと合わせる
			std::string leverposFrame = std::format(_LEVER_POS, id);

			// モデル内のLeverPointフレーム検索
			int frame = MV1SearchFrame(stageHandle, leverposFrame.c_str());
			// 座標を設定
			pos = MV1GetFramePosition(stageHandle, frame);
		}

		// レバーオブジェクト生成
		Lever* obj = new Lever(id, model, pos, vRota, LeverTag);
		// オブジェクトを登録
		GimmickObjectManager::GetInstance().Register(obj);

	}
}
