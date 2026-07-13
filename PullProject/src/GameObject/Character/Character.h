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

	struct Ray_Fan {
		float rayLenght = 1000.0f;	// レイの長さ
		float rayAngle = 100.0f;		// レイの角度
		int rayCount = 30;			// レイの数
		float raySpan = 0.1f;		// レイが更新される間隔
		float rayTime = raySpan;	// レイが更新される時間
	};
protected:
	struct Point {
		VECTOR position;
	};

	Point point;

	struct Fan {
		VECTOR position;		// 中心
		float rangeDegree;		// 範囲
		float length;			// 長さ
		float directionDegree;	// 方向
	};
	// 視界
	Ray_Fan vision;

protected:
	bool rayAnswer;	// 視界内にいるか
protected:
	std::unique_ptr<Animator> pAnimator;
	/* @author Riku */
	bool isGravity;									// 重力を掛けるかどうか
	float fallSpeed;								// 落下速度
	std::unique_ptr<Collider> pGroundingCollider;	// 接地判定用コライダー
	bool hitGroundingFrag;
	
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
	
	virtual void OnTriggerEnter(Collider* _pSelf, Collider* _pOther);

	virtual void OnTriggerExit(Collider* _pSelf, Collider* _pOther);

private:
	/*
	 *	重力による落下処理
	 *	@author Riku
	 */
	void GravityFall();
protected:
	/*
	 * @brief VisionFanのデバッグ表示
	 */
	void DrawVisionFanDebug();
public:
	/*
	 *	落下速度に指定した値を加える
	 *  @param	float	addValue	増加値
	 *	@author Riku
	 */
	void AddFallSpeed(float addValue) {
		fallSpeed += addValue;
	}
	/*
	 *	重力の有効無効変更
	 *  @author Riku
	 */
	void SetIsGravity(bool setValue) {
		isGravity = setValue;
	}

	/*
	 * @brief 扇状の視界
	 */
	virtual bool VisionFan(VECTOR target);

public:

	/*
	 * @brief アニメーターの取得
	 */
	inline Animator* GetAnimator()const { return pAnimator.get(); }
};
#endif // !_CHARACTER_H_