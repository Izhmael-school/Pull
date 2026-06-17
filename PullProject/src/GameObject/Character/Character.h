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

class Character : public GameObject {
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

	/*
	 * @brief アニメーターの取得
	 */
	inline Animator* GetAnimator()const { return pAnimator.get(); }
};
#endif // !_CHARACTER_H_