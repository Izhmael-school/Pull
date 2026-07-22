#pragma once

/*
 * @file MyMath.h
 * @author Sekino
 */

#include <cmath>
#include "DxLib.h"
#include <algorithm>

#undef max
#undef min

 /*
  *  UI実装用の2次元ベクトル構造体
  */
struct Vector2 {
public:
	float x, y;

	/*
	 *  ゼロベクトル
	 */
	static const Vector2 Zero;

	/*
	 * <summary>
	 * X軸の正方向の単位ベクトル
	 * Right,Leftは現状用意しない
	 */
	static const Vector2 XAxis;

	/*
	 *	Y軸の正方向の単位ベクトル
	 *  Up,Downは現状用意しない
	 */
	static const Vector2 YAxis;

	/*
	 *	コンストラクタ
	 */
	constexpr Vector2():x(0.0f),y(0.0f){}
	/*
	 *	引数ありのコンストラクタ
	 */
	constexpr Vector2(float x, float y) : x(x), y(y) {}

	/*
	 *	マイナスのベクトルを返す
	 */
	Vector2 operator-() const;
	
	/*
	 *	ベクトルの加算
	 */
	Vector2 operator+(const Vector2& vec) const;

	/*
	 *	ベクトルの減算
	 */
	Vector2 operator-(const Vector2& vec) const;

	/*
	 *	ベクトルとスカラーの乗算
	 */
	Vector2 operator*(float scale) const;

	/*
	 * ベクトルとスカラーの除算
	 */
	Vector2 operator/(float scale) const;

	/*
	 *	ベクトルとベクトルの加算代入演算子
	 */
	Vector2& operator+= (const Vector2& vec);

	
	/*
	 *	ベクトルとベクトルの減算代入演算子
	 */
	Vector2& operator-= (const Vector2& vec);

	/*
	 *	ベクトルとスカラーの乗算代入演算子
	 */
	Vector2& operator*= (float scale);

	/*
	 *	ベクトルとスカラーの除算代入演算し
	 */
	Vector2& operator/= (float scale);

};

class MyMath {

public:
	static float Deg2Rad(float _radian);

	static float Rad2Deg(float _degree);

	static VECTOR VDeg2Rad(VECTOR _radian);

	static VECTOR VRad2Deg(VECTOR _degree);

	static float Distance(float _f1, float _f2, float _f3);

	inline VECTOR VMult(VECTOR VEC1, VECTOR VEC2) {
		return VGet(VEC1.x * VEC2.x, VEC1.y * VEC2.y, VEC1.z * VEC2.z);
	}

	inline static int Random(int min, int max) { return (min)+GetRand(max - min); }
	inline static float RandomF(int min, int max) { return static_cast<float>((min)+GetRand(max - min)); }
	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="dir"></param>
	/// <returns></returns>
	inline static VECTOR Normalize(VECTOR dir) {
		float norm = static_cast<float>(sqrt((pow(dir.x, 2) + pow(dir.y, 2) + pow(dir.z, 2))));
		const float esp = 1e-6f;
		if (norm <= esp) {
			return VGet(0.0f, 0.0f, 0.0f);
		}
		return VGet(dir.x / norm, dir.y / norm, dir.z / norm);
	}

	/// <summary>
	/// 向き
	/// </summary>
	/// <param name="pos1"></param>
	/// <param name="pos2"></param>
	/// <returns></returns>
	inline static VECTOR Dir(VECTOR pos1, VECTOR pos2) {
		return VSub(pos1, pos2);
	}

	/// <summary>
	/// 正規化付き向き計算
	/// </summary>
	/// <param name="pos1"></param>
	/// <param name="pos2"></param>
	/// <returns></returns>
	static VECTOR NormDir(VECTOR pos1, VECTOR pos2) {
		return Normalize(VSub(pos1, pos2));
	}

	/*
	 *  線形補間
	 *  @author Riku
	 */
	inline static VECTOR Lerp(VECTOR pos1, VECTOR pos2, float t) {
		return VAdd(pos1, VScale((VSub(pos2, pos1)), t));
	}
	inline static float Lerp(float value1, float value2, float t) {
		return value1 + (value2 - value1) * t;
	}

	inline VECTOR Velocity(VECTOR dir, float speed) {

	}

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="dir1"></param>
	/// <param name="dir2"></param>
	/// <returns></returns>
	inline float Dot(VECTOR dir1, VECTOR dir2) {
		return (dir1.x * dir2.x) + (dir1.y * dir2.y) + (dir1.z * dir2.z);
	}

	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="dir1"></param>
	/// <param name="dir2"></param>
	/// <returns></returns>
	inline VECTOR Cross(VECTOR dir1, VECTOR dir2) {
		float x = dir1.y * dir2.z - dir1.z * dir2.y;
		float y = dir1.x * dir2.z - dir1.z * dir2.x;
		float z = dir1.x * dir2.y - dir1.y * dir2.x;
		return VGet(x, y, z);
	}

	/*
	 *  float版イージング
	 *  @author oorui
	 */
	static float EaseQuadIn(const float start, const float end, const float time);

	static VECTOR EaseQuadInVEC(const VECTOR start, const VECTOR end, const float time);

	template<typename ...Args>
	inline static float MaxF(const Args& ...args) {
		return std::max({ static_cast<float>(args)... });

	}

	template<typename ...Args>
	inline static float MinF(const Args& ...args) {
		return std::min({ static_cast<float>(args)... });
	}

	inline static float LerpAngle(float current, float target, float t) {
		t = std::clamp(t, 0.0f, 1.0f);
		float delta = target - current;
		// -180〜180の範囲に正規化(最短方向を選ぶ)
		delta = fmodf(delta + 180.0f, 360.0f);
		if (delta < 0.0f) delta += 360.0f;
		delta -= 180.0f;
		return current + delta * t;
	}
};

