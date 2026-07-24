#include "Transform.h"
#include "../Definition/Const/VECTORConst.h"
#include "../Definition/CommonModule/MyMath.h"
#include "Manager/TimeManager.h"
#include <cmath>
#include <cassert>
#include "ImGui/ImGui.h"

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

void Transform::AddWorldOffset(VECTOR _worldOffset) {
	if (parent) {
		MATRIX parentMat = parent->GetMatrix();
		// 平行移動成分を消して回転・拡縮だけ取り出す
		parentMat.m[3][0] = 0.0f;
		parentMat.m[3][1] = 0.0f;
		parentMat.m[3][2] = 0.0f;

		MATRIX parentInv = MInverse(parentMat);
		VECTOR localOffset = VTransform(_worldOffset, parentInv);

		position = VAdd(position, localOffset);
	}
	else {
		position = VAdd(position, _worldOffset);
	}
	CalcMatrix();
}

VECTOR Transform::GetRotation() {
	MATRIX m = matrix;
	// 行列から抜き出す
	VECTOR rx = VGet(m.m[0][0], m.m[0][1], m.m[0][2]);
	VECTOR ry = VGet(m.m[1][0], m.m[1][1], m.m[1][2]);
	VECTOR rz = VGet(m.m[2][0], m.m[2][1], m.m[2][2]);

	VECTOR scale = GetScale();
	// スケールを抜く
	rx = VScale(rx, 1.0f / scale.x);
	ry = VScale(ry, 1.0f / scale.y);
	rz = VScale(rz, 1.0f / scale.z);

	VECTOR rot = VZero;

	rot.x = MyMath::Rad2Deg(asinf(-rz.y));
	rot.y = MyMath::Rad2Deg(atan2f(rz.x, rz.z));
	rot.z = MyMath::Rad2Deg(atan2f(rx.y, ry.y));

	return rot;
}

VECTOR Transform::GetScale() const {
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
	MATRIX local = MMult(MMult(mScale, mRotXYZ), mTranslate);

	// 親がいないなら帰る
	if (!parent) {
		matrix = local;
		return matrix;
	}
	// 子の行列を先に適用する (子->親 の順で合成)
	matrix = MMult(local, parent->matrix);

	return matrix;
}

void Transform::CalcTransform() {

	SetPosition(GetPosition());
	SetRotation(GetRotation());
	SetScale(GetScale());
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

void Transform::LookAtDir(VECTOR dir){
	// 重なってそうなら戻る
	if (VSize(dir) <= 0.001f) return;

	dir = VNorm(dir);

	rotation.y = MyMath::Rad2Deg(atan2f(dir.x, dir.z));
}

void Transform::LookAt(VECTOR targetPos) {
	// ターゲットへの方向ベクトル
	VECTOR dir = VSub(targetPos, GetPosition());
	// 重なってそうなら戻る
	if (VSize(dir) <= 0.001f) return;
	// 単位ベクトルへ変換
	dir = VNorm(dir);
	// XZ平面上の方向からヨーを算出
	rotation.y = MyMath::Rad2Deg(atan2f(dir.x, dir.z));
	// XZ平面に投影した長さ(水平距離)
	float horizontal = sqrtf(dir.x * dir.x + dir.z * dir.z);
	// 高さと水平距離からピッチを算出
	rotation.x = MyMath::Rad2Deg(-atan2f(dir.y, horizontal));
}

void Transform::GraduallyLookAtY(VECTOR targetPos) {
	// ターゲットへの方向ベクトル
	VECTOR dir = VSub(targetPos, GetPosition());
	// 重なってそうなら戻る
	if (VSize(dir) <= 0.001f) return;
	// 単位ベクトルへ変換
	dir = VNorm(dir);

	// XZ平面上の方向からヨーの目標角度を算出
	float targetYaw = MyMath::Rad2Deg(atan2f(dir.x, dir.z));
	float deltaTime = TimeManager::GetInstance().GetDeltaTime();
	// ヨーだけ現在角度から目標角度へ最短経路で補間
	rotation.y = MyMath::LerpAngle(rotation.y, targetYaw, 5.0f * deltaTime);
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
	parent->children.emplace_back(this);

	// 保持するなら座標を入れる
	if (isHoldWorld) {
		SetPosition(worldPos);
		SetRotation(worldRot);
		SetScale(worldScale);
	}
}

