#include "Character.h"

Character::Character(int _modelHandle, VECTOR _pos, Tag _tag) 
	:GameObject(_modelHandle,_pos,_tag)
{
	Start();
}

Character::~Character(){}

void Character::Start() {
	GameObject::Start();

	pAnimator = std::make_unique<Animator>(modelHandle);
}

void Character::Update() {
	GameObject::Update();

	if (pAnimator != nullptr)
		pAnimator->Update();
}

void Character::Render() {
	GameObject::Render();
}

void Character::Setup() {
	GameObject::Setup();
}
