#include "MyJson.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <Windows.h>
#include <filesystem>
#include <unordered_set>

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


bool MyJson::CollectFilesToJson(const fs::path& rootDir, const fs::path& outputDir, std::string jsonFileName) {
    if (!fs::exists(rootDir) || !fs::is_directory(rootDir)) {
        std::cerr << "指定されたフォルダが存在しません: " << rootDir << std::endl;
        return false;
    }

    // 拡張子が付いていなければ .json を補完
    if (jsonFileName.size() < 5 ||
        jsonFileName.substr(jsonFileName.size() - 5) != ".json") {
        jsonFileName += ".json";
    }

    fs::path outputPath = outputDir / jsonFileName;

    // ---- 1. 既存のJSONがあれば読み込む ----
    json existingList = json::array();
    if (fs::exists(outputPath)) {
        std::ifstream ifs(outputPath);
        if (ifs) {
            try {
                ifs >> existingList;
                if (!existingList.is_array()) {
                    //std::cerr << "既存のJSONが配列形式ではないため、新規として扱います。" << std::endl;
                    existingList = json::array();
                }
            }
            catch (const json::parse_error& e) {
                //std::cerr << "既存JSONの読み込みに失敗しました: " << e.what() << std::endl;
                //std::cerr << "新規として扱います。" << std::endl;
                existingList = json::array();
            }
        }
    }

    // 既存エントリの"path"だけを集めた集合を作る（重複チェック用）
    std::unordered_set<std::string> existingPaths;
    for (const auto& entry : existingList) {
        if (entry.contains("path") && entry["path"].is_string()) {
            existingPaths.insert(entry["path"].get<std::string>());
        }
    }

    // ---- 2. フォルダを走査して新規ファイルを収集 ----
    std::error_code ec;
    for (auto it = fs::recursive_directory_iterator(
        rootDir, fs::directory_options::skip_permission_denied, ec);
        it != fs::recursive_directory_iterator(); it.increment(ec)) {

        if (ec) { ec.clear(); continue; }
        if (!it->is_regular_file(ec)) continue;

        fs::path relPath = fs::relative(it->path(), rootDir, ec);
        if (ec) {
            relPath = it->path();
            ec.clear();
        }
        std::string relPathStr = relPath.generic_string(); // "/"区切りに統一

        // 既に登録済みのpathなら書き換えずスキップ
        if (existingPaths.count(relPathStr) > 0) {
            continue;
        }

        // 新規ファイルとして追加
        existingList.push_back({
            {"name", it->path().stem().string()},
            {"path", relPathStr}
            });
        existingPaths.insert(relPathStr); // 同一走査内での重複追加防止
    }

    // ---- 3. 書き込み(既存分＋新規分をまとめて上書き保存) ----
    std::ofstream ofs(outputPath);
    if (!ofs) {
        std::cerr << "出力ファイルを開けませんでした: " << outputPath << std::endl;
        return false;
    }
    ofs << existingList.dump(2);
    return true;
}

bool CompileBinary(const json& jsonData, const std::string& outputPath) {
    // JSON -> MessagePackバイナリへ変換
    std::vector<uint8_t> binaryData;
    try {
        binaryData = json::to_msgpack(jsonData);
    }
    catch (const std::exception& e) {
        std::cerr << "バイナリ変換に失敗しました: " << e.what() << std::endl;
        return false;
    }

    // バイナリモードで書き込み(出力先フォルダが無い場合はここで失敗する)
    std::ofstream ofs(outputPath, std::ios::binary);
    if (!ofs) {
        std::cerr << "出力ファイルを開けませんでした: " << outputPath << std::endl;
        return false;
    }

    ofs.write(reinterpret_cast<const char*>(binaryData.data()), binaryData.size());
    if (!ofs) {
        std::cerr << "書き込み中にエラーが発生しました: " << outputPath << std::endl;
        return false;
    }

    return true;
}

json MyJson::LoadBinary(const std::string& inputPath) {
    std::ifstream ifs(inputPath, std::ios::binary);
    if (!ifs) {
        std::cerr << "入力ファイルを開けませんでした: " << inputPath << std::endl;
        return json::array(); // 失敗時は空配列を返す
    }

    std::vector<uint8_t> buffer(
        (std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>()
    );

    try {
        return json::from_msgpack(buffer);
    }
    catch (const json::parse_error& e) {
        std::cerr << "バイナリの解析に失敗しました: " << e.what() << std::endl;
        return json::array(); // 失敗時は空配列を返す
    }
}