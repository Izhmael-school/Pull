#pragma once
#include "../EnemyBase.h"
class WalkEnemy : public EnemyBase{
private:
	void Start() override;

public:
	WalkEnemy(int _modelHandle, VECTOR _pos);
	~WalkEnemy();

public:
	void Update() override;

	void Setup() override;

	bool VisionFan(VECTOR target) override;
};

