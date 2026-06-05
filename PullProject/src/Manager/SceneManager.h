#pragma once
#include <memory>
#include "ManagerBase.h"
#include "../Component/Singleton.h"
#include "../Definition/Enum/SceneType.h"
#include "../Scene/SceneBase.h"

class SceneManager : public ManagerBase, public Singleton<SceneManager>{

private:
	std::unique_ptr<SceneBase> scene[static_cast<int>(SceneType::Max)];
	SceneType currentSceneType;

public:
	SceneManager();
	~SceneManager() = default;

private:
	void Start() override;

public:
	void Update() override;
	void Render() override;

	/// <summary>
	/// シーンの変更（フェード無し）
	/// </summary>
	/// <param name="nextSceneType"></param>
	void ChangeScene(SceneType nextSceneType);

	// シーンの取得
	inline SceneType GetCurrentSceneType() const { return currentSceneType; }
};

