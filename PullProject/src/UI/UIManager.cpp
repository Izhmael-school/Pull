/*
 *	@file	UIManager.cpp
 *  @author oorui
 */
#include "UIManager.h"
#include "UIScreen.h"

 /*
  * 最前面のUIScreenを更新
  */
void UIManager::Update(float deltaTime, const UIInput& input) {

	// 管理してるUIが空なら即時return
	if (Empty()) return;

	// 最前面のUIScreenを更新する
	m_screens.back()->Update(deltaTime, input);
}

/*
 * 管理しているUIScreenをすべて描画する
 */
void UIManager::Draw() {

	for (auto& screen : m_screens) {

		screen->Draw();
	}
}

/*
 * UIScreenを最前面に登録する
 */
void UIManager::PushScreen(std::unique_ptr<UIScreen> pScreen) {

	// 渡されたUIScreenが空なら即時return
	if (!pScreen) return;

	// UIScreenの初期化
	pScreen->Init();

	// UIScreenを(末尾に)登録
	m_screens.push_back(std::move(pScreen));
}

/*
 * 最前面のUIScreenを解放する
 */
void UIManager::PopScreen() {

	// 管理してるUIが空なら即時return
	if (Empty()) return;

	// 管理しているUIScreenの末尾の物を削除
	m_screens.pop_back();
}

/*
 * 最前面のUIScreenを取得する
 */
UIScreen* UIManager::GetTopScreen() {

	// 管理してるUIが空ならnullptrを返す
	if (Empty()) return nullptr;

	// 最前面のUIScreenの生ポインタを返す
	return m_screens.back().get();
}

/*
 * 最前面のUICommandを取得する、その後初期値に戻す
 */
UICommand UIManager::ConsumeCommand() {

	if (m_screens.empty()) return UICommand::None;

	return m_screens.back()->ConsumeCommand();
}

