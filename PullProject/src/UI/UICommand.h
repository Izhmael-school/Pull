/*
 *	@file	UICommand.h
 *  @author oorui
 */

#ifndef _UICOMMAND_H_
#define _UICOMMAND_H_

enum class UICommand {

	None,


    // ゲームへ戻る
    ResumeGame,

    // ステージセレクト
    LoadStageSelectScene,

    // タイトルへ戻る
    LoadTitleScene,

    // 音量画面
    OpenVolume,

};

#endif // !_UICOMMAND_H_
