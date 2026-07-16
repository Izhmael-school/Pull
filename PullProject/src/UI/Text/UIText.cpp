#include "UIText.h"

#include "../UIObject.h"

#include <string>
#include <DxLib.h>

#include "../Font/FontManager.h"
#include "Definition/Const/ColorConst.h"

namespace {

	constexpr int kDefaultFontSize = 32;			// デフォルトのフォントサイズ
	constexpr int kDefaultFontThickness = 3;		// デフォルトのフォントの太さ
	const char* const kDefaultFontName = "Meiryo";	// デフォルトのフォントの名前
}

UIText::UIText():
	m_text("Text Unset"),
	m_fontHandle(-1),
	m_isSelected(false)
{

	m_style.normalColor = white;
	m_style.selectedColor = red;
	m_style.fontName = kDefaultFontName;
	m_style.fontSize = kDefaultFontSize;
	m_style.fontThickness = kDefaultFontThickness;
}

UIText::UIText(const std::string& text, const UITextStyle& style, const Vector2& position):
	m_text(text),
	m_style(style),
	m_fontHandle(-1),
	m_isSelected(false)
{

	UIObject::SetPosition(position);
}

void UIText::OnInit()
{

	Build();
}

void UIText::OnUpdate(float deltaTime)
{
}

void UIText::OnDraw()
{

	// 表示するかチェックはDraw関数でやってるのでここではやらない

	// ワールド座標を取得
	// この座標を使って描画する
	Vector2 worldPosition = GetWorldPosition();

	// 色の設定
	int color = m_style.normalColor;
	
	if(m_isSelected) color = m_style.selectedColor;

	// ワールド座標を使って描画
	DrawStringToHandle(
		static_cast<int>(worldPosition.x),
		static_cast<int>(worldPosition.y),
		m_text.c_str(),
		color,
		m_fontHandle,
		m_style.outLineColor
		);
}

void UIText::OnEnd()
{
}

void UIText::Build()
{
	// FontManagerからフォントを取得
	m_fontHandle = FontManager::GetInstance().GetFontHandle(
		m_style.fontName,
		m_style.fontSize,
		m_style.fontThickness,
		DX_FONTTYPE_ANTIALIASING_EDGE,
		2
	);
}
