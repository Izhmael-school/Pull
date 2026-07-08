/*
 *	@file	Turret.cpp
 *  @auhtor oorui
 */


#include "Turret.h"
#include "Manager/Stage/GimmickManager.h"
#include "../../../Component\Collider/Collider.h"

namespace {
	constexpr const char* _FIREPOINT_NAME = "FirePoint";	// 発射位置の名前
}

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
	// コライダー付与
	// コライダーを付与
	pCollider = std::make_unique<AABBCollider>(this, VGet(-200, -100, -200), VGet(200, 300, 200));
	// レイヤーを設定
	pCollider->SetLayer(ColliderLayer::Stage);
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

/*
 *	発射位置の取得
 *  @param[out]	VECTORT 弾の発射位置の取得
 */
VECTOR Turret::GetFirePoint() const {
	// モデルの行列を更新
	MV1SetMatrix(modelHandle, pTransform->GetMatrix());
	// モデル内のFirePointフレーム検索
	int frame = MV1SearchFrame(modelHandle, _FIREPOINT_NAME);

	// 座標を取得
	VECTOR pos = MV1GetFramePosition(modelHandle, frame);

	// 座標を返す
	return pos;
}

