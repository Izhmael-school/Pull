/*
 * @file PlayerDebugScene.h
 * @author Sekino
 */
#pragma once
#include "Scene/SceneBase.h"
#include <memory>
#include "GameObject/GameObject.h"
#include "GameObject/Character/Enemy/WalkEnemy/WalkEnemy.h"

class PlayerDebugScene : public SceneBase {

private:

public:
	PlayerDebugScene();
	~PlayerDebugScene() = default;

private:
	void Start() override;

public:
	void Update() override;

	void Render() override;

};

