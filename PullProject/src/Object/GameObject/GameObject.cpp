#include "GameObject.h"
#include <cassert>
#include "../../Definition/CommonModule/MyMath.h"

GameObject::GameObject(int _mHandle ,VECTOR _pos,Tag _tag)
	:Object(_tag)
	,isVisible(true)
	,modelHandle(_mHandle)
	//,pCollider(nullptr)
{
	transform.SetPosition(_pos);
}

GameObject::~GameObject() {
	MV1DeleteModel(modelHandle);
}

void GameObject::Update() {
	if (!isVisible) return;

	Object::Update();

}

void GameObject::Render() {
	if (!isVisible) return;

#if _DEBUG
	assert(modelHandle != -1);
#endif

	// --- í èÌÉÇÉfÉãï`âÊ ---
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	MV1SetMatrix(modelHandle, transform.GetMatrix());
	MV1DrawModel(modelHandle);
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
