/*
 *	@file	UIManager.h
 *  @author oorui
 */

#include <vector>
#include <memory>

#include "UIInput.h"
#include "UICommand.h"
 // 前方宣言
class UIScreen;

class UIManager {
public:
	UIManager() = default;
	~UIManager() = default;

	/*
	 * 最前面のUIScreenを更新
	 */
	void Update(float deltaTime, const UIInput& input);

	/*
	 * 管理しているUIScreenをすべて描画する
	 */
	void Draw();

	/*
	 * UIScreenを最前面に登録する
	 */
	void PushScreen(std::unique_ptr<UIScreen> pScreen);

	/*
	 * 最前面のUIScreenを解放する
	 */
	void PopScreen();

	/*
	 * 最前面のUIScreenを取得する
	 */
	UIScreen* GetTopScreen();

	/*
	 * 管理しているUIScreenが空か取得する
	 */
	bool Empty() const { return m_screens.empty(); }

	/*
	 * 最前面のUICommandを取得する、その後初期値に戻す
	 */
	UICommand ConsumeCommand();

private:

	/*
	 * 管理しているUIScreen群
	 */
	std::vector<std::unique_ptr<UIScreen>> m_screens;
};