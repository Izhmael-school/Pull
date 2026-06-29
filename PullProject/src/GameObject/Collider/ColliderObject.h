/*
 * @brief 当たり判定だけ持ったオブジェクト
 */
#pragma once

#ifndef _COLLIDEROBJECT_H_
#define _COLLIDEROBJECT_H_

#include "../GameObject.h"
#include <functional>
#include "Dxlib.h"

class Collider;

class ColliderObject : public GameObject{
private:
	std::function<void(Collider* _pOther)> enterEvent;	// 入った時のイベント
	std::function<void(Collider* _pOther)> stayEvent;	// 入ってるときのイベント
	std::function<void(Collider* _pOther)> exitEvent;	// 出た時のイベント

	float lifeTime;	// 生きる時間
	float lifeElapsedTime; // 生きている時間
	bool isDisable;	// 時間経過で消えるか
	bool wantDelete;	// 消してほしいか

public:
	/*
	 * @brief スフィア
	 * @param _lifeTime 何秒で消えるか（0.0fなら消えない）
	 * @param _enterEvent 当たった時のイベント
	 * @param _stayEvent 当たってる時のイベント
	 * @param _exitEvent 出た時のイベント
	 */
	ColliderObject(VECTOR _pos, float _radius, Tag _tag = None,float _lifeTime = 1.0f, std::function<void(Collider* _pOther)> _enterEvent = nullptr,
																std::function<void(Collider* _pOther)> _stayEvent = nullptr,
																std::function<void(Collider* _pOther)> _exitEvent = nullptr);
	/*
	 * @brief ボックス(AABB)
	 * @param _lifeTime 何秒で消えるか（0.0fなら消えない）
	 * @param _enterEvent 当たった時のイベント
	 * @param _stayEvent 当たってる時のイベント
	 * @param _exitEvent 出た時のイベント
	 */
	ColliderObject(VECTOR _pos, VECTOR _min,VECTOR _max, Tag _tag = None, float _lifeTime = 1.0f, std::function<void(Collider* _pOther)> _enterEvent = nullptr,
																								  std::function<void(Collider* _pOther)> _stayEvent = nullptr,
																								  std::function<void(Collider* _pOther)> _exitEvent = nullptr);
	/*
	 * @brief カプセル
	 * @param _lifeTime 何秒で消えるか（0.0fなら消えない）
	 * @param _enterEvent 当たった時のイベント
	 * @param _stayEvent 当たってる時のイベント
	 * @param _exitEvent 出た時のイベント
	 */
	ColliderObject(VECTOR _pos, VECTOR _min, VECTOR _max, float _radius, Tag _tag = None, float _lifeTime = 1.0f, std::function<void(Collider* _pOther)> _enterEvent = nullptr,
																												std::function<void(Collider* _pOther)> _stayEvent = nullptr,
																												std::function<void(Collider* _pOther)> _exitEvent = nullptr);

	~ColliderObject() = default;

private:
	void Start() override;

public:
	void Update() override;

	void Render() override;

	void OnTriggerEnter(Collider* _pSelf, Collider* _pOther) override;
	void OnTriggerStay(Collider* _pSelf, Collider* _pOther) override;
	void OnTriggerExit(Collider* _pSelf, Collider* _pOther) override;

	inline bool WantDelete() const { return wantDelete; }
};

#endif // !_COLLIDEROBJECT_H_
