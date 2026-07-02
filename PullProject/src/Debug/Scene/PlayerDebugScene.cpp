/*
 * @file PlayerDebugScene.cpp
 * @author Sekino
 */
#include "PlayerDebugScene.h"
#include <DxLib.h>
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


PlayerDebugScene::PlayerDebugScene() { Start(); }

void PlayerDebugScene::Start() {


}

void PlayerDebugScene::Setup() {

	CollisionManager::GetInstance().Clear();

	// ステージの初期化処理
	StageManager::GetInstance().Initialize();
	StageManager::GetInstance().LoadStage(4);
	VECTOR pos = StageManager::GetInstance().GetPlayerSpawnPosition();
	// カメラ生成
	CameraManager::GetInstance().CreateCamera();
	// プレイヤー生成
	PlayerManager::GetInstance().CreatePlayer(pos);
	// 敵生成
	// シングルトンをやめたためコメントアウト
	//EnemyManager::GetInstance().UseEnemy(Walker, VGet(0, 400, 0));

	// ===== ギミックの更新 ====
	GimmickObjectManager::GetInstance().Update();
}

void PlayerDebugScene::Update() {
	// カメラの更新
	CameraManager::GetInstance().GetCamera()->Update();
	// プレイヤーの更新
	auto player = PlayerManager::GetInstance().GetPlayer();
	player->Update();	
	player->GetHands()->Update();
	// 敵の更新
	// シングルトンをやめたためコメントアウト
	//EnemyManager::GetInstance().Update();
	// ギミックの更新
	GimmickObjectManager::GetInstance().Update();
	// ===== 当たり判定 =====
	CollisionManager::GetInstance().Update();
	ColliderObjectManager::GetInstance().Update();

}

void PlayerDebugScene::Render(){

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
	// 描画
	StageManager::GetInstance().Render();
	// シングルトンをやめたためコメントアウト
	// EnemyManager::GetInstance().Render();
	auto player = PlayerManager::GetInstance().GetPlayer();
	player->Render();
	player->GetHands()->Render();

	// ==== ギミックの描画 ====
	GimmickObjectManager::GetInstance().Render();

	CollisionManager::GetInstance().Render();
	ColliderObjectManager::GetInstance().Render();

	
}