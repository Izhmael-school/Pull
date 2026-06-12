#include "Transform.h"
#include "../Definition/Const/VECTORConst.h"
#include "../Definition/CommonModule/MyMath.h"
#include <cmath>
#include <cassert>

/*
 * @file Transform.cpp
 * @author Sekino
 */

Transform::Transform()
	:position(VZero)
	, rotation(VZero)
	, scale(VOne)
	, matrix(MGetIdent())
	, parent(nullptr)
	, children()
	, isActive(true)
{
}

Transform::~Transform() {

	// 親から切り離す
	DetachParent();

	// 子オブジェクトの親を解除する
	for (auto c : children) {
		c->parent = nullptr;
	}
	children.clear();
}

void Transform::Update() {
	if (!isActive) return;

	CalcMatrix();

	// 子オブジェクトの行列も更新する
	for (auto* c : children) {
		c->Update();
	}
}

void Transform::DetachParent() {
	if (!parent) return;

	// 親の子リストから自分を探して削除
	auto& siblings = parent->children;
	siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
	parent = nullptr;
}

int Transform::GetChildID() {
	std::vector<Transform*>& siblings = parent->children;
	int size = static_cast<int>(siblings.size());
	for (int i = 0;i < size;i++) {
		if (siblings[i] != this) continue;

		return i;
	}
	return -1;
}

VECTOR Transform::GetScale() {
	MATRIX m = matrix;
	return VGet(VSize(VGet(m.m[0][0], m.m[0][1], m.m[0][2])),
		VSize(VGet(m.m[1][0], m.m[1][1], m.m[1][2])),
		VSize(VGet(m.m[2][0], m.m[2][1], m.m[2][2])));
}

MATRIX Transform::CalcMatrix() {
	// 座標、回転、拡縮から行列を求める
	MATRIX mRotX = MGetRotX(MyMath::Deg2Rad(rotation.x));
	MATRIX mRotY = MGetRotY(MyMath::Deg2Rad(rotation.y));
	MATRIX mRotZ = MGetRotZ(MyMath::Deg2Rad(rotation.z));

	// 回転行列の作成
	MATRIX mRotXYZ = MMult(MMult(mRotZ, mRotX), mRotY);

	// 拡縮行列の作成
	MATRIX mScale = MGetScale(scale);

	// 平行移動行列を取得する
	MATRIX mTranslate = MGetTranslate(position);

	// 回転→拡縮→平行移動の順で掛ける
	matrix = MMult(MMult(mScale, mRotXYZ), mTranslate);

	// 親がいないなら帰る
	if (!parent) return matrix;

	// 親の行列を掛ける
	matrix = MMult(matrix, parent->matrix);
	return matrix;
}

void Transform::LookAtY(VECTOR targetPos){
	VECTOR dir = VSub(targetPos, GetPosition());
	// yは使わない
	dir.y = 0.0f;

	// 重なってそうなら戻る
	if (VSize(dir) <= 0.001f) return;

	dir = VNorm(dir);

	rotation.y = MyMath::Rad2Deg(atan2f(dir.x, dir.z));
}

void Transform::AttachParent(Transform* _parent, bool isHoldWorld) {
	if (!_parent) {
#if _DEBUG
		assert(true && "引数がNULLです");
#endif
		return;
	}
	if (parent == _parent || _parent == this) return;

	// _parentの親に自分がいないか
	Transform* check = _parent;
	do {
		if (check->parent == this) {
#if _DEBUG
			assert(true && "循環参照が発生");
#endif
			return;
		}
		else {
			check = check->parent;
		}
	} while (check != nullptr);

	// ワールド座標を維持
	VECTOR worldPos = GetPosition();
	VECTOR worldRot = GetLocalRotation();
	VECTOR worldScale = GetScale();

	// すでに親がいる場合は前の親を切り離す
	if (parent)
		DetachParent();

	// 親を登録
	parent = _parent;
	// 親に自分を登録
	parent->children.push_back(this);

	// 保持するなら座標を入れる
	if (isHoldWorld) {
		SetPosition(worldPos);
		SetRotation(worldRot);
		SetScale(worldScale);
	}
}

