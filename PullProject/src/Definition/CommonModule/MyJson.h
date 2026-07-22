/*
 * @brief jsonを使いやすくまとめたもの
 * @author Sekino
 */
#pragma once
#ifndef _MYJSON_H_
#define _MYJSON_H_

#include "Data/json.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

class MyJson {
public:

	/*
	 * @brief jsonファイルの読み込み（エンコーディングを自動判定して UTF-8 に変換）
	 */
	static json LoadJsonFile(const std::string& path);

    /*
	 * @brief UTF-8 のバイト列を UTF-16 (std::wstring) に変換
	 */ 
	static std::wstring Utf8ToWString(const std::string& utf8);

	/*
	 * @brief UTF-8 のバイト列を現在の ANSI コードページの std::string に変換
	 */
	static std::string Utf8ToString(const std::string& utf8);

	/*
	 * @brief 指定の階層以下にあるファイルのパスと名前をJsonにまとめる
	 */
	static bool CollectFilesToJson(const fs::path& rootDir, const fs::path& outputJsonPath, std::string jsonFileName);

	/*
	 * @brief jsonDataをバイナリ(MessagePack)に変換し、outputPathへ書き出す
	 */
	static bool CompileBinary(const json& jsonData, const std::string& outputPath);

	/*
	 * @brief inputPathからMessagePackバイナリを読み込み、jsonオブジェクトとして返す
	 */
	static json LoadBinary(const std::string& inputPath);
private:
	/*
	 * @breif バイトバッファを UTF-8 の std::string に変換する
	 */
	static std::string BufferToUtf8String(const std::vector<unsigned char>& buf);
};
#endif