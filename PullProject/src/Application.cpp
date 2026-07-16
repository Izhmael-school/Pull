#include "Application.h"
#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include <ioStream>
#include <random>
#include <memory>
#include "Manager/SceneManager.h"
#include "Manager/InputManager.h"
#include "Manager/TimeManager.h"
#include "Manager/CollisionManager.h"
#include "Manager/InputSystemManager.h"
#include "Manager/FadeManager.h"


Application::Application()
	:audioManager(audioResourceManager)
	, effectManager(effectResourceManager)
	, isGameEnd(true)
{}

int Application::Init() {
	// ImGuiの初期化
	imgui.Init();

	// 乱数調節
	std::random_device rd;
	std::mt19937_64 mt(rd());
	SRand(static_cast<int>(mt()));

	// オーディオリソースの読み込み
	audioResourceManager.LoadAudioFromExternalFile();
	// エフェクトリソースの読み込み
	effectResourceManager.LoadEffectFromExternalFile();

	return 0;
}

int Application::DxLibInit() {
#pragma region // DxLibの初期化処理　触るべからず
	// タイトルの変更
	SetWindowText("ExHand");
	// XInput対応ゲームパッド設定
	SetUseXInputFlag(true);
	// ウィンドウのサイズを変更する
	SetGraphMode(1920, 1080, 32, 60);
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
	SetFullSceneAntiAliasingMode(4, 2);
	// ゲームアイコン
	SetWindowIconID(101);

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
		return 1;

	if (Effekseer_Init(8000) == -1) {
		DxLib_End();
		return 1;
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
	int light = CreateDirLightHandle(VNorm(VGet(-1.0f, -1.0f, 0.0f)));

	// ライティング
	{
		// ライトの計算をどうするか
		SetUseLighting(TRUE); // default : TRUE
		// 標準ライトを使用するかどうか
		SetLightEnable(TRUE);	// default : TRUE
		SetLightDifColorHandle(light, GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
		SetLightSpcColorHandle(light, GetColorF(0.0f, 0.0f, 0.0f, 1.0f)); // スペキュラなし
		SetLightEnableHandle(light, TRUE);
		// グローバル環境光の設定
		SetGlobalAmbientLight(GetColorF(0.4f, 0.4f, 0.4f, 1.0f));
		// 反射光の設定  Diffuse
		//SetLightDifColor(GetColorF(1, 0, 0, 0));
		//// 鏡面反射光の設定　Specular
		//SetLightSpcColor(GetColorF(1, 0, 0.25f, 1));
		// 環境光の設定　Ambient
		SetLightAmbColor(GetColorF(0.3f, 0.3f, 0.3f, 1));
	}
#pragma endregion

    return 0;
}

bool Application::Update() {
	// シーンの更新
	TimeManager::GetInstance().Update();
	InputManager::GetInstance().Update();
	InputSystemManager::GetInstance().Update();
	FadeManager::GetInstance().Update();
	SceneManager::GetInstance().Update();
	audioManager.Update();

	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_ESCAPE)) {
		return true;
	}

	return false;
}

void Application::Render() {
	// シーンの描画
	SceneManager::GetInstance().Render();
	FadeManager::GetInstance().Render();
}

void Application::ResourceDelete() {
	CollisionManager::GetInstance().UnRegisterAll();
	audioResourceManager.Clear();
	effectResourceManager.Clear();
	DeleteLightHandleAll();
	MV1InitModel();
	InitSoundMem();
	InitGraph();
	InitFontToHandle();
}

void Application::End() {
	// ImGuiの終了処理
	imgui.Release();

	DeleteShadowMap(shadowMap.GetShadowMapHandle());
}

void Application::DxLibEnd() {
	// DxLibの終了
	Effkseer_End();
	DxLib_End();
}

void Application::Run() {
	// 初期化
	int dxLibInitComplete = DxLibInit();
	int initComplete = Init();
	// 初期化に失敗したらゲームを終了する
	isGameEnd = initComplete | dxLibInitComplete;

	// メインループ
	while (ProcessMessage() == 0) {
		// 終了
		if (isGameEnd)break;
		// フレーム開始時刻を取得
		int frameStart = GetNowCount();
		// 画面をクリアする
		ClearDrawScreen();
		// ImGuiのフレーム初めに呼ぶ処理
		imgui.BeginFrame();
		// 更新
		isGameEnd = Update();
		// 描画
		Render();
		// ImGuiのフレーム終わりに呼ぶ処理
		imgui.EndFrame();
		// 裏画面と表画面を切り替える
		ScreenFlip();
		// Debugログクリア
		clsDx();

		// 処理にかかった時間を計算
		int elapsed = GetNowCount() - frameStart;
		int update = int(FRAME_TIME * 1000.0f);
		// 処理が速すぎたら待つ
		if (elapsed < update)
			WaitTimer(update - elapsed);
	}
	// 終了前処理
	ResourceDelete();
	End();
	DxLibEnd();
}
