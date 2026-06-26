/*
 * @file DebugScene.h
 * @author Sekino
 */
#pragma once
#include "Scene/SceneBase.h"
#include <memory>
#include "GameObject/GameObject.h"
#include "GameObject/Character/Enemy/WalkEnemy/WalkEnemy.h"
#include "Generator/StageCollisionGenerator.h"

class DebugScene : public SceneBase {

private:
	std::unique_ptr<WalkEnemy> enemy;

	class AABBCollider* AABB;
	class CapsuleCollider* capsule;

	StageCollisionGenerator generator;
public:
	DebugScene();
	~DebugScene() = default;

private:
	void Start() override;

public:
	void Update() override;

	void Setup() override;

	void Render() override;

};

