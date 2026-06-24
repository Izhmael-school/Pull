/*
 *	@file	HookShot.h
 *  @auhtor oorui
 */

#ifndef _HOOKSHOT_H_
#define _HOOKSHOT_H_

#include "GameObject/GameObject.h"
#include "GameObject/Stage/Gimmick/GimmickObject.h";
#include "../../../Definition/CommonModule/MyMath.h"

/*
 *	プレイヤーが掴むギミック
 */
class HookShot :public GimmickObject {
public:
	/*
	 *	コンストラクタ
	 */
	HookShot(int modelHandle, VECTOR pos, VECTOR rota);

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

public:

};

#endif // !_HOOKSHOT_H_