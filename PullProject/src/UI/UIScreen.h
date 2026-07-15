/*
 *	@file	UIScreen.h
 *  @author oorui
 */

#ifndef _UISCREEN_H_
#define _UISCREEN_H_

#include <vector>
#include <memory>
#include <type_traits>

#include "UIInput.h"
#include "UICommand.h"
#include "UIObject.h"

class UIScreen {
public:
	UIScreen() = default;
	virtual ~UIScreen() = default;

	virtual void Init() {};
	virtual void Update(float deltaTime, const UIInput& input);
	virtual void Draw();

	/*
	 *	UIオブジェクトの生成
	 */
	template <class T, class... Args>
	T* CreateUIObject(Args&&... args);

	/*
	 *	コマンドを取得、その後初期値を返す
	 */
	UICommand ConsumeCommand();

protected:

	/*
	 * UIオブジェクトの追加
	 */
	void AddUIObject(std::unique_ptr<UIObject> object);

	/*
	 *	コマンドを設定
	 */
	void SetUICommand(UICommand command) { m_command = command; }


protected:

	/*
	 *	このクラスで管理するオブジェクト
	 */
	std::vector<std::unique_ptr<UIObject>> m_rootObjects;

	/*
	 *	外部に送るUIのコマンド	
	 */
	UICommand m_command = UICommand::None;
};


template <class T, class... Args>
T* UIScreen::CreateUIObject(Args&&... args) {

	// UIObjectクラスを継承しているかチェック
	//static_assert(std::is_base_of<UIObject, T>::value,
	//	"UIScreen:CreateUIObject:UIObjectクラスを継承していない");

	// オブジェクト生成
	auto uiObject = std::make_unique<T>(std::forward<Args>(args)...);

	// 初期化
	uiObject->Init();

	// UIScreenがUIObjectを管理するが、
	// 個別で処理をするために外部参照用に生ポインタを取得する
	T* rawPtr = uiObject.get();

	// スマートポインタをm_rootObjectsに登録
	m_rootObjects.push_back(std::move(uiObject));

	return rawPtr;
}

#endif // !_UISCREEN_H_
