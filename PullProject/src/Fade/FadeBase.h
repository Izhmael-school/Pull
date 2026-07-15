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
	virtual void Render();

	void FadeStart(FadeState _state, float _time = 1.0f);

	inline FadeState GetCurrentState() const { return currentState; }
private:


private:
	FadeState currentState;
	float alpha;
	float time;
};

