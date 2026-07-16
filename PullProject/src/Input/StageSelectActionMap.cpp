/*
 *	@file	StageSelectActionMap.cpp
 *  @author oorui
 */

#include "StageSelectActionMap.h"
#include "../Manager/InputSystemManager.h"
#include "../Definition/Enum/StageSelectActionEnum.h"
#include <DxLib.h>
void StageSelectActionMap::Start() {
	// キーボード
	AddButton(
		static_cast<int>(StageSelectAction::SelectMove_UP),
		InputType::Key,
		KEY_INPUT_UP
	);

	AddButton(
		static_cast<int>(StageSelectAction::SelectMove_DOWN),
		InputType::Key,
		KEY_INPUT_DOWN
	);

	AddButton(
		static_cast<int>(StageSelectAction::Click),
		InputType::Key,
		KEY_INPUT_RETURN
	);

	// ゲームパッド
	// カーソル移動
	AddButton(
		static_cast<int>(StageSelectAction::SelectMove_UP),
		InputType::PadButton,
		XINPUT_BUTTON_DPAD_UP
	);

	AddButton(
		static_cast<int>(StageSelectAction::SelectMove_DOWN),
		InputType::PadButton,
		XINPUT_BUTTON_DPAD_DOWN
	);

	AddButton(
		static_cast<int>(StageSelectAction::Click),
		InputType::PadButton,
		XINPUT_BUTTON_B
	);

	// 値の初期化
	state.buttonDown[static_cast<int>(StageSelectAction::SelectMove_UP)] = false;
	state.buttonDown[static_cast<int>(StageSelectAction::SelectMove_DOWN)] = false;
	state.buttonDown[static_cast<int>(StageSelectAction::Click)] = false;

}
