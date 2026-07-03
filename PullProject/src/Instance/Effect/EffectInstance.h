/*
 * @brief エフェクトのインスタンスを管理するクラス
 * @author Sekino
 */
#pragma once
#ifndef _EFFECTINSTANCE_H_
#define _EFFECTINSTANCE_H_

#include "../InstanceBase.h"
#include "Definition/Const/VECTORConst.h"
#include "DxLib.h"

class EffectResource;

class EffectInstance : public InstanceBase {
private:
	int playHandle;	// リソースが持っているハンドル
	
public:
	EffectInstance(std::shared_ptr<EffectResource> _effectResource);
	~EffectInstance() = default;

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
	 */
	bool Play(VECTOR _pos,float _scale,VECTOR _rot = VZero);

	/*
	 * @brief 停止
	 */
	void Stop();

	/*
	 * @brief 再生が終わってるか
	 */
	const bool IsEffectEnd() const;
};
#endif // !_EFFECTINSTANCE_H_

// 別名
using EffectPtr = std::shared_ptr<EffectInstance>;