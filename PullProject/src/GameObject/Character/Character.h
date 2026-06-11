#pragma once
#include "../GameObject.h"
#include "../../Component/Animator.h"
#include <memory>

class Character : public GameObject {
protected:
	std::unique_ptr<Animator> pAnimator;

public:
	Character(int _modelHandle, VECTOR _pos, Tag _tag = None);
	~Character();

protected:
	virtual void Start() override;

public:
	virtual void Update() override;

	virtual void Render() override;

	virtual void Setup() override;
};

