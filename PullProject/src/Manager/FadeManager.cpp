#include "FadeManager.h"

FadeManager::FadeManager() 
{ Start(); }


void FadeManager::Start() {
	fade[FadeNormal] = std::make_unique<FadeBase>();
}

void FadeManager::Update() {
	// フェードが無ければ帰る
	if (!currentFade) return;

	// フェードの更新
	currentFade->Update();

	prevFadeState = currentFadeState;
	currentFadeState = currentFade->GetCurrentState();

	// フェードインが終わったら終了宣言
	if (currentFade->GetCurrentState() == FadeNone)
		FadeEnd();
}

void FadeManager::Render() {
	if (!currentFade) return;

	currentFade->Render();

}

void FadeManager::FadeStart(FadeState _state, FadeType _type, float _time) {
	if (_state == FadeNone || _type == FadeMax || _time <= 0) return;

	time = _time;
	currentFadeType = _type;
	currentFadeState = _state;
	currentFade = fade[currentFadeType].get();
	currentFade->FadeStart(_state, _time);
}

void FadeManager::FadeEnd() {
	currentFadeState = FadeNone;
	currentFade = nullptr;
}

