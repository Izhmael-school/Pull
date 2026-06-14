#pragma once
#include "Data/json.hpp"

class MyJson {
public:
	static nlohmann::json_abi_v3_12_0::json LoadJsonFile(const std::string& path);
	static std::string BufferToUtf8String(const std::vector<unsigned char>& buf);
    // UTF-8 のバイト列を UTF-16 (std::wstring) に変換
	static std::wstring Utf8ToWString(const std::string& utf8);
	// UTF-8 のバイト列を現在の ANSI コードページの std::string に変換
	static std::string Utf8ToString(const std::string& utf8);
};

