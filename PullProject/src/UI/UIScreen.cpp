/*
 *	@file	UIScreen.cpp
 *  @author oorui
 */

#include "UIScreen.h"
#include "UIObject.h"

/*
 *	更新処理
 */
void UIScreen::Update(float deltaTime, const UIInput& input) {

	for (auto& object : m_rootObjects) {

		// AddUIObjectでnullptrチェックをしているが念のため確認
		if (!object) continue;

		object->Update(deltaTime);
	}
}

/*
 *	描画処理
 */
void UIScreen::Draw() {

	for (auto& object : m_rootObjects) {

		// AddUIObjectでnullptrチェックをしているが念のため確認
		if (!object) continue;

		// 表示設定を確認、非表示ならスルー
		if (!object->IsVisible()) continue;

		object->Draw();
	}
}

/*
 *	コマンドを取得、その後初期化値を返す
 */
UICommand UIScreen::ConsumeCommand() {

	auto command = m_command;
	m_command = UICommand::None;
	return command;
}

/*
 *	UIオブジェクトの生成
 */
void UIScreen::AddUIObject(std::unique_ptr<UIObject> object) {

	// 空だったら即時return
	if (!object) return;

	object->Init();

	m_rootObjects.push_back(std::move(object));
}
