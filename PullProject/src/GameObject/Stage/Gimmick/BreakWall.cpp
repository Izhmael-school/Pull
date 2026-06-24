/*
 *	@file	BreakWall.cpp
 *  @author oorui
 */
#include "BreakWall.h"
#include "Manager/Stage/GimmickManager.h"
#include "../../../Component\Collider/Collider.h"

namespace {
	// レバーの生成位置
	constexpr const char* _LEVER_SPAWNPOS_NAME = "LeverPoint";
}

/*
 *	コンストラクタ
 */
BreakWall::BreakWall(int id, int modelHandle, VECTOR pos, VECTOR rota)
	:GimmickObject(modelHandle, pos, rota)
	, triggerID(id)
	, isBroken(false)
	, isFading(false)
	, opacity(1.0f)
	, fadeSpeed(0.01f) {
}

/*
 *	使用前準備
 */
void BreakWall::Setup() {
	GimmickObject::Setup();
	// レバー対応ギミックの末尾に追加
	GimmickManager::GetInstance().RegisterLeverReceiver(triggerID, this);
	// コライダーを付与
	pCollider = std::make_unique<AABBCollider>(this, VGet(-300, -300, -30), VGet(300, 300, 30));

}

/*
 *	更新処理
 */
void BreakWall::Update() {
	GimmickObject::Update();
	// ギミックが起動したら
	if (isFading) {
		// 徐々に透明にしていく
		OpacityChange();
	};
	pCollider->Update();
}

/*
 *	描画処理
 */
void BreakWall::Render() {
	// モデルハンドルなければ処理を抜ける
	if (modelHandle <= 0) return;
	// 壊れていたら描画しない
	if (isBroken) return;


	// モデルの透明度を設定
	MV1SetOpacityRate(modelHandle, opacity);

	// 描画
	GimmickObject::Render();
	pCollider->Render();
}

/*
 *	通知を受けた時の処理
 */
void BreakWall::OnTriggered() {
	// フェード中、フェード修了していたら抜ける
	if (isFading || isBroken)return;

	// エフェクト　※なんかでっかい煙とか

	// フェード処理を開始判定にする
	isFading = true;
}

/*
 *	レバーの生成位置を取得
 */
VECTOR BreakWall::GetLeverSpawnPosition() const {
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
 *	オブジェクトの不透明度を変更する
 */
void BreakWall::OpacityChange() {
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