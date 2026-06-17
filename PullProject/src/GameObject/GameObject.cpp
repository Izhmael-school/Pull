/*
 * @file GameObject.cpp
 * @author Sekino
 */

#include "GameObject.h"
#include <cassert>
#include "../Definition/CommonModule/MyMath.h"
#include "Component/Collider/Collider.h"

GameObject::GameObject(int _modelHandle ,VECTOR _pos,Tag _tag)
	:tag(_tag)
	,isVisible(true)
	,modelHandle(_modelHandle)
{
	pTransform = std::make_unique<Transform>();
	pTransform->SetPosition(_pos);
}

GameObject::~GameObject() {
	MV1DeleteModel(modelHandle);
}

void GameObject::Start(){
	pTransform->Update();
}

void GameObject::Update() {
	if (!isVisible) return;
	
	pTransform->Update();

	if (pCollider != nullptr)
		pCollider->Update();
}

void GameObject::Render() {
	if (!isVisible) return;

	// ƒ‚ƒfƒ‹‚ª‚È‚¢‚È‚ç•`‰æ‚µ‚È‚¢
	if(modelHandle == -1) return;
	// --- ’Êíƒ‚ƒfƒ‹•`‰æ ---
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	MV1SetMatrix(modelHandle, pTransform->GetMatrix());
	MV1DrawModel(modelHandle);

#if _DEBUG
	if (pCollider != nullptr)
		pCollider->Render();
#endif
}

void GameObject::Setup()
{
}

void GameObject::DeleteModel() {
	MV1DeleteModel(modelHandle);
}
