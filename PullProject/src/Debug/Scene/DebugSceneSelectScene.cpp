#include "DebugSceneSelectScene.h"
#include "Manager/SceneManager.h"
#include "Definition/Enum/SceneType.h"
#include "Manager/InputManager.h"
#include <algorithm>
#include <math.h>

DebugSceneSelectScene::DebugSceneSelectScene()
	:currentScene(0)
{
	Start();
}

DebugSceneSelectScene::~DebugSceneSelectScene()
{
}

void DebugSceneSelectScene::Start() {

	sceneInfoArray.push_back({ "Debug",[]() {SceneManager::GetInstance().ChangeScene(SceneType::Debug);} });
	sceneInfoArray.push_back({ "PlayerDebug",[]() {SceneManager::GetInstance().ChangeScene(SceneType::PlayerDebug);} });
	sceneInfoArray.push_back({ "EnemyDebug",[]() {SceneManager::GetInstance().ChangeScene(SceneType::EnemyDebug);} });
	sceneInfoArray.push_back({ "StageDebug",[]() {SceneManager::GetInstance().ChangeScene(SceneType::StageDebug);} });
	sceneInfoArray.push_back({ "CollisionDebug",[]() {SceneManager::GetInstance().ChangeScene(SceneType::CollisionDebug);} });
}

void DebugSceneSelectScene::Update() {
	int size = static_cast<int>(sceneInfoArray.size());

	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_UP))
		currentScene = max(currentScene - 1, 0);
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_DOWN))
		currentScene = min(currentScene + 1, size - 1);
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN))
		sceneInfoArray[currentScene].SceneChangeFunc();
}

void DebugSceneSelectScene::Render() {
	int size = static_cast<int>(sceneInfoArray.size());
	for (int i = 0;i < size;i++) {
		if (i == currentScene)
			DrawString(100, 100 + (20 * i), sceneInfoArray[i].sceneName.c_str(), 0xffff00);
		else
			DrawString(100, 100 + (20 * i), sceneInfoArray[i].sceneName.c_str(), 0x000000);
	}
}

void DebugSceneSelectScene::Setup() {
	currentScene = 0;
}
