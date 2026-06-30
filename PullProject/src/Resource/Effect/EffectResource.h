/*
 * @brief エフェクトを読み込むリソースクラス
 * @author Sekino
 */

#pragma once
#ifndef _EFFECTRESOURCE_H_
#define _EFFECTRESOURCE_H_

#include "../ResourceBase.h"
class EffectResource : public ResourceBase {
	float magnification;

public:
	EffectResource(const std::string& _name, const std::string& _path,float _magnification = 1.0f);
	~EffectResource() override;

	/*
	 * @brief 読み込み
	 */
	bool Load() override;

};
#endif