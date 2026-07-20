/*
 *	@file	Turret.cpp
 *  @auhtor oorui
 */


#include "Turret.h"
#include "Manager/Stage/GimmickManager.h"
#include "../../../Component\Collider/Collider.h"
#include "Manager/GameObjectManager.h"
#include "GameObject/Missile/Missile.h"
#include "Application.h"
#include "Manager/CollisionManager.h"

 /*
  * @author Sekino
  */
#include "Manager/TimeManager.h"

namespace {
	constexpr const char* _FIREPOINT_NAME = "FirePoint";	// 発射位置の名前
}

/*
 *	コンストラクタ
 */
Turret::Turret(int modelHandle, VECTOR pos, VECTOR rota, Tag tag)
	:GimmickObject(modelHandle, pos, rota, tag)
	, fireRate(1.5f)
	, fireElapsedTime(fireRate) {
}

/*
 *	使用前準備
 */
void Turret::Setup() {
	GimmickObject::Setup();
	// コライダー付与
	// コライダーを付与
	pCollider = std::make_unique<RayCollider>(this, VZero, VScale(GetTransform()->GetForward(), -1), 5000, 20, 30, 50);
	// レイヤーを設定
	pCollider->SetLayer(ColliderLayer::Stage);
}

/*
 *	更新処理
 */
void Turret::Update() {
	GimmickObject::Update();

	/*
	 * @author Sekino
	 */
	fireElapsedTime += TimeManager::GetInstance().GetDeltaTime();
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
 *	ギミック初期化処理
 */
void Turret::Reset() {
	GimmickObject::Reset();

	// コライダーを登録済みか確認して再登録
	CollisionManager::GetInstance().CheckRegister(pCollider.get());
}

/*
 *	片付け処理
 */
void Turret::Cleanup() {
	if (pCollider) {
		// CollisionManagerから登録解除
		CollisionManager::GetInstance().UnRegister(pCollider.get());
	}
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

void Turret::OnTriggerStay(Collider* _pSelf, Collider* _pOther) {
	/*
	 * @author Sekino
	 */
	if (_pOther->GetGameObject()->GetTag() == Player) {
		if (fireElapsedTime < fireRate) return;
		fireElapsedTime = 0.0f;
		EffectManager* effect = &Application::GetInstance().GetEffectManager();
		GameObjectManager::GetInstance().CreateGameObject<Missile>("Missile", this, effect, GetTransform()->GetForward(), GetFirePoint());
	}

}

