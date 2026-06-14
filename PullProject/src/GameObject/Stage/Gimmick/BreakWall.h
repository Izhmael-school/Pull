/*
 *	@file	BreakWall.h
 *  @author oorui
 */

#ifndef _BREALWALL_H_
#define _BREALWALL_H_



#include "GameObject/GameObject.h"
#include "GameObject/Stage/Gimmick/TriggerInterface.h"

 /*
  *	壊れる壁
  *  @param GameObject
  *  @param TriggerInterface		レバー対応ギミックのためインターフェース継承
  */
class BreakWall : public GameObject, public TriggerInterface {
private:
	int triggerID;	// 自身のID
	bool isBroken;	// 壊れたかどうか

public:
	/*
	 *	コンストラクタ
	 */
	BreakWall(int id, int modelHandle, VECTOR pos);

	/*
	 *	初期化処理
	 */
	void Start() override;

	/*
	 *	レバーから通知を受けた時の処理
	 */
	void OnTriggered() override;
public:
	/*
	 *	IDの取得
	 */
	int GetTriggerID() const override { return triggerID; } ;

};

#endif // !_BREALWALL_H_