/*
 * @brief キャラクターの基底クラス
 * @author Sekino
 */

#pragma once

#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "../GameObject.h"
#include "../../Component/Animator.h"
#include <memory>

class Collider;

class Character : public GameObject {
protected:
	std::unique_ptr<Animator> pAnimator;
	/* @author Riku */
	bool isGravity;									// 重力を掛けるかどうか
	float fallSpeed;								// 落下速度
	std::unique_ptr<Collider> pGroundingCollider;	// 接地判定用コライダー
										 
	const float FALL_SPEED_MAX;						// 最大落下速度
	const float GRAVITY_ACCELERATION;				// 重力加速度

public:
	Character(int _modelHandle, VECTOR _pos, Tag _tag = None);
	~Character();

protected:
	virtual void Start() override;

public:
	virtual void Update() override;

	virtual void Render() override;

	virtual void Setup() override;

private:
	/*
	 *	重力による落下処理
	 *	@author Riku
	 */
	void GravityFall();

public:
	/*
	 *	落下速度に指定した値を加える
	 *  @param	float	addValue	増加値
	 *	@author Riku
	 */
	void AddFallSpeed(float addValue) {
		fallSpeed += addValue;
	}

public:

	/*
	 * @brief アニメーターの取得
	 */
	inline Animator* GetAnimator()const { return pAnimator.get(); }
};
#endif // !_CHARACTER_H_