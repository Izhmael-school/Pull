/*
 *	@file	Turret.h
 *  @auhtor oorui
 */

#ifndef _TURRET_H_
#define _TURRET_H_

#include "GameObject/GameObject.h"
#include "GameObject/Stage/Gimmick/GimmickObject.h"
#include "../../../Definition/CommonModule/MyMath.h"

class Turret :public GimmickObject {
public:
	/*
	 *	コンストラクタ
	 */
	Turret(int modelHandle, VECTOR pos, VECTOR rota, Tag tag = None);

public:
	/*
	 *	使用前準備
	 */
	void Setup()override;

	/*
	 *	更新処理
	 */
	void Update()override;

	/*
	 *	描画処理
	 */
	void Render()override;
};

#endif // !_TURRET_H_

