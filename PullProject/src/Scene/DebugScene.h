/*
 * @file DebugScene.h
 * @author Sekino
 */
#pragma once
#include "SceneBase.h"
#include <memory>
#include "../GameObject/GameObject.h"
#include "../GameObject/Character/Enemy/WalkEnemy/WalkEnemy.h"

class DebugScene : public SceneBase {

private:
	std::unique_ptr<WalkEnemy> enemy;

public:
	DebugScene();
	~DebugScene() = default;

private:
	void Start() override;

public:
	void Update() override;

	void Render() override;

};

