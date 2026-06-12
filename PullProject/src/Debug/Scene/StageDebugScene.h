/*
 * @file StageDebugScene.h
 * @author Sekino
 */
#pragma once
#include "Scene/SceneBase.h"
#include <memory>
#include "GameObject/GameObject.h"
#include "GameObject/Character/Enemy/WalkEnemy/WalkEnemy.h"

class StageDebugScene : public SceneBase {

private:
	std::unique_ptr<WalkEnemy> enemy;

	class AABBCollider* AABB;
	class CapsuleCollider* capsule;
public:
	StageDebugScene();
	~StageDebugScene() = default;

private:
	void Start() override;

public:
	void Update() override;

	void Render() override;

};

