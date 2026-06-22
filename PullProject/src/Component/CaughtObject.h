/*
 * @brief プレイヤーがつかめるオブジェクトの基底クラス
 * @author Sekino
 */

#pragma once
#ifndef _CAUGHTOBJECT_H_
#define _CAUGHTOBJECT_H_

#include "../Definition/Enum/CaughtState.h"

class CaughtObject{
public:
	CaughtState currentState = NoneCaughtState;	// 今の状態

public:
	inline CaughtState GetCurrentCaughtState() const { return currentState; }

protected:
	/*
	 * @brief 状態に応じて関数を呼び出す
	 */
	void CaughtUpdate();

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

	/*
	 * @brief 状態変化
	 */
	void ChangeCaughtState(CaughtState _state);
};

#endif