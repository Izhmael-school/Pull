#include "FontManager.h"

#include <string>
#include <vector>

#include <DxLib.h>

FontManager& FontManager::GetInstance()
{

	static FontManager instance;
	return instance;
}

void FontManager::Init() {

}

void FontManager::End() {

	// 生成したフォントを解放する
	for ( auto& font : m_fonts ) {

		if ( font.handle != -1) {

			DeleteFontToHandle(font.handle);
		}
	}

	m_fonts.clear();
}

int FontManager::GetFontHandle(const std::string& fontName, int size, int thickness)
{

	// 同じ設定のフォントがあるかチェック
	for ( const auto& font : m_fonts ) {

		bool isSameFontName = ( font.fontName == fontName );
		bool isSameSize = ( font.size == size);
		bool isSameThickness = ( font.thickness == thickness );

		// 完全に一致していたら生成済みのハンドルを返す
		if ( isSameFontName && isSameSize && isSameThickness ) {

			return font.handle;
		}
	}

	// ここまで来たら作成していないのでハンドルを作成する
	int handle = CreateFontToHandle( fontName.c_str(), size, thickness);

	// FontDataを作成する

	FontData data;

	data.fontName = fontName;
	data.size = size;
	data.thickness = thickness;
	data.handle = handle;

	// 作成したら管理しているフォントリストに登録
	m_fonts.push_back(data);

	return data.handle;
}
