/*
 *	@file	TitleActionMap.h
 *  @author oorui
 */

#ifndef _TITLEACTIONMAP_H_
#define _TITLEACTIONMAP_H_

#include "ActionMapBase.h"

class TitleActionMap : public ActionMapBase {
public:
	TitleActionMap() = default;
	~TitleActionMap() override = default;

public:
	/*
	 *	初期化処理
	 */
	void Start() override;

};

#endif // !_TITLEACTIONMAP_H_
