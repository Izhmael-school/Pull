/*
 *	@file	PlayerActionMap.cpp
 *	@author Riku
 */

#include "PlayerActionMap.h"
#include "../Manager/InputSystemManager.h"
#include "../Definition/Enum/PlayerActionEnum.h"
#include <DxLib.h>

void PlayerActionMap::Start() {
	// 入力設定
#if _DEBUG
	// キーボード
	// 移動
	AddAxisButton(
		static_cast<int>(PlayerAction::Move),
		InputType::Key,
		KEY_INPUT_W,
		KEY_INPUT_S, 
		KEY_INPUT_D,
		KEY_INPUT_A
	);
	// ジャンプ
	AddButton(
		static_cast<int>(PlayerAction::Jump),
		InputType::Key,
		KEY_INPUT_SPACE
	);
	// ウデ伸ばし
	AddButton(
		static_cast<int>(PlayerAction::ArmExtend),
		InputType::Key,
		KEY_INPUT_E
	);
	// 掴みキャンセル
	AddButton(
		static_cast<int>(PlayerAction::CatchCancel),
		InputType::Key,
		KEY_INPUT_F
	);
	// カメラの見渡し
	AddAxisButton(
		static_cast<int>(PlayerAction::CameraMove),
		InputType::Key,
		KEY_INPUT_UP,
		KEY_INPUT_DOWN,
		KEY_INPUT_RIGHT,
		KEY_INPUT_LEFT
	);
#endif
	// ゲームパッド
	// 移動
	AddAxisValue(
		static_cast<int>(PlayerAction::Move),
		InputType::PadStickL
	);
	// ジャンプ
	AddButton(
		static_cast<int>(PlayerAction::Jump),
		InputType::PadButton,
		XINPUT_BUTTON_A
	);
	// ウデ伸ばし
	AddButton(
		static_cast<int>(PlayerAction::ArmExtend),
		InputType::PadButton,
		XINPUT_BUTTON_RIGHT_SHOULDER
	);
	// 掴みキャンセル
	AddButton(
		static_cast<int>(PlayerAction::CatchCancel),
		InputType::PadButton,
		XINPUT_BUTTON_X
	);
	// カメラの見渡し
	AddAxisValue(
		static_cast<int>(PlayerAction::CameraMove),
		InputType::PadStickR
	);

	// 値の初期化
	state.axis[static_cast<int>(PlayerAction::Move)] = { 0.0f, 0.0f };
	state.buttonDown[static_cast<int>(PlayerAction::Jump)] = false;
	state.button[static_cast<int>(PlayerAction::ArmExtend)] = false;
	state.buttonDown[static_cast<int>(PlayerAction::ArmExtend)] = false;
	state.buttonDown[static_cast<int>(PlayerAction::CatchCancel)] = false;
	state.axis[static_cast<int>(PlayerAction::CameraMove)] = { 0.0f, 0.0f };

}
