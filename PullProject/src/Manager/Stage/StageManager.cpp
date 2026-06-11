#include "StageManager.h"

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
void StageManager::LoadStage(const std::vector<int> modelHandleBase) {
	// モデルハンドルの複製、追加
	for (auto model : modelHandleBase) {
		// モデルハンドルの複製
		int duplicatedModel = MV1DuplicateModel(model);
		// ステージの状態管理に、複製したモデルハンドルを渡す
		stageState.AddStageModelHandle(duplicatedModel);
		// デバッグ用
		loadedStage->SetModelHandle(duplicatedModel);
	}
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
