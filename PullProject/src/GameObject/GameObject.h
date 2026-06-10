/*
 * @file GameObject.h
 * @author Sekino
 */
#pragma once
#include "../Component/Transform.h"
#include "../Definition/Enum/TagEnum.h"
#include "../Definition/Const/VECTORConst.h"
#include <memory>

class GameObject {
protected:
	std::unique_ptr<Transform> transform;
	Tag tag;
	bool isVisible;
	int modelHandle;

public:
	GameObject(int _modelHandle = -1 ,VECTOR _pos = VZero,Tag _tag = None);
	virtual ~GameObject();

	virtual void Start();
	virtual void Update();
	virtual void Render();

	virtual void ChangeMaterialColor(int _matIndex, unsigned int _color);

	// ÉÇÉfÉãÇÃçÌèú
	virtual void DeleteModel();

	inline Transform* GetTransform() const { return transform.get(); }

	inline bool IsVisible() const { return isVisible; }
	inline void SetVisible(bool _isVisible) { isVisible = _isVisible; }

	inline void SetModelHandle(int _mHandle) { modelHandle = _mHandle; }
	inline int GetModelHandle() const { return modelHandle; }
};

