#include "MyMath.h"
#include "DxLib.h"

/*
 * @file MyMath.cpp
 * @author Sekino
 */

float MyMath::Deg2Rad(float _radian) {
    return (_radian * (DX_PI_F / 180.0f));
}

float MyMath::Rad2Deg(float _degree) {
    return(_degree * (180.0f / DX_PI_F));
}

VECTOR MyMath::VDeg2Rad(VECTOR _radian) {
    return VScale(_radian, (DX_PI_F / 180.0f));
}

VECTOR MyMath::VRad2Deg(VECTOR _degree) {
    return VScale(_degree, (180.0f / DX_PI_F));
}

float MyMath::Distance(float _f1, float _f2, float _f3) {
    return std::powf(_f1, 2) + std::powf(_f2, 2) + std::powf(_f3, 2);
}

/*
 *  イージング (float)
 *  @param[in]  float   開始位置
 *  @param[in]  float   終了位置
 *  @param[in]  float   時間
 */
float MyMath::EaseQuadIn(const float start, const float end, const float time) {
    if (time >= 1.0f)return end;
    // 距離を計算
    float dis = end - start;
    // 二次関数を計算
    float quadIn = powf(time, 1.5f);

    // 現在位置を計算
    float ref = start + dis * quadIn;

    return ref;
}

/*
 *  イージング（VECTOR）
 *  @param[in]  VECTOR  開始位置
 *  @param[in]  VECTOR  終了位置
 *  @param[in]  float   時間
 */
VECTOR MyMath::EaseQuadInVEC(const VECTOR start, const VECTOR end, const float time) {
    VECTOR result;

    result.x = EaseQuadIn(start.x, end.x, time);
    result.y = EaseQuadIn(start.y, end.y, time);
    result.z = EaseQuadIn(start.z, end.z, time);

    return result;
}

