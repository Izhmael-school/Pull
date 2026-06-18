#pragma once
/*
 * @file Transform.h
 * @author Sekino
 */

#include "DxLib.h"
#include <vector>

class Transform {
protected:
	// 座標
	VECTOR position;
	// 回転(オイラー角)
	VECTOR rotation;
	// 拡縮
	VECTOR scale;

	// 行列
	MATRIX matrix;

	// 親子関係
	Transform* parent;
	std::vector<Transform*> children;

	// 有効かどうか
	bool isActive;

public:
	Transform();
	~Transform();

	void Update();

	// 座標関連
	inline VECTOR GetPosition() const { return VGet(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]); }
	inline VECTOR GetLocalPosition() const { return position; }
	inline void SetPosition(VECTOR _pos) { position = _pos; }
	inline void AddPosition(VECTOR _add) { position = VAdd(position, _add); }
	inline void AddPosition(VECTOR _dir, float _add) { position = VAdd(position, VScale(_dir, _add)); }

	// 回転関連
	inline VECTOR GetLocalRotation() const { return rotation; }
	inline void SetRotation(VECTOR _rot) { rotation = _rot; }
	inline void AddRotation(VECTOR _add) { rotation = VAdd(rotation, _add); }
	inline void AddRotation(VECTOR _dir, float _add) { rotation = VAdd(rotation, VScale(_dir, _add)); }

	// 拡縮関連
	VECTOR GetScale();
	inline VECTOR GetLocalScale() const { return scale; }
	inline void SetScale(VECTOR _sca) { scale = _sca; }
	inline void SetScale(float _sca) { scale = VGet(_sca, _sca, _sca); }
	inline void AddScale(VECTOR _add) { scale = VAdd(scale, _add); }
	inline void AddScale(VECTOR _dir, float _add) { scale = VAdd(scale, VScale(_dir, _add)); }

	// 行列関連
	inline MATRIX GetMatrix() const { return matrix; }
	inline void SetMatrix(MATRIX _mat) { matrix = _mat; }
	MATRIX CalcMatrix();

	// ベクトル関連
	inline VECTOR GetForward() const { return VNorm(VGet(matrix.m[2][0], matrix.m[2][1], matrix.m[2][2])); }
	inline VECTOR GetUp() const { return VNorm(VGet(matrix.m[1][0], matrix.m[1][1], matrix.m[1][2])); }
	inline VECTOR GetRight() const { return VNorm(VGet(matrix.m[0][0], matrix.m[0][1], matrix.m[0][2])); }

	void LookAtY(VECTOR targetPos);
	/*
	 *	ターゲットの方を向く
	 *	@author	Riku
	 */
	void LookAt(VECTOR targetPos);

	// 親子関係関連
	/*
	 * @brief 親子関係を作る
	 * @param _parant 親になるTransform
	 * @param isHoldWorld 現在の座標を維持するか
	 */
	void AttachParent(Transform* _parent, bool isHoldWorld = true);
	/*
	 * @brief 親子関係を解除する
	 */
	void DetachParent();

	Transform* GetParent() const { return parent; }
	Transform* GetChild(int index) const { if (children.size() <= index) return nullptr; else return children[index]; }
	int GetChildCount() const { return static_cast<int>(children.size()); }
	/*
	 * @brief 親子関係を解除する
	 */
	int GetChildID();
};

