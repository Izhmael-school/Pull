#ifndef _COLLISIONMANAGER_H_
#define _COLLISIONMANAGER_H_

#include <vector>
#include "../Component/Singleton.h"

/*
 * @file CollisionManager.h
 * @author Tanaka
 */

// 前方宣言
class Collider;
class SphereCollider;
class AABBCollider;
class CapsuleCollider;

class CollisionManager : public Singleton<CollisionManager>
{
private:
    std::vector<Collider*> colliders;

public:
    void Register(Collider* col);
    void Update();
    void Render();

private:
    bool CheckHit(Collider* a, Collider* b);

    // 判定
    bool SphereVsSphere(SphereCollider* a, SphereCollider* b);
    bool SphereVsAABB(SphereCollider* s, AABBCollider* box);
    bool SphereVsCapsule(SphereCollider* s, CapsuleCollider* cap);

    bool AABBvsAABB(AABBCollider* a, AABBCollider* b);

    bool CapsuleVsSphere(CapsuleCollider* cap, SphereCollider* s);
    bool CapsuleVsAABB(CapsuleCollider* cap, AABBCollider* box);
    bool CapsuleVsCapsule(CapsuleCollider* a, CapsuleCollider* b);

    // 押し出し
    void ResolveSphereSphere(SphereCollider* a, SphereCollider* b);
    void ResolveSphereAABB(SphereCollider* s, AABBCollider* box);
};

#endif // !_COLLISIONMANAGER_H_