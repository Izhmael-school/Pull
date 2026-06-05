#include "MyMath.h"
#include "DxLib.h"

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
