/*
 * @brief GameObjectを継承したクラスの更新描画(全てを更新するわけではない)
 * @author Sekino
 */
#pragma once
#ifndef _GAMEOBJECTMANAGER_H_
#define _GAMEOBJECTMANAGER_H_

#include "ManagerBase.h"
#include "Component/Singleton.h"
#include "Generator/GameObjectGenerator.h"
#include <vector>
#include <memory>

class GameObject;

class GameObjectManager : public ManagerBase,public Singleton<GameObjectManager>{
private:
	std::vector<std::unique_ptr<GameObject>> useObjectArray;
	std::unique_ptr<GameObjectGenerator> generator;

public:
	GameObjectManager();
	~GameObjectManager();

private:
	void Start() override;

public:
	void Update() override;

	void Render() override;

	/*
	 * @brief GameObjectを継承したオブジェクトを生成する
	 * @param _modelName:Jsonに書いた名前
	 * @param args:派生クラスの引数
	 */
	template<typename T , typename... Args>
	T* CreateGameObject(std::string _modelName, Args&&... args);
};

template<typename T, typename ...Args>
inline T* GameObjectManager::CreateGameObject(std::string _modelName, Args&&... args) {
	std::unique_ptr<GameObject> gameObject = generator->CreateGameObject<T>(_modelName, std::forward<Args>(args)...);
	if (gameObject == nullptr) return nullptr;
	useObjectArray.push_back(std::move(gameObject));
	return dynamic_cast<T*>(useObjectArray.back().get());
}
#endif // !_GAMEOBJECTMANAGER_H_