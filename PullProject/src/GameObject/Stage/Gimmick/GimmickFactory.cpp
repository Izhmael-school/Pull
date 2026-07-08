/*
 *  @file GimmickFactory.cpp
 *  @author oorui
 */

#include "GimmickFactory.h"
#include "BreakWall.h"
#include "Lever.h"
#include "BomBreakWall.h"
#include "HookShot.h"
#include "ExitArea.h"
#include "PullOutFloor.h"
#include "Turret.h"
#include "Manager/ModelManager.h"

namespace {
	constexpr const char* _BREAKWALL_PATH = "res/Model/Gimmick/BreakWall/BreakWall.mv1";			// 壊れる壁のファイルパス
	constexpr const char* _BOMBREAKWALL_PATH = "res/Model/Gimmick/BomBreakWall/BomBreakWall.mv1";	// 爆弾で壊れる壁のファイルパス
	constexpr const char* _HOOLSHOT_PATH = "res/Model/Gimmick/HookShot/HookShot.mv1";				// フックショットのファイルパス
	constexpr const char* _PULLOUTFLOOR_PATH = "res/Model/Gimmick/PullOutFloor/PullOutFloor.mv1";	// 引き出し床のファイルパス
	constexpr const char* _TULLET_PATH = "res/Model/Gimmick/Turret/Turret.mv1";						// タレットのファイルパス
}

/*
 *	レバー以外のギミック生成
 */
GimmickObject* GimmickFactory::Create(const std::string& type, int triggerId, VECTOR pos, VECTOR rota) {
	// 壊れる壁生成
	if (type == "BreakWall") {
		// 実体モデル作成
		int model = CraftBaseModel(_BREAKWALL_PATH);
		// オブジェクトを生成
		return new BreakWall(
			triggerId,
			model,
			pos,
			rota
		);
	}

	// 爆弾で壊れる壁生成
	if (type == "BomBreakWall") {
		// 実体モデル作成
		int model = CraftBaseModel(_BOMBREAKWALL_PATH);
		// オブジェクトを生成
		return new BomBreakWall(
			model,
			pos,
			rota
		);
	}

	// フックショットオブジェクト生成
	if (type == "HookShot") {
		// 実体モデル作成
		int model = CraftBaseModel(_HOOLSHOT_PATH);
		// オブジェクトを生成
		return new HookShot(
			model,
			pos,
			rota,
			Hook
		);
	}

	// 引き出し床生成
	if (type == "PullOutFloor") {
		// 実体モデル作成
		int model = CraftBaseModel(_PULLOUTFLOOR_PATH);
		// オブジェクトを生成
		return new PullOutFloor(
			triggerId,
			model,
			pos,
			rota,
			Ground
		);
	}

	// タレット生成
	if (type == "Turret") {
		// 実体モデル作成
		int model = CraftBaseModel(_TULLET_PATH);
		// オブジェクトを生成
		return new Turret(
			model,
			pos,
			rota,
			Turrets
		);
	}

	// 出口オブジェクト生成
	if (type == "ExitArea") {
		// オブジェクトを生成
		return new ExitArea(
			pos,
			rota
		);
	}

	return nullptr;

}

/*
 *	モデル複製
 *  @param[in]	std::string	モデルのファイルパス
 */
int GimmickFactory::CraftBaseModel(std::string path) {
	// ベースモデル取得
	int baseModel = ModelManager::GetInstance().Load(path);
	// 実体モデル生成
	int model = MV1DuplicateModel(baseModel);

	// 実体モデルを返す
	return model;
}
