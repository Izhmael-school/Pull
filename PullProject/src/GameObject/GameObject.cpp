/*
 * @file GameObject.cpp
 * @author Sekino
 */

#include "GameObject.h"
#include <cassert>
#include "../Definition/CommonModule/MyMath.h"

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
}

void GameObject::Update() {
	if (!isVisible) return;
	
	pTransform->Update();
}

void GameObject::Render() {
	if (!isVisible) return;

	// ƒ‚ƒfƒ‹‚ª‚È‚¢‚È‚ç•`‰æ‚µ‚È‚¢
	if(modelHandle == -1) return;
	// --- ’Êíƒ‚ƒfƒ‹•`‰æ ---
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	MV1SetMatrix(modelHandle, pTransform->GetMatrix());
	MV1DrawModel(modelHandle);
}

void GameObject::Setup()
{
}

void GameObject::ChangeMaterialColor(int _matIndex, unsigned int _color) {
	if (modelHandle == -1) return;

	int r, g, b;
	GetColor2(_color, &r, &g, &b);
	MV1SetMaterialDifColor(modelHandle, _matIndex, GetColorF(r, g, b, 255));
}

void GameObject::DeleteModel() {
	MV1DeleteModel(modelHandle);
}
