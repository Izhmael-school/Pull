/*
 *	@file StageSelectScene.cpp
 *  @author oorui
 */

#include "StageSelectScene.h"
#include "Manager/SceneManager.h"
#include "Definition/Enum/SceneType.h"
#include "Manager/InputManager.h"
#include "Manager/Stage/StageManager.h"
#include "../../Manager/InputSystemManager.h"
#include "../../Definition/Enum/StageSelectActionEnum.h"

#include <algorithm>
#include <math.h>

 /*
  *	コンストラクタ
  */
StageSelectScene::StageSelectScene() :currentScene(0) { Start(); }

/*
 *	開始処理
 */
void StageSelectScene::Start() {
	InputSystemManager::GetInstance().SetActionMapIsActive(ActionMap::StageSelect, true);

	selectInfoArray.push_back({ "Stage1 o",[]() {StageManager::GetInstance().SetStageID(1);SceneManager::GetInstance().ChangeScene(SceneType::Game);} });
	selectInfoArray.push_back({ "Stage2 o",[]() {StageManager::GetInstance().SetStageID(2);SceneManager::GetInstance().ChangeScene(SceneType::Game);} });
	selectInfoArray.push_back({ "Stage3 o",[]() {StageManager::GetInstance().SetStageID(3);SceneManager::GetInstance().ChangeScene(SceneType::Game);} });
	selectInfoArray.push_back({ "Stage4 o",[]() {StageManager::GetInstance().SetStageID(4);SceneManager::GetInstance().ChangeScene(SceneType::Game);} });
	selectInfoArray.push_back({ "Stage5 o",[]() {StageManager::GetInstance().SetStageID(5);SceneManager::GetInstance().ChangeScene(SceneType::Game);} });
	selectInfoArray.push_back({ "DebugSelect o",[]() {SceneManager::GetInstance().ChangeScene(SceneType::DebugSceneSelect);} });
	selectInfoArray.push_back({ "Return to Title",[]() {SceneManager::GetInstance().ChangeScene(SceneType::Title);} });
}

/*
 *	準備処理
 */
void StageSelectScene::Setup() {
	// // 当たり判定の複数生成回避
	// CollisionManager::GetInstance().Clear();
	// 
	// // ステージの初期化処理
	// StageManager::GetInstance().Initialize();
	// // セレクトステージの読み込み※仮でDebugStage
	// StageManager::GetInstance().LoadStage(111);
	// 
	// // プレイヤーの生成位置の取得
	// VECTOR playerPos = StageManager::GetInstance().GetPlayerSpawnPosition();
	// // カメラ生成
	// CameraManager::GetInstance().CreateCamera();
	// // プレイヤー生成
	// PlayerManager::GetInstance().CreatePlayer(playerPos);
	// 
	// // ギミックの更新を一度だけ呼ぶ
	// GimmickObjectManager::GetInstance().Update();
	// 
	// 
	// // ステージの当たり判定を生成
	// StageCollisionGenerator generator;
	// generator.GenerateFromUnity("src/Data/DebugStage.json", CollisionManager::GetInstance());
	currentScene = 0;
}

/*
 *	更新処理
 */
void StageSelectScene::Update() {
	//// カメラの更新
	//CameraManager::GetInstance().GetCamera()->Update();
	//
	//// プレイヤーの更新
	//auto player = PlayerManager::GetInstance().GetPlayer();
	//player->Update();
	//player->GetHands()->Update();t
	//
	//// ギミックの更新
	//GimmickObjectManager::GetInstance().Update();
	//
	//
	//// 当たり判定の更新
	//CollisionManager::GetInstance().Update();
	//ColliderObjectManager::GetInstance().Update();
	// 
	// 入力アクションの更新
	
	action = InputSystemManager::GetInstance().GetInputState(ActionMap::StageSelect);
	int size = static_cast<int>(selectInfoArray.size());

	if (action.buttonDown[static_cast<int>(StageSelectAction::SelectMove_UP)]) {
		currentScene = std::max(currentScene - 1, 0);
	}
	if (action.buttonDown[static_cast<int>(StageSelectAction::SelectMove_DOWN)]) {
		currentScene = std::min(currentScene + 1, size - 1);
	}
	if (action.buttonDown[static_cast<int>(StageSelectAction::Click)]) {
		selectInfoArray[currentScene].SceneChangeFunc();
	}

	//if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_UP))
	//	currentScene = std::max(currentScene - 1, 0);
	//if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_DOWN))
	//	currentScene = std::min(currentScene + 1, size - 1);
	//if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN))
	//	selectInfoArray[currentScene].SceneChangeFunc();

}

void StageSelectScene::Render() {

	//// ステージの描画
	//StageManager::GetInstance().Render();
	//
	//// プレイヤーを取得
	//auto player = PlayerManager::GetInstance().GetPlayer();
	//// プレイヤーの描画
	//player->Render();
	//// プレイヤーの腕の描画
	//player->GetHands()->Render();
	//
	//// ギミックの描画 
	//GimmickObjectManager::GetInstance().Render();
	//
	//// 当たり判定の描画
	//CollisionManager::GetInstance().Render();
	//ColliderObjectManager::GetInstance().Render();
	int size = static_cast<int>(selectInfoArray.size());
	for (int i = 0;i < size;i++) {
		if (i == currentScene)
			DrawString(100, 100 + (20 * i), selectInfoArray[i].sceneName.c_str(), 0xffff00);
		else
			DrawString(100, 100 + (20 * i), selectInfoArray[i].sceneName.c_str(), 0x000000);
	}

}