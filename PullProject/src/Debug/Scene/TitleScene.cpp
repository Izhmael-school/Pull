/*
 *	@file TitleScene.cpp
 *  @author oorui
 */

#include "TitleScene.h"
#include "Manager/SceneManager.h"
#include "Definition/Enum/SceneType.h"
#include "Manager/InputManager.h"
#include "Definition/Const/ColorConst.h"
#include "Definition/Const/VECTORConst.h"
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
#include "Manager/InputSystemManager.h"
#include "Manager/GameObjectManager.h"
#include "../../Definition/CommonModule/ActionMapData.h"
#include "Application.h"

#include <DxLib.h>
#include <format>
#include <algorithm>
#include <math.h>


 /*
  *	コンストラクタ
  */
TitleScene::TitleScene()
	:effectManager(effectResourceManager)
	, effectResourceManager()
	, audioManager(audioResourceManager)
	, audioResourceManager()
	, enemyManager({ effectManager ,audioManager }) {
	Start();
}

/*
 *	デストラクタ
 */
TitleScene::~TitleScene() {
}

/*
 *	開始処理
 */
void TitleScene::Start() {
	effectResourceManager.LoadEffectFromExternalFile();
}

/*
 *	準備処理
 */
void TitleScene::Setup() {
	// 複数生成防止処理
	CollisionManager::GetInstance().Clear();

	// ステージの初期化処理
	StageManager::GetInstance().Initialize();
	StageManager::GetInstance().SetStageID(112);
	// ステージIDの取得
	int stageID = StageManager::GetInstance().GetStageID();
	// ステージのロード
	StageManager::GetInstance().LoadStage(stageID);

	// 敵のスポーン位置を取得
	enemyManager.SpawnStageFramePoint(stageID, StageManager::GetInstance());
	// プレイヤーの生成位置を取得
	VECTOR playerPos = StageManager::GetInstance().GetPlayerSpawnPosition();

	// カメラの初期位置を取得
	VECTOR cameraPos = StageManager::GetInstance().GetCameraSpawnPosition();
	// カメラ生成
	CameraManager::GetInstance().CreateCamera(cameraPos);
	// カメラの取得
	auto camera = CameraManager::GetInstance().GetCamera();
	
	camera->ChangeCameraMode(0);

	
	// ステージの当たり判定を作成
	StageCollisionGenerator generator;
	std::string stageFile = std::format("src/Data/Stage_{}.json", stageID);
	generator.GenerateFromUnity(stageFile, CollisionManager::GetInstance());

	// スカイドームのモデルを取得
	SkyModel = MV1LoadModel("res/Model/Stage/SkyBox.mv1");

	// BGMを再生
	AudioManager* pAudioManager = &Application::GetInstance().GetAudioManager();
	pAudioManager->Play("Stage1BGM", 100.0f, true);
}

/*
 *	更新処理
 */
void TitleScene::Update() {
	// カメラの更新
	CameraManager::GetInstance().GetCamera()->Update();
	// カメラ座標を取得
	VECTOR CameraPos = CameraManager::GetInstance().GetCamera()->GetPosition();
	// スカイドームをカメラ位置へ移動
	MV1SetPosition(SkyModel, CameraPos);
	// 敵の更新
	enemyManager.Update();
	// 当たり判定の更新
	CollisionManager::GetInstance().Update();
	ColliderObjectManager::GetInstance().Update();

	// エフェクトの更新
	EffectManager* effect = &Application::GetInstance().GetEffectManager();
	effect->Update();

	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN)) {
		SceneManager::GetInstance().ChangeScene(SceneType::StageSelect);
	}
}

/*
 *	描画処理
 */
void TitleScene::Render() {
	DrawString(940, 600, "Start", 0xffff00);
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
	// ステージの描画処理
	StageManager::GetInstance().Render();

	// ギミックの描画処理
	GimmickObjectManager::GetInstance().Render();

	// 敵の描画処理
	enemyManager.Render();

	// GameObjectの描画
	GameObjectManager::GetInstance().Render();

	// スカイドームを描画
	MV1DrawModel(SkyModel);
	MV1SetScale(SkyModel, VGet(10000, 10000, 10000));

	// エフェクトの描画
	EffectManager* effect = &Application::GetInstance().GetEffectManager();
	effect->Render();
}

/*
 *	シーンの片付け処理
 */
void TitleScene::Cleanup() {

	// ギミックの片付け処理
	GimmickObjectManager::GetInstance().Clear();
	StageManager::GetInstance().RequestStageClear(false);
	// 音
	AudioManager* audio = &Application::GetInstance().GetAudioManager();
	audio->Clean();
	// 使用中の敵全てを未使用化
	enemyManager.UnuseAllEnemy();
	// エフェクト未使用化
	EffectManager* effect = &Application::GetInstance().GetEffectManager();
	effect->Clean();
}