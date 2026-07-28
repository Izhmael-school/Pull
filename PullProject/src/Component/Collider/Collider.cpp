#include "Collider.h"
#include "Definition/Const/ColorConst.h"
#include "Manager/CollisionManager.h"
#include "../../Definition/CommonModule/MyMath.h"
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

/*
 *	回転行列を作成
 */
void AABBCollider::RotateBounds(VECTOR& min, VECTOR& max, const VECTOR& rotation) {
	// 頂点数
	VECTOR corners[8];
	// AABBの八頂点を作成
	CreateCorners(min, max, corners);

	// 回転行列を作成
	MATRIX rotX = MGetRotX(MyMath::Deg2Rad(rotation.x));
	MATRIX rotY = MGetRotY(MyMath::Deg2Rad(rotation.y));
	MATRIX rotZ = MGetRotZ(MyMath::Deg2Rad(rotation.z));

	// Transformと同じ回転軸
	MATRIX mat = MMult(MMult(rotZ, rotX), rotY);

	// 八頂点を回転させる
	for (int i = 0; i < 8; i++) {
		corners[i] = VTransform(corners[i], mat);
	}
	// 回転後のAABBを求める
	CalculateBounds(corners, min, max);
}

/*
 *	AABBの八頂点を作成
 *  @param[in] const VECTOR& 最小点
 *  @param[in] const VECTOR& 最大点
 *  @param[in] VECTOR	     頂点数
 */
void AABBCollider::CreateCorners(const VECTOR& min, const VECTOR& max, VECTOR corners[8]) {
	// AABBの八頂点を作成
	corners[0] = VGet(min.x, min.y, min.z);
	corners[1] = VGet(max.x, min.y, min.z);
	corners[2] = VGet(min.x, max.y, min.z);
	corners[3] = VGet(max.x, max.y, min.z);

	corners[4] = VGet(min.x, min.y, max.z);
	corners[5] = VGet(max.x, min.y, max.z);
	corners[6] = VGet(min.x, max.y, max.z);
	corners[7] = VGet(max.x, max.y, max.z);
}

/*
 *	回転後のAABBを求める
 *  @param[in]	const VECTOR 頂点数
 *  @param[in]	VECTOR&		 最小点
 *  @param[in]	VECTOR&		 最大点
 */
void AABBCollider::CalculateBounds(const VECTOR corners[8], VECTOR& min, VECTOR& max) {
	min = corners[0];
	max = corners[0];

	//　最大、最小点を更新する
	for (int i = 1; i < 8; i++) {
		min.x = std::min(min.x, corners[i].x);
		min.y = std::min(min.y, corners[i].y);
		min.z = std::min(min.z, corners[i].z);

		max.x = std::max(max.x, corners[i].x);
		max.y = std::max(max.y, corners[i].y);
		max.z = std::max(max.z, corners[i].z);
	}
}

bool AABBCollider::Raycast(
	const Ray& ray,
	float maxDistance,
	RayHit& hit) {
	const float EPS = 0.00001f;

	VECTOR dir = VNorm(ray.direction);

	float tMin = 0.0f;
	float tMax = maxDistance;

	// X
	if (fabsf(dir.x) < EPS) {
		if (ray.origin.x < worldMin.x ||
			ray.origin.x > worldMax.x) {
			return false;
		}
	}
	else {
		float tx1 =
			(worldMin.x - ray.origin.x) / dir.x;

		float tx2 =
			(worldMax.x - ray.origin.x) / dir.x;

		tMin = std::max(
			tMin,
			std::min(tx1, tx2));

		tMax = std::min(
			tMax,
			std::max(tx1, tx2));
	}

	// Y
	if (fabsf(dir.y) < EPS) {
		if (ray.origin.y < worldMin.y ||
			ray.origin.y > worldMax.y) {
			return false;
		}
	}
	else {
		float ty1 =
			(worldMin.y - ray.origin.y) / dir.y;

		float ty2 =
			(worldMax.y - ray.origin.y) / dir.y;

		tMin = std::max(
			tMin,
			std::min(ty1, ty2));

		tMax = std::min(
			tMax,
			std::max(ty1, ty2));
	}

	// Z
	if (fabsf(dir.z) < EPS) {
		if (ray.origin.z < worldMin.z ||
			ray.origin.z > worldMax.z) {
			return false;
		}
	}
	else {
		float tz1 =
			(worldMin.z - ray.origin.z) / dir.z;

		float tz2 =
			(worldMax.z - ray.origin.z) / dir.z;

		tMin = std::max(
			tMin,
			std::min(tz1, tz2));

		tMax = std::min(
			tMax,
			std::max(tz1, tz2));
	}

	if (tMax < tMin)
		return false;

	hit.hit = true;
	hit.distance = tMin;

	hit.point =
		VAdd(
			ray.origin,
			VScale(dir, tMin));

	hit.collider = this;

	return true;
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

bool SphereCollider::Raycast(
	const Ray& ray,
	float maxDistance,
	RayHit& hit) {
	VECTOR dir = VNorm(ray.direction);

	VECTOR toCenter =
		VSub(worldCenter, ray.origin);

	float proj =
		VDot(toCenter, dir);

	if (proj < 0.0f)
		return false;

	if (proj > maxDistance)
		return false;

	VECTOR closest =
		VAdd(
			ray.origin,
			VScale(dir, proj));

	float dist =
		VSize(
			VSub(
				worldCenter,
				closest));

	if (dist > radius)
		return false;

	hit.hit = true;
	hit.distance = proj;
	hit.point = closest;
	hit.collider = this;

	return true;
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

bool CapsuleCollider::Raycast(
	const Ray& ray,
	float maxDistance,
	RayHit& hit) {
	VECTOR dir = VNorm(ray.direction);

	VECTOR capsuleDir =
		VSub(worldEnd, worldStart);

	float capsuleLen =
		VSize(capsuleDir);

	if (capsuleLen < 0.0001f)
		return false;

	capsuleDir =
		VScale(
			capsuleDir,
			1.0f / capsuleLen);

	// Ray上の最近点
	float proj =
		VDot(
			VSub(worldStart, ray.origin),
			dir);

	if (proj < 0.0f)
		proj = 0.0f;

	if (proj > maxDistance)
		return false;

	VECTOR rayPoint =
		VAdd(
			ray.origin,
			VScale(dir, proj));

	// Capsule軸への射影
	float capsuleProj =
		VDot(
			VSub(rayPoint, worldStart),
			capsuleDir);

	capsuleProj =
		std::max(
			0.0f,
			std::min(
				capsuleProj,
				capsuleLen));

	VECTOR capsulePoint =
		VAdd(
			worldStart,
			VScale(
				capsuleDir,
				capsuleProj));

	float dist =
		VSize(
			VSub(
				rayPoint,
				capsulePoint));

	if (dist > radius)
		return false;

	hit.hit = true;
	hit.distance = proj;
	hit.point = rayPoint;
	hit.collider = this;

	return true;
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
	auto trans = GetGameObject()->GetTransform();

	worldOrigin = VAdd(
		trans->GetPosition(),
		origin
	);

	worldDirection = VNorm(trans->GetForward());

	if (GetLayer() == ColliderLayer::PlayerRay) {
		worldDirection = VScale(worldDirection, -1.0f);
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

bool RayCollider::CheckHitLine(
	VECTOR start,
	VECTOR end) {
	if (CheckHitPoint(start))
		return true;

	if (CheckHitPoint(end))
		return true;

	float halfRad =
		angle * 0.5f *
		DX_PI_F / 180.0f;

	float baseRad =
		atan2f(
			worldDirection.x,
			worldDirection.z);

	VECTOR leftDir =
		VGet(
			sinf(baseRad - halfRad),
			0,
			cosf(baseRad - halfRad));

	VECTOR rightDir =
		VGet(
			sinf(baseRad + halfRad),
			0,
			cosf(baseRad + halfRad));

	VECTOR leftEnd =
		VAdd(
			worldOrigin,
			VScale(leftDir, length));

	VECTOR rightEnd =
		VAdd(
			worldOrigin,
			VScale(rightDir, length));

	// 左境界
	if (SegmentIntersect2D(
		worldOrigin,
		leftEnd,
		start,
		end)) {
		return true;
	}

	// 右境界
	if (SegmentIntersect2D(
		worldOrigin,
		rightEnd,
		start,
		end)) {
		return true;
	}

	return false;
}

bool RayCollider::CheckHitAABB(
	const VECTOR& min,
	const VECTOR& max) {
	// 8頂点
	VECTOR v[8] =
	{
		VGet(min.x, min.y, min.z),
		VGet(max.x, min.y, min.z),
		VGet(max.x, min.y, max.z),
		VGet(min.x, min.y, max.z),

		VGet(min.x, max.y, min.z),
		VGet(max.x, max.y, min.z),
		VGet(max.x, max.y, max.z),
		VGet(min.x, max.y, max.z)
	};

	// 頂点が扇形柱内
	for (int i = 0; i < 8; i++) {
		if (CheckHitPoint(v[i])) {
			return true;
		}
	}

	// 原点がAABB内
	if (
		worldOrigin.x >= min.x &&
		worldOrigin.x <= max.x &&
		worldOrigin.y >= min.y &&
		worldOrigin.y <= max.y &&
		worldOrigin.z >= min.z &&
		worldOrigin.z <= max.z
	) {
		return true;
	}

	// AABBの12辺
	const int edges[12][2] =
	{
		{0,1},{1,2},{2,3},{3,0}, // 下

		{4,5},{5,6},{6,7},{7,4}, // 上

		{0,4},{1,5},{2,6},{3,7}  // 縦
	};

	for (int i = 0; i < 12; i++) {
		if (CheckHitLine(
			v[edges[i][0]],
			v[edges[i][1]])) {
			return true;
		}
	}

	return false;
}