/*
 * @file GameObject.cpp
 * @author Sekino
 */

#include "GameObject.h"
#include <cassert>
#include "../Definition/CommonModule/MyMath.h"
#include "Component/Collider/Collider.h"
#include "../Manager/CollisionManager.h"
#include "ImGui/ImGui.h"

GameObject::GameObject(int _modelHandle, VECTOR _pos, Tag _tag)
	:tag(_tag)
	, isActive(true)
	, modelHandle(_modelHandle)
	, isGravity(false)
	, fallSpeed(0.0f)
	, FALL_SPEED_MAX(100.0f)
	, GRAVITY_ACCELERATION(100.0f)
	, hitGroundingFrag(false) {
	pTransform = std::make_unique<Transform>();
	pTransform->SetPosition(_pos);
	Start();
}

GameObject::~GameObject() {
	MV1DeleteModel(modelHandle);


	if (pCollider) {
		CollisionManager::GetInstance().UnRegister(pCollider.get());
		pCollider.release();
	}


}

void GameObject::Start() {
	pTransform->Update();
}

void GameObject::Update() {
	if (!isActive) return;

	pTransform->Update();

	ImGui::Begin("HitGround");
	ImGui::Text("%d : %d : %d", tag, groundCount,hitGroundingFrag);
	ImGui::End();

	/*
	 *	@author Riku
	 */
	 // 重力落下
	GravityFall();

	if (pGroundingCollider)
		pGroundingCollider->Update();

	if (pCollider != nullptr)
		pCollider->Update();

}

void GameObject::Render() {
	if (!isActive) return;

	// モデルが無いなら描画しない
	if (modelHandle != -1) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		MV1SetMatrix(modelHandle, pTransform->GetMatrix());
		MV1DrawModel(modelHandle);
	}

#if _DEBUG
	if (pCollider != nullptr)
		pCollider->Render();
#endif


#if _DEBUG
	/*
	 *	@author Riku
	 */
	if (pGroundingCollider)
		pGroundingCollider->Render();
#endif
}

void GameObject::Setup() {
	groundCount = 0;
}

void GameObject::DeleteModel() {
	MV1DeleteModel(modelHandle);
}


/*
 *	@author Riku
 */
void GameObject::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {
	if (!_pSelf || !_pOther) return;

	if (_pSelf == pGroundingCollider.get() &&
		_pOther->GetLayer() == ColliderLayer::Ground) {
		if (tag == Enemy) {
			printfDx("a");
		}
		groundCount++;
		hitGroundingFrag = true;
	}
}

void GameObject::OnTriggerStay(Collider* _pSelf, Collider* _pOther) {
	//if (_pSelf == pGroundingCollider.get() &&
	//	_pOther->GetLayer() == ColliderLayer::Ground) {
	//	if (tag == Enemy) {
	//		printfDx("a");
	//	}
	//	groundCount++;
	//	hitGroundingFrag = (groundCount > 0);
	//}
}

/*
 *	@author Riku
 */
void GameObject::OnTriggerExit(Collider* _pSelf, Collider* _pOther) {
	if (_pSelf == pGroundingCollider.get() &&
		_pOther->GetLayer() == ColliderLayer::Ground && isGravity) {
		groundCount--;

		if (groundCount <= 0) {
			groundCount = 0;
			hitGroundingFrag = false;
			return;
		}

		hitGroundingFrag = (groundCount > 0);
	}
}

/*
 *	重力による落下処理
 *	@author Riku
 */
void GameObject::GravityFall() {
	// 重力がかかるまで処理しない
	if (!isGravity) {
		fallSpeed = 0.0f;
		return;
	}

	// 接地していた場合は落下しない
	if (hitGroundingFrag) {
		// 落下速度を0にする
		fallSpeed = 0.0f;
		return;
	}


	// 落下速度計算
	fallSpeed += GRAVITY_ACCELERATION * 0.01f;

	if (fallSpeed >= FALL_SPEED_MAX)
		fallSpeed = FALL_SPEED_MAX;

	pTransform->AddPosition(VScale(VUp, -fallSpeed));
}