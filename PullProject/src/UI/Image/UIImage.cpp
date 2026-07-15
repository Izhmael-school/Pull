/*
 *	@file	UIImage.cpp
 *  @author oorui
 */

#include"UIImage.h"
#include "../UIObject.h"

UIImage::UIImage() :
	m_graphHandle(-1) {
}

UIImage::UIImage(int handle, const Vector2& position) :
	m_graphHandle(handle) {

	UIObject::SetPosition(position);
}

void UIImage::OnInit() {
}

void UIImage::OnUpdate(float deltaTime) {
}

void UIImage::OnDraw() {

	// 表示するかチェックはDraw関数でやってるのでここではやらない
	// 画像ハンドルが読み込みされていなければ即時return
	if (m_graphHandle == -1) return;

	// ワールド座標を取得
	// この座標を使って描画する
	Vector2 worldPosition = GetWorldPosition();

	// ワールド座標を使って描画
	DrawGraph(
		static_cast<int>(worldPosition.x),
		static_cast<int>(worldPosition.y),
		m_graphHandle, true
	);
}

void UIImage::OnEnd() {
}
