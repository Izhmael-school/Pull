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
#include "Generator/CoinGenerator.h"
#include "../../UI/Scene/MainGameScreen.h"
#include "Manager/FadeManager.h"
#include "../../Definition/Enum/CameraModeEnum.h"

#include <DxLib.h>
#include <format>
#include <ImGui/imgui.h>
#include "Game/GameData.h"
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
	StageStartSetup();
	useEventCamera = false;
}

/*
 *	更新処理
 */
void MainGameScene::Update() {
	
	// イベントカメラ
	if (FadeManager::GetInstance().IsFadeInEnd() && !useEventCamera) {
		CameraManager::GetInstance().GetCamera()->ChangeCameraMode(CameraMode::Event);
		InputSystemManager::GetInstance().SetActionMapIsActive(ActionMap::PlayerAction, false);
		useEventCamera = true;
	}

	m_UIManager.Update(0.0f,UIInput());
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



	// 当たり判定の更新
	CollisionManager::GetInstance().Update();

	if (player->IsDead()) {
		Reset();
	}

	// クリア判定
	if (StageManager::GetInstance().IsStageClear()) {
		// プレイヤーの入力を行わないようにする
		InputSystemManager::GetInstance().SetActionMapIsActive(ActionMap::PlayerAction, false);
		// シーンを切り替える
		SceneManager::GetInstance().ChangeScene(SceneType::StageSelect);
		return;
	}
	ColliderObjectManager::GetInstance().Update();
	Application::GetInstance().GetEffectManager().Update();
}

void MainGameScene::Render() {

#if _DEBUG 線

	// オブジェクトの位置関係がわかるように地面にラインを描画する
	{
		VECTOR pos1, pos2;

		// XZ平面 100.0f毎に1本ライン引き
		{
			pos1 = VGet(-5000.0f, 0, -5000.0f);
			pos2 = VGet(-5000.0f, 0, 5000.0f);

			for (int i = 0; i < 100; i++) {
				DrawLine3D(pos1, pos2, gray);

				pos1.x += 100.0f;
				pos2.x += 100.0f;
			}

			pos1 = VGet(-5000.0f, 0, -5000.0f);
			pos2 = VGet(5000.0f, 0, -5000.0f);
			for (int i = 0; i < 100; i++) {
				DrawLine3D(pos1, pos2, GetColor(100, 100, 100));

				pos1.z += 100.0f;
				pos2.z += 100.0f;
			}
		}

		// X軸
		{
			pos1 = VZero;
			pos2 = VScale(VRight, 5000);	// VRight * 5000 をしてる
			DrawLine3D(pos1, pos2, red);
		}

		// Y軸
		{
			pos1 = VZero;
			pos2 = VScale(VUp, 5000);	    // VUp * 5000 をしてる
			DrawLine3D(pos1, pos2, green);
		}

		// Z軸
		{
			pos1 = VZero;
			pos2 = VScale(VForward, 5000);	// VRight * 5000 をしてる
			DrawLine3D(pos1, pos2, blue);
		}
	}
	// 当たり判定の描画処理
#if _DEBUG
	CollisionManager::GetInstance().Render();
#endif
	ColliderObjectManager::GetInstance().Render();

#endif
	shadowMap.Render();
	shadowMap.Apply();

	// ステージの描画処理
	StageManager::GetInstance().Render();

	// プレイヤーの描画処理
	auto player = PlayerManager::GetInstance().GetPlayer();
	player->Render();
	// プレイヤーの腕の取得
	auto playerHand = player->GetHands();
	// プレイヤーの腕の描画
	playerHand->Render();

	// ギミックの描画処理
	GimmickObjectManager::GetInstance().Render();
	
	// 敵の描画処理
	enemyManager.Render();

	// GameObjectの描画
	GameObjectManager::GetInstance().Render();

	shadowMap.Disable();

	// スカイドームを描画
	MV1DrawModel(SkyModel);
	MV1SetScale(SkyModel, VGet(10000, 10000, 10000));

	Application::GetInstance().GetEffectManager().Render();
	
	// 最前面UIScreenを取得
	auto screen = static_cast<MainGameScreen*>(m_UIManager.GetTopScreen());
	// レバーを掴んだ時のUI表示
	screen->SetLeverUIVisible(playerHand->IsLeverCatch());
	// 敵を掴んだ時のUI
	screen->SetMisileUIVisible(playerHand->IsEnemyCatch());
	// UI表示
    m_UIManager.Draw();

#if _DEBUG
	ImGui::Begin("Score & Coin");
	ImGui::Text("Score:%d", GameData::GetScore());
	ImGui::Text("Coin:%d", GameData::GetCoin());
	ImGui::End();
#endif
}

/*
 * シーンの片付け処理
 */
void MainGameScene::Cleanup() {
	// ギミックの片付け処理
	GimmickObjectManager::GetInstance().Clear();
	StageManager::GetInstance().RequestStageClear(false);
	StageManager::GetInstance().Execute();
	GameObjectManager::GetInstance().Cleanup();
	// 音
	AudioManager* audio = &Application::GetInstance().GetAudioManager();
	audio->Clean();
	// 使用中の敵全てを未使用化
	enemyManager.UnuseAllEnemy();
	Application::GetInstance().GetEffectManager().Clean();
	m_UIManager.PopScreen();
}

void MainGameScene::Reset() {
	// プレイヤーの入力を行わないようにする
	InputSystemManager::GetInstance().SetActionMapIsActive(ActionMap::PlayerAction,false);
	// フェードに入る
	FadeManager::GetInstance().FadeStart(FadeIn, FadeType::FadeNormal, 1.0f);
	// シーンの片付けを呼ぶ
	Cleanup();
	// 自身のセットアップ処理を呼ぶ
	this->StageStartSetup();
}

void MainGameScene::StageStartSetup() {
	// 複数生成防止処理
	CollisionManager::GetInstance().Clear();


	// ステージの初期化処理
	StageManager& stageManager = StageManager::GetInstance();
	stageManager.Initialize();
	// ステージのロード
	int stageID = stageManager.GetStageID();
	stageManager.LoadStage(stageID);
	stageManager.TransitionStage(stageID);

	// 敵のスポーン位置を取得
	enemyManager.SpawnStageFramePoint(stageID, stageManager);
	// プレイヤーの生成位置を取得
	playerPos = stageManager.GetPlayerSpawnPosition();

	// カメラ生成
	CameraManager::GetInstance().CreateCamera();
	// カメラの取得
	auto camera = CameraManager::GetInstance().GetCamera();
	camera->ChangeCameraMode(CameraMode::Player);
	// プレイヤー生成
	PlayerManager::GetInstance().CreatePlayer(playerPos);

	// ギミックの更新
	GimmickObjectManager::GetInstance().Update();

	// コインの生成
	CoinGenerator::GenerateCoin(stageID, stageManager.GetCurrentStage()->GetStageModelHandle());

	shadowMap.SetUp();

	// ステージの当たり判定を作成
	StageCollisionGenerator generator;
#if _DEBUG
	std::string stageFile = std::format("src/Data/Stage_{}.json", stageID);
#else
	std::string stageFile = std::format("res/ExternalFile/Stage/Collision/Stage_{}_Collision.msgpack", stageID);
#endif
	generator.GenerateFromUnity(stageFile, CollisionManager::GetInstance());
	// プレイヤーアクションマップを有効化
	InputSystemManager::GetInstance().SetActionMapIsActive(ActionMap::PlayerAction, true);

	// スカイドームのモデルを取得
	SkyModel = MV1LoadModel("res/Model/Stage/SkyBox.mv1");

	// BGMを再生
	AudioManager* pAudioManager = &Application::GetInstance().GetAudioManager();
	pAudioManager->Play("Stage1BGM", 100.0f, true);

	// メインゲーム用UIの準備
	m_UIManager.PushScreen(std::make_unique<MainGameScreen>());
}
