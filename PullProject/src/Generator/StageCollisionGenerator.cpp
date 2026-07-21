#define NOMINMAX
#include <Windows.h>

#include "StageCollisionGenerator.h"
#include <string>
#include <vector>
#include <cmath>

#include "../Definition/CommonModule/MyJson.h"
#include "../Component/Collider/Collider.h"
#include "../GameObject/GameObject.h"

// AABB構造体の定義
struct AABB {
	VECTOR min;
	VECTOR max;
	std::string type;
};

// 2つの浮動小数点数が近いかどうかを判定する関数
bool IsNear(float a, float b, float eps = 2.0f) {
	return fabsf(a - b) < eps;
}

// 2つのAABBが結合可能かどうかを判定する関数
bool CanMerge(const AABB& a, const AABB& b) {
	// X方向に隣接
	bool xAdjacent =
		IsNear(a.max.x, b.min.x) || IsNear(b.max.x, a.min.x);

	// Z方向に隣接
	bool zAdjacent =
		IsNear(a.max.z, b.min.z) || IsNear(b.max.z, a.min.z);

	// Y方向に隣接（段差の結合）
	bool yAdjacent =
		IsNear(a.max.y, b.min.y) || IsNear(b.max.y, a.min.y);

	// 同じ高さ・奥行き（許容誤差あり）
	bool sameY = IsNear(a.min.y, b.min.y, 0.1f) && IsNear(a.max.y, b.max.y, 0.1f);
	bool sameZ = IsNear(a.min.z, b.min.z, 0.1f) && IsNear(a.max.z, b.max.z, 0.1f);
	bool sameX = IsNear(a.min.x, b.min.x, 0.1f) && IsNear(a.max.x, b.max.x, 0.1f);

	// X方向の結合
	if (xAdjacent && sameY && sameZ) return true;

	// Z方向の結合
	if (zAdjacent && sameY && sameX) return true;

	// Y方向の結合（段差をまとめる）
	if (yAdjacent && sameX && sameZ) return true;

	return false;
}

// 2つのAABBを結合する関数
AABB MergeTwo(const AABB& a, const AABB& b) {
	// 新しいAABBを作成
	AABB r;

	// 最小点と最大点を計算
	r.min.x = (std::min)(a.min.x, b.min.x);
	r.min.y = (std::min)(a.min.y, b.min.y);
	r.min.z = (std::min)(a.min.z, b.min.z);

	r.max.x = (std::max)(a.max.x, b.max.x);
	r.max.y = (std::max)(a.max.y, b.max.y);
	r.max.z = (std::max)(a.max.z, b.max.z);

	return r;
}

// 3D AABBの結合処理
void MergeAABB3D(std::vector<AABB>& boxes) {
	// 結合が行われたかどうかのフラグ
	bool merged = true;

	//	結合可能なAABBがなくなるまでループ
	while (merged) {
		merged = false;

		for (size_t i = 0; i < boxes.size(); i++) {
			for (size_t j = i + 1; j < boxes.size(); j++) {
				if (CanMerge(boxes[i], boxes[j])) {
					boxes[i] = MergeTwo(boxes[i], boxes[j]);
					boxes.erase(boxes.begin() + j);
					merged = true;
					break;
				}
			}

			if (merged) break;
		}
	}
}

// UnityのJSONデータから当たり判定を生成する
void StageCollisionGenerator::GenerateFromUnity(
	const std::string& path,
	CollisionManager& manager) {
	auto json = MyJson::LoadJsonFile(path);

	// JSONが正しく読み込まれなかった場合のエラー表示
	if (json.is_null()) {
		printfDx("JSON load failed\n");
		return;
	}

	// AABBのリストを作成
	std::vector<AABB> boxes;

	//	全体のスケールを設定
	float scale = 100.0f;

	//	JSONのブロック情報を取得
	for (auto& b : json["blocks"]) {

		// "type"が"tree"または"subground"の場合はスキップ
		if (b.contains("type") && (b["type"] == "tree" || b["type"] == "subground")) {
			continue;
		}

		// "minX"と"maxX"が存在しない場合はスキップ
		if (!b.contains("minX") || !b.contains("maxX"))
			continue;

		float minX = b["minX"];
		float minY = b["minY"];
		float minZ = b["minZ"];

		float maxX = b["maxX"];
		float maxY = b["maxY"];
		float maxZ = b["maxZ"];

		std::string type = b.contains("type") ? b["type"].get<std::string>() : "";

		VECTOR min = VGet(-minX, minY, -minZ);
		VECTOR max = VGet(-maxX, maxY, -maxZ);

		min = VScale(min, scale);
		max = VScale(max, scale);

		if (min.x > max.x) std::swap(min.x, max.x);
		if (min.z > max.z) std::swap(min.z, max.z);

		boxes.push_back({ min, max, type });
	}

	// AABBの結合処理を実行
	MergeAABB3D(boxes);

	int colCount = 0;

	// 結合後のAABBをColliderとして登録
	for (auto& b : boxes) {
		GameObject* obj = new GameObject(-1, VZero, Tag::Ground);

		AABBCollider* col = std::make_unique<AABBCollider>(obj, VZero, VZero).release();
		col->SetMin(b.min);
		col->SetMax(b.max);

		if (b.type == "missilewall") {
			col->SetLayer(ColliderLayer::MissileWall);
		}
		else if (b.type == "wall") {
			col->SetLayer(ColliderLayer::Wall);
		}
		else if (b.type == "ground") {
			col->SetLayer(ColliderLayer::Ground);
		}
		else if (b.type == "retry") {
			col->SetLayer(ColliderLayer::Retry);
			col->SetResolve(false);
		}
		else if (b.type == "stage1") {
			col->SetLayer(ColliderLayer::Stage1);
			col->SetResolve(false);
		}
		else if (b.type == "stage2") {
			col->SetLayer(ColliderLayer::Stage2);
			col->SetResolve(false);
		}
		else if (b.type == "stage3") {
			col->SetLayer(ColliderLayer::Stage3);
			col->SetResolve(false);
		}
		else if (b.type == "stage4") {
			col->SetLayer(ColliderLayer::Stage4);
			col->SetResolve(false);
		}
		else if (b.type == "stage5") {
			col->SetLayer(ColliderLayer::Stage5);
			col->SetResolve(false);
		}
		else {
			col->SetLayer(ColliderLayer::Stage);
		}
		colCount++;
	}
	// 結合後のAABBの数を表示
	printfDx("collider: %d\n", colCount);
}
