#include "FadeBase.h"
#include "DxLib.h"
#include "Manager/TimeManager.h"

void FadeBase::Render() {
	if (currentState == FadeNone)return;

	// アルファの増減
	alpha += (BLEND_MAX * TimeManager::GetInstance().GetDeltaTime() / time) * static_cast<int>(currentState);

	switch (currentState) {
	case FadeIn:
		if (alpha > 0.0f) break;
		alpha = 0;
		currentState = FadeNone;
		break;
	case FadeOut:
		if (alpha < BLEND_MAX) break;
		alpha = BLEND_MAX;
		currentState = FadeNone;
		break;
	}

	//	透明度を変化させてフェード処理とする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha);
	DrawFillBox(0, 0, 1920, 1080, 0x000000);
	// ブレンドモードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, -1);
}

void FadeBase::FadeStart(FadeState _state, float _time) {
	if (_state == FadeNone || currentState != FadeNone) return;
	currentState = _state;
	time = _time;
	alpha = currentState == FadeIn ? BLEND_MAX : 0.0f;
}