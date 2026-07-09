/*
 *	@file	ExitArea.h
 *  @author oorui
 */

#ifndef _EXITAREA_H_
#define _EXITAREA_H_

#include "GimmickObject.h"
#include "GameObject/GameObject.h"

#include  "../../../Definition/CommonModule/MyMath.h"

 /*
  *	出口エリア
  */
class ExitArea :public GimmickObject {

public:
	/*
	 *	コンストラクタ
	 */
	ExitArea(VECTOR pos, VECTOR rot);

public:

	/*
	 *	使用前準備
	 */
	void Setup() override;

	/*
	 *	更新処理
	 */
	void Update() override;

	/*
	 *	描画処理
	 */
	void Render() override;

	/*
	 *	片付け処理
	 */
	void Cleanup()override;

	/*
	 *	当たったとき
	 */
	void OnTriggerEnter(Collider* _pSelf, Collider* _pOther)override;
};

#endif // !_EXITAREA_H_
