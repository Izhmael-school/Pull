/*
 *	@file	MainGameActionMap.h
 *  @author oorui
 */

#ifndef _MAINGAMEACTIONMAP_H_
#define _MAINGAMEACTIONMAP_H_

#include "ActionMapBase.h"

class MainGameActionMap : public ActionMapBase {
public:
	MainGameActionMap() = default;
	~MainGameActionMap() override = default;

public:
	/*
	 *	初期化処理
	 */
	void Start() override;

};

#endif // !_MAINGAMEACTIONMAP_H_
