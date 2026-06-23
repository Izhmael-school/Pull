#pragma once
#include <map>
#include <string>
#include <memory>
#include "DxLib.h"
#include <cassert>
#include <type_traits>
class GameObject;
class GameObjectGenerator {
private:
	std::map<std::string, int> modelArray;

public:
	GameObjectGenerator();
	~GameObjectGenerator() = default;

private:
	void Start();

public:
	/*
	 * @brief GameObjectを継承したオブジェクトの生成
	 */
	template<typename T, typename... Args>
	std::unique_ptr<GameObject> CreateGameObject(std::string _modelName, Args&&... args);
};

template<typename T, typename... Args>
inline std::unique_ptr<GameObject> GameObjectGenerator::CreateGameObject(std::string _modelName, Args&&... args) {
	int modelHandle = -1;
	// 配列に何もなければ帰る
	if (modelArray.size() == 0) return nullptr;

	// モデルを探す
	for (auto& obj : modelArray) {
		if (obj.first != _modelName) continue;

		modelHandle = obj.second;
		break;
	}

	// モデルが無ければ一応警告出す
#if _DEBUG
	if (modelHandle == -1) {
		assert(false && "Nothing Model");
	}
#endif

	// モデルの複製
	modelHandle = MV1DuplicateModel(modelHandle);

	// 生成
	std::unique_ptr<T> object = std::make_unique<T>(modelHandle, std::forward<Args>(args)...);
	// GameObjectを継承しているか
	bool haveGameObject = std::is_base_of<GameObject, T>::value;

	// 継承していなければもろもろ消してnullptrを返す
	if (!haveGameObject) {
		object.reset();
		MV1DeleteModel(modelHandle);
		return nullptr;
	}

	return std::move(object);
}