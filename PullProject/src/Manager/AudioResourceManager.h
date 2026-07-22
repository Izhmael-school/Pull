/*
 * @brief オーディオのリソースを管理するクラス
 * @author Sekino
 */
#pragma once
#ifndef _AUDIORESOURCEMANAGER_H_
#define _AUDIORESOURCEMANAGER_H_

#include <string>
#include <memory>
#include <unordered_map>
#include "Resource/Audio/AudioResource.h"


const char* const AUDIO_FILEPATH = "res/Audio/";	// オーディオのファイルパス
const char* const AUDIODATA_FILEPATH = "src/Data/AudioData.json";	// オーディオデータのファイルパス
const char* const RELEASE_AUDIODATA_FILEPATH = "res/ExternalFile/Resource/AudioData.msgpack";	// オーディオデータのファイルパス

class AudioResourceManager {
private:
	std::unordered_map<std::string, AudioResourcePtr> resources;	// リソースの名前とハンドルのマップ

public:
	/*
	 * @brief オーディオの読み込み
	 */
	bool LoadAudio(const std::string& _name, const std::string& _path, bool _is3D = false);

	/*
	 * @brief 外部ファイルからのオーディオ読み込み
	 */
	void LoadAudioFromExternalFile();

	/*
	 * @brief オーディオリソースの取得
	 */
	AudioResourcePtr GetResource(const std::string& _name) const;

	/*
	 * @brief 読み込んだリソースの数取得
	 */
	int GetAudioResourceCount() const { return resources.size(); }

	/*
	 * @brief 全リソースの削除
	 */
	void Clear() { resources.clear(); }
};
#endif