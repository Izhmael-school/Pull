#pragma once
#include "Component/Singleton.h"
#include "Manager/ManagerBase.h"
#include <memory>
#include "../Fade/FadeBase.h"

class FadeManager : public Singleton<FadeManager>, public ManagerBase {
public:
	FadeManager();
	~FadeManager() = default;

	void Start() override;
	void Update() override;
	void Render() override;

	void FadeStart(FadeState _state, FadeType _type, float _time = 1.0f);

	inline FadeState GetCurrentState() const { return currentFadeState; }

	/*
	 * @brief フェードインの終了検知
	 */
	inline bool IsFadeInEnd() const { return prevFadeState == FadeState::FadeIn && currentFadeState == FadeState::FadeNone; }

	/*
	 * @brief フェードアウトの終了検知
	 */
	inline bool IsFadeOutEnd() const { return prevFadeState == FadeState::FadeOut && currentFadeState == FadeState::FadeNone; }

	/*
	 * @brief フェードの終了検知
	 */
	inline bool IsFadeEnd() const { return prevFadeState != FadeState::FadeNone && currentFadeState == FadeState::FadeNone; }

private:
	void FadeEnd();

private:
	std::unique_ptr<FadeBase> fade[FadeMax];
	FadeBase* currentFade;

	FadeType currentFadeType;
	FadeState currentFadeState;
	FadeState prevFadeState;

	float time;
};
