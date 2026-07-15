/*
 * @file SceneManager.cpp
 * @author Sekino
 */
#include "SceneManager.h"
#include "Debug/Scene/DebugSceneSelectScene.h"
#include "Debug/Scene/PlayerDebugScene.h"
#include "Debug/Scene/EnemyDebugScene.h"
#include "Debug/Scene/CollisionDebugScene.h"
#include "Debug/Scene/StageDebugScene.h"
#include "Debug/Scene/DebugScene.h"
#include "Manager/InputManager.h"
#include "Manager/FadeManager.h"
#include "Debug/Scene/TitleScene.h"
#include "Debug/Scene/StageSelectScene.h"
#include "Debug/Scene/MainGameScene.h"

SceneManager::SceneManager() 
	: currentSceneType(SceneType::Title)
{ Start(); }

void SceneManager::Start(){

	// シーンの生成
	scene[static_cast<int>(SceneType::DebugSceneSelect)] = std::make_unique<DebugSceneSelectScene>();
	scene[static_cast<int>(SceneType::Debug)] = std::make_unique<DebugScene>();
	scene[static_cast<int>(SceneType::PlayerDebug)] = std::make_unique<PlayerDebugScene>();
	scene[static_cast<int>(SceneType::EnemyDebug)] = std::make_unique<EnemyDebugScene>();
	scene[static_cast<int>(SceneType::StageDebug)] = std::make_unique<StageDebugScene>();
	scene[static_cast<int>(SceneType::CollisionDebug)] = std::make_unique<CollisionDebugScene>();
	
	scene[static_cast<int>(SceneType::Title)] = std::make_unique<TitleScene>();
	scene[static_cast<int>(SceneType::StageSelect)] = std::make_unique<StageSelectScene>();
	scene[static_cast<int>(SceneType::Game)] = std::make_unique<MainGameScene>();


	// 初期シーンの準備
	scene[static_cast<int>(currentSceneType)]->Setup();
}

void SceneManager::Update(){
	scene[static_cast<int>(currentSceneType)]->Update();

#if _DEBUG
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_BACK))
		ChangeScene(SceneType::DebugSceneSelect);
#endif
}

void SceneManager::Render(){
	scene[static_cast<int>(currentSceneType)]->Render();
}

void SceneManager::ChangeScene(SceneType nextSceneType){
	if(currentSceneType == nextSceneType || nextSceneType == SceneType::Invalid) return;
	// フェードに入る
	FadeManager::GetInstance().FadeStart(FadeOut, FadeType::FadeNormal,0.2f);
	// 現在のシーンの後処理
	scene[static_cast<int>(currentSceneType)]->Cleanup();
	// 次のシーンの前処理
	scene[static_cast<int>(nextSceneType)]->Setup();
	currentSceneType = nextSceneType;
}
