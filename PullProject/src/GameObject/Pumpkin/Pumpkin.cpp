#include "Pumpkin.h"
#include "Component/Collider/Collider.h"
#include "GameObject/Character/Player/PlayerHands.h"

Pumpkin::Pumpkin(int _modelHandle, VECTOR _pos, Transform* _parent)
	:GameObject(_modelHandle,_pos)
{
	GetTransform()->AttachParent(_parent);
	pCollider = std::make_unique<SphereCollider>(this, VZero, 150.0f);
	pCollider->SetResolve(false);
	
}

void Pumpkin::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {

	auto arms = dynamic_cast<PlayerHands*>(_pOther->GetGameObject());
	if (arms) {

	}
}

void Pumpkin::CaughtAction() {
	ChangeCaughtState(Catch);
}

void Pumpkin::ThrownAction(VECTOR _dir) {
	GetTransform()->LookAtDir(VScale(_dir, -1));
	ChangeCaughtState(Throw);
}

void Pumpkin::CatchStart() {
	CaughtObject::CatchStart();
}

void Pumpkin::Catching() {
}

void Pumpkin::ThrowStart() {
	CaughtObject::ThrowStart();
}

void Pumpkin::Throwing() {
}

void Pumpkin::HitObject() {
}
