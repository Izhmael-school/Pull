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

    // AABBの八頂点を作成
    VECTOR corners[8] =
    {
        VGet(outMin.x, outMin.y, outMin.z),
        VGet(outMax.x, outMin.y, outMin.z),
        VGet(outMin.x, outMax.y, outMin.z),
        VGet(outMax.x, outMax.y, outMin.z),

        VGet(outMin.x, outMin.y, outMax.z),
        VGet(outMax.x, outMin.y, outMax.z),
        VGet(outMin.x, outMax.y, outMax.z),
        VGet(outMax.x, outMax.y, outMax.z),
    };

    // 回転行列を作成

    MATRIX rotX = MGetRotX(MyMath::Deg2Rad(rotation.x));
    MATRIX rotY = MGetRotY(MyMath::Deg2Rad(rotation.y));
    MATRIX rotZ = MGetRotZ(MyMath::Deg2Rad(rotation.z));

    // Transformと同じ回転順
    MATRIX rotMat = MMult(MMult(rotZ, rotX), rotY);

    // 八頂点を回転させる
    for (int i = 0; i < 8; i++) {
        corners[i] = VTransform(corners[i], rotMat);
    }

    // 回転後のAABBを求める
    outMin = corners[0];
    outMax = corners[0];

    for (int i = 1; i < 8; i++) {
        outMin.x = std::min(outMin.x, corners[i].x);
        outMin.y = std::min(outMin.y, corners[i].y);
        outMin.z = std::min(outMin.z, corners[i].z);

        outMax.x = std::max(outMax.x, corners[i].x);
        outMax.y = std::max(outMax.y, corners[i].y);
        outMax.z = std::max(outMax.z, corners[i].z);
    }
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
