#pragma once
#include "SceneBase.h"
#include "../Debug/DebugCamera.h"
#include <memory>

class DebugScene : public SceneBase {

public:
	DebugScene();
	~DebugScene() = default;

private:
	void Start() override;

public:
	void Update() override;

	void Render() override;

private:
	std::unique_ptr<DebugCamera> debugCamera;
};

