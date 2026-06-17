/*
 * @brief 時間管理クラス
 * @author Sekino
 */

#pragma once
#ifndef _TIMEMANAGER_H_
#define _TIMEMANAGER_H_

#include "Component/Singleton.h"
#include "ManagerBase.h"

class TimeManager : public Singleton<TimeManager> ,public ManagerBase{
private:
	// 初期化処理
	void Start() override;
public:
	TimeManager();
	~TimeManager() = default;
	// 更新処理
	void Update() override;
	// 時間の取得
	float GetDeltaTime() const { return deltaTime; }

private:
	int prevTime;
	int currentTime;

	float deltaTime;
};

#endif