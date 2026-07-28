#pragma once

#include <vector>
enum class ColliderLayer;
class Collider;

struct RayCastSetting {
    float maxDistance = 1000.0f;

    float startHorizontalAngle = 0.0f;
    float endHorizontalAngle = 0.0f;

    float startVerticalAngle = 0.0f;
    float endVerticalAngle = 0.0f;

    int horizontalRayCount = 1;
    int verticalRayCount = 1;

    VECTOR origin = VZero;

    VECTOR direction = VGet(0, 0, 1);

    Collider* ignoreCollider = nullptr;

    std::vector<ColliderLayer> targetLayers;
    std::vector<ColliderLayer> ignoreLayers;

    bool nearestOnly = true;
    bool drawDebug = false;
};