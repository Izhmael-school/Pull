/*
 * @brief モデルを読み込むリソースクラス
 * @author Sekino
 */
#pragma once

#ifndef _MODELRESOURCE_H_
#define _MODELRESOURCE_H_

#include "../ResourceBase.h"

class ModelResource : public ResourceBase {
public:
	ModelResource(const std::string& _name, const std::string& _path);
	~ModelResource() override;

	/*
	 * @brief 読み込み
	 */
	bool Load() override;

	/*
	 * @brief 複製したモデルハンドルの取得
	 */
	int GetDupricateHandle();
};
#endif // !_MODELRESOURCE_H_