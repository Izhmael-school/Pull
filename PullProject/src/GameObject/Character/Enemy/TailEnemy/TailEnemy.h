#pragma once
#include "../EnemyBase.h"

class ColliderObject;

class TailEnemy : public EnemyBase {
private:
	std::unique_ptr<ColliderObject> pTailCollider;
	int tailFrameIndex;
public:
	TailEnemy(int _modelHandle, VECTOR _pos);
	~TailEnemy() = default;

private:
	void Start() override;

public:
	void Update() override;

	void Render() override;

	void Setup() override;

	void AttackAction();

	void WanderingAction() override;

	void TracingAction() override;
};

