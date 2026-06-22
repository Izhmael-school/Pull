/*
 * @file EnemyDebugScene.h
 * @author Sekino
 */
#pragma once
#include "Scene/SceneBase.h"
#include <memory>
#include "GameObject/GameObject.h"
#include "GameObject/Character/Enemy/WalkEnemy/WalkEnemy.h"
#include "GameObject/Missile/Missile.h"

class PlayerCharacrerPtr;

class EnemyDebugScene : public SceneBase {
	std::unique_ptr<Missile> pMissile;

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

