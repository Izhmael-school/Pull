/*
 *	@file	ActionMapData.h
 *	@author Riku
 */

#ifndef _ACTIONMAPDATA_H_
#define _ACTIONMAPDATA_H_

#include <unordered_map>

 // 入力機の種類 
enum class InputType {
	Key,			// キーボード
	MouseButton,	// マウスのボタン
	MouseMove,		// マウスの移動
	PadButton,		// パッドのボタン
	PadStickL,		// パッドのLスティック
	PadStickR,		// パッドのRスティック
};

// 入力の種類
enum class BindingType {
	AxisButton,	// 軸のボタン入力
	Button,		// ボタン入力
	AxisValue,	// 軸の入力量
};

// 2軸
struct Axis2D {
	float x;
	float y;
};

// 行動の状態
struct ActionState {
	std::unordered_map<int, Axis2D> axis;
	std::unordered_map<int, bool> button;
	std::unordered_map<int, bool> buttonDown;
	std::unordered_map<int, bool> buttonUp;
};

// 各アクションマップ
enum class ActionMap {
	Invalid = -1,
	PlayerAction,
	StageSelect,

	Max
};

#endif // !_ACTIONMAPDATA_H_
