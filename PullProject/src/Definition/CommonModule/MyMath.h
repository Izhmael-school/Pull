#pragma once
#include <cmath>
#include "DxLib.h"
#include <algorithm>

#undef max
#undef min

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
    /// <summary>
    /// ê≥ãKâª
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
    /// å¸Ç´
    /// </summary>
    /// <param name="pos1"></param>
    /// <param name="pos2"></param>
    /// <returns></returns>
    inline static VECTOR Dir(VECTOR pos1, VECTOR pos2) {
        return VSub(pos1, pos2);
    }

    /// <summary>
    /// ê≥ãKâªïtÇ´å¸Ç´åvéZ
    /// </summary>
    /// <param name="pos1"></param>
    /// <param name="pos2"></param>
    /// <returns></returns>
    static VECTOR NormDir(VECTOR pos1, VECTOR pos2) {
        return Normalize(VSub(pos1, pos2));
    }

    inline VECTOR Velocity(VECTOR dir, float speed) {

    }

    /// <summary>
    /// ì‡êœ
    /// </summary>
    /// <param name="dir1"></param>
    /// <param name="dir2"></param>
    /// <returns></returns>
    inline float Dot(VECTOR dir1, VECTOR dir2) {
        return (dir1.x * dir2.x) + (dir1.y * dir2.y) + (dir1.z * dir2.z);
    }

    /// <summary>
    /// äOêœ
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

    template<typename ...Args>
    inline static float MaxF(const Args& ...args) {
        return std::max({ static_cast<float>(args)... });

    }

    template<typename ...Args>
    inline static float MinF(const Args& ...args) {
        return std::min({ static_cast<float>(args)... });
    }
};

