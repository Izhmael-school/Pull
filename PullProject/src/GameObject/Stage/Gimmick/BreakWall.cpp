/*
 *	@file	BreakWall.cpp
 *  @author oorui
 */
#include "BreakWall.h"
#include "Manager/Stage/GimmickManager.h"
 /*
  *	コンストラクタ
  */
BreakWall::BreakWall(int id, int modelHandle, VECTOR pos)
	:GameObject(modelHandle, pos)
	, triggerID(id)
	, isBroken(false) {
}

/*
 *	初期化処理
 */
void BreakWall::Start() {
	// レバー対応ギミックの末尾に追加
	GimmickManager::GetInstance().RegisterLeverReceiver(triggerID, this);
}

/*
 *	描画処理
 */
void BreakWall::Render() {
	// モデルハンドルがあれば描画処理を行う
	if (modelHandle >= 0) MV1DrawModel(modelHandle);




}

/*
 *	通知を受けた時の処理
 */
void BreakWall::OnTriggered() {
	// エフェクト　※なんかでっかい煙とか

	// 徐々に消える


	// 壊れた判定にする
	isBroken = true;
}
