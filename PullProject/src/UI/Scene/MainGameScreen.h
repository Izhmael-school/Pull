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
#include "Manager/PlayerManager.h"

class MainGameScreen :public UIScreen {
	UIImage* leverUI = nullptr;
	UIImage* misileUI = nullptr;
	UIImage* operationUI = nullptr;
	UIText* coinCount;
	UIText* scoreCount;

	bool isLeverVisible = false;
	bool isMisileVisible = false;
public:

	void Init() override {
		// レバー掴み時UIを生成
		leverUI = CreateUIObject<UIImage>(
			LoadGraph("res/UI/actionUI.png"),
			Vector2(0, 0)
		);
		// 最初は非表示
		leverUI->SetVisible(false);

		// 敵掴み時UIを生成
		misileUI = CreateUIObject<UIImage>(
			LoadGraph("res/UI/enemyCatchUI.png"),
			Vector2(0, 0)
		);
		// 最初は非表示
		misileUI->SetVisible(false);

		// 操作UI表示
		operationUI = CreateUIObject<UIImage>(
			LoadGraph("res/UI/operationsUI.png"),
			Vector2(0, 0)
		);
		operationUI->SetVisible(false);

		UITextStyle defaultStyle;
		defaultStyle.fontSize = 50;
		auto coinIcon = CreateUIObject<UIImage>(LoadGraph("res/Sprite/Game/CoinIcon.png"), Vector2(50, 50));
		coinCount = CreateUIObject<UIText>("", defaultStyle, Vector2(100, 50));
		scoreCount = CreateUIObject<UIText>("", defaultStyle, Vector2(1600, 50));

	}

	void Update(float deltaTime, const UIInput& input) override;

	/*
	 *	レバーを掴んだとき
	 */
	void SetLeverUIVisible(bool visible) {
		isLeverVisible = visible;
		leverUI->SetVisible(visible);

		UpdateOperationUI();
	}

	/*
	 *	ミサイルを掴んだ時
	 */
	void SetMisileUIVisible(bool visible) {
		isMisileVisible = visible;
		misileUI->SetVisible(visible);

		UpdateOperationUI();
	}

	void UpdateOperationUI() {
		// レバー・ミサイルのどちらも表示していないときだけ操作UIを表示
		operationUI->SetVisible(!(isLeverVisible || isMisileVisible));
	}
};

#endif // !_MAINGAMESCREEN_H_
