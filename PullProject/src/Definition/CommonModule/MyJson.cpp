#include "MyJson.h"
#include "../../Data/json.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <Windows.h>

/// <summary>
/// バイトバッファを UTF-8 の std::string に変換する
/// 対応: UTF-8 (BOM 有/無), UTF-16 LE (BOM), UTF-16 BE (BOM)
/// </summary>
std::string MyJson::BufferToUtf8String(const std::vector<unsigned char>& buf) {
    size_t n = buf.size();
    if (n >= 3 && buf[0] == 0xEF && buf[1] == 0xBB && buf[2] == 0xBF) {
        // UTF-8 with BOM
        return std::string(buf.begin() + 3, buf.end());
    }

    if (n >= 2 && buf[0] == 0xFF && buf[1] == 0xFE) {
        // UTF-16 LE
        const wchar_t* wptr = reinterpret_cast<const wchar_t*>(buf.data() + 2);
        size_t wlen = (n - 2) / 2;
        std::wstring wstr(wptr, wptr + wlen);

        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
        if (size_needed == 0) return std::string();
        std::string out(size_needed, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &out[0], size_needed, nullptr, nullptr);
        return out;
    }

    if (n >= 2 && buf[0] == 0xFE && buf[1] == 0xFF) {
        // UTF-16 BE: 手動でワイド文字へ変換（バイトを入れ替える）
        std::wstring wstr;
        wstr.reserve((n - 2) / 2);
        for (size_t i = 2; i + 1 < n; i += 2) {
            wchar_t wc = static_cast<wchar_t>((buf[i] << 8) | buf[i + 1]);
            wstr.push_back(wc);
        }

        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
        if (size_needed == 0) return std::string();
        std::string out(size_needed, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &out[0], size_needed, nullptr, nullptr);
        return out;
    }

    // それ以外は UTF-8 と仮定
    return std::string(buf.begin(), buf.end());
}

/// <summary>
/// jsonファイルの読み込み（エンコーディングを自動判定して UTF-8 に変換します）
/// </summary>
 nlohmann::json_abi_v3_12_0::json MyJson::LoadJsonFile(const std::string& path) {
     std::ifstream file(path, std::ios::binary);
     if (!file.is_open()) {
         std::cerr << "Failed to open: " << path << std::endl;
         return nullptr;
     }

     std::vector<unsigned char> buf((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

     std::string utf8 = BufferToUtf8String(buf);

     try {
         nlohmann::json data = nlohmann::json::parse(utf8);
         return data;
     }
     catch (const std::exception& e) {
         std::cerr << "Failed to parse json: " << e.what() << " (" << path << ")" << std::endl;
         return nullptr;
     }
 }

std::wstring MyJson::Utf8ToWString(const std::string& utf8) {
    if (utf8.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), (int)utf8.size(), nullptr, 0);
    if (size_needed == 0) return std::wstring();
    std::wstring wstr(size_needed, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8.data(), (int)utf8.size(), &wstr[0], size_needed);
    return wstr;
}

std::string MyJson::Utf8ToString(const std::string& utf8) {
    if (utf8.empty()) return std::string();
    // UTF-8 -> UTF-16
    int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), (int)utf8.size(), nullptr, 0);
    if (wlen == 0) return std::string();
    std::wstring wstr(wlen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8.data(), (int)utf8.size(), &wstr[0], wlen);

    // UTF-16 -> ANSI (current code page)
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    if (len == 0) return std::string();
    std::string out(len, '\0');
    WideCharToMultiByte(CP_ACP, 0, wstr.data(), (int)wstr.size(), &out[0], len, nullptr, nullptr);
    return out;
}
