/*
 * @file InputManager.cpp
 * @author Sekino
 */
#include "InputManager.h"
#include "../Pad/DirectInputPad.h"
#include "../Pad/XInputPad.h"
#include "../Pad/PadBase.h"

InputManager::InputManager()
	:currentKeyState{}
	, prevKeyState{}
	, currentMouseState(-1)
	, prevMouseState(-1) {
	Start();
}

InputManager::~InputManager()
{
}

void InputManager::Start() {
	memset(currentKeyState, 0, KEY_NUM);
	memset(prevKeyState, 0, KEY_NUM);
}

void InputManager::Update() {
	// キーの更新
	memcpy_s(prevKeyState, KEY_NUM, currentKeyState, KEY_NUM);
	GetHitKeyStateAll(currentKeyState);

	// マウスの更新
	prevMouseState = currentMouseState;
	currentMouseState = GetMouseInput();

	// コントローラの更新
	UpdatePad();
}

void InputManager::UpdatePad() {
	for (int i = 0; i < MAX_PAD_PORT; i++) {
		// パッドが存在しない場合は作成
		if (!pads[i]) {
			int index = DX_INPUT_PAD1 + i;
			XINPUT_STATE xi;
			// -1じゃなければXInputとして扱う
			int result = GetJoypadXInputState(index, &xi);
			if (result != -1)
				pads[i] = std::make_unique<XInputPad>(index);
			else {
				DINPUT_JOYSTATE di;
				result = GetJoypadDirectInputState(index, &di);
				// -1じゃなければDirectInputとして扱う
				if(result != -1)
					pads[i] = std::make_unique<DirectInputPad>(index);
			}
		}

		// パッドを更新
		if (pads[i]) {
			pads[i]->Update();

			// 接続していない場合はリセット
			if (!pads[i]->IsConnect()) {
				pads[i].reset();
			}
		}
	}
}

int InputManager::ExchangeXInputButton(int _XINPUT, int _padNum) {
	int padType = GetJoypadType(_padNum);

	int buttonNum = -1;

	switch (padType) {
	case DX_PADTYPE_SWITCH_PRO_CTRL:
		switch (_XINPUT) {
		case XINPUT_BUTTON_A:
			buttonNum = DI_SWITCH_BUTTON_A;
			break;
		case XINPUT_BUTTON_B:
			buttonNum = DI_SWITCH_BUTTON_B;
			break;
		case XINPUT_BUTTON_X:
			buttonNum = DI_SWITCH_BUTTON_X;
			break;
		case XINPUT_BUTTON_Y:
			buttonNum = DI_SWITCH_BUTTON_Y;
			break;
		case XINPUT_BUTTON_START:

			break;
		case XINPUT_BUTTON_BACK:
			break;
		case XINPUT_BUTTON_RIGHT_SHOULDER:
			break;
		case XINPUT_BUTTON_RIGHT_THUMB:
			break;
		case XINPUT_BUTTON_LEFT_SHOULDER:
			break;
		case XINPUT_BUTTON_LEFT_THUMB:
			break;
		default:
			break;
		}
		break;
	case DX_PADTYPE_OTHER:
		break;
	default:
		break;
	}

	return buttonNum;
}
