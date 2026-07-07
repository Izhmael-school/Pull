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
#include "../../Definition/CommonModule/ActionMapData.h"

#include <DxLib.h>
#include <format>
 /*
  *	コンストラクタ
  */
MainGameScene::MainGameScene() {
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

	// ステージの初期化処理
	StageManager::GetInstance().Initialize();
	// ステージのロード
	int stageID = StageManager::GetInstance().GetStageID();
	StageManager::GetInstance().LoadStage(stageID);
	// プレイヤーの生成位置を取得
	VECTOR playerPos = StageManager::GetInstance().GetPlayerSpawnPosition();

	// カメラ生成
	CameraManager::GetInstance().CreateCamera();
	// プレイヤー生成
	PlayerManager::GetInstance().CreatePlayer(playerPos);

	// ギミックの更新s
	GimmickObjectManager::GetInstance().Update();

	// ステージの当たり判定を作成
	StageCollisionGenerator generator;
	std::string stageFile = std::format("src/Data/Stage_{}.json", stageID);
	generator.GenerateFromUnity(stageFile, CollisionManager::GetInstance());
	// プレイヤーアクションマップを有効化
	InputSystemManager::GetInstance().SetActionMapIsActive(ActionMap::PlayerAction, true);
}

/*
 *	更新処理
 */
void MainGameScene::Update() {

	// カメラの更新
	CameraManager::GetInstance().GetCamera()->Update();

	// プレイヤーの更新
	auto player = PlayerManager::GetInstance().GetPlayer();
	player->Update();
	player->GetHands()->Update();


	// ギミックの更新
	GimmickObjectManager::GetInstance().Update();

	// 当たり判定の更新
	CollisionManager::GetInstance().Update();
	// クリア判定
	if (StageManager::GetInstance().IsStageClear()) {
		// シーンを切り替える
		SceneManager::GetInstance().ChangeScene(SceneType::StageSelect);
		return;
	}
	ColliderObjectManager::GetInstance().Update();
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

#endif
	// ステージの描画処理
	StageManager::GetInstance().Render();

	// プレイヤーの描画処理
	auto player = PlayerManager::GetInstance().GetPlayer();
	player->Render();
	player->GetHands()->Render();

	// ギミックの描画処理
	GimmickObjectManager::GetInstance().Render();

	// 当たり判定の描画処理
	CollisionManager::GetInstance().Render();
	ColliderObjectManager::GetInstance().Render();

}

/*
 * シーンの片付け処理
 */
void MainGameScene::Cleanup() {
	// ギミックの片付け処理
	GimmickObjectManager::GetInstance().Clear();
	StageManager::GetInstance().RequestStageClear(false);
}
