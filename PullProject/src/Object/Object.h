#pragma once
#include "../Component/Transform.h"
#include "../Definition/Enum/TagEnum.h"

//class Collider;

/// <summary>
/// 空のオブジェクトクラス
/// </summary>
class Object {
protected:
	Transform transform;
	Tag tag;

protected:
	// 生成時に一度だけ呼ばれる初期化処理
	virtual void Start();
public:
	Object(Tag _tag = None);
	~Object();

	// 更新処理
	virtual void Update();
	// 描画処理
	virtual void Render();
	// 初期化処理
	virtual void Setup();
	// 未使用化処理
	virtual void Teardown();

	inline Transform& GetTransform() { return transform; }
	inline VECTOR GetPosition() const { return transform.GetPosition(); }
	inline VECTOR GetRotation() const { return transform.GetRotation(); }
	inline VECTOR GetScale() const { return transform.GetScale(); }
	inline MATRIX GetMatrix() const { return transform.GetMatrix(); }
	inline void SetTag(Tag _tag) { tag = _tag; }
	inline Tag GetTag() const { return tag; }
	//virtual void OnTriggerEnter(Collider* _pOther);
	//virtual void OnTriggerStay(Collider* _pOther);
	//virtual void OnTriggerExit(Collider* _pOther);
};

