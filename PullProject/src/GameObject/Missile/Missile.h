/*
 * @brief ミサイルクラス
 * @author Sekino
 */
#pragma once
#ifndef _MISSILE_H_
#define _MISSILE_H_

#include "../GameObject.h"
#include "Component/CaughtObject.h"
#include "Instance/Effect/EffectInstance.h"

class EffectManager;

class Missile : public GameObject, public CaughtObject{
public:
	float moveSpeed;
	float lifeLimitTime;
	float lifeElapsedTime;

	EffectManager& pEffectManager;
	EffectPtr pEffect;

public:
	Missile(int _modelHandle = -1,EffectManager* _effect = nullptr, VECTOR _dir = VForward, VECTOR _pos = VZero);
	~Missile() noexcept override;

private:
	void Start() override;

public:
	/*
	 * @brief 更新
	 */
	void Update() override;

public:
	/*
	 * @brief 爆発
	 */
	void Exprosion();

	/*
	 * @brief ブーストエフェクトのフレーム位置を取得
	 */
	VECTOR GetBoostEffectPoint();

protected:
	/*
	 * @brief 捕まった時
	 */
	virtual void CatchStart();

	/*
	 * @brief 捕まってる時
	 */
	virtual void Catching();

	/*
	 * @brief 投げられた時
	 */
	virtual void ThrowStart();

	/*
	 * @brief 投げられてる時
	 */
	virtual void Throwing();

	/*
	 * @brief 何かに当たった時
	 */
	virtual void HitObject();
};
#endif // !_MISSILE_H_