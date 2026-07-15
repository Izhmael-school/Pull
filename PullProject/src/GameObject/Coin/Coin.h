/*
 * @brief コイン
 * `author Sekino
 */
#pragma once
#ifndef _COIN_H_
#define _COIN_H_

#include "../GameObject.h"

constexpr char const BOTTOM_FRAME_NAME[] = "BottomPoint";

class Coin : public GameObject {
private:
	int addScore;

public:
	Coin(int _modelHandle = -1, VECTOR _pos = VZero, Tag _tag = None);
	~Coin() = default;

private:
	void Start();

public:
	void OnTriggerEnter(Collider* _pSelf, Collider* _pOther) override;

	VECTOR GetBottomFramePos() { return MV1GetFramePosition(modelHandle, MV1SearchFrame(modelHandle, BOTTOM_FRAME_NAME)); }
};
#endif