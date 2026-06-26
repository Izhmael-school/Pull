/*
 *	@file	PullOutFloor.cpp
 *  @author oorui
 */

#include "PullOutFloor.h"
#include "Manager/Stage/GimmickManager.h"
#include "../../../Component\Collider/Collider.h"
#include "Manager/TimeManager.h";

namespace {
	// レバーの生成位置
	constexpr const char* _LEVER_SPAWNPOS_NAME = "LeverPoint";	// レバー生成位置の名前
	constexpr const float _MAX_MOVEFLOOR = 50.0f;					// 床の移動距離
}


/*
 *	コンストラクタ
 */
PullOutFloor::PullOutFloor(int id, int modelHandle, VECTOR pos, VECTOR rota)
	:GimmickObject(modelHandle, pos, rota)
	, triggerID(id)
	, isMoving(false)
	, moveTime(0.0f) {
}

/*
 *	使用前準備
 */
void PullOutFloor::Setup() {
	GimmickObject::Setup();
	// コライダーを付与
	pCollider = std::make_unique<AABBCollider>(this, VGet(-100, -50, -100), VGet(100, 50, 100));

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
 *  @tips	移動方向はレバーが置かれている位置で決める
 */
void PullOutFloor::Moving() {
	// 現在の自身の座標を取得
	VECTOR startPosition = GetPosition();
	// 移動方向を設定
	VECTOR leverPos = GetLeverSpawnPosition();
	// 正規化付き向きを求める
	VECTOR moveDir = MyMath::NormDir(startPosition, leverPos);
	moveDir.y = 0.0f;
	// 移動先を設定
	VECTOR endPosition = VAdd(startPosition, VScale(moveDir, _MAX_MOVEFLOOR));

	moveTime += TimeManager::GetInstance().GetDeltaTime() / 2.0f;
	// 1.0で止める
	moveTime = std::min(moveTime, 1.0f);

	// 移動開始
	pTransform->SetPosition(MyMath::EaseQuadInVEC(startPosition, endPosition, moveTime));

	// 移動終了
	isMoving = false;

}