/*
 * @brief 敵が装備するかぼちゃ
 * @author Sekino
 */
#pragma once
#include "../GameObject.h"
#include "Component/CaughtObject.h"

class Pumpkin : public GameObject ,public CaughtObject{
private:
	VECTOR thrownDir;

public:
	Pumpkin(int _modelHandle = -1, VECTOR _pos = VZero, Transform* _parent = nullptr);
	~Pumpkin() = default;

public:
	void Update();

	void OnTriggerEnter(Collider* _pSelf, Collider* _pOther) override;

	/*
	 * @brief 捕まった
	 */
	void CaughtAction() override;

	/*
	 * @brief 投げられた
	 */
	void ThrownAction(VECTOR _dir) override;

private:
	/*
	 * @brief 捕まった時
	 */
	void CatchStart() override;

	/*
	 * @brief 捕まってる時
	 */
	void Catching() override;

	/*
	 * @brief 投げられた時
	 */
	void ThrowStart() override;

	/*
	 * @brief 投げられてる時
	 */
	void Throwing() override;

	/*
	 * @brief 何かに当たった時
	 */
	void HitObject() override;
};

