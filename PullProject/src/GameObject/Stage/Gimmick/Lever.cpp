/*
 *	@file	Lever.cpp
 *  @author oorui
 */

#include "Lever.h"
#include "Manager/Stage/GimmickManager.h"


/*
 *  コンストラクタ
 */
Lever::Lever(int id, int modelHandle, VECTOR pos)
	:GameObject(modelHandle, pos)
	,triggerID(id)
	,isActivated(false) {
}


/*
 *  更新
 */
void Lever::Update() {
    // 後に実装
    // プレイヤー側でフラグ変更
    // フラグの変更が確認されたらActivate関数を呼ぶ

    
}

/*
 *  ギミック起動
 */
void Lever::Activate() {
    // 現在使用可能か
    if (isActivated) return;
    // 使用状態に変更
    isActivated = true;
    // 起動したレバーのIDを渡す
    GimmickManager::GetInstance().ActivateLever(triggerID);
}