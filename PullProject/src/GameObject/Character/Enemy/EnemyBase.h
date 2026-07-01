/*
 * @brief 敵の基底クラス
 * @author Sekino
 */
#pragma once

#ifndef _ENEMYBASE_H_
#define _ENEMYBASE_H_

#include "GameObject/Character/Character.h"
#include "Component/CaughtObject.h"
#include "Definition/Enum/EnemyState.h"
#include "Definition/Enum/EnemyType.h"
#include <string>

class EffectManager;

class EnemyBase : public Character, public CaughtObject {
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

private:
	VECTOR spawnPoint;
	VECTOR wanderingGoalPos;
	VECTOR tracingTargetPos;
protected:
	EnemyType type;

protected:
	EnemyActionState currentState;
	EnemyActionState prevState;
	EnemyActionState nextState;
	float moveSpeed = 1000;
	float standbyTime;	// 立ち止まる時間
	float standbyElapsedTime;	// 経過時間
	bool isAttacking;	// 攻撃中判定
	bool canAttack;		// 攻撃できるか
	bool endAttack;		// 攻撃が終了したか
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
	virtual void Move(VECTOR targetPos);

	/*
	 * @brief 徘徊
	 */
	virtual void WanderingAction();

	/*
	 * @brief 追跡
	 */
	virtual void TracingAction();

	/*
	 * @brief 攻撃
	 */
	virtual void AttackAction();

	/*
	 * @brief 待機
	 */
	void Wait();

	/*
	 * @brief 死亡
	 */
	void Dead();
	/*
	 * @brief 攻撃終了時処理
	 */
	void EndAttack();
public:
	/*
	 * @brief 扇状の視界
	 */
	virtual bool VisionFan(VECTOR target);
private:
	/*
	 * @brief VisionFanのデバッグ表示
	 */
	void DrawVisionFanDebug();


protected:
	/*
	 * @brief 行動変更
	 */
	void ChangeNextState(EnemyActionState _state);
public:
	/*
	 * @brief アニメーションのループ化
	 */
	void LoopAnim(std::string _animName);

	virtual void OnTriggerEnter(Collider* _pSelf, Collider* _pOther) override;
	virtual void OnTriggerStay(Collider* _pSelf, Collider* _pOther) override;

public:
	/*
	 * @brief 捕まった
	 */
	virtual void CaughtAction();

	/*
	 * @brief 投げられた
	 */
	virtual void ThrownAction();

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
#endif // !_ENEMYBASE_H_