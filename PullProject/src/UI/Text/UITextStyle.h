#pragma once

#include <string>

#include "Definition/Const/ColorConst.h"

/// <summary>
/// UITextの見た目に関する情報をまとめた構造体
/// ゲームの情報(座標や状態は含めない)
/// </summary>
struct UITextStyle
{

	/// <summary>
	/// 通常時の文字の色
	/// </summary>
	int normalColor = white;

	/// <summary>
	/// 縁の色
	/// </summary>
	int outLineColor = black;

	/// <summary>
	/// 選択時の文字の色
	/// </summary>
	int selectedColor = red;

	/// <summary>
	/// フォント名
	/// </summary>
	std::string fontName = "Meiryo";

	/// <summary>
	/// フォントのサイズ
	/// </summary>
	int fontSize = 10;

	/// <summary>
	/// フォントの太さ
	/// </summary>
	int fontThickness = 5;

};
