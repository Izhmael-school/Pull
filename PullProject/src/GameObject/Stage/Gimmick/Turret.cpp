/*
 *	@file	Turret.cpp
 *  @auhtor oorui
 */


#include "Turret.h"
#include "Manager/Stage/GimmickManager.h"
#include "../../../Component\Collider/Collider.h"

/*
 *	コンストラクタ
 */
Turret::Turret(int modelHandle, VECTOR pos, VECTOR rota, Tag tag)
	:GimmickObject(modelHandle, pos, rota, tag) {
}

/*
 *	使用前準備
 */
void Turret::Setup() {
	GimmickObject::Setup();
}

/*
 *	更新処理
 */
void Turret::Update() {
	GimmickObject::Update();

}

/*
 *	描画処理
 */
void Turret::Render() {
	// モデルハンドルがなければ処理を抜ける
	if (!modelHandle)return;

	// 描画
	GimmickObject::Render();
}

