#ifndef _STAGECOLLISIONGENERATOR_H_
#define _STAGECOLLISIONGENERATOR_H_

#pragma once
#include <string>
#include "Manager/CollisionManager.h"

/*
 * @file StageCollisionGenerator.h
 * @author Tanaka
 */

class StageCollisionGenerator {
public:
	//	UnityのJSONデータから当たり判定を生成する
	void GenerateFromUnity(const std::string& path, CollisionManager& manager);
};

#endif // !_STAGECOLLISIONGENERATOR_H_