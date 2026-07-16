#pragma once

#include <string>
#include <vector>

// DXライブラリではフォントは思いのほか気軽には使いにくい
// https://dxlib.xsrv.jp/function/dxfunc_graph2.html#R17N7
// https://dxlib.xsrv.jp/function/dxfunc_graph2.html#R17N10
// なので少しでも使いやすくするために読み込み対応やデータ対応をしておく

/// <summary>
/// フォントを管理するクラス
/// 使用するフォントの重複を防ぎ、共有する
/// シングルトンクラスで運用
/// </summary>
class FontManager
{

public: 

	/// <summary>
	/// フォントの情報をまとめた構造体
	/// FontManagerクラスで管理する
	/// </summary>
	struct FontData {

		//=================================================
		// フォントの設定に必要なもの
		//=================================================

		/// <summary>
		/// フォント名
		/// DXLibのCreateFontToHandle関数にて使用
		/// </summary>
		std::string fontName;

		/// <summary>
		/// フォントのサイズ
		/// </summary>
		int size = 0;

		/// <summary>
		/// フォントの太さ
		/// </summary>
		int thickness = 0;


		//=================================================
		// DXライブラリでフォントを使用するために必要なもの
		//=================================================

		/// <summary>
		/// DXライブラリのフォントハンドル
		/// CreateFontToHandle関数で生成されるID
		/// </summary>
		int handle = -1;

	};


public:

	/// <summary>
	/// シングルトンのインスタンスを取得
	/// </summary>
	/// <returns></returns>
	static FontManager& GetInstance();

	~FontManager() = default;

	void Init();
	void End();

	/// <summary>
	/// 指定したフォントデータのハンドルを取得
	/// すでにあれば既存のハンドルを取得
	/// なければ新規に生成して取得
	/// </summary>
	/// <param name="fontName"></param>
	/// <param name="size"></param>
	/// <param name="thickness"></param>
	/// <returns></returns>
	int GetFontHandle( const std::string& fontName, int size, int thickness);

private:

	/// <summary>
	/// コンストラクタ
	/// シングルトンだからprivate
	/// </summary>
	FontManager() = default;

	// シングルトンクラスなのでprivateにして生成できないようにする
	// コピー禁止
	FontManager(const FontManager&) = delete;
	// コピー代入禁止
	FontManager& operator=( const FontManager&) = delete;
	// ムーブ禁止
	FontManager(FontManager&&) = delete;
	// ムーブ代入禁止
	FontManager& operator=(const FontManager&&) = delete;

private:

	/// <summary>
	/// 管理しているフォントデータ
	/// すでに生成しているものがある
	/// </summary>
	std::vector<FontData> m_fonts;


};


