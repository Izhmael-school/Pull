/*
 * @file GameObject.h
 * @author Sekino
 */
#pragma once

#ifndef _GAMEOBJECT_
#define _GAMEOBJECT_

#include "Component/Transform.h"
#include "Definition/Enum/TagEnum.h"
#include "Definition/Const/VECTORConst.h"
#include <memory>

class Collider;

class GameObject {
protected:
	std::unique_ptr<Transform> pTransform;
	std::unique_ptr<Collider> pCollider;
	Tag tag;
	bool isVisible;
	int modelHandle;

public:
	GameObject(int _modelHandle = -1 ,VECTOR _pos = VZero,Tag _tag = None);
	virtual ~GameObject();

protected:
	virtual void Start();

public:
	virtual void Update();
	virtual void Render();
	virtual void Setup();

	// ƒ‚ƒfƒ‹‚Ìíœ
	virtual void DeleteModel();

	inline Transform* GetTransform() const { return pTransform.get(); }

	inline VECTOR GetPosition() const { return pTransform->GetPosition(); }
	inline VECTOR GetRotation() const { return pTransform->GetLocalRotation(); }
	inline VECTOR GetScale() const { return pTransform->GetScale(); }

	inline bool IsVisible() const { return isVisible; }
	inline void SetVisible(bool _isVisible) { isVisible = _isVisible; }

	inline void SetModelHandle(int _mHandle) { modelHandle = _mHandle; }
	inline int GetModelHandle() const { return modelHandle; }
};
#endif // !_GAMEOBJECT_