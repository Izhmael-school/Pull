#include "TimeManager.h"
#include "DxLib.h"

TimeManager::TimeManager() 
	:prevTime(-1)
	,currentTime(-1)
	,deltaTime(0.0f)
{ Start(); }

void TimeManager::Start() {
	// 起動時の時間を取得
	prevTime = GetNowCount();
	currentTime = prevTime;
	Update();
}

void TimeManager::Update() {
	// 今の時間を取得
	currentTime = GetNowCount();
	// 前フレームとの差を計算
	deltaTime = static_cast<float>(currentTime - prevTime);
	// Unity風にするため
	deltaTime /= 1000.0f;

	prevTime = currentTime;
}
