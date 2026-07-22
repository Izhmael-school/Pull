#include "AudioResourceManager.h"
#include "Definition/CommonModule/MyJson.h"
#include <cassert>
#include "Definition/CommonModule/MyString.h"

bool AudioResourceManager::LoadAudio(const std::string& _name, const std::string& _path, bool _is3D) {
	// 同名の登録禁止
	if (!resources.empty())
		if (resources.contains(_name)) {
#if _DEBUG
			std::string fail = MyString::MergeString("Audio Loaded:", _name);
			assert(false && fail.c_str());
#endif
			return false;
		}
	// 生成
	auto resource = std::make_shared<AudioResource>(_name, _path, _is3D);
	// 失敗したら帰る
	if (!resource->Load()) return false;
	// 成功したら配列に
	resources.emplace(_name.c_str(), resource);
	return true;
}

void AudioResourceManager::LoadAudioFromExternalFile() {
#if _DEBUG
	auto data = MyJson::LoadJsonFile(AUDIODATA_FILEPATH);
#else
	auto data = MyJson::LoadBinary(RELEASE_AUDIODATA_FILEPATH);
#endif
	for (auto& d : data) {
		std::string name = d["name"];
		std::string fileName = d["path"];
		bool is3D = d["is3D"];
		std::string path = MyString::MergeString(AUDIO_FILEPATH, fileName);

		LoadAudio(name, path, is3D);
	}
}

AudioResourcePtr AudioResourceManager::GetResource(const std::string& _name) const {
	auto itr = resources.find(_name);

	if (itr == resources.end()) {
#if _DEBUG
		std::string fail = MyString::MergeString("Nothing Audio:", _name);
		assert(false && fail.c_str());
#endif
		return nullptr;
	}
	return itr->second;
}
