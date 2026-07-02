/*
 * @brief オーディオを管理するクラス
 * @author Sekino
 */
#pragma once

#ifndef _AUDIOMANAGER_H_
#define _AUDIOMANAGER_H_

#include "ManagerBase.h"
#include <memory>
#include <string>
#include <vector>
#include "DxLib.h"
#include "Definition/Const/VECTORConst.h"

class AudioInstance;
class AudioResourceManager;

using AudioPtr = std::shared_ptr<AudioInstance>;

class AudioManager : public ManagerBase {
private:
	std::vector<AudioPtr> instances;	// 管理下にあるインスタンス
public:
	AudioResourceManager& pAudioResourceManager;// 読み込み用

public:
	AudioManager(AudioResourceManager& _resourceManager);

	/*
	 * @brief 生成
	 */
	AudioPtr Play(const std::string& _audioName, float _volume = 255.0f, bool _isLoop = false, const VECTOR& _pos = VZero, float _distance = 10.0f);

	/*
	 * @brief 更新
	 */
	void Update() override;

	/*
	 * @brief 描画
	 */
	void Render() override;

	/*
	 * @brief インスタンスの全削除
	 */
	void Clean();

	/*
	 * @brief 管理してるインスタンスの数
	 */
	int GetInstanceCount() const { return instances.size(); }

};
#endif // !_AUDIOMANAGER_H_