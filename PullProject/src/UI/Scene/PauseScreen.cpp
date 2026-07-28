/*
 *	@file	PauseScreen.cpp
 *  @author oorui
 */

#include "PauseScreen.h"
#include "../../Definition/Const/ColorConst.h"

#include <DxLib.h>


 /*
  * 初期化
  */
void PauseScreen::Init() {
	// フォントの登録
	AddFontResourceEx(
		"res/UI/FONT/Kaisotai-Next-UP-B.ttf",
		FR_PRIVATE,
		NULL
	);

	// UIを生成
	SetupUICreate();

	// ゲームを再開を初期位置に設定
	m_selectIndex = 0;
	UpdateSelect();

	// 各ボタンのイベント登録
	m_resumeButton->SetOnClick([this]() {
		SetUICommand(UICommand::ResumeGame);
		});

	m_stageSelectButton->SetOnClick([this]() {
		SetUICommand(UICommand::LoadStageSelectScene);
		});

	m_titleButton->SetOnClick([this]() {
		SetUICommand(UICommand::LoadTitleScene);
		});

	//m_volumeButton->SetOnClick([this]() {
	//	// SetUICommand(UICommand::OpenVolume);
	//	});
}

/*
 * 更新処理
 */
void PauseScreen::Update(float deltaTime, const UIInput& uiInput) {

	// 上方向へ移動
	if (uiInput.up) {
		m_selectIndex--;

		if (m_selectIndex < 0) {
			m_selectIndex = 3;
		}
		// 選択状態更新
		UpdateSelect();
	}

	// 下方向へ移動
	if (uiInput.down) {
		m_selectIndex++;

		if (m_selectIndex > 3) {
			m_selectIndex = 0;
		}
		// 選択状態更新
		UpdateSelect();
	}

	// ゲームを再開
	if (uiInput.cancel || uiInput.pause) {
		// コマンドを実行
		SetUICommand(UICommand::ResumeGame);
		return;
	}

	// 決定
	if (uiInput.decide) {
		ExecuteSelect();
	}

	// UIの更新
	UIScreen::Update(deltaTime, uiInput);
}

void PauseScreen::Draw() {
	UIScreen::Draw();
}

/*
 * 選択状態更新
 */
void PauseScreen::UpdateSelect() {

	// 選択状態を一度解除
	ALLSetSelect(false);


	// 選択中のものだけ
	switch (m_selectIndex) {
		// ゲーム再開
	case 0:
		m_resumeButton->SetSelected(true);
		m_resumeText->SetSelected(true);
		break;
		// ステージセレクト
	case 1:
		m_stageSelectButton->SetSelected(true);
		m_stageSelectText->SetSelected(true);
		break;
		// タイトルに戻る
	case 2:
		m_titleButton->SetSelected(true);
		m_titleText->SetSelected(true);
		break;
		// 音量変更
	case 3:
		// m_volumeButton->SetSelected(true);
		// m_volumeText->SetSelected(true);
		break;
	}
}

/*
 * 決定処理
 */
void PauseScreen::ExecuteSelect() {
	switch (m_selectIndex) {
		// ゲーム再開
	case 0:
		// ボタンの処理を実行
		m_resumeButton->Execute();
		break;
		// ステージセレクト
	case 1:
		// ボタンの処理を実行
		m_stageSelectButton->Execute();
		break;
		// タイトルに戻る
	case 2:
		// ボタンの処理を実行
		m_titleButton->Execute();
		break;
		// 音量変更
	case 3:
		// ボタンの処理を実行
		//m_volumeButton->Execute();
		break;
	default:
		break;
	}
}


/*
 *  初期化時の処理をまとめるs
 */
void PauseScreen::SetupUICreate() {
	// 背景の黒画像登録
	m_backGround = CreateUIObject<UIImage>(
		LoadGraph("res/UI/PauseBack.png"),
		Vector2(0, 0)
	);

	// ポーズ画面用のUIを生成
	m_pauseWindow = CreateUIObject<UIImage>(
		LoadGraph("res/UI/PauseWindow.png"),
		Vector2(0, 0)
	);

	// テキストのスタイルを設定
	UITextStyle style;

	style.fontName = "Kaisotai-Next-UP-B";
	style.fontSize = 100;
	style.fontThickness = 8;

	style.normalColor = white;
	style.selectedColor = red;
	style.outLineColor = black;

	// ゲーム再開ボタンを生成
	m_resumeButton = CreateUIObject<UIButton>();
	// テキストを生成
	m_resumeText = CreateUIObject<UIText>(
		"Restart",
		style,
		Vector2(800, 300)
	);

	// ステージセレクトに戻るボタンを生成
	m_stageSelectButton = CreateUIObject<UIButton>();
	// テキスト生成
	m_stageSelectText = CreateUIObject<UIText>(
		"Stage Select",
		style,
		Vector2(800, 500)
	);

	// タイトルに戻るボタンを生成
	m_titleButton = CreateUIObject<UIButton>();
	// テキストを生成
	m_titleText = CreateUIObject<UIText>(
		"Return Title",
		style,
		Vector2(800, 700)
	);

	//// 音量変更ボタンを生成
	//m_volumeButton = CreateUIObject<UIButton>();
	//// テキストを生成
	//m_volumeText = CreateUIObject<UIText>(
	//	"Volume",
	//	style,
	//	Vector2(800, 460)
	//);


	//// 音量表示用のスタイルを変更
	//UITextStyle volumeStyle;
	//
	//volumeStyle.fontName = "Kaisotai-Next-UP-B";
	//volumeStyle.fontSize = 45;
	//volumeStyle.fontThickness = 4;
	//volumeStyle.normalColor = yellow;
	//volumeStyle.outLineColor = black;
	//
	//// 音量変更ＵＩを生成
	//m_volumeValue = CreateUIObject<UIText>(
	//	"100",
	//	volumeStyle,
	//	Vector2(800, 460)
	//);
}

/*
 *  選択状態を一度に変更する
 */
void PauseScreen::ALLSetSelect(bool flag) {
	m_resumeButton->SetSelected(flag);
	m_stageSelectButton->SetSelected(flag);
	m_titleButton->SetSelected(flag);
	//m_volumeButton->SetSelected(flag);

	m_resumeText->SetSelected(flag);
	m_stageSelectText->SetSelected(flag);
	m_titleText->SetSelected(flag);
	//m_volumeText->SetSelected(flag);
}
