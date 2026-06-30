#include "EffectResourceManager.h"
#include "Resource/Effect/EffectResource.h"
#include "Definition/CommonModule/MyJson.h"
#include <cassert>
#include "Definition/CommonModule/MyString.h"

bool EffectResourceManager::LoadEffect(const std::string& _name, const std::string& _path) {
	// 同名の登録禁止
	if (!resources.empty())
		if (resources.contains(_name)) {
#if _DEBUG
			std::string fail = MyString::MergeString("Effect Loaded:", _name);
			assert(false && fail.c_str());
#endif
			return false;
		}
	// 生成
	auto resource = std::make_shared<EffectResource>(_name, _path);
	// 失敗したら帰る
	if (!resource->Load()) return false;
	// 成功したら配列に
	//resources[_name] = resource;
	resources.emplace(_name.c_str(), resource);
	return true;
}

void EffectResourceManager::LoadEffectFromExternalFile() {
	auto data = MyJson::LoadJsonFile("src/Data/EffectData.json");

	for (auto& d : data) {
		std::string name = d["name"];
		std::string fileName = d["path"];
		std::string path = MyString::MergeString(EFFECT_FILEPATH, fileName);

		LoadEffect(name, path);
	}

}

std::shared_ptr<EffectResource> EffectResourceManager::GetResource(const std::string& _name) const {
	auto itr = resources.find(_name);

	if (itr == resources.end()) {
#if _DEBUG
		std::string fail = MyString::MergeString("Nothing Effect:", _name);
		assert(false && fail.c_str());
#endif 
		return nullptr;
	}

	return itr->second;
}