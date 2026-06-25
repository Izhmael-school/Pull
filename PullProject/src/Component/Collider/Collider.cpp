#include "Collider.h"
#include "Definition/Const/ColorConst.h"
#include "Manager/CollisionManager.h"
#pragma region Collider

/*
 * @file Collider.cpp
 * @author Tanaka
 */

 // コンストラクタ
Collider::Collider(GameObject* _pObj)
	: isEnable(true)
	, pGameObject(_pObj)
	, layer(ColliderLayer::Default) {
	CollisionManager::GetInstance().Register(this);
}

// デストラクタ
Collider::~Collider() {
}

#pragma endregion


#pragma region AABBCollider
AABBCollider::AABBCollider(GameObject* obj, VECTOR min, VECTOR max)
	: Collider(obj) {
	localMin = min;
	localMax = max;
	worldMin = min;
	worldMax = max;
}

void AABBCollider::Update() {
	if (!pGameObject) return;
	VECTOR pos = pGameObject->GetPosition();
	worldMin = VAdd(localMin, pos);
	worldMax = VAdd(localMax, pos);
}

void AABBCollider::Render() {
	DrawCube3D(worldMin, worldMax, red, red, FALSE);
}

VECTOR AABBCollider::GetMin() const {
	return worldMin;
}

VECTOR AABBCollider::GetMax() const {
	return worldMax;
}

void AABBCollider::SetMin(VECTOR min) {
	localMin = min;
	worldMin = min;
}

void AABBCollider::SetMax(VECTOR max) {
	localMax = max;
	worldMax = max;
}

void AABBCollider::Move(VECTOR offset) {
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
	, radius(_radius) {
}

// 更新
void SphereCollider::Update() {
	VECTOR pos = VZero;

	if (pGameObject)
		pos = pGameObject->GetPosition();

	worldCenter = VAdd(localCenter, pos);
}

// 描画
void SphereCollider::Render() {
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
	, radius(_radius) {
}

// 更新
void CapsuleCollider::Update() {

	VECTOR pos = VZero;

	if (pGameObject)
		pos = pGameObject->GetPosition();

	if (!hasErrorShown &&
	localStart.x == localEnd.x &&
	localStart.y == localEnd.y &&
	localStart.z == localEnd.z) {
		printfDx("CapsuleCollider Error: localStart and localEnd are the same on %s\n");

		hasErrorShown = true; // 次回以降は表示しない
	}

	worldStart = VAdd(localStart, pos);
	worldEnd = VAdd(localEnd, pos);
}

// 描画
void CapsuleCollider::Render() {
	if (!isEnable) return;

	DrawCapsule3D(worldStart, worldEnd, radius, 16, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);
}


void CapsuleCollider::Move(VECTOR offset) {
	localStart = VAdd(localStart, offset);
	localEnd = VAdd(localEnd, offset);
}
#pragma endregion




RayCollider::RayCollider(GameObject* owner, VECTOR ori, VECTOR dir, float len, float h, float angDeg, float _bottomOffset)
	: Collider(owner),
	origin(ori),
	direction(VNorm(dir)),
	length(len),
	height(h),
	angle(angDeg)
	, worldDirection(VZero) 
	, worldOrigin(VZero)
	, bottomOffset(_bottomOffset) {
}

void RayCollider::Update() {
	VECTOR pos = GetGameObject()->GetTransform()->GetPosition();
	worldOrigin = VAdd(pos, origin);

	MATRIX mat = GetGameObject()->GetTransform()->GetMatrix();
	worldDirection = VGet(mat.m[2][0], mat.m[2][1], mat.m[2][2]);

	if (VSize(worldDirection) < 0.0001f) {
		worldDirection = VGet(0, 0, 1); // デフォルト方向
	}
	else {
		worldDirection = VNorm(worldDirection);
	}
}

void RayCollider::Render() {

	if (VSize(worldDirection) < 0.0001f) {
		worldDirection = VGet(0, 0, 1);
	}

	float baseRad = atan2f(worldDirection.x, worldDirection.z);
	if (isnan(baseRad)) baseRad = 0.0f;

	// ★ bottomOffset を基準に上下を決める
	float bottom = bottomOffset;
	float top = bottomOffset + height;

	VECTOR bottomOrigin = VAdd(worldOrigin, VGet(0, bottom, 0));
	VECTOR topOrigin = VAdd(worldOrigin, VGet(0, top, 0));

	float halfRad = angle * 0.5f * DX_PI_F / 180.0f;
	unsigned int col = GetColor(0, 255, 0);

	// 中心線（上下）
	DrawLine3D(bottomOrigin, VAdd(bottomOrigin, VScale(worldDirection, length)), col);
	DrawLine3D(topOrigin, VAdd(topOrigin, VScale(worldDirection, length)), col);

	float leftRad = baseRad - halfRad;
	float rightRad = baseRad + halfRad;

	VECTOR leftDir = VGet(sinf(leftRad), 0, cosf(leftRad));
	VECTOR rightDir = VGet(sinf(rightRad), 0, cosf(rightRad));

	// 左右端線（上下）
	DrawLine3D(bottomOrigin, VAdd(bottomOrigin, VScale(leftDir, length)), col);
	DrawLine3D(bottomOrigin, VAdd(bottomOrigin, VScale(rightDir, length)), col);

	DrawLine3D(topOrigin, VAdd(topOrigin, VScale(leftDir, length)), col);
	DrawLine3D(topOrigin, VAdd(topOrigin, VScale(rightDir, length)), col);

	// 扇形の外周（円弧）上下
	const int div = 20;

	for (int i = 0; i < div; i++) {
		float t1 = (float)i / div;
		float t2 = (float)(i + 1) / div;

		float arcRad1 = baseRad - halfRad + (halfRad * 2.0f) * t1;
		float arcRad2 = baseRad - halfRad + (halfRad * 2.0f) * t2;

		VECTOR p1_top = VAdd(topOrigin, VScale(VGet(sinf(arcRad1), 0, cosf(arcRad1)), length));
		VECTOR p2_top = VAdd(topOrigin, VScale(VGet(sinf(arcRad2), 0, cosf(arcRad2)), length));

		VECTOR p1_bottom = VAdd(bottomOrigin, VScale(VGet(sinf(arcRad1), 0, cosf(arcRad1)), length));
		VECTOR p2_bottom = VAdd(bottomOrigin, VScale(VGet(sinf(arcRad2), 0, cosf(arcRad2)), length));

		DrawLine3D(p1_top, p2_top, col);
		DrawLine3D(p1_bottom, p2_bottom, col);
	}

	// 上下の側面
	DrawLine3D(VAdd(topOrigin, VScale(leftDir, length)),
			   VAdd(bottomOrigin, VScale(leftDir, length)), col);

	DrawLine3D(VAdd(topOrigin, VScale(rightDir, length)),
			   VAdd(bottomOrigin, VScale(rightDir, length)), col);
}

bool RayCollider::CheckHitPoint(VECTOR target) {
	float bottom = bottomOffset;
	float top = bottomOffset + height;

	float localY = target.y - worldOrigin.y;

	if (localY < bottom || localY > top)
		return false;

	VECTOR toPoint = VSub(target, worldOrigin);
	toPoint.y = 0;

	float dist = VSize(toPoint);
	if (dist > length)
		return false;

	if (dist < 0.0001f)
		return true;

	VECTOR dir = worldDirection;
	dir.y = 0;
	dir = VNorm(dir);

	VECTOR nToPoint = VNorm(toPoint);

	float dot = VDot(nToPoint, dir);

	float halfRad = angle * 0.5f * DX_PI_F / 180.0f;
	float fanCos = cosf(halfRad);

	if (dot < fanCos)
		return false;

	return true;
}

