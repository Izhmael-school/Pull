#include "XInputPad.h"

XInputPad::XInputPad(int _portNum)
	:PadBase(_portNum)
{}

void XInputPad::Update() {
	prevPadState = currentPadState;

	int result = GetJoypadXInputState(connectIndex, &currentPadState);

	if (result == 0) {
		isConnect = true;

		lx = StickNorm(currentPadState.ThumbLX);
		ly = StickNorm(currentPadState.ThumbLY);
		rx = StickNorm(currentPadState.ThumbRX);
		ry = StickNorm(currentPadState.ThumbRY);

		lt = currentPadState.LeftTrigger / 255.0f;
		rt = currentPadState.RightTrigger / 255.0f;
	}
	else {
		isConnect = false;
		currentPadState = {};
		lx = ly = rx = ry = 0;
		lt = rt = 0;
	}
}

bool XInputPad::IsPad(int _pad) const { return isConnect && (currentPadState.Buttons[_pad] != 0); }

bool XInputPad::IsPadDown(int _pad) const { return isConnect && (currentPadState.Buttons[_pad] != 0 && prevPadState.Buttons[_pad] == 0); }

inline bool XInputPad::IsPadUp(int _pad) const { return isConnect && (currentPadState.Buttons[_pad] == 0 && prevPadState.Buttons[_pad] != 0); }
