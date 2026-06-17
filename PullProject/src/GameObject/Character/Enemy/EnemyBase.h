/*
 * @brief 敵の基底クラス
 * @author Sekino
 */
#pragma once

#ifndef _ENEMYBASE_H_
#define _ENEMYBASE_H_

#include "GameObject/Character/Character.h"
#include "Definition/Enum/EnemyState.h"
#include "Definition/Enum/EnemyType.h"
#include <string>

class EnemyBase : public Character{
protected:

	struct Ray_Fan {
		float rayLenght = 1000.0f;	// レイの長さ
		float rayAngle = 100.0f;		// レイの角度
		int rayCount = 30;			// レイの数
		float raySpan = 0.1f;		// レイが更新される間隔
		float rayTime = raySpan;	// レイが更新される時間
	};
private:
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

	bool rayAnswer;

private:
	VECTOR spawnPoint;
	VECTOR wanderingGoalPos;
	EnemyType type;

protected:
	EnemyActionState currentState;
	EnemyActionState nextState;
	float moveSpeed = 1000;
	float standbyTime;	// 立ち止まる時間
	bool isAttacking;	// 攻撃中判定
	bool wantUnuse;		// 未使用化希望判定

public:
	EnemyBase(int _modelHandle, VECTOR _pos);
	~EnemyBase();

protected:
	virtual void Start() override;

public:
	virtual void Update() override;
	virtual void Render() override;
	virtual void Setup() override;
	virtual void Cleanup();

	/*
	 * @brief 敵の種類取得
	 */
	inline EnemyType GetType() const { return type; }

	/*
	 * @brief 未使用化希望判定取得
	 */
	inline bool IsWantUnuse() const { return wantUnuse; }
protected:	// 行動
	/*
	 * @brief 移動
	 */
	void Move(VECTOR targetPos);

	/*
	 * @brief 徘徊
	 */
	void WanderingAction();

	/*
	 * @brief 追跡
	 */
	void TracingAction();

	/*
	 * @brief 攻撃
	 */
	void AttackAction();

private:
	/*
	 * @brief 扇状の視界
	 */
	bool VisionFan(VECTOR target);
	/*
	 * @brief VisionFanのデバッグ表示
	 */
	void DrawVisionFanDebug();

public:
	/*
	 * @brief アニメーションのループ化
	 */
	void LoopAnim(std::string _animName);
};
#endif // !_ENEMYBASE_H_