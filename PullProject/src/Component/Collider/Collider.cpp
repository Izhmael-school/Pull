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


AABBCollider::AABBCollider()
	: Collider(nullptr)
{
	localMin = VGet(0, 0, 0);
	localMax = VGet(0, 0, 0);
	worldMin = localMin;
	worldMax = localMax;
}

AABBCollider::AABBCollider(GameObject* obj, VECTOR min, VECTOR max)
	: Collider(obj)
{
	localMin = min;
	localMax = max;
	worldMin = min;
	worldMax = max;
}

void AABBCollider::Update()
{
	// シンプル同期
	worldMin = localMin;
	worldMax = localMax;
}

void AABBCollider::Render()
{
	DrawCube3D(worldMin, worldMax, red, red, FALSE);
}

VECTOR AABBCollider::GetMin() const
{
	return worldMin;
}

VECTOR AABBCollider::GetMax() const
{
	return worldMax;
}

void AABBCollider::SetMin(VECTOR min)
{
	localMin = min;
	worldMin = min;
}

void AABBCollider::SetMax(VECTOR max)
{
	localMax = max;
	worldMax = max;
}

void AABBCollider::Move(VECTOR offset)
{
	localMin = VAdd(localMin, offset);
	localMax = VAdd(localMax, offset);
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

	DrawSphere3D(worldCenter, radius, 16, GetColor(0, 255, 0), GetColor(0, 0, 0), TRUE);
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


void CapsuleCollider::Move(VECTOR offset)
{
	localStart = VAdd(localStart, offset);
	localEnd = VAdd(localEnd, offset);
}


#pragma endregion