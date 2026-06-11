/*
 * @file DebugScene.h
 * @author Sekino
 */
#pragma once
#include "SceneBase.h"
#include <memory>
#include "../GameObject/GameObject.h"

class DebugScene : public SceneBase {

public:
	DebugScene();
	~DebugScene() = default;

private:
	void Start() override;

public:
	void Update() override;

	void Render() override;

};

