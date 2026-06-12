#pragma once
#include "Scene/SceneBase.h"
#include <string>
#include <functional>
#include <vector>

struct SceneInfo{
	std::string sceneName;
	std::function<void()> SceneChangeFunc;
};

class DebugSceneSelectScene : public SceneBase{
private:
	std::vector<SceneInfo> sceneInfoArray;

	int currentScene;

public:
	DebugSceneSelectScene();
	~DebugSceneSelectScene();

private:
	void Start() override;

public:
	void Update() override;

	void Render() override;

	void Setup() override;
};
