/*
 * @file CollisionDebugScene.cpp
 * @author Sekino
 */
#include "CollisionDebugScene.h"
#include <DxLib.h>
#include "Definition/Const/ColorConst.h"
#include "Definition/Const/VECTORConst.h"
#include "Manager/InputManager.h"
#include "Manager/Stage/StageManager.h"
#include "Generator/StageCollisionGenerator.h"
#include "Manager/CameraManager.h"
#include "GameObject/Camera/CameraObject.h"
#include "Component/Collider/Collider.h"
#include "Manager/CollisionManager.h"


CollisionDebugScene::CollisionDebugScene() { Start(); }

void CollisionDebugScene::Start() {
	// カメラ生成
	CameraManager::GetInstance().CreateCamera();

	StageManager::GetInstance().Initialize();
	StageManager::GetInstance().LoadStage(4);

	StageCollisionGenerator generator;
	generator.GenerateFromUnity("src/Data/Stage_4.json", CollisionManager::GetInstance());

	auto obj = new GameObject();
	auto ray = new RayCollider(obj, VZero, VGet(1, 0, 0), 500.0f, 900.0f, 90.0f, 0.0f);
	CollisionManager::GetInstance().Register(ray);

	auto debugObj = new GameObject();
	debugObj->GetTransform()->SetPosition(VGet(0, 400, 0));
	auto capsule = new CapsuleCollider(debugObj,
		VGet(0, 361, 0),   // start
		VGet(0, 500, 0),   // end
		30.0f,
		VGet(0, 0, 100));
	CollisionManager::GetInstance().Register(capsule);
}

void CollisionDebugScene::Update() {
	CameraManager::GetInstance().GetCamera()->Update();

	CollisionManager::GetInstance().Update();
}
void CollisionDebugScene::Render() {
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
	//StageManager::GetInstance().Render();

	CollisionManager::GetInstance().Render();
}