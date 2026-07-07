/*
 *	@file	StageManager.cpp
 *  @author oorui
 */

#include "StageManager.h"
#include "GimmickObjectManager.h"
#include <format>
 /*
  *	コンストラクタ
  */
StageManager::StageManager() {
}


/*
 *	初期化
 */
void StageManager::Initialize() {
	// ステージの実体を生成
	loadedStage = std::make_unique<Stage>();
}

/*
 *	ステージの読み込み
 */
void StageManager::LoadStage(int stageID) {
	GimmickObjectManager::GetInstance().Clear();
	//ステージ設定JSONのパス生成
	if (stageID == 111) {
		stageFile = "src/Data/Gimmick/DebugStageGimmick.json";
		// ステージモデルロード	
		stageModel = ModelManager::GetInstance().Load("res/Model/Stage/DebugStage/DebugStage.mv1");

	}
	else {
		stageFile = std::format("src/Data/Gimmick/Stage{}Gimmick.json", stageID);
		// ステージモデルロード
		std::string filePath = std::format("res/Model/Stage/Stage{}/Stage_{}.mv1", stageID, stageID);
		stageModel =ModelManager::GetInstance().Load(filePath);
	}


	// モデルの複製
	int duplicate = MV1DuplicateModel(stageModel);
	// ステージ管理用リストに登録
	stageState.AddStageModelHandle(duplicate);
	// 現在のステージの実体にモデルを登録
	loadedStage->SetModelHandle(duplicate);


	// 生成位置の読み込み
	characterSpawnPos.Load(duplicate);
	// ギミック生成
	StageLoader::Load(stageFile,duplicate );

}

/*
 *	ステージの切り替え
 */
void StageManager::ChangeStage() {
	int currentHandle = stageState.GetCurrentStageHandle();
	if (currentHandle == -1) return;
	// 実体にモデルハンドルを渡す
	loadedStage->SetModelHandle(currentHandle);
}

/*
 *	ステージのIDを切り替える
 */
void StageManager::TransitionStage(int setID) {
	// 状態管理側でIDを切り替える
	stageState.TransitionStage(setID);
	// 切り替わったIDを使用してステージを切り替える
	ChangeStage();
}

/*
 *	更新
 */
void StageManager::Update() {
	WithCurrentStage([](StageBase& stage) {stage.Update();});
}

/*
 *  描画
 */
void StageManager::Render() {
	WithCurrentStage([](StageBase& stage) { stage.Render(); });
}


/*
 *  終了
 */
void StageManager::Execute() {
	stageState.ResetStageModelHandle();
}
