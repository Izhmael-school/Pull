/*
 *	@file	UIButton.cpp
 *  @atthor oorui
 */

#include "UIButton.h"
#include "../UIGroup.h"

#include <cassert>

UIButton::UIButton() :
	m_isSelected(false) {
}

void UIButton::OnDraw() {
}

void UIButton::Execute() {

	// 実行する処理があるかチェック
	if (!m_onClick) {

		// なければ警告
		assert(0 && "UIButton:Execute:onClick is not set");
		return;

	}

	// 設定された処理を実行
	m_onClick();
}
