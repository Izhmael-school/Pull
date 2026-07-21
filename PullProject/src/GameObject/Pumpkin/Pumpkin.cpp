#include "Pumpkin.h"
#include "Component/Collider/Collider.h"
#include "GameObject/Character/Player/PlayerHands.h"

Pumpkin::Pumpkin(int _modelHandle, VECTOR _pos, Transform* _parent)
	:GameObject(_modelHandle,_pos)
{
	GetTransform()->AttachParent(_parent);
	pCollider = std::make_unique<SphereCollider>(this, VZero, 150.0f);
	pCollider->SetResolve(false);
	pGroundingCollider = std::make_unique<SphereCollider>(this, VScale(VUp, -115), 5);
}

void Pumpkin::Update() {
	GameObject::Update();
	CaughtUpdate();
}

void Pumpkin::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {
	GameObject::OnTriggerEnter(_pSelf, _pOther);

	if (GetCurrentCaughtState() == ::Throwing)
		HitObject();
}

void Pumpkin::CaughtAction() {
	ChangeCaughtState(Catch);
}

void Pumpkin::ThrownAction(VECTOR _dir) {
	GetTransform()->LookAtDir(VScale(_dir, -1));
	thrownDir = _dir;
	ChangeCaughtState(Throw);
}

void Pumpkin::CatchStart() {
	CaughtObject::CatchStart();
}

void Pumpkin::Catching() {
}

void Pumpkin::ThrowStart() {
	CaughtObject::ThrowStart();
	AddFallSpeed(-10);
	isGravity = true;
	hitGroundingFrag = false;
}

void Pumpkin::Throwing() {
	GetTransform()->AddPosition(VScale(thrownDir, -10));
}

void Pumpkin::HitObject() {
	thrownDir = VZero;
}
