#pragma once

#include "../UIObject.h"

#include <string>

#include "UITextStyle.h"

/// <summary>
/// 汎用テキストUIオブジェクト
/// </summary>
class UIText : public UIObject
{

public:

	UIText();
	UIText( const std::string& text, const UITextStyle& style, const Vector2& position);
	virtual ~UIText() override = default;

	//====================================================
	//	UIObjectクラスのメンバ関数
	//====================================================

protected:

	void OnInit() override;
	void OnUpdate(float deltaTime)override;
	void OnDraw()override;
	void OnEnd() override;

	//====================================================
	//	UITextクラスの固有処理
	//====================================================

public:

	/// <summary>
	/// フォントデータからFontManagerにハンドルを取得
	/// </summary>
	void Build();

	void SetText( const std::string& text) { m_text = text; }
	void SetStyle(const UITextStyle& style) { m_style = style; }
	void SetSelected(bool isSelected) { m_isSelected = isSelected; }

protected:

	/// <summary>
	/// 表示する文字列
	/// </summary>
	std::string m_text;

	/// <summary>
	/// テキストの見た目の設定
	/// </summary>
	UITextStyle m_style;

	/// <summary>
	/// フォントハンドル
	/// FontManagerから取得する
	/// </summary>
	int m_fontHandle;

	/// <summary>
	/// 選択状態かどうか
	/// </summary>
	bool m_isSelected;

};

