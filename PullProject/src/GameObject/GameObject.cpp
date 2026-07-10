/*
 * @file GameObject.cpp
 * @author Sekino
 */

#include "GameObject.h"
#include <cassert>
#include "../Definition/CommonModule/MyMath.h"
#include "Component/Collider/Collider.h"
#include "../Manager/CollisionManager.h"

GameObject::GameObject(int _modelHandle, VECTOR _pos, Tag _tag)
	:tag(_tag)
	, isActive(true)
	, modelHandle(_modelHandle)
{
	pTransform = std::make_unique<Transform>();
	pTransform->SetPosition(_pos);
	Start();
}

GameObject::~GameObject() {
	MV1DeleteModel(modelHandle);


	if (pCollider)
	{
		CollisionManager::GetInstance().UnRegister(pCollider.get());
		pCollider.release();
	}


}

void GameObject::Start() {
	pTransform->Update();
}

void GameObject::Update() {
	if (!isActive) return;

	pTransform->Update();

	if (pCollider != nullptr)
		pCollider->Update();
}

void GameObject::Render() {
	if (!isActive) return;

	// モデルが無いなら描画しない
	if (modelHandle != -1) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		MV1SetMatrix(modelHandle, pTransform->GetMatrix());
		MV1DrawModel(modelHandle);
	}

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

void GameObject::OnTriggerEnter(Collider* _pSelf, Collider* _pOther)
{
}

void GameObject::OnTriggerStay(Collider* _pSelf, Collider* _pOther)
{
}

void GameObject::OnTriggerExit(Collider* _pSelf, Collider* _pOther)
{
}
