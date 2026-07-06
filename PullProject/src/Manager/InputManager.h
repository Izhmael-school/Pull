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
#include "../Definition/Const/VECTORConst.h"

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

	/*
	 *	マウスポインターの更新
	 *	@author Riku
	 */
	void UpdateMousePointer();

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

	/*
	 *	マウスの移動量取得
	 *	@retrun VECETOR	移動量
	 *  @outhor Riku
	 */
	inline VECTOR GetMouseMove() {
		VECTOR move = VZero;
		move.x = prevMousePosX - nowMousePosX;
		move.y = prevMousePosY - nowMousePosY;
		return move;
	}
	/*
	 *	マウスの位置取得
	 *	@param[out]	int mousePosX	マウスの位置X
	 *	@param[out] int mousePosY	マウスの位置Y
	 *  @outhor Riku
	 */
	inline void GetMousePosition(int& mousePosX, int& mousePosY) {
		if (!mouseVisible) {
			mousePosX = -1;
			mousePosY = -1;
		}
		else {
			mousePosX = nowMousePosX;
			mousePosY = nowMousePosY;
		}
	}

#pragma endregion

	// コントローラの取得
	PadBase* GetPad(int _index) const { return pads[_index].get(); }

	// XINPUTのボタン番号をDirectInputのボタン番号に変換
	int ExchangeXInputButton(int _XINPUT, int _padNum);

	/*
	 *	マウスカーソルの表示非表示切り替え
	 *	@param[in]	bool setVisible	切り替え先
	 *  @outhor Riku
	 */
	inline void SetMouseVisible(bool setVisible) {
		mouseVisible = setVisible;
		mouseMoveSkip = true;
	}
	/*
	 *	直前の入力がマウスかどうか取得
	 *  @return bool
	 *  @author Riku
	 */
	inline bool GetPrevInputMouse() {
		return prevInputMouse;
	}

private:
	// キーボードの入力状況
	char currentKeyState[KEY_NUM];
	char prevKeyState[KEY_NUM];

	// マウスの入力状況
	int currentMouseState;
	int prevMouseState;

	// コントローラ管理配列
	std::array<std::unique_ptr<PadBase>, MAX_PAD_PORT> pads;

	/* @author Riku */
	int nowMousePosX;		// 現在のマウス位置X
	int prevMousePosX;		// 直前のマウス位置X
	int nowMousePosY;		// 現在のマウス位置Y
	int prevMousePosY;		// 直前のマウス位置Y
	bool mouseVisible;		// マウスカーソルの表示非表示フラグ
	bool prevInputMouse;	// 直前の入力がマウスかどうか
	bool mouseMoveSkip;		// マウスの中央固定を1フレームスキップするためのフラグ

	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
};