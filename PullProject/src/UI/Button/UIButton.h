/*
 *	@file	UIButton.h
 *  @author oorui
 */

#ifndef _UIBUTTON_H_
#define _UIBUTTON_H_

#include "../UIGroup.h"
#include <functional>

class UIButton : public UIGroup {

public:

	UIButton();
	~UIButton() override = default;

protected:

	void OnDraw() override;

public:

	void SetSelected(bool selected) { m_isSelected = selected; }
	bool IsSelected() const { return m_isSelected; }

	/*
	 *	決定時のイベントを設定する
	 */
	void SetOnClick(const std::function<void()>& onClick) { m_onClick = onClick; }

	/*
	 * 決定時の処理を実行する
	 */
	void Execute();


private:

	/*
	 * ボタンの選択状態
	 */
	bool m_isSelected;

	/*
	 *	決定時のイベント処理
	 */
	std::function<void()> m_onClick;
};



#endif // !_UIBUTTON_H_
