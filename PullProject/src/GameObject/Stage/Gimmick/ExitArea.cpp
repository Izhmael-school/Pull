/*
 *	@file	ExitArea.cpp
 *  @author oorui
 */
#include "ExitArea.h"
#include "../../../Component\Collider/Collider.h"
#include "../../Character/Player/PlayerCharacter.h"
#include "../../../Manager/Stage/StageManager.h"
#include "../../../Manager/Stage/GimmickObjectManager.h"

 /*
  *	コンストラクタ
  */
ExitArea::ExitArea(VECTOR pos, VECTOR rot)
	:GimmickObject(-1, pos, rot) {
}

/*
 *	使用前準備
 */
void ExitArea::Setup() {
	GimmickObject::Setup();
	// コライダーを付与
	pCollider = std::make_unique<AABBCollider>(this, VGet(-30, -300, -200), VGet(30, 300, 200));
	pCollider->SetLayer(ColliderLayer::ExitArea);
}

/*
 *	更新処理
 */
void ExitArea::Update() {
	GimmickObject::Update();
	// 当たり判定の更新
	pCollider->Update();
}

/*
 *	描画処理
 */
void ExitArea::Render() {
	// 当たり判定の更新
	pCollider->Render();
}

/*
 *	当たったとき
 */
void ExitArea::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {
	// 当たった相手を取得
	auto other = _pOther->GetGameObject();
	// 当たったのがプレイヤーの場合
	auto player = dynamic_cast<PlayerCharacter*>(other);
	if (player) {
		// プレイヤーの初期位置を取得
		VECTOR pos = StageManager::GetInstance().GetPlayerSpawnPosition();
		// プレイヤーの座標を変更
		player->GetTransform()->SetPosition(pos);
		// 登録されているギミックをリセット
		GimmickObjectManager::GetInstance().Reset();
	}

	// シーンを変更させる※後日実装
	

}




