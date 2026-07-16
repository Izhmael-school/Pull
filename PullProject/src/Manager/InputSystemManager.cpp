/*
 *	@file	InputSystemManager.cpp
 *	@author Riku
 */

#include "InputSystemManager.h"
#include "InputManager.h"
#include "../Pad/PadBase.h"
#include "../Input/PlayerActionMap.h"
#include "../Input/StageSelectActionMap.h"
#include "../Input/TitleActionMap.h"

InputSystemManager::InputSystemManager() {
	Start();
}

InputSystemManager::~InputSystemManager()
{}

// 初期化処理
void InputSystemManager::Start() {
	// 全ての入力受付の生成
	actionMaps[ActionMap::PlayerAction] = std::make_shared<PlayerActionMap>();
	actionMaps[ActionMap::StageSelect] = std::make_shared<StageSelectActionMap>();
	actionMaps[ActionMap::TitleAction] = std::make_shared<TitleActionMap>();
	for (auto [mapNum, actionMap] : actionMaps) {
		// 全アクションマップの初期化
		actionMap->Start();
	}
}

// 更新処理
void InputSystemManager::Update() {
	// 各アクションマップの更新処理
	for (auto [mapNum, actionMap] : actionMaps) {
		// まず入力状態のリセット
		actionMap->InputReset();
		// 有効なら更新処理
		if (actionMap->isActive)
			actionMap->InputUpdate();
	}
}

/*
 *	ボタンが押されたかどうか
 *  @param[in]	InputType _type	種類
 *  @param[in]	int _button						入力
 *	@return		bool
 */
bool InputSystemManager::IsInputDown(InputType _type, int _button) {
	auto& input = InputManager::GetInstance();
	switch (_type) {
	case InputType::Key:
		return input.IsKeyDown(_button);
	case InputType::MouseButton:
		return input.IsMouseDown(_button);
	case InputType::PadButton:
		auto pad = input.GetPad(0);
		if (!pad)
			return false;
		return pad->IsPadDown(_button);
	}
}
/*
 *	ボタンが押されているかどうか
 *  @param[in]	InputType _type	種類
 *  @param[in]	int _button						入力
 *	@return		bool
 */
bool InputSystemManager::IsInput(InputType _type, int _button) {
	auto& input = InputManager::GetInstance();
	switch (_type) {
	case InputType::Key:
		return input.IsKey(_button);
	case InputType::MouseButton:
		return input.IsMouse(_button);
	case InputType::PadButton:
		auto pad = input.GetPad(0);
		if (!pad)
			return false;
		return pad->IsPad(_button);
	}
}
/*
 *	ボタンを離したかどうか
 *  @param[in]	InputType _type	種類
 *  @param[in]	int _button						入力
 *	@return		bool
 */
bool InputSystemManager::IsInputUp(InputType _type, int _button) {
	auto& input = InputManager::GetInstance();
	switch (_type) {
	case InputType::Key:
		return input.IsKeyUp(_button);
	case InputType::MouseButton:
		return input.IsMouseUp(_button);
	case InputType::PadButton:
		auto pad = input.GetPad(0);
		if (!pad)
			return false;
		return pad->IsPadUp(_button);
	}
}

/*
 *	2軸入力の値取得
 *  @param[in]	InputType _type	種類
 *	@return		bool
 */
VECTOR InputSystemManager::GetAxisValue(InputType _type) {
	auto& input = InputManager::GetInstance();
	auto pad = input.GetPad(0);
	VECTOR move = VZero;
	// マウスの移動入力量
	if (_type == InputType::MouseMove) {
		move = input.GetMouseMove();
	}
	// コントローラースティック
	else if (pad) {
		if (_type == InputType::PadStickL)
			move = pad->GetLStick();
		if (_type == InputType::PadStickR)
			move = pad->GetRStick();
	}
	return move;
}
