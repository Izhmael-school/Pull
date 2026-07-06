/*
 *	@file	InputSystemManager.cpp
 *	@author Riku
 */

#include "InputSystemManager.h"
#include "InputManager.h"
#include "../Input/ActionMapBase.h"
#include "../Pad/PadBase.h"

InputSystemManager::InputSystemManager()
{}

InputSystemManager::~InputSystemManager()
{}

// 初期化処理
void InputSystemManager::Start() {
}

// 更新処理
void InputSystemManager::Update() {
	
}

/*
 *	ボタンが押されたかどうか
 *  @param[in]	ActionMapBase::InputType _type	種類
 *  @param[in]	int _button						入力
 *	@return		bool
 */
bool InputSystemManager::IsInputDown(ActionMapBase::InputType _type, int _button) {
	auto& input = InputManager::GetInstance();
	switch (_type) {
	case ActionMapBase::InputType::Key:
		return input.IsKeyDown(_button);
	case ActionMapBase::InputType::MouseButton:
		return input.IsMouseDown(_button);
	case ActionMapBase::InputType::PadButton:
		auto pad = input.GetPad(0);
		if (!pad)
			return false;
		return pad->IsPadDown(_button);
	}
}
/*
 *	ボタンが押されているかどうか
 *  @param[in]	ActionMapBase::InputType _type	種類
 *  @param[in]	int _button						入力
 *	@return		bool
 */
bool InputSystemManager::IsInput(ActionMapBase::InputType _type, int _button) {
	auto& input = InputManager::GetInstance();
	switch (_type) {
	case ActionMapBase::InputType::Key:
		return input.IsKey(_button);
	case ActionMapBase::InputType::MouseButton:
		return input.IsMouse(_button);
		auto pad = input.GetPad(0);
		if (!pad)
			return false;
		return pad->IsPad(_button);
	}
}
/*
 *	ボタンを離したかどうか
 *  @param[in]	ActionMapBase::InputType _type	種類
 *  @param[in]	int _button						入力
 *	@return		bool
 */
bool InputSystemManager::IsInputUp(ActionMapBase::InputType _type, int _button) {
	auto& input = InputManager::GetInstance();
	switch (_type) {
	case ActionMapBase::InputType::Key:
		return input.IsKeyUp(_button);
	case ActionMapBase::InputType::MouseButton:
		return input.IsMouseUp(_button);
		auto pad = input.GetPad(0);
		if (!pad)
			return false;
		return pad->IsPadUp(_button);
	}
}
