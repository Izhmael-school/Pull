/*
 * @brief 尻尾を掴んで倒す敵
 * @author Sekino
 */

#pragma once
#ifndef _TAILENEMY_H_
#define _TAILENEMY_H_

#include "../EnemyBase.h"

class ColliderObject;

class TailEnemy : public EnemyBase {
private:
	std::unique_ptr<ColliderObject> pTailCollider;
	int tailFrameIndex;
	int shotFrameIndex;

	float tailRadius;
public:
	TailEnemy(int _modelHandle, VECTOR _pos);
	~TailEnemy() = default;

private:
	void Start() override;

public:
	void Update() override;

	void Render() override;

	void Setup() override;

	void AttackAction();

	void WanderingAction() override;

	void TracingAction() override;

	void ThrownAction(VECTOR _dir) override;

	void Catching() override;

private:

	void Dead() override;

	/*
	 * @brief ミサイルの生成
	 */
	void CreateMissile();

};
#endif // !_TAILENEMY_H_