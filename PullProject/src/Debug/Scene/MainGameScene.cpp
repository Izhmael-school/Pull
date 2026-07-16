/*
 *	@file	MainGameScene.cpp
 *  @author oorui
 */

#include "MainGameScene.h"
#include "Definition/Const/ColorConst.h"
#include "Definition/Const/VECTORConst.h"
#include "Manager/InputManager.h"
#include "Manager/Stage/StageManager.h"
#include "Manager/CameraManager.h"
#include "Manager/EnemyManager.h"
#include "GameObject/Camera/CameraObject.h"
#include "Component/Collider/Collider.h"
#include "Manager/CollisionManager.h"
#include "Manager/Playermanager.h"
#include "../../Manager/Stage/GimmickObjectManager.h"
#include "Manager/ColliderObjectManager.h"
#include "Generator/StageCollisionGenerator.h"
#include "Manager/SceneManager.h"
#include "Manager/InputSystemManager.h"
#include "Manager/GameObjectManager.h"
#include "../../Definition/CommonModule/ActionMapData.h"
#include "Application.h"

#include <DxLib.h>
#include <format>
 /*
  *	コンストラクタ
  */
MainGameScene::MainGameScene()
	: enemyManager({ Application::GetInstance().GetEffectManager() ,Application::GetInstance().GetAudioManager()}) {
	Start();
}

void MainGameScene::Start() {

}

/*
 *	準備処理
 */
void MainGameScene::Setup() {
	// 複数生成防止処理
	CollisionManager::GetInstance().Clear();

	//	シャドウマップのSetUp
	shadowMap.SetUp();

	// ステージの初期化処理
	StageManager::GetInstance().Initialize();
	// ステージのロード
	int stageID = StageManager::GetInstance().GetStageID();
	StageManager::GetInstance().LoadStage(stageID);

	// 敵のスポーン位置を取得
	enemyManager.SpawnStageFramePoint(stageID, StageManager::GetInstance());
	// プレイヤーの生成位置を取得
	VECTOR playerPos = StageManager::GetInstance().GetPlayerSpawnPosition();

	// カメラ生成
	CameraManager::GetInstance().CreateCamera();
	// カメラの取得
	auto camera = CameraManager::GetInstance().GetCamera();
	camera->ChangeCameraMode(1);
	// プレイヤー生成
	PlayerManager::GetInstance().CreatePlayer(playerPos);

	// ギミックの更新
	GimmickObjectManager::GetInstance().Update();

	// ステージの当たり判定を作成
	StageCollisionGenerator generator;
	std::string stageFile = std::format("src/Data/Stage_{}.json", stageID);
	generator.GenerateFromUnity(stageFile, CollisionManager::GetInstance());
	// プレイヤーアクションマップを有効化
	InputSystemManager::GetInstance().SetActionMapIsActive(ActionMap::PlayerAction, true);

	// スカイドームのモデルを取得
	SkyModel = MV1LoadModel("res/Model/Stage/SkyBox.mv1");

	// BGMを再生
	AudioManager* pAudioManager = &Application::GetInstance().GetAudioManager();
	pAudioManager->Play("Stage1BGM", 100.0f, true);
}

/*
 *	更新処理
 */
void MainGameScene::Update() {

	// カメラの更新
	CameraManager::GetInstance().GetCamera()->Update();
	// カメラ座標を取得
	VECTOR CameraPos = CameraManager::GetInstance().GetCamera()->GetPosition();
	// スカイドームをカメラ位置へ移動
	MV1SetPosition(SkyModel, CameraPos);
	// プレイヤーの更新
	auto player = PlayerManager::GetInstance().GetPlayer();
	player->Update();
	// プレイヤーの腕の更新
	player->GetHands()->Update();

	shadowMap.Update();

	// ギミックの更新
	GimmickObjectManager::GetInstance().Update();
	// 敵の更新
	enemyManager.Update();

	// GameObjectの更新
	GameObjectManager::GetInstance().Update();

	// 当たり判定の更新
	CollisionManager::GetInstance().Update();
	// クリア判定
	if (StageManager::GetInstance().IsStageClear()) {
		// シーンを切り替える
		SceneManager::GetInstance().ChangeScene(SceneType::StageSelect);
		return;
	}
	ColliderObjectManager::GetInstance().Update();
	Application::GetInstance().GetEffectManager().Update();
}

void MainGameScene::Render() {
	
	shadowMap.Render();

	//SetDrawScreen(DX_SCREEN_BACK);
	//SetUseZBuffer3D(TRUE);
	//SetWriteZBuffer3D(TRUE);

	MV1SetScale(SkyModel, VGet(10000, 10000, 10000));
	MV1DrawModel(SkyModel);

	shadowMap.Apply();

	// ステージの描画処理
	StageManager::GetInstance().Render();

	// プレイヤーの描画処理
	auto player = PlayerManager::GetInstance().GetPlayer();
	if (player) {
		player->Render();
		if (player->GetHands()) {
			player->GetHands()->Render();
		}
	}

	// ギミックの描画処理
	GimmickObjectManager::GetInstance().Render();

	// 敵の描画処理
	enemyManager.Render();

	GameObjectManager::GetInstance().Render();

	shadowMap.Disable();

	Application::GetInstance().GetEffectManager().Render();

#if _DEBUG // デバッグ線の描画
	{
		VECTOR pos1, pos2;
		// XZ平面 100.0f毎に1本ライン引き
		pos1 = VGet(-5000.0f, 0, -5000.0f);
		pos2 = VGet(-5000.0f, 0, 5000.0f);
		for (int i = 0; i < 100; i++) {
			DrawLine3D(pos1, pos2, gray);
			pos1.x += 100.0f; pos2.x += 100.0f;
		}
		pos1 = VGet(-5000.0f, 0, -5000.0f);
		pos2 = VGet(5000.0f, 0, -5000.0f);
		for (int i = 0; i < 100; i++) {
			DrawLine3D(pos1, pos2, GetColor(100, 100, 100));
			pos1.z += 100.0f; pos2.z += 100.0f;
		}
		// X, Y, Z軸
		DrawLine3D(VZero, VScale(VRight, 5000), red);
		DrawLine3D(VZero, VScale(VUp, 5000), green);
		DrawLine3D(VZero, VScale(VForward, 5000), blue);
	}
	CollisionManager::GetInstance().Render();
	ColliderObjectManager::GetInstance().Render();
#endif
}

/*
 * シーンの片付け処理
 */
void MainGameScene::Cleanup() {
	// ギミックの片付け処理
	GimmickObjectManager::GetInstance().Clear();
	StageManager::GetInstance().RequestStageClear(false);
	// 音
	AudioManager* audio = &Application::GetInstance().GetAudioManager();
	audio->Clean();
	// 使用中の敵全てを未使用化
	enemyManager.UnuseAllEnemy();
	Application::GetInstance().GetEffectManager().Clean();
}
