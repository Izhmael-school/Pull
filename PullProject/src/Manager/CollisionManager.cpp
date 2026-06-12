#include "CollisionManager.h"
#include "../Component/Collider/Collider.h"

/*
 * @file CollisionManager.cpp
 * @author Tanaka
 */

#pragma region 登録
// 登録
void CollisionManager::Register(Collider* col)
{
    if (!col) return;
    colliders.push_back(col);
}

#pragma endregion


#pragma region  更新
// 更新
void CollisionManager::Update()
{


    for (auto col : colliders)
    {
        col->SetPrevHit(col->IsHit());
        col->SetHit(false);           
    }

    int n = colliders.size();

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            Collider* a = colliders[i];
            Collider* b = colliders[j];

            if (!a || !b) continue;
            if (!a->IsEnable() || !b->IsEnable()) continue;

            if (CheckHit(a, b))
            {

                a->SetHit(true);
                b->SetHit(true);

                // 当たった瞬間だけ出す
                if ((!a->IsPrevHit() && a->IsHit()) ||
                    (!b->IsPrevHit() && b->IsHit()))
                {
                    printfDx("Hit!\n");
                }

                //  押し出し
                if (auto s1 = dynamic_cast<SphereCollider*>(a))
                    if (auto s2 = dynamic_cast<SphereCollider*>(b))
                        ResolveSphereSphere(s1, s2);

                if (auto s = dynamic_cast<SphereCollider*>(a))
                    if (auto box = dynamic_cast<AABBCollider*>(b))
                        ResolveSphereAABB(s, box);

                if (auto box = dynamic_cast<AABBCollider*>(a))
                    if (auto s = dynamic_cast<SphereCollider*>(b))
                        ResolveSphereAABB(s, box);


                if (auto cap = dynamic_cast<CapsuleCollider*>(a))
                    if (auto box = dynamic_cast<AABBCollider*>(b))
                        ResolveCapsuleAABB(cap, box);

                if (auto box = dynamic_cast<AABBCollider*>(a))
                    if (auto cap = dynamic_cast<CapsuleCollider*>(b))
                        ResolveCapsuleAABB(cap, box);

            }
        }
    }
}

#pragma endregion


#pragma region 判定
//  判定
bool CollisionManager::CheckHit(Collider* a, Collider* b)
{
    // Sphere vs Sphere
    if (auto s1 = dynamic_cast<SphereCollider*>(a))
        if (auto s2 = dynamic_cast<SphereCollider*>(b))
            return SphereVsSphere(s1, s2);

    // AABB vs AABB
    if (auto b1 = dynamic_cast<AABBCollider*>(a))
        if (auto b2 = dynamic_cast<AABBCollider*>(b))
            return AABBvsAABB(b1, b2);

    // Sphere vs AABB
    if (auto s = dynamic_cast<SphereCollider*>(a))
        if (auto box = dynamic_cast<AABBCollider*>(b))
            return SphereVsAABB(s, box);
    
    if (auto box = dynamic_cast<AABBCollider*>(a))
        if (auto s = dynamic_cast<SphereCollider*>(b))
            return SphereVsAABB(s, box);

    // Capsule vs Capsule
    if (auto c1 = dynamic_cast<CapsuleCollider*>(a))
        if (auto c2 = dynamic_cast<CapsuleCollider*>(b))
            return CapsuleVsCapsule(c1, c2);

    // Capsule vs Sphere
    if (auto c = dynamic_cast<CapsuleCollider*>(a))
        if (auto s = dynamic_cast<SphereCollider*>(b))
            return CapsuleVsSphere(c, s);

    if (auto s = dynamic_cast<SphereCollider*>(a))
        if (auto c = dynamic_cast<CapsuleCollider*>(b))
            return CapsuleVsSphere(c, s);

    // Capsule vs AABB
    if (auto c = dynamic_cast<CapsuleCollider*>(a))
        if (auto box = dynamic_cast<AABBCollider*>(b))
            return CapsuleVsAABB(c, box);

    if (auto box = dynamic_cast<AABBCollider*>(a))
        if (auto c = dynamic_cast<CapsuleCollider*>(b))
            return CapsuleVsAABB(c, box);


    return false;
}

#pragma endregion


#pragma region 各判定

#pragma region Sphere判定
//  Sphere vs Sphere 判定
bool CollisionManager::SphereVsSphere(SphereCollider* a, SphereCollider* b)
{
    VECTOR diff = VSub(a->GetWorldCenter(), b->GetWorldCenter());
    float distSq = VDot(diff, diff);

    float r = a->GetRadius() + b->GetRadius();

    return distSq <= r * r;
}

//  Sphere vs AABB 判定
bool CollisionManager::SphereVsAABB(SphereCollider* s, AABBCollider* b)
{
    VECTOR center = s->GetWorldCenter();
    VECTOR min = b->GetMin();
    VECTOR max = b->GetMax();

    VECTOR closest;

    // clamp
    closest.x = (center.x < min.x) ? min.x : (center.x > max.x) ? max.x : center.x;
    closest.y = (center.y < min.y) ? min.y : (center.y > max.y) ? max.y : center.y;
    closest.z = (center.z < min.z) ? min.z : (center.z > max.z) ? max.z : center.z;

    VECTOR diff = VSub(center, closest);
    float distSq = VDot(diff, diff);

    return distSq <= (s->GetRadius() * s->GetRadius());
}

//  Sphere vs Capsule 判定
bool CollisionManager::SphereVsCapsule(SphereCollider* _sphere, CapsuleCollider* _capsule)
{
    return false;
}

#pragma endregion

#pragma region AABB判定
//  AABB vs AABB 判定
bool CollisionManager::AABBvsAABB(AABBCollider* a, AABBCollider* b)
{
    VECTOR amin = a->GetMin();
    VECTOR amax = a->GetMax();

    VECTOR bmin = b->GetMin();
    VECTOR bmax = b->GetMax();

    return (
        amin.x <= bmax.x && amax.x >= bmin.x &&
        amin.y <= bmax.y && amax.y >= bmin.y &&
        amin.z <= bmax.z && amax.z >= bmin.z
        );
}

#pragma endregion

#pragma region Capsule判定
//  Capsule vs Sphere 判定
bool CollisionManager::CapsuleVsSphere(CapsuleCollider* _capsule, SphereCollider* _sphere)
{

    VECTOR p1 = _capsule->GetWorldStart();
    VECTOR p2 = _capsule->GetWorldEnd();
    VECTOR center = _sphere->GetWorldCenter();

    VECTOR seg = VSub(p2, p1);
    VECTOR toCenter = VSub(center, p1);

    float t = VDot(toCenter, seg) / VDot(seg, seg);

    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    VECTOR closest = VAdd(p1, VScale(seg, t));

    VECTOR diff = VSub(center, closest);
    float distSq = VDot(diff, diff);

    float r = _capsule->GetRadius() + _sphere->GetRadius();

    return distSq <= r * r;

}

//  Capsule vs AABB 判定
bool CollisionManager::CapsuleVsAABB(CapsuleCollider* _capsule, AABBCollider* _box)
{


    VECTOR p1 = _capsule->GetWorldStart();
    VECTOR p2 = _capsule->GetWorldEnd();

    VECTOR bmin = _box->GetMin();
    VECTOR bmax = _box->GetMax();
    // 線分とAABBの最短距離を求める
    // 近似ではなく「最近点」を取る

    // 線分上の最近点を探すためのパラメータ
    const int div = 10; // 精度

    float minDistSq = FLT_MAX;

    for (int i = 0; i <= div; i++)
    {
        float t = (float)i / div;
        VECTOR point = VAdd(p1, VScale(VSub(p2, p1), t));

        // AABBにクランプ（最近点）
        VECTOR closest;
        closest.x = max(bmin.x, min(point.x, bmax.x));
        closest.y = max(bmin.y, min(point.y, bmax.y));
        closest.z = max(bmin.z, min(point.z, bmax.z));

        VECTOR diff = VSub(point, closest);
        float distSq = VDot(diff, diff);

        if (distSq < minDistSq)
            minDistSq = distSq;
    }

    float r = _capsule->GetRadius();

    return minDistSq <= r * r;


}

//  Capsule vs Capsule 判定
bool CollisionManager::CapsuleVsCapsule(CapsuleCollider* _capsule, CapsuleCollider* _anotherCapsule)
{
    float dist = Segment_Segment_MinLength(
        _capsule->GetWorldStart(),
        _capsule->GetWorldEnd(),
        _anotherCapsule->GetWorldStart(),
        _anotherCapsule->GetWorldEnd()
    );

    return dist <= (_capsule->GetRadius() + _anotherCapsule->GetRadius());

}
#pragma endregion

#pragma endregion

#pragma region 押し出し

// Sphere vs Sphere 押し出し
void CollisionManager::ResolveSphereSphere(SphereCollider* a, SphereCollider* b)
{
    VECTOR pos1 = a->GetWorldCenter();
    VECTOR pos2 = b->GetWorldCenter();

    VECTOR diff = VSub(pos2, pos1);
    float dist = VSize(diff);

    if (dist == 0.0f) return;

    float overlap = (a->GetRadius() + b->GetRadius()) - dist;
    if (overlap <= 0.0f) return;

    VECTOR dir = VNorm(diff);

    pos2 = VAdd(pos2, VScale(dir, overlap));
}


//  Sphere vs AABB 押し出し
void CollisionManager::ResolveSphereAABB(SphereCollider* s, AABBCollider* b)
{
    VECTOR center = s->GetWorldCenter();
    VECTOR min = b->GetMin();
    VECTOR max = b->GetMax();

    VECTOR closest;

    closest.x = (center.x < min.x) ? min.x : (center.x > max.x) ? max.x : center.x;
    closest.y = (center.y < min.y) ? min.y : (center.y > max.y) ? max.y : center.y;
    closest.z = (center.z < min.z) ? min.z : (center.z > max.z) ? max.z : center.z;

    VECTOR diff = VSub(center, closest);
    float dist = VSize(diff);

    if (dist == 0.0f) return;

    float overlap = s->GetRadius() - dist;
    if (overlap <= 0.0f) return;

    VECTOR dir = VNorm(diff);

    center = VAdd(center, VScale(dir, overlap));
}


void CollisionManager::ResolveCapsuleAABB(CapsuleCollider* cap, AABBCollider* box)
{
    VECTOR p1 = cap->GetWorldStart();
    float r = cap->GetRadius();

    float bottom = p1.y - r;
    float boxTop = box->GetMax().y;

    if (bottom < boxTop)
    {
        float push = boxTop - bottom;

        cap->Move(VGet(0, push, 0));
    }
}

#pragma endregion


#pragma region 描画
// 描画
void CollisionManager::Render()
{
    for (auto col : colliders)
    {
        if (!col) continue;
        col->Render();
    }
}

#pragma endregion