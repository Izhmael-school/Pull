/*
 *	@file	Lever.cpp
 *  @author oorui
 */

#include "Lever.h"
#include "Manager/Stage/GimmickManager.h"
#include "../../../Component\Collider/Collider.h"

 /*
  *  コンストラクタ
  */
Lever::Lever(int id, int modelHandle, VECTOR pos)
	:GimmickObject(modelHandle, pos)
	, triggerID(id)
	, isActivated(false)
	, onLever(false)
	, opacity(1.0f) {
}

void Lever::Start() {

}

void Lever::Setup() {
	GimmickObject::Setup();
	pCollider = std::make_unique<AABBCollider>(this, VGet(-50, -50, -50), VGet(50, 50, 50));
}


/*
 *  更新
 */
void Lever::Update() {
	// 後に実装
	// プレイヤー側でフラグ変更
	// フラグの変更が確認されたらActivate関数を呼ぶ
	if (onLever) {
		// ギミック起動
		Activate();
	}
	pCollider->Update();

}

/*
 *  描画処理
 */
void Lever::Render() {
	// モデルハンドルなければ処理を抜ける
	if (modelHandle <= 0) return;
	// 壊れていたら描画しない
	if (isActivated) return;

	// モデルの透明度を設定
	MV1SetOpacityRate(modelHandle, opacity);

	// 描画
	GimmickObject::Render();
	pCollider->Render();
}

/*
 *  ギミック起動
 */
void Lever::Activate() {
	// 現在使用可能か
	if (isActivated) return;
	// 使用状態に変更
	isActivated = true;
	// レバーを透明にする
	opacity = 0.0f;
	// 起動したレバーのIDを渡す
	GimmickManager::GetInstance().ActivateLever(triggerID);

}