#pragma once
#include "DxLib.h"

class Transform {
protected:
	// ç¿ïW
	VECTOR position;
	// âÒì](ÉIÉCÉâÅ[äp)
	VECTOR rotation;
	// ägèk
	VECTOR scale;

	// çsóÒ
	MATRIX matrix;

	// óLå¯Ç©Ç«Ç§Ç©
	bool isActive;

public:
	Transform();
	~Transform() = default;

	void Update();

	inline VECTOR GetPosition() const { return position; }
	inline void SetPosition(VECTOR _pos) { position = _pos; }
	inline void AddPosition(VECTOR _add) { position = VAdd(position, _add); }
	inline void AddPosition(VECTOR _dir, float _add) { position = VAdd(position, VScale(_dir, _add)); }
	inline VECTOR GetRotation() const { return rotation; }
	inline void SetRotation(VECTOR _rot) { rotation = _rot; }
	inline void AddRotation(VECTOR _add) { rotation = VAdd(rotation, _add); }
	inline void AddRotation(VECTOR _dir, float _add) { rotation = VAdd(rotation, VScale(_dir, _add)); }
	inline VECTOR GetScale() const { return scale; }
	inline void SetScale(VECTOR _sca) { scale = _sca; }
	inline void AddScale(VECTOR _add) { scale = VAdd(scale, _add); }
	inline void AddScale(VECTOR _dir, float _add) { scale = VAdd(scale, VScale(_dir, _add)); }
	inline MATRIX GetMatrix() const { return matrix; }
	inline void SetMatrix(MATRIX _mat) { matrix = _mat; }

	inline VECTOR GetForward() const { return VNorm(VGet(matrix.m[2][0], matrix.m[2][1], matrix.m[2][2])); }
	inline VECTOR GetUp() const { return VNorm(VGet(matrix.m[1][0], matrix.m[1][1], matrix.m[1][2])); }
	inline VECTOR GetRight() const { return VNorm(VGet(matrix.m[0][0], matrix.m[0][1], matrix.m[0][2])); }
};

