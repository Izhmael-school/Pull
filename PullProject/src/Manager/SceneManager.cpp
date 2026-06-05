#include "SceneManager.h"
#include "../Scene/DebugScene.h"

SceneManager::SceneManager() 
	: currentSceneType(SceneType::Debug)
{ Start(); }

void SceneManager::Start(){

	// シーンの生成
	scene[static_cast<int>(SceneType::Debug)] = std::make_unique<DebugScene>();
}

void SceneManager::Update(){
	scene[static_cast<int>(currentSceneType)]->Update();
}

void SceneManager::Render(){
	scene[static_cast<int>(currentSceneType)]->Render();
}

void SceneManager::ChangeScene(SceneType nextSceneType){
	if(currentSceneType == nextSceneType || nextSceneType == SceneType::Invalid) return;
	// 現在のシーンの後処理
	scene[static_cast<int>(currentSceneType)]->Cleanup();
	// 次のシーンの前処理
	scene[static_cast<int>(nextSceneType)]->Setup();
	currentSceneType = nextSceneType;
}
