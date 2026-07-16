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
#include <functional>

class EffectManager;

using EffectEvent = std::function<void(const std::string& _effectName, VECTOR _pos, float _scale, VECTOR rot)>;
using AudioEvent = std::function<void(const std::string& _audioName, float _volume, bool _isLoop, VECTOR _pos, float distance)>;
using MissileEvent = std::function<void(std::string _modelName,GameObject* _pOwner, VECTOR _dir, VECTOR _pos)>;
using SphereEvent = std::function<void(VECTOR _pos, float _radius,Tag _tag,float _lifeTime)>;
using AABBEvent = std::function<void(VECTOR _pos, VECTOR _min, VECTOR _max, Tag _tag, float _lifeTime)>;

class EnemyBase : public Character, public CaughtObject {
protected:
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

	VECTOR thrownDir;	// 投げられた方向

	float footPos;		// 地面に埋まらないようにする足元の座標

	bool isWallHit;		// 壁に当たったか

	int addScore;
protected:
	EffectEvent effectEvent;
	AudioEvent audioEvent;
	MissileEvent createEvent;
	SphereEvent sphereEvent;
	AABBEvent aabbEvent;
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

	void SetFootPos(float _footPos) { footPos = _footPos; }
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
	virtual void Dead();

	/*
	 * @brief 攻撃終了時処理
	 */
	void EndAttack();

public:
	/*
	 * @brief 視界
	 */
	bool VisionFan(VECTOR target) override;

private:
	void Death();

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

	/*
	 * @brief イベントのセット
	 */
	void SetEvent(EffectEvent _effectEvent, AudioEvent _audioEvent, MissileEvent _createEvent, SphereEvent _sphereEvent, AABBEvent _aabbEvent);

protected:
	/*
	 * @brief アニメーションイベントのセット
	 * @param _frameCount 0未満ならアニメーションの最後のフレームにセット
	 */
	void SetAnimEvent(std::string _animName, int _frameCount, std::function<void()> _animEvent);

public:
	virtual void OnTriggerEnter(Collider* _pSelf, Collider* _pOther) override;
	virtual void OnTriggerStay(Collider* _pSelf, Collider* _pOther) override;
	virtual void OnTriggerExit(Collider* _pSelf, Collider* _pOther) override;

public:
	/*
	 * @brief 捕まった
	 */
	virtual void CaughtAction(VECTOR _rot,VECTOR _pos);

	/*
	 * @brief 捕まった
	 */
	virtual void CaughtAction();

	/*
	 * @brief 投げられた
	 */
	virtual void ThrownAction(VECTOR _dir) override;

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