/*
 * @file CollisionDebugScene.h
 * @author Sekino
 */
#pragma once
#include "Scene/SceneBase.h"
#include <memory>
#include "GameObject/GameObject.h"
#include "GameObject/Character/Enemy/WalkEnemy/WalkEnemy.h"
#include "Generator/StageCollisionGenerator.h"

class CollisionDebugScene : public SceneBase {

private:

	class AABBCollider* AABB;
	StageCollisionGenerator generator;

public:
	CollisionDebugScene();
	~CollisionDebugScene() = default;

private:
	void Start() override;

public:
	void Update() override;

	void Setup() override;

	void Render() override;

};

