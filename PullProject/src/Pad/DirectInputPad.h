/*
 * @file DirectInputPad.h
 * @author Sekino
 */
#pragma once
#include "PadBase.h"
class DirectInputPad : public PadBase {
private:
	// ƒRƒ“ƒgƒ[ƒ‰(XInput)‚Ì“ü—Íó‹µ
	DINPUT_JOYSTATE currentPadState{};
	DINPUT_JOYSTATE prevPadState{};

	int padType;

private:
	void Start();

	float StickNorm(int _v);

public:
	DirectInputPad(int _portNum);
	~DirectInputPad() = default;

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

