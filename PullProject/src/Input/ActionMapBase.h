/*
 *	@file	ActionMapBase.h
 *  @author Riku
 */

#ifndef _ACTIONMAPBASE_H_
#define _ACTIONMAPBASE_H_

#include "../Definition/CommonModule/ActionMapData.h"
#include <variant>

/*
 *	アクションマップの基底クラス
 */
class ActionMapBase {
public:
	// 入力
	struct Input {
		InputType type;
		int forward = -1;
		int back = -1;
		int right = -1;
		int left = -1;
	};
	// 入力の設定
	struct Binding {
		int action;			// 入力に対する行動
		BindingType type;	// 入力の種類
		Input input;		// 確認する入力
	};

	std::vector<Binding> bindings;	// アクションマップの入力設定
	ActionState state;				// 各アクションマップの入力状態
	bool isActive = false;			// アクションマップの有効状態


public:
	ActionMapBase() = default;
	virtual ~ActionMapBase() = default;

public:
	/*
	 *	ボタンの軸入力設定
	 *	@param	int	action		行動設定
	 *	@param	InputType type	入力機の種類
	 *	@param	int forward		前方向ボタン
	 *	@param	int back		後方向ボタン
	 *	@param	int right		右方向ボタン
	 *	@param	int left		左方向ボタン
	 */
	void AddAxisButton(int action, InputType type, int forward, int back, int right, int left) {
		bindings.push_back({
			action, 
			BindingType::AxisButton,
			{type, forward, back, right, left}
			});
	}

	/*
	 *	ボタン入力設定
	 *	@param	int	action		行動設定
	 *	@param	InputType type	入力機の種類
	 *	@param	int button		入力ボタン
	 */
	void AddButton(int action, InputType type, int button) {
		bindings.push_back({
			action,
			BindingType::Button, 
			{type, button}
			});
	}

	/*
	 *	2軸入力機の軸入力設定
	 *	@param	int action		行動設定
	 *	@param	InputType type	入力機の種類
	 */
	void AddAxisValue(int action, InputType type) {
		bindings.push_back({
			action,
			BindingType::AxisValue,
			type
			});
	}

	/*
	 *	入力の更新
	 */
	void InputUpdate();

	/*
	 *	入力状態のリセット
	 */
	void InputReset() {
		for (auto& [key, vaule] : state.axis)		vaule = { 0.0f, 0.0f };
		for (auto& [key, vaule] : state.button)     vaule = false;
		for (auto& [key, vaule] : state.buttonDown) vaule = false;
		for (auto& [key, vaule] : state.buttonUp)   vaule = false;
	}

public:
	/*
	 *	初期化処理
	 */
	virtual void Start() = 0;
};

#endif // !_ACTIONMAPBASE_H_