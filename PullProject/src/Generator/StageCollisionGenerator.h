#pragma once
#include <string>
#include "../Manager/CollisionManager.h"


class StageCollisionGenerator
{
public:
	void GenerateFromUnity(const std::string& path, CollisionManager& manager);
};