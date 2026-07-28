#include "RayCast.h"

#include <cfloat>

#include "Manager/CollisionManager.h"
#include "Component/Collider/Collider.h"

bool RayCast::Cast(
    const RayCastSetting& setting,
    RayHit& result) {
    result = {};

    Ray castRay;

    castRay.origin =
        setting.origin;

    castRay.direction =
        GetDirection(setting);

    float nearestDistance =
        FLT_MAX;

    const auto& colliders =
        CollisionManager::
        GetInstance().
        GetColliders();

    for (auto collider : colliders) {
        if (!collider)
            continue;

        if (!collider->IsEnable())
            continue;

        if (collider ==
            setting.ignoreCollider) {
            continue;
        }

        bool ignore = false;

        for (auto layer :
            setting.ignoreLayers) {
            if (collider->GetLayer()
                == layer) {
                ignore = true;
                break;
            }
        }

        if (ignore)
            continue;

        if (!setting.targetLayers.empty()) {
            bool target = false;

            for (auto layer :
                setting.targetLayers) {
                if (collider->GetLayer()
                    == layer) {
                    target = true;
                    break;
                }
            }

            if (!target)
                continue;
        }

        RayHit temp;

        if (!collider->Raycast(
            castRay,
            setting.maxDistance,
            temp)) {
            continue;
        }

        if (temp.distance <
            nearestDistance) {
            nearestDistance =
                temp.distance;

            result =
                temp;
        }
    }

    result.hit =
        (result.collider != nullptr);

    return result.hit;
}

bool RayCast::FanCast(
    const RayCastSetting& setting,
    std::vector<RayHit>& hits) {
    hits.clear();

    if (setting.horizontalRayCount <= 0)
        return false;

    bool hitAny = false;

    float interval = 0.0f;

    if (setting.horizontalRayCount > 1) {
        interval =
            (setting.endHorizontalAngle -
             setting.startHorizontalAngle)
            / (setting.horizontalRayCount - 1);
    }

    for (int i = 0;
         i < setting.horizontalRayCount;
         i++) {
        RayCastSetting currentSetting =
            setting;

        currentSetting.startHorizontalAngle =
            setting.startHorizontalAngle +
            interval * i;

        currentSetting.endHorizontalAngle =
            currentSetting.startHorizontalAngle;

        RayHit hit;

        if (Cast(
            currentSetting,
            hit)) {
            hits.push_back(hit);
            hitAny = true;
        }
    }

    return hitAny;
}

VECTOR RayCast::GetDirection(
    const RayCastSetting& setting) {
    VECTOR dir =
        VNorm(setting.direction);

    MATRIX yaw =
        MGetRotY(
            setting.startHorizontalAngle *
            DX_PI_F / 180.0f);

    MATRIX pitch =
        MGetRotX(
            setting.startVerticalAngle *
            DX_PI_F / 180.0f);

    dir = VTransform(dir, yaw);
    dir = VTransform(dir, pitch);

    return VNorm(dir);
}