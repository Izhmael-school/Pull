/*
 *	@file	StageSelectActionMap.h
 *  @author oorui
 */

#ifndef _STAGESELECTACTIONMAP_H_
#define _STAGESELECTACTIONMAP_H_

#include "ActionMapBase.h"

class StageSelectActionMap : public ActionMapBase {
public:
	StageSelectActionMap() = default;
	~StageSelectActionMap() override = default;

public:
	/*
	 *	初期化処理
	 */
	void Start() override;

};

#endif // !_STAGESELECTACTIONMAP_H_
