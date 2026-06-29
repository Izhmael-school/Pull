#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "GameObject/GameObject.h"
#include <string>

/*
 * @file Collider.h
 * @author Tanaka
 */

enum class ColliderLayer {
	Default,
	Player,
	Enemy,
	Stage
};

class Collider {
protected:
	bool isEnable;
	GameObject* pGameObject;

	ColliderLayer layer;

	bool isHit = false;
	bool prevHit = false;

	bool isResolve = true;

public:
	Collider(GameObject* _pObj);
	virtual ~Collider();

public:
	virtual void Update() = 0;
	virtual void Render() = 0;

public:
	bool IsEnable() const { return isEnable; }
	void SetEnable(bool _v) { isEnable = _v; }

	void SetResolve(bool v) { isResolve = v; }
	bool IsResolve() const { return isResolve; }

	GameObject* GetGameObject() const { return pGameObject; }
	void SetGameObject(GameObject* _pObj) { pGameObject = _pObj; }

	void SetHit(bool v) { isHit = v; }
	bool IsHit() const { return isHit; }

	void SetPrevHit(bool v) { prevHit = v; }
	bool IsPrevHit() const { return prevHit; }

	ColliderLayer GetLayer() const { return layer; }
	void SetLayer(ColliderLayer l) { layer = l; }

public:
	virtual const char* GetTypeName() const = 0;

};


// AABB Collider
class AABBCollider : public Collider {
private:
	VECTOR localMin;
	VECTOR localMax;

	VECTOR worldMin;
	VECTOR worldMax;

public:
	// コンストラクタ
	AABBCollider(GameObject* obj, VECTOR min, VECTOR max);

	// 必須
	void Update() override;
	void Render() override;

	VECTOR GetMin() const;
	VECTOR GetMax() const;

	void SetMin(VECTOR min);
	void SetMax(VECTOR max);

	void Move(VECTOR offset);

	const char* GetTypeName() const override { return "AABB"; }
};



//  SphereCollider
class SphereCollider : public Collider {
private:
	VECTOR localCenter;
	VECTOR worldCenter;
	float radius;

public:
	SphereCollider(GameObject* _pObj, VECTOR _localCenter, float _radius);
	~SphereCollider() = default;

	void Update() override;
	void Render() override;

	float GetRadius() const { return radius; }
	void SetRadius(float _radius) { radius = _radius; }

	VECTOR GetWorldCenter() const { return worldCenter; }

	/*
	 * @author Sekion
	 */
	void SetLocalCenter(VECTOR _center) { localCenter = _center; }


	const char* GetTypeName() const override { return "Sphere"; }
};


//  CapsuleCollider
class CapsuleCollider : public Collider {
private:
	VECTOR localStart;
	VECTOR localEnd;
	VECTOR worldStart;
	VECTOR worldEnd;

	bool hasErrorShown = false;
	
	float radius;

public:
	CapsuleCollider(GameObject* _pObj, VECTOR _localStart, VECTOR _localEnd, float _radius, VECTOR _offset);

	void Update() override;
	void Render() override;

	float GetRadius() const { return radius; }
	VECTOR GetWorldStart() const { return worldStart; }
	VECTOR GetWorldEnd() const { return worldEnd; }

	void Move(VECTOR offset);

	const char* GetTypeName() const override { return "Capsule"; }
};

class RayCollider : public Collider {
private:
	VECTOR origin;      // ローカル原点
	VECTOR direction;   // 中心方向（正規化）
	float length;       // 射程
	float height;       // 上下方向の許容範囲
	float angle;        // 左右の扇形角度（度数法）

	float bottomOffset;

	VECTOR worldOrigin;
	VECTOR worldDirection;

public:
	RayCollider(GameObject* owner, VECTOR ori, VECTOR dir, float len, float h, float angDeg, float _bottomOffset);

	void Update() override;
	void Render() override;

	bool CheckHitPoint(VECTOR target);
	
	const char* GetTypeName() const override { return "Ray"; }

	VECTOR GetOriginWorld() const {
		return VAdd(GetGameObject()->GetTransform()->GetPosition(), origin);
	}

	VECTOR GetDirectionWorld() const {
		return direction; // 必要なら回転を適用
	}

	float GetLength() const { return length; }
	float GetHeight() const { return height; }
	float GetAngleDeg() const { return angle; }

	VECTOR GetWorldOrigin() const { return worldOrigin; }
	VECTOR GetWorldDirection() const { return worldDirection; }

};

#endif