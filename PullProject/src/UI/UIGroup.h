/*
 *	@file	UIGroup.h
 *  @author oorui
 */

#include "UIObject.h"
#include "../Definition/CommonModule/MyMath.h"

/*
 * UIObjectをまとめて挙動させるためのクラス
 * 座標や表示をグループ単位でまとめて管理できる 
 * このクラスは描画する情報は持たない 
 */
class UIGroup : public UIObject {

public:

	UIGroup() = default;
	UIGroup(const Vector2& position) { UIObject::SetPosition(position); }
	~UIGroup() override = default;

protected:

	/*
	 * UIGroupクラスでは描画はしない
	 * 空の処理にしておく 
	 */
	void OnDraw() override {}

};

