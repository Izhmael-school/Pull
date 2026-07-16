#include "GameObjectManager.h"
#include "GameObject/GameObject.h"

GameObjectManager::GameObjectManager(){
	Start();
}

GameObjectManager::~GameObjectManager(){}

void GameObjectManager::Start(){
	generator = std::make_unique<GameObjectGenerator>();
}

void GameObjectManager::Update(){
	for (auto& obj : useObjectArray) {
		obj->Update();
	}

	// 配列からの除外
	std::erase_if(useObjectArray, [this](std::unique_ptr<GameObject>& obj) {
		// 有効でなければ
		if (obj->IsActive()) return false;
		// 消す
		obj->~GameObject();
		obj.release();
		obj.reset();
		return true;
		});
}

void GameObjectManager::Render(){
	for (auto& obj : useObjectArray) {
		obj->Render();
	}
}

void GameObjectManager::Cleanup() {
	// 配列からの除外
	std::erase_if(useObjectArray, [this](std::unique_ptr<GameObject>& obj) {
		// 消す
		obj->~GameObject();
		obj.release();
		obj.reset();
		return true;
		});
}
