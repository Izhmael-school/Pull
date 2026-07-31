#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "GameObject/GameObject.h"
#include <string>
#include "Component/Ray/RayCast.h"

/*
 * @file Collider.h
 * @author Tanaka
 */

 //	レイヤー
enum class ColliderLayer {
	Default,
	Player,
	PlayerArm,
	Enemy,
	Stage,
	Gimmick,
	BreakWall,
	ExitArea,
	BomBreakWall,
	Camera,
	Tail,
	Missile,
	MissileWall,
	Wall,
	Ground,
	PlayerRay,
	Retry,
	Stage1,
	Stage2,
	Stage3,
	Stage4,
	Stage5,
	Stage6,
	Stage7,
	GroundCheck,
	Coin
};

class Collider {
public:
	virtual bool Raycast(
		const Ray& ray,
		float maxDistance,
		RayHit& hit) {
		return false;
	}

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


	static void RotateBounds(
		VECTOR& min,
		VECTOR& max,
		const VECTOR& rotation);
private:
	static void CreateCorners(
		const VECTOR& min,
		const VECTOR& max,
		VECTOR corners[8]);

	static void CalculateBounds(
		const VECTOR corners[8],
		VECTOR& min,
		VECTOR& max);
	public:
		bool Raycast(
			const Ray& ray,
			float maxDistance,
			RayHit& hit) override;
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

public:
	bool Raycast(
		const Ray& ray,
		float maxDistance,
		RayHit& hit) override;

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

public:
	bool Raycast(
		const Ray& ray,
		float maxDistance,
		RayHit& hit) override;
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

	std::vector<Collider*> hitColliders;
	std::vector<GameObject*> hitObjects;


public:
	RayCollider(GameObject* owner, VECTOR ori, VECTOR dir, float len, float h, float angDeg, float _bottomOffset);

	void Update() override;
	void Render() override;

	bool CheckHitPoint(VECTOR target);

	bool CheckHitLine(VECTOR start, VECTOR end);
	bool CheckHitAABB(const VECTOR& min, const VECTOR& max);

	const char* GetTypeName() const override { return "Ray"; }

	VECTOR GetOriginWorld() const {
		return VAdd(GetGameObject()->GetTransform()->GetPosition(), origin);
	}

	VECTOR GetDirectionWorld() const {
		return direction; // 必要なら回転を適用
	}

	void SetOrigin(VECTOR ori) { origin = ori; }
	void SetWorldOrigin(VECTOR ori) { worldOrigin = ori; }

	float GetLength() const { return length; }
	float GetHeight() const { return height; }
	float GetAngleDeg() const { return angle; }

	VECTOR GetWorldOrigin() const { return worldOrigin; }
	VECTOR GetWorldDirection() const { return worldDirection; }
	float GetBottomOffset() const { return bottomOffset; }

	void ClearHitColliders() {
		hitColliders.clear();
		hitObjects.clear();
	}

	void AddHitColliders(Collider* col) {
		auto it =
			std::find(
				hitColliders.begin(),
				hitColliders.end(),
				col);

		if (it != hitColliders.end()) {
			return;
		}

		hitColliders.push_back(col);

		GameObject* obj = col->GetGameObject();

		if (obj) {
			auto objIt =
				std::find(
					hitObjects.begin(),
					hitObjects.end(),
					obj);

			if (objIt == hitObjects.end()) {
				hitObjects.push_back(obj);
			}
		}
	}


	const std::vector<Collider*>& GetHitColliders() const {
		return hitColliders;
	}

	const std::vector<GameObject*>& GetHitObjects() const {
		return hitObjects;
	}


	static float Cross2D(VECTOR a, VECTOR b) {
		return a.x * b.z - a.z * b.x;
	}

	static bool SegmentIntersect2D(
		VECTOR a1,
		VECTOR a2,
		VECTOR b1,
		VECTOR b2) {
		VECTOR da = VSub(a2, a1);
		VECTOR db = VSub(b2, b1);

		float c1 = Cross2D(
			da,
			VSub(b1, a1));

		float c2 = Cross2D(
			da,
			VSub(b2, a1));

		float c3 = Cross2D(
			db,
			VSub(a1, b1));

		float c4 = Cross2D(
			db,
			VSub(a2, b1));

		return c1 * c2 <= 0 &&
			c3 * c4 <= 0;
	}

};

#endif