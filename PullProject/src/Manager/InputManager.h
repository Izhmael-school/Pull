/*
 * @file InputManager.h
 * @author Sekino
 */
#pragma once
#include "../Component/Singleton.h"	
#include "ManagerBase.h"
#include "DxLib.h"
#include <array>
#include <memory>
#include "../Definition/Const/InputConst.h"

class PadBase;

/// <summary>
/// 入力管理クラス
/// </summary>
class InputManager : public ManagerBase, public Singleton<InputManager> {
private:
	// 初期化処理
	void Start() override;
public:
	InputManager();
	~InputManager();

	// 更新処理
	void Update() override;

private:
	// コントローラの更新
	void UpdatePad();

public:
#pragma region KeyBoard
	/**
	押されているか
	KEY_INPUT_##
	*/
	inline bool IsKey(int _key) const { return currentKeyState[_key]; }
	/**
	押されたか
	KEY_INPUT_##
	*/
	inline bool IsKeyDown(int _key) const { return currentKeyState[_key] && !prevKeyState[_key]; }
	/**
	離したか
	KEY_INPUT_##
	*/
	inline bool IsKeyUp(int _key) const { return  !currentKeyState[_key] && prevKeyState[_key]; }
#pragma endregion

#pragma region Mouse

	/**
	  押されているか
	  MOUSE_INPUT_##
	*/
	inline bool IsMouse(int _mouse) const { return currentMouseState & _mouse; }
	/**
	  押されたか
	  MOUSE_INPUT_##
	*/
	inline bool IsMouseDown(int _mouse) const { return (currentMouseState & _mouse) && !(prevMouseState & _mouse); }
	/**
	  離したか
	  MOUSE_INPUT_##
	*/
	inline bool IsMouseUp(int _mouse) const { return !(currentMouseState & _mouse) && (prevMouseState & _mouse); }
#pragma endregion

	// コントローラの取得
	PadBase* GetPad(int _index) const { return pads[_index].get(); }

	// XINPUTのボタン番号をDirectInputのボタン番号に変換
	int ExchangeXInputButton(int _XINPUT, int _padNum);
private:
	// キーボードの入力状況
	char currentKeyState[KEY_NUM];
	char prevKeyState[KEY_NUM];

	// マウスの入力状況
	int currentMouseState;
	int prevMouseState;

	// コントローラ管理配列
	std::array<std::unique_ptr<PadBase>, MAX_PAD_PORT> pads;

};