/*
 * @file PlayerDebugScene.h
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
#include "Manager/AudioManager.h"
#include "Manager/AudioResourceManager.h"
#include "Manager/EnemyManager.h"

class PlayerDebugScene : public SceneBase {
private:
	StageCollisionGenerator generator;

	EffectManager effectManager;

	EffectResourceManager effectResourceManager;

	AudioManager audioManager;

	AudioResourceManager audioResourceManager;

	EnemyManager enemyManager;

public:
	PlayerDebugScene();
	~PlayerDebugScene() = default;

private:
	void Start() override;

public:
	void Setup() override;

	void Update() override;

	void Render() override;

};

