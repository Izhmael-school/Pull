/*
 * @file EnemyDebugScene.cpp
 * @author Sekino
 */
#include "EnemyDebugScene.h"
#include <DxLib.h>
#include "Definition/Const/ColorConst.h"
#include "Definition/Const/VECTORConst.h"
#include "Manager/InputManager.h"
#include "Manager/Stage/StageManager.h"
#include "Manager/EnemyManager.h"
#include "Manager/CameraManager.h"
#include "Manager/GameObjectManager.h"
#include "GameObject/Camera/CameraObject.h"
#include "Component/Collider/Collider.h"
#include "Manager/CollisionManager.h"
#include "Manager/ColliderObjectManager.h"
#include "Manager/PlayerManager.h"
#include "GameObject/Missile/Missile.h"

EnemyDebugScene::EnemyDebugScene() { Start(); }

void EnemyDebugScene::Start()
{
	PlayerManager::GetInstance().CreatePlayer();
	PlayerManager::GetInstance().GetPlayer()->GetTransform()->AddPosition(VScale(VUp,200));

	// カメラ生成
	CameraManager::GetInstance().CreateCamera();
	
	// ステージの初期化処理
	StageManager::GetInstance().Initialize();
	// モデルハンドルを複製してStageの実体にハンドルを渡す
	StageManager::GetInstance().LoadStage(4);
		
}

void EnemyDebugScene::Update(){
	GameObjectManager::GetInstance().Update();
	ColliderObjectManager::GetInstance().Update();
	CollisionManager::GetInstance().Update();
	PlayerManager::GetInstance().GetPlayer()->Update();
	// カメラの更新
	CameraManager::GetInstance().GetCamera()->Update();
	// 敵の更新
	EnemyManager::GetInstance().Update();
}

void EnemyDebugScene::Render(){

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
	ColliderObjectManager::GetInstance().Render();
	GameObjectManager::GetInstance().Render();
#endif
	auto player = PlayerManager::GetInstance().GetPlayer();
	player->Render();
	DrawSphere3D(player->GetPosition(), 100, 16, 0xff0000, 0xff0000, TRUE);

	// 描画
	StageManager::GetInstance().Render();

	EnemyManager::GetInstance().Render();
}

void EnemyDebugScene::Setup(){
	EnemyManager::GetInstance().UseEnemy(Shooter,VGet(0, 400, 0));
}

void EnemyDebugScene::Cleanup(){
	EnemyManager::GetInstance().UnuseAllEnemy();
}
