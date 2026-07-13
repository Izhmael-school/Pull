/*
 *	@file	PullOutFloor.cpp
 *  @author oorui
 */

#include "PullOutFloor.h"
#include "Manager/Stage/GimmickManager.h"
#include "../../../Component\Collider/Collider.h"
#include "Manager/TimeManager.h";
#include "Manager/CollisionManager.h"
#include <ImGui/imgui.h>
#include "Manager/EffectManager.h"
#include "Application.h"


namespace {
	// レバーの生成位置
	constexpr const char* _PULL_DIR = "PullDir";	// 引き出し方向指定のフレームの名前
	constexpr const char* _LEVER_SPAWNPOS_NAME = "LeverPoint";
	constexpr const float _MAX_MOVEFLOOR = 950.0f;	// 床の移動距離
}


/*
 *	コンストラクタ
 */
PullOutFloor::PullOutFloor(int id, int modelHandle, VECTOR pos, VECTOR rota, Tag tag)
	:GimmickObject(modelHandle, pos, rota, tag)
	, triggerID(id)
	, isMoving(false)
	, moveTime(0.0f)
	, reStartPos(pos)
	, isActiv(false) {
}

/*
 *	使用前準備
 */
void PullOutFloor::Setup() {
	GimmickObject::Setup();
	// レバー対応ギミックの末尾に追加
	GimmickManager::GetInstance().RegisterLeverReceiver(triggerID, this);
	// モデルのローカルAABBを取得
	VECTOR minPos;
	VECTOR maxPos;
	VECTOR scale = VGet(24.0f, 8.0f, 8.0f);
	CalculateLocalAABB(minPos, maxPos, scale, this->GetRotation());

	// コライダーを付与
	pCollider = std::make_unique<AABBCollider>(this, minPos, maxPos);
	// レイヤーを設定
	pCollider->SetLayer(ColliderLayer::Stage);

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

void PullOutFloor::Reset() {
	// 変更がなければ行わない
	if (!isActiv) return;


	GimmickObject::Reset();
	// 初期位置に戻す
	this->GetTransform()->SetPosition(reStartPos);
	isMoving = false;

	// コライダーを登録済みか確認して再登録
	CollisionManager::GetInstance().CheckRegister(pCollider.get());
}

/*
 *	レバー起動時の処理
 */
void PullOutFloor::OnTriggered() {
	// 開始位置を記録
	moveStartPos = GetPosition();
	// 進行方向を取得
	VECTOR PullOutPos = GetPullDirPos();
	// 正規化
	VECTOR dir = MyMath::NormDir(moveStartPos, PullOutPos);
	// Y軸は無視
	dir.y = 0;

	// 移動後の座標
	moveEndPos = VAdd(moveStartPos, VScale(dir, -_MAX_MOVEFLOOR));
	// 移動時間を初期化
	moveTime = 0.0f;
	// ギミック起動
	isMoving = true;
}

VECTOR PullOutFloor::GetPullDirPos() {
	// モデルの行列を更新
	MV1SetMatrix(modelHandle, pTransform->GetMatrix());
	// モデル内のPullDirフレーム検索
	int frame = MV1SearchFrame(modelHandle, _PULL_DIR);
	// 座標を返す
	return MV1GetFramePosition(modelHandle, frame);
}

/*
 *	レバー生成位置を取得
 */
bool PullOutFloor::GetLeverSpawnPosition(VECTOR& pos) const {
	// バグ対策
	pos = VGet(0, 0, 0);
	// レバーはモデルにつかないためfalseを返す
	return false;
}

/*
 *	移動処理
 *  @tips	移動方向はレバーが置かれている位置で決める
 */
void PullOutFloor::Moving() {
	// 時間を取得
	moveTime += TimeManager::GetInstance().GetDeltaTime() / 2.0f;
	// 移動時間を最大1秒に固定
	moveTime = std::min(moveTime, 1.0f);

	// 自身の座標を時間で移動
	pTransform->SetPosition(MyMath::EaseQuadInVEC(
		moveStartPos,
		moveEndPos,
		moveTime)
	);

	// 移動が終了していればフラグを切り替える
	if (moveTime >= 1.0f) {
		// エフェクト再生
		EffectManager* effect = &Application::GetInstance().GetEffectManager();
		effect->Play("PullOutWave", GetPullDirPos(), 70.0f);
		isMoving = false;
		isActiv = true;
	}
}