/*
 *	@file	HookShot.cpp
 *  @author oorui
 */

#include "HookShot.h"
#include "Manager/Stage/GimmickManager.h"
#include "../../../Component\Collider/Collider.h"

 /*
  *	コンストラクタ
  */
HookShot::HookShot(int modelHandle, VECTOR pos, VECTOR rota)
	:GimmickObject(modelHandle, pos, rota) {
}

/*
 *	使用前準備
 */
void HookShot::Setup() {
	GimmickObject::Setup();
	// コライダーを付与:縦長
	pCollider = std::make_unique<AABBCollider>(this, VGet(-50, -90, -50), VGet(50, 90, 50));
}

/*
 *	更新処理
 */
void HookShot::Update() {
	GimmickObject::Update();
	pCollider->Update();
}


/*
 *	描画処理
 */
void HookShot::Render() {
	// モデルハンドルがなければ処理を抜ける
	if (modelHandle <= 0)return;
	
	// 描画
	GimmickObject::Render();
	pCollider->Render();
}

