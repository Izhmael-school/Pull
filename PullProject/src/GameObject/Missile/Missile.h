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
#include "Definition/Const/ColorConst.h"

constexpr int EXPLOSION_LEVEL = 3;		// 爆発の段階
constexpr float COLOR_CHANGE_INTERVAL[EXPLOSION_LEVEL] = { 0.5f,0.25f,0.1f };

class EffectManager;
class AudioManager;

class Missile : public GameObject, public CaughtObject {
private:
	float moveSpeed;
	float lifeLimitTime;
	float lifeElapsedTime;

	EffectManager& pEffectManager;
	EffectPtr pEffect;
	AudioManager& pAudioManager;

	GameObject* pOwner;

	float explosionTime;		// 爆発するまでの時間
	float explosionElapsedTime;	// 爆発までの経過時間

	float texChangeTime;		// テクスチャ変更するまでの時間
	float texChangeElapsedTime;	// 変更までの経過時間
	bool isNoTexture;

	int currentExplosionLevel;
	COLOR_F BLINKING_COLOR[EXPLOSION_LEVEL] = { white_f,yellow_f,red_f };	// 爆発表現用の色

public:
	Missile(int _modelHandle = -1, GameObject* _owner = nullptr, EffectManager* _effect = nullptr, VECTOR _dir = VForward, VECTOR _pos = VZero);
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
	void Explosion();

	/*
	 * @brief ブーストエフェクトのフレーム位置を取得
	 */
	VECTOR GetBoostEffectPoint();

	void OnTriggerEnter(Collider* _pSelf, Collider* _pOther) override;

	/*
	 * @brief 捕まった
	 */
	void CaughtAction() override;

	/*
	 * @brief 投げられた
	 */
	void ThrownAction(VECTOR _dir) override;

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

	/*
	 * @brief 点滅
	 */
	void Blinking();

	/*
	 * @brief 移動
	 */
	void Move();
};
#endif // !_MISSILE_H_