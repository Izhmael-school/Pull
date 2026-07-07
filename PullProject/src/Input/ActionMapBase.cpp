/*
 *	@file	ActionMapBase.cpp
 *  @author Riku
 */

#include "ActionMapBase.h"
#include "../Manager/InputSystemManager.h"
#include "../Manager/InputManager.h"
#include "../Pad/PadBase.h"

void ActionMapBase::InputUpdate() {
	// 入力のタイプ別で入力判定
	for (auto& bind : bindings) {
		auto& input = InputSystemManager::GetInstance();
		switch (bind.type) {
		case BindingType::AxisButton:{
			// 軸入力
			if (input.IsInput(bind.input.type, bind.input.forward)) {
				state.axis[bind.action].y += 1.0f;
			}
			if (input.IsInput(bind.input.type, bind.input.back)) {
				state.axis[bind.action].y -= 1.0f;
			}
			if (input.IsInput(bind.input.type, bind.input.right)) {
				state.axis[bind.action].x += 1.0f;
			}
			if (input.IsInput(bind.input.type, bind.input.left)) {
				state.axis[bind.action].x -= 1.0f;
			}
			break;
		}
		case BindingType::Button:{
			// ボタン入力
			// 押した瞬間
			if (input.IsInputDown(bind.input.type, bind.input.forward)) {
				state.buttonDown[bind.action] = true;
			}
			// 押している間
			if (input.IsInput(bind.input.type, bind.input.forward)) {
				state.button[bind.action] = true;
			}
			// 離した瞬間
			if (input.IsInputUp(bind.input.type, bind.input.forward)) {
				state.buttonUp[bind.action] = true;
			}
			break;
		}
		case BindingType::AxisValue:{
			VECTOR move = input.GetAxisValue(bind.input.type);
			if (move.x == 0.0f && move.y == 0.0f)
				break;
			state.axis[bind.action].x = move.x;
			state.axis[bind.action].y = move.y;
			break;
		}
		}
	}
}
