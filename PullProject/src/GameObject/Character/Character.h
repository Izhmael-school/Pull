#pragma once
#include "../GameObject.h"
class Character : public GameObject{
private:
	virtual void Start() override;

public:
	virtual void Update() override;

	virtual void Render() override;

	virtual void Setup() override;
};

