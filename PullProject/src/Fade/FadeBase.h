/*
 * @brief フェードの基底（徐々に暗くなっていくだけのフェード）
 * @author Sekino
 */
#pragma once
#include "Definition/Enum/FadeEnum.h"

constexpr float BLEND_MAX = 255.0f;

class FadeBase {

public:
	FadeBase() = default;
	~FadeBase() = default;
	virtual void Update();
	virtual void Render();

	void FadeStart(FadeState _state, float _time = 1.0f , int _color = 0x000000);

	inline FadeState GetCurrentState() const { return currentState; }
private:
	FadeState currentState = FadeState::FadeNone;
	float alpha = 255.0f;
	float time = 0.0f;
	int color = 0x000000;
};

