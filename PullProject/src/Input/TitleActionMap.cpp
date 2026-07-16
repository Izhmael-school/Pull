#include "TitleActionMap.h"
#include "../Manager/InputSystemManager.h"
#include "../Definition/Enum/TitleActionEnum.h"
#include <DxLib.h>


void TitleActionMap::Start() {
	// キーボード
	AddButton(
		static_cast<int>(TitleAction::SelectMove_UP),
		InputType::Key,
		KEY_INPUT_UP
	);

	AddButton(
		static_cast<int>(TitleAction::SelectMove_DOWN),
		InputType::Key,
		KEY_INPUT_DOWN
	);

	AddButton(
		static_cast<int>(TitleAction::Click),
		InputType::Key,
		KEY_INPUT_RETURN
	);

	// ゲームパッド
	// カーソル移動
	AddButton(
		static_cast<int>(TitleAction::SelectMove_UP),
		InputType::PadButton,
		XINPUT_BUTTON_DPAD_UP
	);

	AddButton(
		static_cast<int>(TitleAction::SelectMove_DOWN),
		InputType::PadButton,
		XINPUT_BUTTON_DPAD_DOWN
	);

	AddButton(
		static_cast<int>(TitleAction::Click),
		InputType::PadButton,
		XINPUT_BUTTON_A
	);

	// 値の初期化
	state.buttonDown[static_cast<int>(TitleAction::SelectMove_UP)] = false;
	state.buttonDown[static_cast<int>(TitleAction::SelectMove_DOWN)] = false;
	state.buttonDown[static_cast<int>(TitleAction::Click)] = false;

}
