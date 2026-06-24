/*
 *	@file	PullOutFloor.cpp
 *  @author oorui
 */

#include "PullOutFloor.h"
#include "Manager/Stage/GimmickManager.h"
#include "../../../Component\Collider/Collider.h"

namespace {
	// レバーの生成位置
	constexpr const char* _LEVER_SPAWNPOS_NAME = "LeverPoint";
}


 /*
  *	コンストラクタ
  */
PullOutFloor::PullOutFloor(int id, int modelHandle, VECTOR pos, VECTOR rota)
	:GimmickObject(modelHandle, pos, rota)
	, triggerID(id)
	, isMoving(false) {
}

/*
 *	使用前準備
 */
void PullOutFloor::Setup() {
	GimmickObject::Setup();
	// コライダーを付与
	pCollider = std::make_unique<AABBCollider>(this, VGet(-50, -50, -50), VGet(50, 50, 50));
}

/*
 *	更新処理
 */
void PullOutFloor::Update() {
	GimmickObject::Update();
	// ギミックが起動したら
	if (isMoving) {
		// 床を動かす
		Moving();
	}

	pCollider->Update();
}

/*
 *	描画処理
 */
void PullOutFloor::Render() {
	// モデルハンドルがなければ処理を抜ける
	if (modelHandle <= 0)return;

	// 描画
	GimmickObject::Render();
	pCollider->Render();
}

/*
 *	レバー生成位置を取得
 */
VECTOR PullOutFloor::GetLeverSpawnPosition() const {
	// モデル内のLeverPointフレーム検索
	int frame = MV1SearchFrame(modelHandle, _LEVER_SPAWNPOS_NAME);
	// ローカル座標
	VECTOR localPos = MV1GetFramePosition(modelHandle, frame);
	// ワールド座標
	VECTOR worldPos = VAdd(GetPosition(), localPos);

	// 生成位置を返す
	return worldPos;
}

/*
 *	移動処理
 */
void PullOutFloor::Moving() {


}