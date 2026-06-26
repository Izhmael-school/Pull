#include "CollisionManager.h"
#include "Definition/CommonModule/MyJson.h"
#include <map>

static const std::unordered_map<std::string, HitFunc> funcMap = {
	{ "SphereVsSphere",   &CollisionManager::SphereVsSphere },
	{ "SphereVsAABB",     &CollisionManager::SphereVsAABB },
	{ "AABBvsAABB",       &CollisionManager::AABBvsAABB },
	{ "CapsuleVsSphere",  &CollisionManager::CapsuleVsSphere },
	{ "CapsuleVsAABB",    &CollisionManager::CapsuleVsAABB },
	{ "CapsuleVsCapsule", &CollisionManager::CapsuleVsCapsule },
	{ "RayVsSphere",	  &CollisionManager::RayVsSphere },
	{ "RayVsAABB",        &CollisionManager::RayVsAABB },
	{ "RayVsCapsule",	  &CollisionManager::RayVsCapsule}
};

static const std::unordered_map<std::string, ResolveFunc> resolveFuncMap = {
	{ "SphereVsSphere",   &CollisionManager::ResolveSphereSphere },
	{ "SphereVsAABB",     &CollisionManager::ResolveSphereAABB },
	{ "CapsuleVsAABB",    &CollisionManager::ResolveCapsuleAABB }
};

CollisionManager::CollisionManager() {
	LoadCollisionRules("src/Data/collision_rules.json");
}
CollisionManager::~CollisionManager() {
	UnRegisterAll();
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


	int stage = 0, player = 0, enemy = 0, other = 0;

	for (auto c : pColliderArray) {
		switch (c->GetLayer()) {
		case ColliderLayer::Stage: stage++; break;
		case ColliderLayer::Player: player++; break;
		case ColliderLayer::Enemy: enemy++; break;
		default: other++; break;
		}
	}

	//printfDx("Stage:%d Player:%d Enemy:%d Other:%d\n", stage, player, enemy, other);

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

			bool hit = CheckHit(a, b);
			currents[i][j] = hit;

			if (hit) {

				if (a->IsResolve() && b->IsResolve()) {
					Resolve(a, b);
				}
			}

			if (!prevs[i][j] && hit) {
				goA->OnTriggerEnter(b);
				goB->OnTriggerEnter(a);
			}
			else if (prevs[i][j] && hit) {
				goA->OnTriggerStay(b);
				goB->OnTriggerStay(a);
			}
			else if (prevs[i][j] && !hit) {
				goA->OnTriggerExit(b);
				goB->OnTriggerExit(a);
			}

			prevs[i][j] = hit;

		}
	}
}

#pragma endregion

#pragma region 判定
bool CollisionManager::CheckHit(Collider* a, Collider* b) {
	if (a->GetLayer() == ColliderLayer::Stage &&
	b->GetLayer() == ColliderLayer::Stage)
		return false;

	std::string key =
		std::string(a->GetTypeName()) + "_" + b->GetTypeName();

	auto it = hitFuncTable.find(key);
	if (it == hitFuncTable.end()) return false;

	return it->second(a, b);
}
#pragma endregion

void CollisionManager::Resolve(Collider* a, Collider* b) {
	std::string key =
		std::string(a->GetTypeName()) + "_" + b->GetTypeName();

	auto it = resolveFuncTable.find(key);
	if (it == resolveFuncTable.end()) return;

	it->second(a, b);
}


void CollisionManager::LoadCollisionRules(const std::string& path) {
	auto json = MyJson::LoadJsonFile(path);

	for (auto& rule : json["collision_rules"]) {

		std::string key =
			rule["a"].get<std::string>() + "_" +
			rule["b"].get<std::string>();

		// ★ 判定関数（hit）
		std::string hitName = rule["hit"].get<std::string>();
		auto it = funcMap.find(hitName);
		if (it == funcMap.end()) {
			printfDx("Unknown hit func: %s\n", hitName.c_str());
			continue;
		}
		hitFuncTable[key] = it->second;

		// ★ 押し出し関数（resolve）
		std::string resolveName = rule["resolve"].get<std::string>();
		if (resolveName != "None") {
			auto it2 = resolveFuncMap.find(resolveName);
			if (it2 != resolveFuncMap.end()) {
				resolveFuncTable[key] = it2->second;
			}
		}
	}
}


#pragma region 判定処理

bool CollisionManager::SphereVsSphere(Collider* a, Collider* b) {
	auto sa = static_cast<SphereCollider*>(a);
	auto sb = static_cast<SphereCollider*>(b);
	VECTOR diff = VSub(sa->GetWorldCenter(), sb->GetWorldCenter());
	float distSq = VDot(diff, diff);
	float r = sa->GetRadius() + sb->GetRadius();
	return distSq <= r * r;
}

bool CollisionManager::SphereVsAABB(Collider* a, Collider* b) {
	auto s = static_cast<SphereCollider*>(a);
	auto box = static_cast<AABBCollider*>(b);
	VECTOR center = s->GetWorldCenter();
	VECTOR min = box->GetMin();
	VECTOR max = box->GetMax();

	VECTOR closest;
	closest.x = max(min.x, min(center.x, max.x));
	closest.y = max(min.y, min(center.y, max.y));
	closest.z = max(min.z, min(center.z, max.z));

	VECTOR diff = VSub(center, closest);
	float distSq = VDot(diff, diff);

	return distSq <= (s->GetRadius() * s->GetRadius());
}

bool CollisionManager::AABBvsAABB(Collider* a, Collider* b) {
	auto boxA = static_cast<AABBCollider*>(a);
	auto boxB = static_cast<AABBCollider*>(b);

	VECTOR amin = boxA->GetMin();
	VECTOR amax = boxA->GetMax();
	VECTOR bmin = boxB->GetMin();
	VECTOR bmax = boxB->GetMax();
	return (
		amin.x <= bmax.x && amax.x >= bmin.x &&
		amin.y <= bmax.y && amax.y >= bmin.y &&
		amin.z <= bmax.z && amax.z >= bmin.z
		);
}

bool CollisionManager::CapsuleVsSphere(Collider* a, Collider* b) {
	auto cap = static_cast<CapsuleCollider*>(a);
	auto sph = static_cast<SphereCollider*>(b);

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

bool CollisionManager::CapsuleVsAABB(Collider* a, Collider* b) {
	auto cap = static_cast<CapsuleCollider*>(a);
	auto box = static_cast<AABBCollider*>(b);

	VECTOR p1 = cap->GetWorldStart();
	VECTOR p2 = cap->GetWorldEnd();

	VECTOR min = box->GetMin();
	VECTOR max = box->GetMax();

	const int steps = 5;

	for (int i = 0; i <= steps; i++) {
		float t = (float)i / steps;

		VECTOR point = VAdd(p1, VScale(VSub(p2, p1), t));

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

bool CollisionManager::CapsuleVsCapsule(Collider* a, Collider* b) {
	auto capA = static_cast<CapsuleCollider*>(a);
	auto capB = static_cast<CapsuleCollider*>(b);

	float dist = Segment_Segment_MinLength(
		capA->GetWorldStart(), capA->GetWorldEnd(),
		capB->GetWorldStart(), capB->GetWorldEnd()
	);

	return dist <= (capA->GetRadius() + capB->GetRadius());
}

#pragma endregion

void CollisionManager::ResolveSphereSphere(Collider* aCol, Collider* bCol) {
	auto a = static_cast<SphereCollider*>(aCol);
	auto b = static_cast<SphereCollider*>(bCol);

	VECTOR pa = a->GetWorldCenter();
	VECTOR pb = b->GetWorldCenter();

	VECTOR diff = VSub(pa, pb);
	float dist = sqrtf(VDot(diff, diff));
	float r = a->GetRadius() + b->GetRadius();

	if (dist == 0.0f) return;

	float push = r - dist;
	VECTOR dir = VScale(diff, 1.0f / dist);

	VECTOR move = VScale(dir, push * 0.5f);

	a->GetGameObject()->GetTransform()->AddPosition(move);
	b->GetGameObject()->GetTransform()->AddPosition(VScale(move, -1.0f));
}

void CollisionManager::ResolveSphereAABB(Collider* sCol, Collider* boxCol) {
	auto s = static_cast<SphereCollider*>(sCol);
	auto box = static_cast<AABBCollider*>(boxCol);

	VECTOR center = s->GetWorldCenter();
	VECTOR min = box->GetMin();
	VECTOR max = box->GetMax();
	float r = s->GetRadius();

	VECTOR closest;
	closest.x = max(min.x, min(center.x, max.x));
	closest.y = max(min.y, min(center.y, max.y));
	closest.z = max(min.z, min(center.z, max.z));

	VECTOR diff = VSub(center, closest);
	float distSq = VDot(diff, diff);

	if (distSq <= 0.00001f) return;

	float dist = sqrtf(distSq);
	float push = r - dist;

	VECTOR dir = VScale(diff, 1.0f / dist);
	VECTOR move = VScale(dir, push);

	s->GetGameObject()->GetTransform()->AddPosition(move);
}

void CollisionManager::ResolveCapsuleAABB(Collider* capCol, Collider* boxCol) {
	auto cap = static_cast<CapsuleCollider*>(capCol);
	auto box = static_cast<AABBCollider*>(boxCol);

	VECTOR p1 = cap->GetWorldStart();
	VECTOR p2 = cap->GetWorldEnd();
	VECTOR min = box->GetMin();
	VECTOR max = box->GetMax();
	float r = cap->GetRadius();

	const int steps = 32;
	float bestPush = 0.0f;
	VECTOR bestMove = VGet(0, 0, 0);

	for (int i = 0; i <= steps; i++) {
		float t = (float)i / steps;
		VECTOR point = VAdd(p1, VScale(VSub(p2, p1), t));

		VECTOR closest;
		closest.x = max(min.x, min(point.x, max.x));
		closest.y = max(min.y, min(point.y, max.y));
		closest.z = max(min.z, min(point.z, max.z));

		VECTOR diff = VSub(point, closest);
		float distSq = VDot(diff, diff);

		if (distSq <= r * r) {
			float dist = sqrtf(distSq);
			float push = r - dist;

			if (push > bestPush) {
				bestPush = push;
				VECTOR dir = (dist > 0.0001f) ? VScale(diff, 1.0f / dist) : VGet(0, 1, 0);
				bestMove = VScale(dir, push);
			}
		}
	}

	if (bestPush > 0.0f) {
		cap->GetGameObject()->GetTransform()->AddPosition(bestMove);
	}
}

bool CollisionManager::RayVsSphere(Collider* a, Collider* b) {
	RayCollider* ray = (RayCollider*)a;
	SphereCollider* sphere = (SphereCollider*)b;

	VECTOR center = sphere->GetWorldCenter();
	float radius = sphere->GetRadius();

	// 中心点が当たっているなら true
	if (ray->CheckHitPoint(center))
		return true;

	// 半径分だけ内側に寄せた点をチェック
	VECTOR dir = VNorm(VSub(center, ray->GetWorldOrigin()));
	VECTOR nearPoint = VSub(center, VScale(dir, radius));

	return ray->CheckHitPoint(nearPoint);
}


bool CollisionManager::RayVsAABB(Collider* a, Collider* b) {
	RayCollider* ray = (RayCollider*)a;
	AABBCollider* box = (AABBCollider*)b;

	VECTOR min = box->GetMin();
	VECTOR max = box->GetMax();

	VECTOR points[8] = {
		VGet(min.x, min.y, min.z),
		VGet(max.x, min.y, min.z),
		VGet(min.x, max.y, min.z),
		VGet(max.x, max.y, min.z),
		VGet(min.x, min.y, max.z),
		VGet(max.x, min.y, max.z),
		VGet(min.x, max.y, max.z),
		VGet(max.x, max.y, max.z),
	};

	for (int i = 0; i < 8; i++) {
		if (ray->CheckHitPoint(points[i]))
			return true;
	}

	return false;
}

bool CollisionManager::RayVsCapsule(Collider* a, Collider* b) {
	RayCollider* ray = (RayCollider*)a;
	CapsuleCollider* cap = (CapsuleCollider*)b;

	VECTOR aPos = cap->GetWorldStart();
	VECTOR bPos = cap->GetWorldEnd();
	float radius = cap->GetRadius();

	const int div = 8;

	for (int i = 0; i <= div; i++) {
		float t = (float)i / div;
		VECTOR p = VAdd(aPos, VScale(VSub(bPos, aPos), t));

		// 中心線の点
		if (ray->CheckHitPoint(p))
			return true;

		// 半径分だけ内側に寄せた点
		VECTOR dir = VNorm(VSub(p, ray->GetWorldOrigin()));
		VECTOR nearP = VSub(p, VScale(dir, radius));

		if (ray->CheckHitPoint(nearP))
			return true;
	}

	return false;
}


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
