/*
 * @brief 鎧を着ている敵
 * @author Sekino
 */
#pragma once
#ifndef _ARMORENEMY_H_
#define _ARMORENEMY_H_

#include "../WalkEnemy.h"
class ArmorEnemy : public EnemyBase {
public:
	ArmorEnemy(int _modelHandle, VECTOR _pos);
	~ArmorEnemy();

public:
	void Setup() override;

	void Cleanup() override;

	virtual void OnTriggerEnter(Collider* _pSelf, Collider* _pOther) override;
};
#endif
