#ifndef _COLLISIONMANAGER_H_
#define _COLLISIONMANAGER_H_

#include <vector>
#include "../Component/Singleton.h"
#include "../Component/Collider/Collider.h"

/*
 * @file CollisionManager.h
 * @author Tanaka
 */

class CollisionManager : public Singleton<CollisionManager>
{
private:
    std::vector<class Collider*> pColliderArray;

    std::vector<std::vector<bool>> prevs;
    std::vector<std::vector<bool>> currents;

    int index = 0;
public:
    ~CollisionManager();

    void Register(Collider* col);
    void Update();
    void Render();
    void Clear();

	void CheckRegister(Collider* _pCol);
	void UnRegister(Collider* _pCol);
	void UnRegisterAll();

    const std::vector<Collider*>& GetColliders() const { return pColliderArray; }

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
	void ResolveCapsuleAABB(CapsuleCollider* cap, AABBCollider* box);

};

#endif // !_COLLISIONMANAGER_H_