/*
 * @file PadBase.h
 * @author Sekino
 */
#pragma once
#include "DxLib.h"
#include <cmath>
#include "../Definition/Const/InputConst.h"


/// <summary>
/// コントローラクラス
/// </summary>
class PadBase {
protected:
	// ポート番号
	int connectIndex;
	// 接続されてるか
	bool isConnect;

	// スティックの位置
	float lx, ly, rx, ry;
	// トリガー
	float lt, rt;

protected:
	// スティックの正規化
	virtual float StickNorm(float _v);

public:
	PadBase(int _connectIndex);
	// 更新処理
	virtual void Update() = 0;
	// 接続されているか
	bool IsConnect() const { return isConnect; }

#pragma region Stick
	VECTOR GetRStick() const { return VGet(rx, ry, 0); }
	VECTOR GetLStick() const { return VGet(lx, ly, 0); }
#pragma endregion

#pragma region Trigger
	float GetRTrigger() const { return rt; }
	float GetLTrigger() const { return lt; }
#pragma endregion

#pragma region Pad

	/**
	  押されているか
	  XINPUT_BUTTON_##
   */
	virtual bool IsPad(int _pad) const = 0;
	/**
	  押されたか
	  MOUSE_BUTTON_##
	*/
	virtual bool IsPadDown(int _pad) const = 0;
	/**
	  離したか
	  MOUSE_BUTTON_##
	*/
	virtual bool IsPadUp(int _pad) const = 0;
#pragma endregion
};


