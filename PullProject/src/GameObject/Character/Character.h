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


protected:
	/*
	 * @brief VisionFanのデバッグ表示
	 */
	void DrawVisionFanDebug();
public:


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