/*
 * @brief エフェクトのリソースを管理するクラス
 */
#pragma once
#ifndef _EFFECTRESOURCEMANAGER_H_
#define _EFFECTRESOURCEMANAGER_H_

#include "ManagerBase.h"
#include <string>
#include <memory>
#include <unordered_map>

class EffectResource;

const char* const EFFECT_FILEPATH = "res/Model/Effect/";	// エフェクトのファイルパス
const char* const EFFECTDATA_FILEPATH = "src/Data/EffectData.json";	// エフェクトデータのファイルパス
const char* const RELEASE_EFFECTDATA_FILEPATH = "res/ExternalFile/Resource/EffectData.msgpack";	// エフェクトデータのファイルパス

using EffectResourcePtr = std::shared_ptr<EffectResource>;

class EffectResourceManager {
private:
	std::unordered_map<std::string, EffectResourcePtr> resources;

public:
	/*
	 * @brief エフェクトの読み込み
	 */
	bool LoadEffect(const std::string& _name, const std::string& _path);

	/*
	 * @brief 外部ファイルからのエフェクト読み込み
	 */
	void LoadEffectFromExternalFile();

	/*
	 * @brief エフェクトリソースの取得
	 */
	EffectResourcePtr GetResource(const std::string& _name) const;

	/*
	 * @brief 読み込んだリソースの数取得
	 */
	int GetEffectResourceCount() const { return resources.size(); }

	/*
	 * @brief 全リソースの削除
	 */
	void Clear() { resources.clear(); }
};
#endif // !_EFFECTRESOURCEMANAGER_H_