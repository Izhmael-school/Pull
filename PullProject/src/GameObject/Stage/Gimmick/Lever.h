/*
 *	@file	Lever.h
 *  @author oorui
 */

#ifndef _LEVER_H_
#define _LEVER_H_


#include "../../GameObject.h"

class Lever : public GameObject {
private:
	int triggerID;		// レバーのID
	bool isActivated;	// 使用可能状態可否

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
	 *	使用状態
	 */
	void Activate();

public:

	/*
	 *	レバーIDの取得
	 *  @return int レバーのID
	 */
	int GetTriggerID() const { return triggerID; }
};

#endif // !_LEVER_H_