#pragma once
#include "PadBase.h"
class XInputPad : public PadBase {
private:
	// ƒRƒ“ƒgƒ[ƒ‰(XInput)‚Ì“ü—Íó‹µ
	XINPUT_STATE currentPadState{};
	XINPUT_STATE prevPadState{};

public:
	XInputPad(int _portNum);
	~XInputPad() = default;

	void Update() override;

#pragma region Pad

	/**
	  ‰Ÿ‚³‚ê‚Ä‚¢‚é‚©
	  XINPUT_BUTTON_##
   */
	bool IsPad(int _pad) const override;
	/**
	  ‰Ÿ‚³‚ê‚½‚©
	  MOUSE_BUTTON_##
	*/
	bool IsPadDown(int _pad) const override;
	/**
	  —£‚µ‚½‚©
	  MOUSE_BUTTON_##
	*/
	inline bool IsPadUp(int _pad) const override;
#pragma endregion
};

