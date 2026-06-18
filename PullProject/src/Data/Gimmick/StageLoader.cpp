/*
 *	@file StageLoader.cpp
 *  @author oorui
 */

#include "StageLoader.h"
#include "Data/json.hpp"
#include "../../Manager/ModelManager.h"
#include "../../GameObject/Stage/Gimmick/GimmickFactory.h"
#include "../../Manager/Stage/GimmickObjectManager.h"

#include <fstream>
#include "GameObject/Stage/Gimmick/Lever.h"

using json = nlohmann::json;

void StageLoader::Load(const std::string& fileName, int stageHandle) {
	std::ifstream file(fileName);
	json data;
	file >> data;

	// ギミック生成
	for (auto& gimmick : data["gimmicks"]) {
		// ギミックの種類を取得
		std::string type = gimmick["type"];
		// レバーオブジェクトなら、IDを設定
		int leverID = gimmick["LeverID"];
		// ギミックの生成位置の名前を取得
		std::string point = gimmick["position"];
		// ステージフレームから位置をｓ取得
		int frame = MV1SearchFrame(stageHandle, point.c_str());

		// 位置を設定
		VECTOR pos = MV1GetFramePosition(stageHandle, frame);
		
		// モデルを設定
		int model = ModelManager::GetInstance().Load(gimmick["model"]);
		
		// 生成
		GimmickObject* object =
			GimmickFactory::Create(
				type,
				model,
				leverID,
				pos
			);

		// オブジェクトを登録
		GimmickObjectManager::GetInstance().Register(object);

	}

	// レバー生成
	for (auto& lever : data["Levers"]) {
		// レバーIDを取得
		int id = lever["LeverID"];
		// レバーの生成位置の名前を取得
		std::string point = lever["position"];
		// ステージフレームから位置取得
		int frame = MV1SearchFrame(stageHandle, point.c_str());
		// 座標を取得
		VECTOR pos = MV1GetFramePosition(stageHandle, frame);

		// モデルを設定
		int model = ModelManager::GetInstance().Load(lever["model"]);

		// レバーオブジェクトを生成
		Lever* obj = new Lever(id, model, pos);

		// オブジェクトを登録
		GimmickObjectManager::GetInstance().Register(obj);

	}
}
