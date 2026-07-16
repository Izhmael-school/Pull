/*
 *	@file	MainGameScreen.h
 *  @author oorui
 */

#ifndef _MAINGAMESCREEN_H_
#define _MAINGAMESCREEN_H_

#include "../UIScreen.h"
#include "../Button/UIButton.h"
#include "../Image/UIImage.h"
#include "../Text/UIText.h"
#include "../UIManager.h"
#include "Game/GameData.h"

class MainGameScreen :public UIScreen {
	UIText* coinCount;
	UIText* scoreCount;
public:
	// UI描画準備
	void Init() override {
		UITextStyle defaultStyle;
		defaultStyle.fontSize = 50;
		auto coinIcon = CreateUIObject<UIImage>(LoadGraph("res/Sprite/Game/CoinIcon.png"), Vector2(50, 50));
		coinCount = CreateUIObject<UIText>(std::to_string(GameData::GetCoin()), defaultStyle, Vector2(100, 50));
		scoreCount = CreateUIObject<UIText>(std::to_string(GameData::GetScore()), defaultStyle, Vector2(1600, 50));

	}

	void Update(float deltaTime, const UIInput& input) override;
};

#endif // !_MAINGAMESCREEN_H_
