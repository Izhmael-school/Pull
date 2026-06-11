#include "Collider.h"
#include "../../Definition/Const/ColorConst.h"
#pragma region Collider

/*
 * @file Collider.cpp
 * @author Tanaka
 */

// コンストラクタ
Collider::Collider(GameObject* _pObj)
    : isEnable(true)
    , pGameObject(_pObj)
{
}

// デストラクタ
Collider::~Collider()
{
}

#pragma endregion


#pragma region AABBCollider

// コンストラクタ
AABBCollider::AABBCollider(GameObject* obj, VECTOR min, VECTOR max)
    : Collider(obj)
    , localMin(min)
    , localMax(max)
    , worldMin(VZero)
    , worldMax(VZero)
{
}

// 更新
void AABBCollider::Update()
{


    VECTOR pos = VZero;

    if (pGameObject)
        pos = pGameObject->GetPosition();

    worldMin = VAdd(localMin, pos);
    worldMax = VAdd(localMax, pos);


    /*if (!isEnable) return;

    VECTOR pos = pGameObject->GetPosition();

    worldMin = VAdd(localMin, pos);
    worldMax = VAdd(localMax, pos);*/

}

// 描画（デバッグ用）
void AABBCollider::Render()
{
    if (!isEnable) return;

    // 8頂点をその場で生成
    VECTOR v[8];

    v[0] = worldMin;
    v[1] = VGet(worldMax.x, worldMin.y, worldMin.z);
    v[2] = VGet(worldMin.x, worldMax.y, worldMin.z);
    v[3] = VGet(worldMin.x, worldMin.y, worldMax.z);
    v[4] = VGet(worldMin.x, worldMax.y, worldMax.z);
    v[5] = VGet(worldMax.x, worldMin.y, worldMax.z);
    v[6] = VGet(worldMax.x, worldMax.y, worldMin.z);
    v[7] = worldMax;

    // 線で描画
    DrawLine3D(v[0], v[1], red);
    DrawLine3D(v[1], v[6], red);
    DrawLine3D(v[6], v[2], red);
    DrawLine3D(v[2], v[0], red);
                          
    DrawLine3D(v[3], v[5], red);
    DrawLine3D(v[5], v[7], red);
    DrawLine3D(v[7], v[4], red);
    DrawLine3D(v[4], v[3], red);
                           
    DrawLine3D(v[0], v[3], red);
    DrawLine3D(v[1], v[5], red);
    DrawLine3D(v[2], v[4], red);
    DrawLine3D(v[6], v[7], red);
}

#pragma endregion


#pragma region SphereCollider

// コンストラクタ
SphereCollider::SphereCollider(GameObject* _pObj, VECTOR _localCenter, float _radius)
    : Collider(_pObj)
    , localCenter(_localCenter)
    , worldCenter(VZero)
    , radius(_radius)
{
}

// 更新
void SphereCollider::Update()
{

    VECTOR pos = VZero;

    if (pGameObject)
        pos = pGameObject->GetPosition();

    worldCenter = VAdd(localCenter, pos);


   /* if (!isEnable) return;

    worldCenter = VAdd(pGameObject->GetPosition(), localCenter);*/
}

// 描画
void SphereCollider::Render()
{
    if (!isEnable) return;

    DrawSphere3D(worldCenter, radius, 16, GetColor(0, 255, 0), GetColor(0, 0, 0), FALSE);
}

#pragma endregion


#pragma region CapsuleCollider
// コンストラクタ
CapsuleCollider::CapsuleCollider(GameObject* _pObj, VECTOR _localStart, VECTOR _localEnd, float _radius, VECTOR _offset)
    : Collider(_pObj)
    , localStart(_localStart)
    , localEnd(_localEnd)
    , worldStart(VZero)
    , worldEnd(VZero)
    , radius(_radius)
{
}

// 更新
void CapsuleCollider::Update()
{

    VECTOR pos = VZero;

    if (pGameObject)
        pos = pGameObject->GetPosition();

    worldStart = VAdd(localStart, pos);
    worldEnd = VAdd(localEnd, pos);

    /*if (!isEnable) return;

    VECTOR pos = pGameObject->GetPosition();

    worldStart = VAdd(localStart, pos);
    worldEnd = VAdd(localEnd, pos);*/
}

// 描画
void CapsuleCollider::Render()
{
    if (!isEnable) return;

    DrawCapsule3D(worldStart, worldEnd, radius, 16, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);
}

#pragma endregion