#pragma once
#include "../../GameObject/GameObject.h"

/*
 * @file Collider.h
 * @author Tanaka
 */

enum class ColliderType {
	Box,
	Sphere,
	Capsule,
	AABB,
	OBB
};

class Collider {
public:
	Collider(GameObject* owner);
	~Collider();
};

