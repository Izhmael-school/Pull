#include "CollisionManager.h"

CollisionManager::~CollisionManager() {
	pColliderArray.clear();
}

#pragma region 登録

void CollisionManager::Register(Collider* _pCol) {
	if (!_pCol) return;

	pColliderArray.push_back(_pCol);
}


void CollisionManager::CheckRegister(Collider* _pCol) {
	if (!_pCol) return;

	for (auto c : pColliderArray) {
		if (c == _pCol) return;
	}

	Register(_pCol);
}
#pragma endregion

#pragma region 削除
void CollisionManager::UnRegister(Collider* _pCol) {
	auto itr = std::find(pColliderArray.begin(), pColliderArray.end(), _pCol);
	if (itr == pColliderArray.end()) return;

	pColliderArray.erase(itr);
}

void CollisionManager::UnRegisterAll() {
	pColliderArray.clear();

	prevs.clear();
	currents.clear();
}

#pragma endregion

#pragma region 更新

void CollisionManager::Update() {

	for (auto col : pColliderArray) {
		if (col && col->IsEnable()) {
			col->Update();
		}
	}

	int n = pColliderArray.size();

	static int prevSize = -1;

	if (prevSize != n) {
		prevs.assign(n, std::vector<bool>(n, false));
		currents.assign(n, std::vector<bool>(n, false));

		prevSize = n;
	}

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			Collider* a = pColliderArray[i];
			Collider* b = pColliderArray[j];

			if (!a || !b) continue;

			auto goA = a->GetGameObject();
			auto goB = b->GetGameObject();

			if (!goA || !goB) continue;
			if (!goA->IsActive() || !goB->IsActive()) continue;

			currents[i][j] = CheckHit(a, b);

			if (!prevs[i][j] && currents[i][j]) {
				goA->OnTriggerEnter(b);
				goB->OnTriggerEnter(a);
			}
			else if (currents[i][j]) {
				goA->OnTriggerStay(b);
				goB->OnTriggerStay(a);
			}
			else if (prevs[i][j] && !currents[i][j]) {
				goA->OnTriggerExit(b);
				goB->OnTriggerExit(a);
			}

			prevs[i][j] = currents[i][j];
		}
	}
}

#pragma endregion

#pragma region 判定
bool CollisionManager::CheckHit(Collider* a, Collider* b) {
	if (auto s1 = dynamic_cast<SphereCollider*>(a))
		if (auto s2 = dynamic_cast<SphereCollider*>(b))
			return SphereVsSphere(s1, s2);

	if (auto b1 = dynamic_cast<AABBCollider*>(a))
		if (auto b2 = dynamic_cast<AABBCollider*>(b))
			return AABBvsAABB(b1, b2);

	if (auto s = dynamic_cast<SphereCollider*>(a))
		if (auto box = dynamic_cast<AABBCollider*>(b))
			return SphereVsAABB(s, box);

	if (auto box = dynamic_cast<AABBCollider*>(a))
		if (auto s = dynamic_cast<SphereCollider*>(b))
			return SphereVsAABB(s, box);

	if (auto c1 = dynamic_cast<CapsuleCollider*>(a))
		if (auto c2 = dynamic_cast<CapsuleCollider*>(b))
			return CapsuleVsCapsule(c1, c2);

	if (auto c = dynamic_cast<CapsuleCollider*>(a))
		if (auto s = dynamic_cast<SphereCollider*>(b))
			return CapsuleVsSphere(c, s);

	if (auto s = dynamic_cast<SphereCollider*>(a))
		if (auto c = dynamic_cast<CapsuleCollider*>(b))
			return CapsuleVsSphere(c, s);


	if (auto c = dynamic_cast<CapsuleCollider*>(a))
		if (auto box = dynamic_cast<AABBCollider*>(b))
			return CapsuleVsAABB(c, box);

	if (auto box = dynamic_cast<AABBCollider*>(a))
		if (auto c = dynamic_cast<CapsuleCollider*>(b))
			return CapsuleVsAABB(c, box);

	return false;
}
#pragma endregion

#pragma region 判定処理

bool CollisionManager::SphereVsSphere(SphereCollider* a, SphereCollider* b) {
	VECTOR diff = VSub(a->GetWorldCenter(), b->GetWorldCenter());
	float distSq = VDot(diff, diff);
	float r = a->GetRadius() + b->GetRadius();
	return distSq <= r * r;
}

bool CollisionManager::SphereVsAABB(SphereCollider* s, AABBCollider* b) {
	VECTOR center = s->GetWorldCenter();
	VECTOR min = b->GetMin();
	VECTOR max = b->GetMax();

	VECTOR closest;
	closest.x = max(min.x, min(center.x, max.x));
	closest.y = max(min.y, min(center.y, max.y));
	closest.z = max(min.z, min(center.z, max.z));

	VECTOR diff = VSub(center, closest);
	float distSq = VDot(diff, diff);

	return distSq <= (s->GetRadius() * s->GetRadius());
}

bool CollisionManager::AABBvsAABB(AABBCollider* a, AABBCollider* b) {
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

bool CollisionManager::CapsuleVsSphere(CapsuleCollider* cap, SphereCollider* sph) {
	VECTOR p1 = cap->GetWorldStart();
	VECTOR p2 = cap->GetWorldEnd();
	VECTOR center = sph->GetWorldCenter();

	VECTOR seg = VSub(p2, p1);
	VECTOR toCenter = VSub(center, p1);

	float t = VDot(toCenter, seg) / VDot(seg, seg);
	t = max(0.0f, min(1.0f, t));

	VECTOR closest = VAdd(p1, VScale(seg, t));

	VECTOR diff = VSub(center, closest);
	float distSq = VDot(diff, diff);

	float r = cap->GetRadius() + sph->GetRadius();
	return distSq <= r * r;
}

bool CollisionManager::CapsuleVsAABB(CapsuleCollider* cap, AABBCollider* box) {

	VECTOR p1 = cap->GetWorldStart();
	VECTOR p2 = cap->GetWorldEnd();

	VECTOR min = box->GetMin();
	VECTOR max = box->GetMax();

	// カプセルの線分を何分割かしてチェック（簡易だけど強い）
	const int steps = 5;

	for (int i = 0; i <= steps; i++) {
		float t = (float)i / steps;

		VECTOR point = VAdd(p1, VScale(VSub(p2, p1), t));

		// point vs AABB（最近接点）
		VECTOR closest;
		closest.x = max(min.x, min(point.x, max.x));
		closest.y = max(min.y, min(point.y, max.y));
		closest.z = max(min.z, min(point.z, max.z));

		VECTOR diff = VSub(point, closest);
		float distSq = VDot(diff, diff);

		if (distSq <= cap->GetRadius() * cap->GetRadius()) {
			return true;
		}
	}

	return false;

}

bool CollisionManager::CapsuleVsCapsule(CapsuleCollider* a, CapsuleCollider* b) {
	float dist = Segment_Segment_MinLength(
		a->GetWorldStart(), a->GetWorldEnd(),
		b->GetWorldStart(), b->GetWorldEnd()
	);

	return dist <= (a->GetRadius() + b->GetRadius());
}

#pragma endregion

#pragma region 描画
void CollisionManager::Render() {
	for (auto col : pColliderArray) {
		if (!col) continue;
		col->Render();
	}
}
#pragma endregion

void CollisionManager::Clear() {
	pColliderArray.clear();
	prevs.clear();
	currents.clear();
}
