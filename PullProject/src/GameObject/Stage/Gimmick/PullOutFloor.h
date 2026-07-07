/*
 *	@file	PullOutFloor.h
 *  @author oorui
 */

#ifndef _PULLOUTFLOOR_H_
#define _PULLOUTFLOOR_H_

#include "GimmickObject.h"
#include "../../GameObject.h"
#include "GameObject/Stage/Gimmick/TriggerInterface.h"
#include "../../../Definition/CommonModule/MyMath.h"

/*
 *	レバーで引き出される床
 *  @param	GameObject
 *  @param	TriggerInterface	レバー対応ギミックの為インターフェース継承
 */
class PullOutFloor :public GimmickObject, public TriggerInterface {
private:
	int triggerID;		// 自身のID

	bool isMoving;		// 動いているかどうか
	bool isActiv;		// 起動したかどうか

	float moveTime;		// 移動時間
	VECTOR reStartPos;	// 初期位置

	VECTOR moveStartPos;	// 初期位置
	VECTOR moveEndPos;		// 終了位置

public:
	/*
	 *	コンストラクタ
	 */
	PullOutFloor(int id, int modelHandle, VECTOR pos, VECTOR rota);

public:
	/*
	 *	床を動かす
	 */
	void Moving();

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

	void Reset() override;
	/*
	 *	レバーから通知を受けた時の処理
	 */
	void OnTriggered() override;
public:
	/*
	 *	IDの取得
	 */
	int GetTriggerID() const override { return triggerID; }
	/*
	 *	進行方向の取得
	 */
	VECTOR GetPullDirPos();

	/*
	 *	レバー配置位置取得
	 */
	bool GetLeverSpawnPosition(VECTOR& pos) const override;
};

#endif // !_PULLOUTFLOOR_H_
