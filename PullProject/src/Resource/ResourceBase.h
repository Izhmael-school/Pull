/*
 * @brief DXライブラリを使って読み込むリソース基底クラス
 * @brief 1ファイルにつき1インスタンス
 * @author Sekino
 */

#pragma once
#ifndef _RESOURCEBASE_H_
#define _RESOURCEBASE_H_

#include <string>

class ResourceBase {
protected:
	// 名前
	std::string name;
	// ファイルパス
	std::string path;
	// 読み込んだハンドル
	int loadHandle;

public:
	ResourceBase(const std::string& _name, const std::string& _path);
	virtual ~ResourceBase() = default;

	/*
	 * @brief 読み込み
	 */
	virtual bool Load() = 0;

	/*
	 * @brief 読み込めたか
	 */
	bool IsLoaded() const { return loadHandle != -1; }

	/*
	 * @brief ハンドルの取得
	 */
	int GetHandle() const { return loadHandle; }

	const std::string GetName() const { return name; }
	const std::string GetPath() const { return path; }

public:
	// コピー禁止
	ResourceBase(const ResourceBase&) = delete;
	ResourceBase& operator = (const ResourceBase&) = delete;
	// 移動禁止
	ResourceBase(ResourceBase&&) = delete;
	ResourceBase& operator = (ResourceBase&&) = delete;

};
#endif // !_RESOURCEBASE_H_