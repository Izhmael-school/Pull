#define NOMINMAX
#include <Windows.h>

#include "StageCollisionGenerator.h"
#include <string>
#include <vector>
#include <cmath>

#include "../Definition/CommonModule/MyJson.h"
#include "../Component/Collider/Collider.h"
#include "../GameObject/GameObject.h"

struct AABB
{
    VECTOR min;
    VECTOR max;
};

bool IsNear(float a, float b, float eps = 0.01f)
{
    return fabsf(a - b) < eps;
}

bool CanMerge(const AABB& a, const AABB& b) {
    // X方向に隣接
    bool xAdjacent =
        IsNear(a.max.x, b.min.x) || IsNear(b.max.x, a.min.x);

    // Z方向に隣接
    bool zAdjacent =
        IsNear(a.max.z, b.min.z) || IsNear(b.max.z, a.min.z);

    // Y方向に隣接（段差の結合）
    bool yAdjacent =
        IsNear(a.max.y, b.min.y) || IsNear(b.max.y, a.min.y);

    // 同じ高さ・奥行き（許容誤差あり）
    bool sameY = IsNear(a.min.y, b.min.y, 0.1f) && IsNear(a.max.y, b.max.y, 0.1f);
    bool sameZ = IsNear(a.min.z, b.min.z, 0.1f) && IsNear(a.max.z, b.max.z, 0.1f);
    bool sameX = IsNear(a.min.x, b.min.x, 0.1f) && IsNear(a.max.x, b.max.x, 0.1f);

    // X方向の結合
    if (xAdjacent && sameY && sameZ) return true;

    // Z方向の結合
    if (zAdjacent && sameY && sameX) return true;

    // Y方向の結合（段差をまとめる）
    if (yAdjacent && sameX && sameZ) return true;

    return false;
}

    AABB MergeTwo(const AABB & a, const AABB & b)
    {
        AABB r;

        r.min.x = (std::min)(a.min.x, b.min.x);
        r.min.y = (std::min)(a.min.y, b.min.y);
        r.min.z = (std::min)(a.min.z, b.min.z);

        r.max.x = (std::max)(a.max.x, b.max.x);
        r.max.y = (std::max)(a.max.y, b.max.y);
        r.max.z = (std::max)(a.max.z, b.max.z);

        return r;
    }

void MergeAABB3D(std::vector<AABB>& boxes)
{
    bool merged = true;

    while (merged)
    {
        merged = false;

        for (size_t i = 0; i < boxes.size(); i++)
        {
            for (size_t j = i + 1; j < boxes.size(); j++)
            {
                if (CanMerge(boxes[i], boxes[j]))
                {
                    boxes[i] = MergeTwo(boxes[i], boxes[j]);
                    boxes.erase(boxes.begin() + j);
                    merged = true;
                    break;
                }
            }

            if (merged) break;
        }
    }
}

void StageCollisionGenerator::GenerateFromUnity(
    const std::string& path,
    CollisionManager& manager)
{
    auto json = MyJson::LoadJsonFile(path);

    if (json.is_null())
    {
        printfDx("JSON load failed\n");
        return;
    }

    std::vector<AABB> boxes;

    float scale = 100.0f;

    for (auto& b : json["blocks"])
    {

        if (b.contains("type") && b["type"] == "tree" || b.contains("type") && b["type"] == "bridge ")
        {
            continue;
        }


        if (!b.contains("minX") || !b.contains("maxX"))
            continue;

        float minX = b["minX"];
        float minY = b["minY"];
        float minZ = b["minZ"];

        float maxX = b["maxX"];
        float maxY = b["maxY"];
        float maxZ = b["maxZ"];

        VECTOR min = VGet(-minX, minY, -minZ);
        VECTOR max = VGet(-maxX, maxY, -maxZ);

        min = VScale(min, scale);
        max = VScale(max, scale);

        if (min.x > max.x) std::swap(min.x, max.x);
        if (min.z > max.z) std::swap(min.z, max.z);

        boxes.push_back({ min, max });
    }
   
    MergeAABB3D(boxes);

    int colCount = 0;

    for (auto& b : boxes)
    {
        GameObject* obj = new GameObject(-1, VZero, Tag::None);

        AABBCollider* col = std::make_unique<AABBCollider>(obj, VZero, VZero).release();
        col->SetMin(b.min);
        col->SetMax(b.max);

        col->SetLayer(ColliderLayer::Stage);
        colCount++;
    }
    printfDx("collider: %d\n", colCount);
}
