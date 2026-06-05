#include "DirectInputPad.h"

DirectInputPad::DirectInputPad(int _portNum)
	:PadBase(_portNum) {
	Start();
}

void DirectInputPad::Start() {
	padType = GetJoypadType(connectIndex);
}

float DirectInputPad::StickNorm(int _v) {
	float f = _v / 1000.0f;

	// 絶対値
	if (fabs(f) < DEADZONE)
		return 0.0f;

	return f;
}

void DirectInputPad::Update() {
	prevPadState = currentPadState;

	int result = GetJoypadDirectInputState(connectIndex, &currentPadState);

	if (result == 0) {
		isConnect = true;

		lx = StickNorm(currentPadState.X);
		ly = StickNorm(currentPadState.Y);
		// コントローラによって右スティックの取得が違うため分ける
		switch (padType) {
		case DX_PADTYPE_SWITCH_PRO_CTRL:
			rx = StickNorm(currentPadState.Rx);
			ry = StickNorm(currentPadState.Ry);
			break;
		case DX_PADTYPE_OTHER:
			rx = StickNorm(currentPadState.Z);
			ry = StickNorm(currentPadState.Rz);
			break;
		default:
			rx = StickNorm(currentPadState.Rx);
			ry = StickNorm(currentPadState.Ry);
			break;
		}

		// 向きを変える
		ly *= -1;
		ry *= -1;
	}
	else {
		isConnect = false;
		currentPadState = {};
		lx = ly = rx = ry = 0;
	}
}

bool DirectInputPad::IsPad(int _pad) const {
	return currentPadState.Buttons[_pad] == DI_PUSH_NUM;
}

bool DirectInputPad::IsPadDown(int _pad) const {
	return currentPadState.Buttons[_pad] == DI_PUSH_NUM && prevPadState.Buttons[_pad] != DI_PUSH_NUM;
}

inline bool DirectInputPad::IsPadUp(int _pad) const {
	return currentPadState.Buttons[_pad] != DI_PUSH_NUM && prevPadState.Buttons[_pad] == DI_PUSH_NUM;
}
