#pragma once

class Collider;

struct RayHit {
    bool hit = false;

    VECTOR point = VGet(0, 0, 0);

    float distance = 0.0f;

    Collider* collider = nullptr;
};