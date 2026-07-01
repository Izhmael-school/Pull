/*
 * @brief オーディオのインスタンスを管理するクラス
 * @author Sekino
 */
#pragma once
#ifndef _AUDIOINSTANCE_H_
#define _AUDIOINSTANCE_H_

#include "../InstanceBase.h"
#include "Definition/Const/VECTORConst.h"
#include "Resource/Audio/AudioResource.h"
#include "DxLib.h"

class AudioInstance : public InstanceBase {
private:
	int playHandle;	// リソースが持っているハンドル
	float volume;	// 音量
	float distance;	// 3D音源の距離
	bool isLoop;	// ループ再生するか
	bool is3D;		// 3D音源かどうか

public:
	AudioInstance(AudioResourcePtr _audioResource, float _volume = 255.0f, bool _isLoop = false, float _distance = 10.0f);
	~AudioInstance();

	/*
	 * @brief 更新
	 */
	void Update() override;

	/*
	 * @brief 描画
	 */
	void Render() override;

	/*
	 * @brief 再生
	 * @param _pos 再生する座標(is3D音源の場合のみ使用)
	 */
	bool Play(VECTOR _pos = VZero);

	/*
	 * @brief 停止
	 */
	void Stop();

	/*
	 * @brief 再生が終わってるか
	 */
	const bool IsAudioEnd() const;
};
#endif