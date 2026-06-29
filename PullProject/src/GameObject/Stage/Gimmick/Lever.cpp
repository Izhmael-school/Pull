/*
 *	@file	Lever.cpp
 *  @author oorui
 */

#include "Lever.h"
#include "Manager/Stage/GimmickManager.h"
#include "../../../Component\Collider/Collider.h"
#include "Manager/CollisionManager.h"

 /*
  *  コンストラクタ
  */
Lever::Lever(int id, int modelHandle, VECTOR pos,VECTOR rota)
	:GimmickObject(modelHandle, pos,rota)
	, triggerID(id)
	, isActivated(false)
	, onLever(false)
	, opacity(1.0f) {
}

void Lever::Start() {

}

void Lever::Setup() {
	GimmickObject::Setup();
	// コライダーを付与
	pCollider = std::make_unique<AABBCollider>(this, VGet(-50, -50, -50), VGet(50, 50, 50));
}


/*
 *  更新
 */
void Lever::Update() {
	GimmickObject::Update();
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

	// 描画
	GimmickObject::Render();
	pCollider->Render();
}

/*
 *	終了処理
 */
void Lever::Execute() {
	GimmickObject::Execute();
	if (isActivated) {
		pCollider->SetEnable(false);
	}
}

/*
 *  ギミック起動
 */
void Lever::Activate() {
	// 現在使用可能か
	if (isActivated) return;
	// 使用状態に変更
	isActivated = true;
	
	// 当たり判定削除
	CollisionManager::GetInstance().UnRegister(pCollider.get());
	
	// 起動したレバーのIDを渡す
	GimmickManager::GetInstance().ActivateLever(triggerID);

}