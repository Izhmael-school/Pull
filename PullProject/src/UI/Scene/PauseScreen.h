/*
 *	@file	PauseScreen.h
 *  @author oorui
 */

#ifndef _PAUSESCREEN_H_
#define _PAUSESCREEN_H_

#include "../UIScreen.h"
#include "../Button/UIButton.h"
#include "../Image/UIImage.h"
#include "../Text/UIText.h"
#include "../UICommand.h"
#include "../UIManager.h"
#include "../../Manager/SceneManager.h"

class PauseScreen : public UIScreen {
private:
	UIImage* m_backGround = nullptr;    // ポーズ中の背景の画像
	UIImage* m_pauseWindow = nullptr;   // ポーズ中に表示する画面

	UIButton* m_resumeButton = nullptr;         // ゲーム再開ボタン
	UIButton* m_stageSelectButton = nullptr;    // ステージセレクトに戻るボタン
	UIButton* m_titleButton = nullptr;          // タイトルに戻るボタン
	// UIButton* m_volumeButton = nullptr;         // 音量変更ボタン

	UIText* m_resumeText = nullptr;             // ゲーム再開テキスト
	UIText* m_stageSelectText = nullptr;        // ステージセレクトに戻るテキスト
	UIText* m_titleText = nullptr;              // タイトルに戻るテキスト
	// UIText* m_volumeText = nullptr;             // ボリュームテキスト
	// UIText* m_volumeValue = nullptr;            // 音量

	int m_selectIndex = 0;  // 現在選択中かどうか
	int m_volume = 100;     // 音量の値
protected:

	/*
	 * ボタンの選択状態更新
	 */
	void UpdateSelect();

	/*
	 * 決定処理
	 */
	void ExecuteSelect();

	/*
	 *  初期化時の処理をまとめる
	 */
	void SetupUICreate();

	/*
	 *  選択状態を一度に変更する
	 */
	void ALLSetSelect(bool flag);

public:

	/*
	 * 初期設定
	 */
	void Init() override;

	/*
	 * 更新処理
	 */
	void Update(float deltaTime, const UIInput& uiInput) override;

	void Draw()override;


};

#endif // !_PAUSESCREEN_H_
