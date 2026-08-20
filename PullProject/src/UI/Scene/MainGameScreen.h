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
	UIImage* leverJumpUI = nullptr;
	UIText* coinCount;
	UIText* scoreCount;
	UIText* eventText = nullptr;


	bool isLeverVisible = false;
	bool isMisileVisible = false;
	bool isLeverJumpVisible = false;
public:

	void Init() override {
		// フォントを追加
		AddFontResourceEx(
			"res/UI/FONT/Kaisotai-Next-UP-B.ttf",
			FR_PRIVATE,
			NULL
		);

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

		// レバージャンプUI表示
		leverJumpUI = CreateUIObject<UIImage>(
			LoadGraph("res/UI/Lever_JumpUI.png"),
			Vector2(0, 0)
		);
		// 最初は非表示
		leverJumpUI->SetVisible(false);

		// イベントカメラ時UIテキスト
		// テキストスタイルを生成
		UITextStyle eventStyle;
		eventStyle.fontName = "Kaisotai Next UP B";
		eventStyle.fontSize = 220;
		eventStyle.fontThickness = 5;
		eventStyle.normalColor = white;
		eventStyle.outLineColor = black;

		// スタートテキスト生成
		eventText = CreateUIObject<UIText>(
			"ゴールをめざせ",
			eventStyle,
			Vector2(190, 140)
		);
		eventText->SetVisible(false);

		UITextStyle defaultStyle;
		defaultStyle.fontSize = 50;
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

	/*
	 *	ジャンプレバーを掴んだ時
	 */
	void SetLeverJumpUIVisible(bool visible) {
		isLeverJumpVisible = visible;
		leverJumpUI->SetVisible(visible);

		// UI表示
		UpdateOperationUI();
	}

	void UpdateOperationUI() {
		// レバー・ミサイルのどちらも表示していないときだけ操作UIを表示
		operationUI->SetVisible(!(isLeverVisible || isMisileVisible || isLeverJumpVisible));
	}

	/*
	 *	イベントテキストを表示
	 */
	void eventTextSetvisible(bool visible) {
		eventText->SetVisible(visible);
	}
};

#endif // !_MAINGAMESCREEN_H_
