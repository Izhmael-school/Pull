#include "MainGameActionMap.h"
#include "../Manager/InputSystemManager.h"
#include "../Definition/Enum/MainGameActionEnum.h"
#include <DxLib.h>

void MainGameActionMap::Start() {
	// ゲームパッド
	
	// 上移動
	AddButton(
		static_cast<int>(MainGameAction::Up),
		InputType::PadButton,
		XINPUT_BUTTON_DPAD_UP
	);

	// 下移動
	AddButton(
		static_cast<int>(MainGameAction::Down),
		InputType::PadButton,
		XINPUT_BUTTON_DPAD_DOWN
	);

	// キャンセル
	AddButton(
		static_cast<int>(MainGameAction::cancel),
		InputType::PadButton,
		XINPUT_BUTTON_B
	);

	// 決定
	AddButton(
		static_cast<int>(MainGameAction::decide),
		InputType::PadButton,
		XINPUT_BUTTON_A
	);

	// START
	AddButton(
		static_cast<int>(MainGameAction::pause),
		InputType::PadButton,
		XINPUT_BUTTON_START
	);

	// 値の初期化
	state.buttonDown[static_cast<int>(MainGameAction::cancel)] = false;
	state.buttonDown[static_cast<int>(MainGameAction::pause)] = false;
	state.buttonDown[static_cast<int>(MainGameAction::decide)] = false;
	state.buttonDown[static_cast<int>(MainGameAction::Down)] = false;
	state.buttonDown[static_cast<int>(MainGameAction::Up)] = false;

}
