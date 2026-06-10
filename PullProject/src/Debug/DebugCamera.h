#pragma once
#include "../Component/Transform.h"

/*
 * @file Transform.h
 * @author Sekino
 */

class DebugCamera
{
public:
	void Update();

	void Render();
	Transform transform;

	bool moveStop;
};

