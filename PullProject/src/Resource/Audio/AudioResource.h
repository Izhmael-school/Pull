/*
 * @brief オーディオを読み込むリソースクラス
 * @author Sekino
 */

#pragma once
#ifndef _AUDIORESOURCE_H_
#define _AUDIORESOURCE_H_

#include "../ResourceBase.h"

class AudioResource : public ResourceBase {
	bool is3D;

public:
	AudioResource(const std::string& _name, const std::string& _path, bool _is3D = false);
	~AudioResource() override;

	/*
	 * @brief 読み込み
	 */
	bool Load() override;

	/*
	 * @brief 3D音源かどうか
	 */
	const bool Is3D() const { return is3D; }

};
#endif

#include <memory>
using AudioResourcePtr = std::shared_ptr<AudioResource>;