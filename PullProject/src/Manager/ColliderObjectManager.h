/*
 * @brief 当たり判定だけ持ったオブジェクトを管理・生成するクラス
 */
#pragma once

#ifndef _COLLIDEROBJECTMANAGER_H_
#define _COLLIDEROBJECTMANAGER_H_

#include "ManagerBase.h"
#include "Component/Singleton.h"
#include <memory>
#include <vector>
#include <functional>
#include "Dxlib.h"
#include "Definition/Enum/TagEnum.h"

class ColliderObject;
class Collider;

class ColliderObjectManager : public ManagerBase ,public Singleton<ColliderObjectManager>{
private:
	std::vector<std::unique_ptr<ColliderObject>> aliveColliderObject;

public:
	ColliderObjectManager();
	~ColliderObjectManager();

public:
	void Update() override;

	void Render() override;

public:
	/*
	 * @brief スフィア
	 * @param _lifeTime 何秒で消えるか（0.0fなら消えない）
	 * @param _enterEvent 当たった時のイベント
	 * @param _stayEvent 当たってる時のイベント
	 * @param _exitEvent 出た時のイベント
	 */
	void CreateSphere(VECTOR _pos, float _radius, Tag _tag = None, float _lifeTime = 1.0f, std::function<void(Collider* _pOther)> _enterEvent = nullptr,
		std::function<void(Collider* _pOther)> _stayEvent = nullptr,
		std::function<void(Collider* _pOther)> _exitEvent = nullptr);
	/*
	 * @brief ボックス(AABB)
	 * @param _lifeTime 何秒で消えるか（0.0fなら消えない）
	 * @param _enterEvent 当たった時のイベント
	 * @param _stayEvent 当たってる時のイベント
	 * @param _exitEvent 出た時のイベント
	 */
	void CreateAABB(VECTOR _pos, VECTOR _min, VECTOR _max, Tag _tag = None, float _lifeTime = 1.0f, std::function<void(Collider* _pOther)> _enterEvent = nullptr,
		std::function<void(Collider* _pOther)> _stayEvent = nullptr,
		std::function<void(Collider* _pOther)> _exitEvent = nullptr);
	/*
	 * @brief カプセル
	 * @param _lifeTime 何秒で消えるか（0.0fなら消えない）
	 * @param _enterEvent 当たった時のイベント
	 * @param _stayEvent 当たってる時のイベント
	 * @param _exitEvent 出た時のイベント
	 */
	void CreateCapsule(VECTOR _pos, VECTOR _min, VECTOR _max, float _radius, Tag _tag = None, float _lifeTime = 1.0f, std::function<void(Collider* _pOther)> _enterEvent = nullptr,
		std::function<void(Collider* _pOther)> _stayEvent = nullptr,
		std::function<void(Collider* _pOther)> _exitEvent = nullptr);
};
#endif