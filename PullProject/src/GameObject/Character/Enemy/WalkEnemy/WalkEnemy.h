#pragma once
#include "../EnemyBase.h"
class WalkEnemy : public EnemyBase{
	static int originModelHandle;

private:
	void Start() override;

public:
	WalkEnemy(int _modelHandle, VECTOR _pos);
	~WalkEnemy();

public:
	void Update() override;
};

