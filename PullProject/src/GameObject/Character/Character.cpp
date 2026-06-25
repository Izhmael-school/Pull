#include "Character.h"

Character::Character(int _modelHandle, VECTOR _pos, Tag _tag)
	:GameObject(_modelHandle, _pos, _tag)
	,isGravity(false)
	, fallSpeed(0.0f)
	, FALL_SPEED_MAX(100.0f)
	, GRAVITY_ACCELERATION(100.0f)
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

	// 重力落下
	GravityFall();

	if (pAnimator != nullptr)
		pAnimator->Update();
}

void Character::Render() {
	GameObject::Render();
}

void Character::Setup() {
	GameObject::Setup();
}

/*
 *	重力による落下処理
 *	@author Riku
 */
void Character::GravityFall() {
	// 重力がかかるまで処理しない
	if (!isGravity) {
		fallSpeed = 0.0f;
		return;
	}

	// 接地していた場合は落下しない
	//if (hitGroundingFrag) {
	//	// 落下速度を0にする
	//	fallSpeed = 0.0f;
	//	return;
	//}


	// 落下速度計算
	fallSpeed += GRAVITY_ACCELERATION;
	if (fallSpeed >= FALL_SPEED_MAX)
		fallSpeed = FALL_SPEED_MAX;

	pTransform->AddPosition(VScale(VUp, -fallSpeed));
	//isFirstFrame = false;
}
