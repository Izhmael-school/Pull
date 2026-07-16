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

class MainGameScreen :public UIScreen {
public:
	
	void Init() override {
		// UI表示
		auto actionUI = CreateUIObject<UIImage>(
			LoadGraph("res/UI/actionUI.png"),
			Vector2(0, 0)
		);
	}
};

#endif // !_MAINGAMESCREEN_H_
