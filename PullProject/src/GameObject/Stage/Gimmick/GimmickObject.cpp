/*
 *	@file GimmickObject.cpp
 *  @author oorui
 */

#include <cassert>
#include "../../../Definition/CommonModule/MyMath.h"
#include "Component/Collider/Collider.h"

#include "GimmickObject.h"
#include <algorithm>
#include <cfloat>
 /*
  *	コンストラクタ
  */
GimmickObject::GimmickObject(int _modelHandle, VECTOR _pos, VECTOR _rota, Tag _tag)
	: GameObject(_modelHandle, _pos, _tag) {
	pTransform->SetRotation(_rota);
	Start();
}

/*
 *	デストラクタ
 */
GimmickObject::~GimmickObject() {
	MV1DeleteModel(modelHandle);
}

/*
 *	開始処理
 */
void GimmickObject::Start() {
}

/*
 *	モデルのローカル座標からAABBを求める
 *  @param[in]	VECTOR& 最小値
 *  @param[in]	VECTOR& 最大値
 */
void GimmickObject::CalculateLocalAABB(VECTOR& outMin, VECTOR& outMax, VECTOR scale, VECTOR rotation) const {
    
    // メッシュの最大、最小点を探す
    outMin = MV1GetMeshMinPosition(modelHandle, 0);
    outMax = MV1GetMeshMaxPosition(modelHandle, 0);

    int meshNum = MV1GetMeshNum(modelHandle);

    for (int i = 1; i < meshNum; i++) {
        VECTOR minPos = MV1GetMeshMinPosition(modelHandle, i);
        VECTOR maxPos = MV1GetMeshMaxPosition(modelHandle, i);

        outMin.x = std::min(outMin.x, minPos.x);
        outMin.y = std::min(outMin.y, minPos.y);
        outMin.z = std::min(outMin.z, minPos.z);

        outMax.x = std::max(outMax.x, maxPos.x);
        outMax.y = std::max(outMax.y, maxPos.y);
        outMax.z = std::max(outMax.z, maxPos.z);
    }

    // スケールを反映させる
    outMin.x *= scale.x;
    outMin.y *= scale.y;
    outMin.z *= scale.z;

    outMax.x *= scale.x;
    outMax.y *= scale.y;
    outMax.z *= scale.z;

    // 回転を適用させる
    AABBCollider::RotateBounds(outMin, outMax, rotation);
}

/*
 *	更新処理
 */
void GimmickObject::Update() {
	if (!isActive) return;

	pTransform->Update();
	if (pCollider != nullptr)
		pCollider->Update();
}

/*
 *	描画処理
 */
void GimmickObject::Render() {
	if (!isActive) return;

	// モデルがないなら描画しない
	if (modelHandle == -1) return;
	// --- 通常モデル描画 ---
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	MV1SetMatrix(modelHandle, pTransform->GetMatrix());
	MV1DrawModel(modelHandle);

#if _DEBUG
	if (pCollider != nullptr)
		pCollider->Render();
#endif
}

/*
 *	準備処理
 */
void GimmickObject::Setup() {
	pTransform->Update();
}

/*
 *	終了処理
 */
void GimmickObject::Execute() {
	DeleteModel();
}

/*
 *	モデルを削除
 */
void GimmickObject::DeleteModel() {
	// モデルがないなら行わない
	if (modelHandle == -1) return;
	MV1DeleteModel(modelHandle);
}

void GimmickObject::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {
}

void GimmickObject::OnTriggerStay(Collider* _pSelf, Collider* _pOther) {
}

void GimmickObject::OnTriggerExit(Collider* _pSelf, Collider* _pOther) {
}
