/*
 * @brief アプリケーションクラス
 * @author Sekino
 */
#pragma once
#include "Component/Singleton.h"
#include "ImGui/ImGuiManager.h"
#include "Manager/AudioManager.h"
#include "Manager/EffectManager.h"
#include "Manager/AudioResourceManager.h"
#include "Manager/EffectResourceManager.h"

constexpr double FRAME_TIME = 1.0 / 60.0;
class Application : public Singleton<Application>  {
private:
	ImGuiManager imgui;
	EffectResourceManager effectResourceManager;
	EffectManager effectManager;
	AudioResourceManager audioResourceManager;
	AudioManager audioManager;

	bool isGameEnd;
public:
	Application();
	~Application() = default;

private:
	/*
	 * @brief 初期化
	 */
	int Init();

	/*
	 * @brief DxLibの初期化
	 */
	int DxLibInit();

	/*
	 * @brief 更新
	 */
	bool Update();

	/*
	 * @brief 描画
	 */
	void Render();

	/*
	 * @brief リソースの削除
	 */
	void ResourceDelete();

	/*
	 * @brief 終了前処理
	 */
	void End();

	/*
	 * @brief DxLibの終了前処理
	 */
	void DxLibEnd();

public:
	/*
	 * @brief ゲームのメインループ
	 */
	void Run();

	/*
	 * @brief ゲーム終了
	 */
	void GameEnd() { isGameEnd = true; }

	/*
	 * @brief エフェクト管理クラスの参照
	 */
	EffectManager& GetEffectManager() { return effectManager; }

	/*
	 * @brief オーディオ管理クラスの参照
	 */
	AudioManager& GetAudioManager() { return audioManager; }
};

