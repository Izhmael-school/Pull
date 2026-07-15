#include "FadeManager.h"

FadeManager::FadeManager() 
{ Start(); }


void FadeManager::Start() {
	fade[FadeNormal] = std::make_unique<FadeBase>();
}

void FadeManager::Update() {
	// フェードが無ければ帰る
	if (!currentFade) return;

	prevFadeState = currentFadeState;
	currentFadeState = currentFade->GetCurrentState();
	// フェードアウトが終わったらフェードインに入る
	if (prevFadeState == FadeOut && currentFadeState == FadeNone) {
		FadeEnd();
		FadeStart(FadeIn, currentFadeType,time);
	}
	// フェードインが終わったら終了宣言
	if (prevFadeState == FadeIn && currentFade->GetCurrentState() == FadeNone)
		FadeEnd();
}

void FadeManager::Render() {
	if (!currentFade) return;

	currentFade->Render();

}

void FadeManager::FadeStart(FadeState _state, FadeType _type, float _time) {
	if (currentFade || _state == FadeNone || _type == FadeMax || _time <= 0) return;

	time = _time;
	currentFade = std::move(fade[_type]);
	currentFade->FadeStart(_state, _time / 2);
	currentFadeType = _type;
	currentFadeState = _state;
}

void FadeManager::FadeEnd() {
	fade[currentFadeType] = std::move(currentFade);
	currentFadeState = FadeNone;
}
