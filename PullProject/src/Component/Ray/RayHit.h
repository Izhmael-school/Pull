#pragma once
#include "Definition/Const/VECTORConst.h"
class Collider;

struct RayHit {
    bool hit = false;

    VECTOR point = VZero;

    float distance = 0.0f;

    Collider* collider = nullptr;

    VECTOR GetPoint() const {
        return point;
    }
};