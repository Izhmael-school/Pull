/*
 *	@file	BomBreakWall.cpp
 *  @author oorui
 */

#include "BomBreakWall.h"
#include "Manager/Stage/GimmickManager.h"
#include "../../../Component\Collider/Collider.h"
#include "Manager/CollisionManager.h"

#include <ImGUI/imgui.h>
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
	// モデルのローカルAABBを取得
	VECTOR minPos;
	VECTOR maxPos;
	VECTOR scale = VGet(15.9f, 7.0f, 8.12f);
	CalculateLocalAABB(minPos, maxPos, scale, this->GetRotation());
	// コライダーを付与
	pCollider = std::make_unique<AABBCollider>(this, minPos,maxPos);
	pCollider->SetLayer(ColliderLayer::BreakWall);
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
	float y = this->GetPosition().y;

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
 *	初期状態に戻す
 */
void BomBreakWall::Reset() {
	if (!isActiv)return;
	GimmickObject::Reset();
	isActiv = false;
	isBroken = false;
	isFading = false;

	opacity = 1.0f;

	// コライダーを登録済みか確認して再登録
	CollisionManager::GetInstance().CheckRegister(pCollider.get());
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

		// 当たり判定削除
		CollisionManager::GetInstance().UnRegister(pCollider.get());

		// 消えた判定にする
		isFading = false;
		isBroken = true;
	}
}