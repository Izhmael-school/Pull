/*
 *	@file	BreakWall.cpp
 *  @author oorui
 */
#include "BreakWall.h"
#include "Manager/Stage/GimmickManager.h"
#include "../../../Component\Collider/Collider.h"
#include "Manager/CollisionManager.h"

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

	// モデルのローカルAABBを取得
	VECTOR minPos;
	VECTOR maxPos;
	VECTOR scale = VGet(15.9f, 7.0f, 0.46f);
	CalculateLocalAABB(minPos, maxPos, scale, this->GetRotation());

	// コライダーを付与
	pCollider = std::make_unique<AABBCollider>(this, minPos, maxPos);
	// レイヤーを設定
	pCollider->SetLayer(ColliderLayer::BreakWall);
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
 *	初期状態に戻す
 */
void BreakWall::Reset() {
	// 変更がなければ行わない
	if (!isBroken && !isFading) return;
	
	
	GimmickObject::Reset();

	isBroken = false;
	isFading = false;
	opacity = 1.0f;

	// コライダーを登録済みか確認して再登録
	CollisionManager::GetInstance().CheckRegister(pCollider.get());
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
	MV1SetMatrix(modelHandle, pTransform->GetMatrix());
	// モデル内のLeverPointフレーム検索
	int frame = MV1SearchFrame(modelHandle, _LEVER_SPAWNPOS_NAME);
	// // ローカル座標
	// VECTOR localPos = MV1GetFramePosition(modelHandle, frame);
	// // ワールド座標
	// VECTOR worldPos = VAdd(GetPosition(), localPos);
	// 
	// // 生成位置を返す
	// return worldPos;
	return MV1GetFramePosition(modelHandle, frame);
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
		// 当たり判定削除
		CollisionManager::GetInstance().UnRegister(pCollider.get());
		// 消えた判定にする
		isFading = false;
		isBroken = true;
	}
}