#include "CollisionManager.h"
#include "Definition/CommonModule/MyJson.h"
#include <map>

// 当たり判定関数のマップ
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

// 押し出し関数のマップ
static const std::unordered_map<std::string, ResolveFunc> resolveFuncMap = {
	{ "SphereVsSphere",   &CollisionManager::ResolveSphereSphere },
	{ "SphereVsAABB",     &CollisionManager::ResolveSphereAABB },
	{ "CapsuleVsAABB",    &CollisionManager::ResolveCapsuleAABB },
	{ "AABBVsAABB" ,	  &CollisionManager::ResolveAABBVsAABB}
};

//	コンストラクタ・デストラクタ
CollisionManager::CollisionManager() {
	LoadCollisionRules("src/Data/collision_rules.json");
}
CollisionManager::~CollisionManager() {
	UnRegisterAll();
}

#pragma region 登録
// 登録
void CollisionManager::Register(Collider* _pCol) {
	if (!_pCol) return;

	pColliderArray.push_back(_pCol);
}
// 登録済みか確認して登録
void CollisionManager::CheckRegister(Collider* _pCol) {
	if (!_pCol) return;

	for (auto c : pColliderArray) {
		if (c == _pCol) return;
	}

	Register(_pCol);
}
#pragma endregion

#pragma region 削除
// 登録解除
void CollisionManager::UnRegister(Collider* _pCol) {
	auto itr = std::find(pColliderArray.begin(), pColliderArray.end(), _pCol);
	if (itr == pColliderArray.end()) return;

	pColliderArray.erase(itr);
}

//	すべての登録解除
void CollisionManager::UnRegisterAll() {
	pColliderArray.clear();

	prevs.clear();
	currents.clear();
}

#pragma endregion

#pragma region 更新
//	更新
void CollisionManager::Update() {


	int stage = 0, player = 0, enemy = 0, other = 0;

	// レイヤーごとのコライダー数をカウント
	for (auto c : pColliderArray) {
		switch (c->GetLayer()) {
		case ColliderLayer::Stage: stage++; break;
		case ColliderLayer::Player: player++; break;
		case ColliderLayer::Enemy: enemy++; break;
		default: other++; break;
		}
	}

	//printfDx("Stage:%d Player:%d Enemy:%d Other:%d\n", stage, player, enemy, other);

	// コライダーの更新
	for (auto col : pColliderArray) {
		if (col && col->IsEnable()) {
			col->Update();
		}
	}

	//	当たり判定の処理
	int n = pColliderArray.size();

	static int prevSize = -1;

	//	前回のサイズと異なる場合は、prevsとcurrentsをリサイズ
	if (prevSize != n) {
		prevs.assign(n, std::vector<bool>(n, false));
		currents.assign(n, std::vector<bool>(n, false));

		prevSize = n;
	}

	//	当たり判定の処理
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {

			Collider* a = pColliderArray[i];
			Collider* b = pColliderArray[j];

			if (!a || !b) continue;

			if (!a->IsEnable() || !b->IsEnable()) continue;

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

			// イベントの呼び出し
			if (!prevs[i][j] && hit) {
				goA->OnTriggerEnter(a, b);
				goB->OnTriggerEnter(b, a);
			}
			// 前回当たっていて今回も当たっている場合はOnTriggerStay
			else if (prevs[i][j] && hit) {
				goA->OnTriggerStay(a, b);
				goB->OnTriggerStay(b, a);
			}
			//	前回当たっていて今回当たっていない場合はOnTriggerExit
			else if (prevs[i][j] && !hit) {
				goA->OnTriggerExit(a, b);
				goB->OnTriggerExit(b, a);
			}

			prevs[i][j] = hit;

		}
	}
}

#pragma endregion

#pragma region 判定
//	判定関数の呼び出し
bool CollisionManager::CheckHit(Collider* a, Collider* b) {
	//	コライダーの型名を組み合わせてキーを作成
	std::string key =
		std::string(a->GetTypeName()) + "_" + b->GetTypeName();

	//	キーに対応する判定関数を検索
	auto it = hitFuncTable.find(key);
	if (it != hitFuncTable.end()) {
		return it->second(a, b);
	}

	//	逆順のキーを作成して検索
	std::string revKey =
		std::string(b->GetTypeName()) + "_" + a->GetTypeName();

	//	逆順のキーに対応する判定関数を検索
	auto it2 = hitFuncTable.find(revKey);
	if (it2 != hitFuncTable.end()) {
		return it2->second(b, a);
	}

	return false;
}

#pragma endregion
//	押し出し関数の呼び出し
void CollisionManager::Resolve(Collider* a, Collider* b) {
	// コライダーの型名を組み合わせてキーを作成
	std::string key =
		std::string(a->GetTypeName()) + "_" + b->GetTypeName();

	// キーに対応する押し出し関数を検索
	auto it = resolveFuncTable.find(key);
	// 逆順のキーを作成して検索
	if (it == resolveFuncTable.end()) return;

	// 逆順のキーに対応する押し出し関数を検索
	it->second(a, b);
}

// JSONから当たり判定ルールを読み込む
void CollisionManager::LoadCollisionRules(const std::string& path) {
	// JSONファイルを読み込む
	auto json = MyJson::LoadJsonFile(path);

	// ルールを読み込む
	for (auto& rule : json["collision_rules"]) {

		//	コライダーの型名を組み合わせてキーを作成
		std::string key =
			rule["a"].get<std::string>() + "_" +
			rule["b"].get<std::string>();

		//	判定関数の名前を取得して、funcMapから対応する関数ポインタを取得
		std::string hitName = rule["hit"].get<std::string>();
		auto it = funcMap.find(hitName);
		if (it == funcMap.end()) {
			printfDx("Unknown hit func: %s\n", hitName.c_str());
			continue;
		}
		hitFuncTable[key] = it->second;

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

	const int steps = 32;

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

#pragma endregion

#pragma region 押し出し
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

	// 床の隙間吸収
	min.y -= 0.02f;

	VECTOR closest;
	closest.x = max(min.x, min(center.x, max.x));
	closest.y = max(min.y, min(center.y, max.y));
	closest.z = max(min.z, min(center.z, max.z));

	VECTOR diff = VSub(center, closest);
	float distSq = VDot(diff, diff);

	if (distSq >= r * r) return;

	float dist = sqrtf(distSq);
	float push = r - dist;

	VECTOR dir;

	if (dist < 0.0001f) {
		// 深いめり込み → 上方向に押し出す
		dir = VGet(0, 1, 0);
	}
	else {
		// 通常 → 上方向に限定
		dir = VGet(0, 1, 0);
	}

	VECTOR move = VScale(dir, push);
	s->GetGameObject()->GetTransform()->AddPosition(move);
}

void CollisionManager::ResolveCapsuleAABB(Collider* capCol, Collider* boxCol) {
	auto cap = static_cast<CapsuleCollider*>(capCol);
	auto box = static_cast<AABBCollider*>(boxCol);

	if (cap->GetLayer() == ColliderLayer::PlayerArm && box->GetLayer() == ColliderLayer::Gimmick) {
		return;
	}

	VECTOR p1 = cap->GetWorldStart();
	VECTOR p2 = cap->GetWorldEnd();
	VECTOR bmin = box->GetMin();
	VECTOR bmax = box->GetMax();
	float radius = cap->GetRadius();

	// 線分上の最近点を解析的に求める
	VECTOR seg = VSub(p2, p1);
	float segLenSq = VDot(seg, seg);

	VECTOR boxCenter = VScale(VAdd(bmin, bmax), 0.5f);

	float t = VDot(VSub(boxCenter, p1), seg) / segLenSq;
	t = max(0.0f, min(1.0f, t));

	VECTOR point = VAdd(p1, VScale(seg, t));

	// AABB 内の最近点
	VECTOR closest;
	closest.x = max(bmin.x, min(point.x, bmax.x));
	closest.y = max(bmin.y, min(point.y, bmax.y));
	closest.z = max(bmin.z, min(point.z, bmax.z));

	VECTOR diff = VSub(point, closest);
	float distSq = VDot(diff, diff);

	if (distSq >= radius * radius) return;

	float penetrationDist = sqrtf(distSq);
	float push = radius - penetrationDist;

	float diffLen = VSize(diff);

	VECTOR dir;

	if (diffLen < 0.0001f) {
		// 深いめり込み → AABB の中心から押し出す
		dir = VNorm(VSub(point, boxCenter));
	}
	else {
		// 通常 → 最近点方向へ押し出す（正しい法線）
		dir = VNorm(diff);
	}

	VECTOR move = VScale(dir, push);

	cap->GetGameObject()->GetTransform()->AddPosition(move);
}

void CollisionManager::ResolveAABBVsAABB(Collider* aCol, Collider* bCol) {
	auto a = static_cast<AABBCollider*>(aCol);
	auto b = static_cast<AABBCollider*>(bCol);

	if (a->GetLayer() == ColliderLayer::Stage && b->GetLayer() == ColliderLayer::Stage) {
		return;
	}

	if (a->GetLayer() == ColliderLayer::Gimmick || b->GetLayer() == ColliderLayer::Gimmick) {
		return;
	}

	if (a->GetLayer() == ColliderLayer::BreakWall && b->GetLayer() == ColliderLayer::Stage || b->GetLayer() == ColliderLayer::BreakWall && a->GetLayer() == ColliderLayer::Stage) {
		return;
	}

	VECTOR aMin = a->GetMin();
	VECTOR aMax = a->GetMax();
	VECTOR bMin = b->GetMin();
	VECTOR bMax = b->GetMax();

	// まず重なっているかチェック
	bool overlapX = (aMin.x <= bMax.x) && (aMax.x >= bMin.x);
	bool overlapY = (aMin.y <= bMax.y) && (aMax.y >= bMin.y);
	bool overlapZ = (aMin.z <= bMax.z) && (aMax.z >= bMin.z);

	if (!(overlapX && overlapY && overlapZ)) {
		return; // 重なっていない
	}

	// 各軸の押し出し量を計算
	float pushX1 = bMax.x - aMin.x; // A を +X に押す量
	float pushX2 = aMax.x - bMin.x; // A を -X に押す量

	float pushY1 = bMax.y - aMin.y;
	float pushY2 = aMax.y - bMin.y;

	float pushZ1 = bMax.z - aMin.z;
	float pushZ2 = aMax.z - bMin.z;

	// 最小押し出し量を選ぶ
	float minPush = FLT_MAX;
	VECTOR move = { 0,0,0 };

	// X軸
	if (pushX1 > 0 && pushX1 < minPush) {
		minPush = pushX1;
		move = { minPush, 0, 0 };
	}
	if (pushX2 > 0 && pushX2 < minPush) {
		minPush = pushX2;
		move = { -minPush, 0, 0 };
	}

	// Y軸
	if (pushY1 > 0 && pushY1 < minPush) {
		minPush = pushY1;
		move = { 0, minPush, 0 };
	}
	if (pushY2 > 0 && pushY2 < minPush) {
		minPush = pushY2;
		move = { 0, -minPush, 0 };
	}

	// Z軸
	if (pushZ1 > 0 && pushZ1 < minPush) {
		minPush = pushZ1;
		move = { 0, 0, minPush };
	}
	if (pushZ2 > 0 && pushZ2 < minPush) {
		minPush = pushZ2;
		move = { 0, 0, -minPush };
	}

	// A の Transform を押し出す
	a->GetGameObject()->GetTransform()->AddPosition(move);
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
