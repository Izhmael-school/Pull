/*
 *	@file	Lever.h
 *  @author oorui
 */

#ifndef _LEVER_H_
#define _LEVER_H_


#include "../../GameObject.h"
#include "../Gimmick/GimmickObject.h"

class Lever : public GimmickObject {
private:
	int triggerID;		// レバーのID
	bool isActivated;	// 使用可能状態可否
	bool OnLever;		// ギミック起動

	float opacity;		// α値

public:
	
	/*
	 *	コンストラクタ
	 */
	Lever(int id, int modelHandle, VECTOR pos);

	/*
	 *	更新処理
	 */
	void Update()override;

	
	/*
	 *	描画処理
	 */
	void Render()override;


	/*
	 *	使用状態
	 */
	void Activate();

public:

	/*
	 *	レバーIDの取得
	 *  @return int レバーのID
	 */
	int GetTriggerID() const { return triggerID; }

	/*
	 *	ギミックを起動する
	 */

};

#endif // !_LEVER_H_