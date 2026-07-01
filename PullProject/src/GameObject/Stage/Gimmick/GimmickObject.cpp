/*
 *	@file GimmickObject.cpp
 *  @author oorui
 */

#include <cassert>
#include "../../../Definition/CommonModule/MyMath.h"
#include "Component/Collider/Collider.h"

#include "GimmickObject.h"

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
