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
#include "Manager/CameraManager.h"
#include "Manager/Playermanager.h"
#include "../../Manager/Stage/GimmickObjectManager.h"
#include "Manager/ColliderObjectManager.h"
#include "Manager/GameObjectManager.h"
#include "Manager/AudioManager.h"
#include "Manager/EnemyManager.h"
#include "Application.h"

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
#if _DEBUG
	selectInfoArray.push_back({ "Stage1 o",[]() {StageManager::GetInstance().SetStageID(1);SceneManager::GetInstance().ChangeScene(SceneType::Game);} });
	selectInfoArray.push_back({ "Stage2 o",[]() {StageManager::GetInstance().SetStageID(2);SceneManager::GetInstance().ChangeScene(SceneType::Game);} });
	selectInfoArray.push_back({ "Stage3 o",[]() {StageManager::GetInstance().SetStageID(3);SceneManager::GetInstance().ChangeScene(SceneType::Game);} });
	selectInfoArray.push_back({ "Stage4 o",[]() {StageManager::GetInstance().SetStageID(4);SceneManager::GetInstance().ChangeScene(SceneType::Game);} });
	selectInfoArray.push_back({ "Stage5 o",[]() {StageManager::GetInstance().SetStageID(5);SceneManager::GetInstance().ChangeScene(SceneType::Game);} });
	selectInfoArray.push_back({ "DebugSelect o",[]() {SceneManager::GetInstance().ChangeScene(SceneType::DebugSceneSelect);} });
	selectInfoArray.push_back({ "Return to Title",[]() {SceneManager::GetInstance().ChangeScene(SceneType::Title);} });
#endif
}

/*
 *	準備処理
 */
void StageSelectScene::Setup() {
	 // 当たり判定の複数生成回避
	 CollisionManager::GetInstance().Clear();
	 
	 auto camera = CameraManager::GetInstance().GetCamera();
	 camera->ChangeCameraMode(CameraMode::Player);

	 // ステージの初期化処理
	 StageManager& stageManager = StageManager::GetInstance();
	 stageManager.Initialize();
	 // セレクトステージの読み込み※仮でDebugStage
	 stageManager.LoadStage(113);
	 stageManager.TransitionStage(113);

	 // プレイヤーの生成位置の取得
	 VECTOR playerPos = StageManager::GetInstance().GetPlayerSpawnPosition();
	 // カメラ生成
	 CameraManager::GetInstance().CreateCamera();
	 // プレイヤー生成
	 PlayerManager::GetInstance().CreatePlayer(playerPos);
	 InputSystemManager::GetInstance().SetActionMapIsActive(ActionMap::PlayerAction, true);
	 // ギミックの更新を一度だけ呼ぶ
	 GimmickObjectManager::GetInstance().Update();
	 
	 // GameObjectの更新
	 GameObjectManager::GetInstance().Update();
	 
	 // ステージの当たり判定を生成
	 StageCollisionGenerator generator;
	 generator.GenerateFromUnity("src/Data/Stage_113.json", CollisionManager::GetInstance());
	//currentScene = 0;
	 // スカイドームのモデルを取得
	 SkyModel = MV1LoadModel("res/Model/Stage/SkyBox.mv1");
}

/*
 *	更新処理
 */
void StageSelectScene::Update() {
	// カメラの更新
	CameraManager::GetInstance().GetCamera()->Update();
	// カメラ座標を取得
	VECTOR CameraPos = CameraManager::GetInstance().GetCamera()->GetPosition();
	// スカイドームをカメラ位置へ移動
	MV1SetPosition(SkyModel, CameraPos);
	// プレイヤーの更新
	auto player = PlayerManager::GetInstance().GetPlayer();
	player->Update();
	player->GetHands()->Update();
	
	// ギミックの更新
	GimmickObjectManager::GetInstance().Update();

	// 当たり判定の更新
	CollisionManager::GetInstance().Update();
	ColliderObjectManager::GetInstance().Update();
	//InputSystemManager::GetInstance().GetInputState(ActionMap::PlayerAction);
	/*int size = static_cast<int>(selectInfoArray.size());

	if (action.buttonDown[static_cast<int>(StageSelectAction::SelectMove_UP)]) {
		currentScene = std::max(currentScene - 1, 0);
	}
	if (action.buttonDown[static_cast<int>(StageSelectAction::SelectMove_DOWN)]) {
		currentScene = std::min(currentScene + 1, size - 1);
	}
	if (action.buttonDown[static_cast<int>(StageSelectAction::Click)]) {
		selectInfoArray[currentScene].SceneChangeFunc();
	}*/

	//if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_UP))
	//	currentScene = std::max(currentScene - 1, 0);
	//if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_DOWN))
	//	currentScene = std::min(currentScene + 1, size - 1);
	//if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN))
	//	selectInfoArray[currentScene].SceneChangeFunc();

}

void StageSelectScene::Render() {

	// ステージの描画
	StageManager::GetInstance().Render();
	
	// プレイヤーを取得
	auto player = PlayerManager::GetInstance().GetPlayer();
	// プレイヤーの描画
	player->Render();
	// プレイヤーの腕の描画
	player->GetHands()->Render();
	// ギミックの描画 
	GimmickObjectManager::GetInstance().Render();

	// スカイドームを描画
	MV1DrawModel(SkyModel);
	MV1SetScale(SkyModel, VGet(10000, 10000, 10000));
#if _DEBUG
	// 当たり判定の描画
	CollisionManager::GetInstance().Render();
	ColliderObjectManager::GetInstance().Render();
	
	int size = static_cast<int>(selectInfoArray.size());
	for (int i = 0;i < size;i++) {
		if (i == currentScene)
			DrawString(100, 100 + (20 * i), selectInfoArray[i].sceneName.c_str(), 0xffff00);
		else
			DrawString(100, 100 + (20 * i), selectInfoArray[i].sceneName.c_str(), 0x000000);
	}
#endif
	

}

void StageSelectScene::Cleanup() {
	// ギミックの片付け処理
	GimmickObjectManager::GetInstance().Clear();
	StageManager::GetInstance().RequestStageClear(false);
	StageManager::GetInstance().Execute();
	GameObjectManager::GetInstance().Cleanup();
	// 音
	AudioManager* audio = &Application::GetInstance().GetAudioManager();
	audio->Clean();
}
