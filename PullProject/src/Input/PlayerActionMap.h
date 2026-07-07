/*
 *	@file	PlayerActionMap.h
 *	@author Riku
 */

#ifndef _PLAYERACTIONMAP_H_
#define _PLAYERACTIONMAP_H_

#include "ActionMapBase.h"

/*
 *	プレイヤー操作のアクションマップ
 */
class PlayerActionMap : public ActionMapBase {
public:
	PlayerActionMap() = default;
	~PlayerActionMap() override = default;

public:
	/*
	 *	初期化処理
	 */
	void Start() override;
};

#endif // !_PLAYERACTIONMAP_H_
