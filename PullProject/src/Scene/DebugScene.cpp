/*
 * @file DebugScene.cpp
 * @author Sekino
 */
#include "DebugScene.h"
#include <DxLib.h>
#include "../Definition/Const/ColorConst.h"
#include "../Definition/Const/VECTORConst.h"
#include "../Manager/InputManager.h"
#include "../Manager/Stage/StageManager.h"

DebugScene::DebugScene() { Start(); }

void DebugScene::Start()
{
	debugCamera = std::make_unique<DebugCamera>();
	
	// ステージの初期化処理
	StageManager::GetInstance().Initialize();
	// モデルの仮ロード
	int stageHandle = MV1LoadModel("res/Model/Stage/Stage4/Stage_4.mv1");
	// 仮モデルのみのリストを作成
	std::vector<int> stageHandleList{stageHandle};
	// モデルハンドルを複製してStageの実体にハンドルを渡す
	StageManager::GetInstance().LoadStage(stageHandleList);
	
}

void DebugScene::Update()
{
	debugCamera->Update();
}

void DebugScene::Render(){

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
}