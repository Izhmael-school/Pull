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
	, prevMouseState(-1) 
	, nowMousePosX(0.0f)
	, prevMousePosX(0.0f)
	, nowMousePosY(0.0f)
	, prevMousePosY(0.0f)
	, mouseVisible(true)
	, prevInputMouse(false)
	, mouseMoveSkip(false) 
	, WINDOW_WIDTH(1920)
	, WINDOW_HEIGHT(1080){
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

	// マウスポインターの更新	@author Riku
	UpdateMousePointer();
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

/*
 *	マウスポインターの更新
 *	@author Riku
 */
void InputManager::UpdateMousePointer() {
	// マウスの表示非表示切り替え
	SetMouseDispFlag(mouseVisible);

	// 1フレーム前のマウスの位置を保存
	prevMousePosX = nowMousePosX;
	prevMousePosY = nowMousePosY;
	// 現在のマウスの位置を保存
	GetMousePoint(&nowMousePosX, &nowMousePosY);
	// マウスカーソルが非表示ならカーソルは中央固定
	if (!mouseVisible) {
		// 1フレーム目はスキップ
		if (mouseMoveSkip) {
			mouseMoveSkip = false;
		}
		else {
			// 画面中央
			int windowWidthCenter = WINDOW_WIDTH / 2;
			int windowHeightCenter = WINDOW_HEIGHT / 2;
			// マウスを画面中央に固定
			SetMousePoint(windowWidthCenter, windowHeightCenter);
			// 1フレーム前位置は画面中央とする
			prevMousePosX = windowWidthCenter;
			prevMousePosY = windowHeightCenter;
		}
	}

	// 直前の入力がマウスかどうか管理
	for (auto key : currentKeyState) {
		if (key == 1)
			prevInputMouse = false;
	}
	if (currentMouseState != 0 ||
		prevMousePosX - nowMousePosX != 0 ||
		prevMousePosY - nowMousePosY != 0) {
		prevInputMouse = true;
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
