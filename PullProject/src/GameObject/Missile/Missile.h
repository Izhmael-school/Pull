/*
 * @brief ミサイルクラス
 * @author Sekino
 */
#pragma once
#ifndef _MISSILE_H_
#define _MISSILE_H_

#include "../GameObject.h"
#include "Component/CaughtObject.h"

class Missile : public GameObject, public CaughtObject{
public:
	float moveSpeed;
	float lifeLimitTime;
	float lifeElapsedTime;

public:
	Missile(int _modelHandle = -1, VECTOR _pos = VZero);
	~Missile() noexcept override;

private:
	void Start() override;

public:
	/*
	 * @brief 更新
	 */
	void Update() override;

public:
	void Exprosion();

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