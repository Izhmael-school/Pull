#include <ioStream>
#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include <random>
#include <memory>
#include "Manager/SceneManager.h"
#include "Manager/InputManager.h"
#include "Manager/TimeManager.h"
#include "Manager/CollisionManager.h"
#include "ImGui/ImGuiManager.h"

constexpr double FRAME_TIME = 1.0f / 60.0f;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

#pragma region // DxLibの初期化処理　触るべからず
	// タイトルの変更
	SetWindowText("Pull");
	// XInput対応ゲームパッド設定
	SetUseXInputFlag(true);
	// ウィンドウのサイズを変更する
	SetGraphMode(1920, 1080, 32, 60);
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
	SetFullSceneAntiAliasingMode(4, 2);
	// ゲームアイコン
	SetWindowIconID(001);

	// ログファイルを残さない
#if _DEBUG
	SetOutApplicationLogValidFlag(TRUE);
#else
	SetOutApplicationLogValidFlag(FALSE);
#endif

	// 起動時のウィンドウのモードの設定
#if _DEBUG
	ChangeWindowMode(TRUE);	// TRUE : ウィンドウモード FALSE : フルスクリーン
#else
	ChangeWindowMode(FALSE);	// TRUE : ウィンドウモード FALSE : フルスクリーン
#endif

	// 背景色の設定
#if _DEBUG
	SetBackgroundColor(196, 196, 196);
#else 
	SetBackgroundColor(0, 0, 0);
#endif

	// Dxlibの初期化
	if (DxLib_Init() == -1)
		return 0;

	if (Effekseer_Init(8000) == -1) {
		DxLib_End();
		return 0;
	}

	// 描画する先を設定する 裏画面に変更する
	SetDrawScreen(DX_SCREEN_BACK);

	// 図形描画のZバッファの有効化
	{
		// Zバッファを使用するかどうか
		SetUseZBuffer3D(TRUE);	// default : FALSE
		// Zバッファに書き込みを行うか
		SetWriteZBuffer3D(TRUE); // default : FALSE
	}
	int light = CreateDirLightHandle(VGet(-1.0f, -1.0f, 0.0f));
	//int light2 = CreateDirLightHandle(VGet(1.0f, -1.0f, 0.0f));
	// ライティング
	{
		// ライトの計算をどうするか
		SetUseLighting(TRUE); // default : TRUE
		// 標準ライトを使用するかどうか
		SetLightEnable(TRUE);	// default : TRUE
		SetLightDifColorHandle(light, GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
		SetLightSpcColorHandle(light, GetColorF(0.0f, 0.0f, 0.0f, 1.0f)); // スペキュラなし
		SetLightEnableHandle(light, TRUE);
		//SetLightDifColorHandle(light2, GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
		//SetLightSpcColorHandle(light2, GetColorF(0.0f, 0.0f, 0.0f, 1.0f)); // スペキュラなし
		//SetLightEnableHandle(light2, TRUE);
		// グローバル環境光の設定
		SetGlobalAmbientLight(GetColorF(0.5f, 0.5f, 0.5f, 0.5f));
		//// 反射光の設定  Diffuse
		//SetLightDifColor(GetColorF(1, 0, 0, 0));
		//// 鏡面反射光の設定　Specular
		//SetLightSpcColor(GetColorF(1, 0, 0.25f, 1));
		//// 環境光の設定　Ambient
		//SetLightAmbColor(GetColorF(1, 1, 1, 1));
	}
#pragma endregion

	// ImGuiの初期化
	ImGuiManager imgui;
	imgui.Init();

	// 乱数調節(ガチ)
	std::random_device rd;
	std::mt19937_64 mt(rd());
	SRand(static_cast<int>(mt()));


	while (ProcessMessage() == 0) {
		// フレーム開始時刻を取得
		int frameStart = GetNowCount();

		// 画面をクリアする
		ClearDrawScreen();

		// ImGuiのフレーム初めに呼ぶ処理
		imgui.BeginFrame();
		
		// シーンの更新
		TimeManager::GetInstance().Update();
		InputManager::GetInstance().Update();
		SceneManager::GetInstance().Update();

		if(InputManager::GetInstance().IsKeyDown(KEY_INPUT_ESCAPE)) {
			break;
		}
		// シーンの描画
		SceneManager::GetInstance().Render();

		// ImGuiのフレーム終わりに呼ぶ処理
		imgui.EndFrame();

		// 裏画面と表画面を切り替える
		ScreenFlip();

		// 処理にかかった時間を計算
		int elapsed = GetNowCount() - frameStart;
		int update = int(FRAME_TIME * 1000.0f);

		// 処理が速すぎたら待つ
		if (elapsed < update)
			WaitTimer(update - elapsed);

	}

	CollisionManager::GetInstance().UnRegisterAll();
	DeleteLightHandleAll();
	MV1InitModel();
	InitSoundMem();
	InitGraph();
	InitFontToHandle();
	
	// ImGuiの終了処理
	imgui.Release();
	

	// DxLibの終了
	Effkseer_End();
	DxLib_End();

	return 0;
}