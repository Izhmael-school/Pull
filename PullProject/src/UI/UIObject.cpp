/*
 *	@file	UIObject.cpp
 *  @author oorui
 */

#include "UIObject.h"

#include<vector>
#include <memory>

UIObject::UIObject() :
	m_position(Vector2::Zero),
	m_visible(true),
	m_parent(nullptr) {
}

/*
 *	初期設定
 */
void UIObject::Init() {

	// 自身固有の初期化処理
	OnInit();

	// 子オブジェクトの初期化
	for (auto& child : m_children) {

		// 子オブジェクトがなければスルー
		if (!child) continue;

		child->Init();
	}
}

/*
 *	更新処理
 */
void UIObject::Update(float deltaTime) {

	// 自身固有の更新処理
	OnUpdate(deltaTime);

	// 子オブジェクトの更新処理
	for (auto& child : m_children) {

		// 子オブジェクトがなければスルー
		if (!child) continue;

		child->Update(deltaTime);
	}
}

/*
 *	描画処理
 */
void UIObject::Draw() {

	// 自身か親が非表示なら描画しないので即時return
	if (!IsVisibleInHierarchy()) return;

	// 自身固有の描画処理
	OnDraw();

	// 子オブジェクトの描画処理
	for (auto& child : m_children) {

		// 子オブジェクトがなければスルー
		if (!child) continue;

		child->Draw();
	}
}

/*
 *	終了処理
 */
void UIObject::End() {
	// 子オブジェクトの終了処理
	for (auto& child : m_children) {

		// 子オブジェクトがなければスルー
		if (!child) continue;

		child->End();
	}

	// 自身固有の終了処理
	// 終了処理は最後にする
	OnEnd();
}

/*
 *	ワールド座標の取得
 */
Vector2 UIObject::GetWorldPosition() const {

	// 親オブジェクトがなければ
	if (m_parent == nullptr) {

		// 自分が親オブジェクトなので自分の座標を返す
		return m_position;
	}

	// ここに来たら、自身が子のオブジェクトなので、親の座標を足して返す
	return m_parent->GetWorldPosition() + m_position;
}

/*
 *	親階層の表示状態を考慮した結果を取得
 */
bool UIObject::IsVisibleInHierarchy() const {

	// 自分が非表示かチェック
	if (!m_visible) return false;

	// 親オブジェクトがあるか確認
	// 親がいれば、親の表示状態を返す
	if (m_parent) return m_parent->IsVisibleInHierarchy();

	// ここまで来たら表示で返す
	return true;
}

/*
 *	子オブジェクトの追加
 */
void UIObject::AddChild(std::unique_ptr<UIObject> child) {

	// オブジェクトが不正だったら即時return
	if (!child) return;

	// 子のオブジェクトの親を設定
	child->m_parent = this;

	// 子オブジェクトの初期設定
	child->Init();

	// 子のオブジェクトとして登録
	m_children.push_back(std::move(child));
}
