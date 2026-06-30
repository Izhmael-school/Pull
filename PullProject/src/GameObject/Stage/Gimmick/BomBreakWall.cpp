/*
 *	@file	BomBreakWall.cpp
 *  @author oorui
 */

#include "BomBreakWall.h"
#include "Manager/Stage/GimmickManager.h"
#include "../../../Component\Collider/Collider.h"

 /*
  *	コンストラクタ
  */
BomBreakWall::BomBreakWall(int modelHandle, VECTOR pos, VECTOR rota)
	:GimmickObject(modelHandle, pos, rota)
	, isBroken(false)
	, isFading(false)
	, opacity(1.0f)
	, fadeSpeed(0.01f) {

}

/*
 *	使用前準備
 */
void BomBreakWall::Setup() {
	GimmickObject::Setup();
	// コライダーを付与
	pCollider = std::make_unique<AABBCollider>(this, VGet(-300, -300, -200), VGet(300, 300, 200));
}

/*
 *	更新処理
 */
void BomBreakWall::Update() {
	GimmickObject::Update();
	// ギミックが起動したら
	if (isActiv) {
		// 徐々に透明にしていく
		OpacityChange();
	}
	pCollider->Update();
}

/*
 *	描画
 */
void BomBreakWall::Render() {
	// モデルハンドルがなければ処理を抜ける
	if (modelHandle <= 0)return;
	// 壊れていたら描画しない
	if (isBroken)return;

	// モデルの透明度を設定
	MV1SetOpacityRate(modelHandle, opacity);

	// 描画
	GimmickObject::Render();
	pCollider->Render();
}

/*
 *	オブジェクトの不透明度を変更する
 */
void BomBreakWall::OpacityChange() {
	// α値を徐々に減らしていく
	opacity -= fadeSpeed;

	// 0以下対策
	if (opacity <= 0.0f) {
		opacity = 0.0f;

		// 消えた判定にする
		isFading = false;
		isBroken = true;
	}
}