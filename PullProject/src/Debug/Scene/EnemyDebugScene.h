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
#include "Manager/AudioManager.h"
#include "Manager/AudioResourceManager.h"
#include "Manager/EnemyManager.h"


class PlayerCharacrerPtr;

class EnemyDebugScene : public SceneBase {
private:
	StageCollisionGenerator generator;

	EffectManager effectManager;

	EffectResourceManager effectResourceManager;

	AudioManager audioManager;

	AudioResourceManager audioResourceManager;

	EnemyManager enemyManager;
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

