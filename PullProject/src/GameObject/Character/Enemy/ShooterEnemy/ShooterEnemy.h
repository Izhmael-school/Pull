#pragma once
#include "../EnemyBase.h"
class ShooterEnemy : public EnemyBase {
public:
	ShooterEnemy(int _modelHandle, VECTOR _pos);
	~ShooterEnemy();

private:
	void Start() override;

public:
	void Setup() override;

	void WanderingAction() override;

	void TracingAction() override;
};

