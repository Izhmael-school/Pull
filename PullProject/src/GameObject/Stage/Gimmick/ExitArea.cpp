/*
 *	@file	ExitArea.cpp
 *  @author oorui
 */
#include "ExitArea.h"
#include "../../../Component\Collider/Collider.h"
#include "../../Character/Player/PlayerCharacter.h"
#include "../../../Manager/Stage/StageManager.h"
#include "../../../Manager/Stage/GimmickObjectManager.h"
#include "Manager/SceneManager.h"

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
	pCollider = std::make_unique<AABBCollider>(this, VGet(-200, -200, -200), VGet(200, 200, 200));
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
#if _DEBUG
	pCollider->Render();
#endif
}

/*
 *	片付け処理
 */
void ExitArea::Cleanup() {
	if (pCollider) {
		// CollisionManagerから登録解除
		CollisionManager::GetInstance().UnRegister(pCollider.get());

	}
}

/*
 *	当たったとき
 */
void ExitArea::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {
	// 当たった相手を取得
	auto other = _pOther->GetLayer();
	// 当たったのがプレイヤーの場合
	
	if (other == ColliderLayer::Player) {
		// ギミックをリセット
		GimmickObjectManager::GetInstance().Reset();
		// クリア判定
		StageManager::GetInstance().RequestStageClear(true);
	}



}




