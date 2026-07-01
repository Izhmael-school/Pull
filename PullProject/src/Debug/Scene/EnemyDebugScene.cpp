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
#include "Manager/CameraManager.h"
#include "Manager/GameObjectManager.h"
#include "GameObject/Camera/CameraObject.h"
#include "Component/Collider/Collider.h"
#include "Manager/CollisionManager.h"
#include "Manager/ColliderObjectManager.h"
#include "Manager/CollisionManager.h"
#include "Manager/PlayerManager.h"
#include "GameObject/Missile/Missile.h"
#include "Generator/StageCollisionGenerator.h"

EnemyDebugScene::EnemyDebugScene() 
	:effectManager(effectResourceManager)
	,effectResourceManager()
{ Start(); }

void EnemyDebugScene::Start()
{
	PlayerManager::GetInstance().CreatePlayer();
	auto p = PlayerManager::GetInstance().GetPlayer();
	p->GetTransform()->SetPosition(VGet(200,200,-600));


	generator.GenerateFromUnity("src/Data/Stage_4.json", CollisionManager::GetInstance());
}

void EnemyDebugScene::Update(){
	GameObjectManager::GetInstance().Update();
	ColliderObjectManager::GetInstance().Update();
	CollisionManager::GetInstance().Update();
	PlayerManager::GetInstance().GetPlayer()->Update();
	PlayerManager::GetInstance().GetPlayer()->GetHands()->Update();
	// カメラの更新
	CameraManager::GetInstance().GetCamera()->Update();
	// 敵の更新
	enemyManager.Update();
	effectManager.Update();
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
	player->GetHands()->Render();
	DrawSphere3D(player->GetPosition(), 100, 16, 0xff0000, 0xff0000, TRUE);

	// 描画
	StageManager::GetInstance().Render();
	effectManager.Render();
	enemyManager.Render();
	CollisionManager::GetInstance().Render();
}

void EnemyDebugScene::Setup(){
	//EnemyManager::GetInstance().UseEnemy(Walker,VGet(0, 400, 0));
	//EnemyManager::GetInstance().UseEnemy(Bomber,VGet(1000, 400, 0));
	//EnemyManager::GetInstance().UseEnemy(Shooter,VGet(0, 400, -1000));
	enemyManager.UseEnemy(Tail,VGet(1000, 400, -1000));
	CameraManager::GetInstance().CreateCamera();
	effectManager.pEffectResourceManager.LoadEffectFromExternalFile();
}

void EnemyDebugScene::Cleanup(){
	enemyManager.UnuseAllEnemy();
}
