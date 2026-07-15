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

private:
	void FadeEnd();

private:
	std::unique_ptr<FadeBase> fade[FadeMax];
	std::unique_ptr<FadeBase> currentFade;

	FadeType currentFadeType;
	FadeState currentFadeState;
	FadeState prevFadeState;

	float time;
};

