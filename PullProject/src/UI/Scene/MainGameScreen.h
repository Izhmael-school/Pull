/*
 *	@file	MainGameScreen.h
 *  @author oorui
 */

#ifndef _MAINGAMESCREEN_H_
#define _MAINGAMESCREEN_H_

#include "../UIScreen.h"
#include "../Button/UIButton.h"
#include "../Image/UIImage.h"
#include "../UIManager.h"
#include "UI/Text/UIText.h"

class MainGameScreen :public UIScreen {
	UIText* coinCount;
	UIText* scoreCount;
public:

	void Init() override {
		// UI表示
		auto actionUI = CreateUIObject<UIImage>(
			LoadGraph("res/UI/actionUI.png"),
			Vector2(0, 0)
		);

		UITextStyle defaultStyle;
		defaultStyle.fontSize = 50;
		auto coinIcon = CreateUIObject<UIImage>(LoadGraph("res/Sprite/Game/CoinIcon.png"), Vector2(50, 50));
		coinCount = CreateUIObject<UIText>("", defaultStyle, Vector2(100, 50));
		scoreCount = CreateUIObject<UIText>("", defaultStyle, Vector2(1600, 50));

	}

	void Update(float deltaTime, const UIInput& input) override;
};

#endif // !_MAINGAMESCREEN_H_
