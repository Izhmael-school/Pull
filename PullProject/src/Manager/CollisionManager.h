#ifndef _COLLISIONMANAGER_H_
#define _COLLISIONMANAGER_H_

#include <vector>
#include "Component/Singleton.h"
#include "Component/Collider/Collider.h"
#include <unordered_map>

/*
 *  CollisionManager.h
 *  Tanaka
 */

using HitFunc = bool(*)(Collider*, Collider*);
using ResolveFunc = void(*)(Collider*, Collider*);

class CollisionManager : public Singleton<CollisionManager> {
private:
    std::vector<class Collider*> pColliderArray;

    std::vector<std::vector<bool>> prevs;
    std::vector<std::vector<bool>> currents;

    std::unordered_map<std::string, HitFunc> hitFuncTable;
    std::unordered_map<std::string, ResolveFunc> resolveFuncTable;


    int index = 0;

public:
    CollisionManager();
    ~CollisionManager();

    void Register(Collider* col);
    void Update();
    void Render();
    void Clear();

    void CheckRegister(Collider* _pCol);
    void UnRegister(Collider* _pCol);
    void UnRegisterAll();

    const std::vector<Collider*>& GetColliders() const { return pColliderArray; }

    void Resolve(Collider* a, Collider* b);

    void LoadCollisionRules(const std::string& path);

private:
    bool CheckHit(Collider* a, Collider* b);

public:
    // 判定
    static bool SphereVsSphere(Collider* a, Collider* b);
    static bool SphereVsAABB(Collider* a, Collider* b);
    static bool AABBvsAABB(Collider* a, Collider* b);
    static bool CapsuleVsSphere(Collider* a, Collider* b);
    static bool CapsuleVsAABB(Collider* a, Collider* b);
    static bool CapsuleVsCapsule(Collider* a, Collider* b);
    static bool RayVsSphere(Collider* a, Collider* b);
    static bool RayVsAABB(Collider* a, Collider* b);
    static bool RayVsCapsule(Collider* a, Collider* b);

    // 押し出し
    static void ResolveSphereSphere(Collider* a, Collider* b);
    static void ResolveSphereAABB(Collider* s, Collider* box);
    static void ResolveCapsuleAABB(Collider* cap, Collider* box);
};

#endif