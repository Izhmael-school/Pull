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
#include "Manager/Stage/GimmickObjectManager.h"
#include "Manager/PlayerManager.h"
#include "GameObject/Missile/Missile.h"
#include "Generator/StageCollisionGenerator.h"
#include "../../Manager/InputSystemManager.h"
#include <ImGui/imgui.h>
#include "Application.h"

EnemyDebugScene::EnemyDebugScene() 
	:effectManager(effectResourceManager)
	,effectResourceManager()
	, audioManager(audioResourceManager)
	,audioResourceManager()
	, enemyManager({ Application::GetInstance().GetEffectManager() ,Application::GetInstance().GetAudioManager() })
{ Start(); }

void EnemyDebugScene::Start()
{


}

void EnemyDebugScene::Update(){
	// プレイヤーアクションマップを有効化
	InputSystemManager::GetInstance().SetActionMapIsActive(ActionMap::PlayerAction, true);
	GameObjectManager::GetInstance().Update();
	ColliderObjectManager::GetInstance().Update();
	CollisionManager::GetInstance().Update();
	auto player = PlayerManager::GetInstance().GetPlayer();
	player->Update();
	player->GetHands()->Update();
	// カメラの更新
	CameraManager::GetInstance().GetCamera()->Update();

	// 仮
	//effectManager.Play("Earthquake", player->GetPosition(), 1.0f, VZero);
	GimmickObjectManager::GetInstance().Update();
	// 敵の更新
	enemyManager.Update();
	effectManager.Update();
	audioManager.Update();
}

void EnemyDebugScene::Render(){
	VECTOR p = PlayerManager::GetInstance().GetPlayer()->GetPosition();

	int i = StageManager::GetInstance().GetCurrentStage()->GetStageModelHandle();
	VECTOR sp = MV1GetPosition(i);

	ImGui::Begin("PlayerPos");
	ImGui::Text("%f, %f, %f", p.x, p.y, p.z);
	ImGui::Text("%f, %f, %f", sp.x, sp.y, sp.z);
	ImGui::End();
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
	CollisionManager::GetInstance().Render();
#endif
	auto player = PlayerManager::GetInstance().GetPlayer();
	player->Render();
	player->GetHands()->Render();
	//DrawSphere3D(player->GetPosition(), 100, 16, 0xff0000, 0xff0000, TRUE);
	GimmickObjectManager::GetInstance().Render();
	// 描画
	StageManager::GetInstance().Render();
	effectManager.Render();
	enemyManager.Render();
}

void EnemyDebugScene::Setup(){
	StageManager::GetInstance().Initialize();
	StageManager::GetInstance().LoadStage(111);
	StageManager::GetInstance().TransitionStage(0);
	enemyManager.SpawnStageFramePoint(111,StageManager::GetInstance());
	CameraManager::GetInstance().CreateCamera();
	audioManager.Play("test", 100.0f, true);
	GimmickObjectManager::GetInstance().Update();
	//generator.GenerateFromUnity("src/Data/Stage_4.json", CollisionManager::GetInstance());
	generator.GenerateFromUnity("src/Data/DebugStage.json", CollisionManager::GetInstance());

	VECTOR pos = StageManager::GetInstance().GetPlayerSpawnPosition();
	PlayerManager::GetInstance().CreatePlayer(pos);
	PlayerManager::GetInstance().GetPlayer()->GetTransform()->SetPosition(pos);
}

void EnemyDebugScene::Cleanup(){
	enemyManager.UnuseAllEnemy();
}
