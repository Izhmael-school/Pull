#pragma once
#include "../Object.h"
#include "../../Definition/Enum/TagEnum.h"
#include "../../Definition/Const/VECTORConst.h"
#include <memory>

//class Collider;

/// <summary>
/// モデルを持つオブジェクトクラス
/// </summary>
class GameObject : public Object {
protected:
	bool isVisible;
	int modelHandle;

	//std::unique_ptr<Collider> pCollider;

public:
	GameObject(int _mHandle ,VECTOR _pos = VZero,Tag _tag = None);
	virtual ~GameObject();

	virtual void Start() = 0;
	virtual void Update();
	virtual void Render();

	virtual void ChangeMaterialColor(int _matIndex, unsigned int _color);

	// モデルの削除
	virtual void DeleteModel();

	inline bool IsVisible() const { return isVisible; }
	inline void SetVisible(bool _isVisible) { isVisible = _isVisible; }

	inline void SetModelHandle(int _mHandle) { modelHandle = _mHandle; }
	inline int GetModelHandle() const { return modelHandle; }

	//inline Collider* GetCollider() const { return pCollider.get(); }
};

