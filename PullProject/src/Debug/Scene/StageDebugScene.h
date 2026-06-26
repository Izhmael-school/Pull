/*
 * @file StageDebugScene.h
 * @author Sekino
 */
#pragma once
#include "Scene/SceneBase.h"
#include <memory>
#include "GameObject/GameObject.h"
#include "GameObject/Character/Enemy/WalkEnemy/WalkEnemy.h"
#include "GameObject/Stage/Gimmick/Lever.h"
#include "GameObject/Stage/Gimmick/BreakWall.h"

class StageDebugScene : public SceneBase {

private:

	class AABBCollider* AABB;
	class CapsuleCollider* capsule;

	// デバッグ用レバー
	std::unique_ptr<Lever> lever;
	// デバッグ用ギミック
	std::unique_ptr<BreakWall> breakWall;

public:
	StageDebugScene();
	~StageDebugScene() = default;

private:
	void Start() override;

public:
	void Update() override;

	void Render() override;

};

