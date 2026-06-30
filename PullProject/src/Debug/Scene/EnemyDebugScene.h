/*
 * @file EnemyDebugScene.h
 * @author Sekino
 */
#pragma once
#include "Scene/SceneBase.h"
#include <memory>
#include "GameObject/GameObject.h"
#include "GameObject/Character/Enemy/WalkEnemy/WalkEnemy.h"
#include "Generator/StageCollisionGenerator.h"
#include "Manager/EffectManager.h"
#include "Manager/EffectResourceManager.h"

class PlayerCharacrerPtr;

class EnemyDebugScene : public SceneBase {
private:
	StageCollisionGenerator generator;

	//EffectManager effectManager;

	//EffectResourceManager effectResourceManager;
public:
	EnemyDebugScene();
	~EnemyDebugScene() = default;

private:
	void Start() override;

public:
	void Update() override;

	void Render() override;

	void Setup() override;

	void Cleanup() override;
};

