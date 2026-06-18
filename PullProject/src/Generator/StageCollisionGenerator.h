#pragma once
#include <string>
#include "../Manager/CollisionManager.h"

class StageCollisionGenerator
{
public:
    void Generate(const std::string& path, CollisionManager& manager);
};
