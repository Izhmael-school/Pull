/*
 *	@file	UIInput.h 
 *  @author oorui
 */

#ifndef _UIINPUT_H_
#define _UIINPUT_H_

/*
 *	Sceneで検知した入力を構造体にまとめる
 *  この構造体でUIに渡すことで入力処理をUIに持たせないようにする
 */
struct UIInput {

	bool up = false;
	bool down = false;
	bool right = false;
	bool left = false;

	bool decide = false;
	bool cancel = false;
};

#endif // !_UIINPUT_H_