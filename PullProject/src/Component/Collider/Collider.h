#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "../../GameObject/GameObject.h"

/*
 * @file Collider.h
 */

class Collider {
protected:
    bool isEnable;
    GameObject* pGameObject;

    bool isHit = false;
    bool prevHit = false;

public:
    Collider(GameObject* _pObj);
    virtual ~Collider();

public:
    virtual void Update() = 0;
    virtual void Render() = 0;

public:
    bool IsEnable() const { return isEnable; }
    void SetEnable(bool _v) { isEnable = _v; }

    GameObject* GetGameObject() const { return pGameObject; }
    void SetGameObject(GameObject* _pObj) { pGameObject = _pObj; }

    void SetHit(bool v) { isHit = v; }
    bool IsHit() const { return isHit; }

    void SetPrevHit(bool v) { prevHit = v; }
    bool IsPrevHit() const { return prevHit; }
};


//
// ✅ AABB Collider
//
class AABBCollider : public Collider {
private:
    VECTOR localMin;
    VECTOR localMax;

    VECTOR worldMin;
    VECTOR worldMax;

public:
    // コンストラクタ
    AABBCollider();
    AABBCollider(GameObject* obj, VECTOR min, VECTOR max);

    // 必須
    void Update() override;
    void Render() override;

    VECTOR GetMin() const;
    VECTOR GetMax() const;

    void SetMin(VECTOR min);
    void SetMax(VECTOR max);

    void Move(VECTOR offset);
};


//
// ✅ Sphere
//
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
};


//
// ✅ Capsule
//
class CapsuleCollider : public Collider {
private:
    VECTOR localStart;
    VECTOR localEnd;
    VECTOR worldStart;
    VECTOR worldEnd;

    float radius;

public:
    CapsuleCollider(GameObject* _pObj, VECTOR _localStart, VECTOR _localEnd, float _radius, VECTOR _offset);

    void Update() override;
    void Render() override;

    float GetRadius() const { return radius; }
    VECTOR GetWorldStart() const { return worldStart; }
    VECTOR GetWorldEnd() const { return worldEnd; }

    void Move(VECTOR offset);
};

#endif
