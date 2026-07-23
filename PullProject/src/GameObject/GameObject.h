/*
 * @file GameObject.h
 * @author Sekino
 */
#pragma once

#ifndef _GAMEOBJECT_
#define _GAMEOBJECT_

#include "Component/Transform.h"
#include "Definition/Enum/TagEnum.h"
#include "Definition/Const/VECTORConst.h"
#include <memory>

class Collider;

class GameObject {
protected:
	std::unique_ptr<Transform> pTransform;
	std::unique_ptr<Collider> pCollider;
	Tag tag;
	bool isActive;
	int modelHandle;

	/* @author Riku */
	bool isGravity;									// 重力を掛けるかどうか
	float fallSpeed;								// 落下速度
	std::unique_ptr<Collider> pGroundingCollider;	// 接地判定用コライダー
	bool hitGroundingFrag;

	const float FALL_SPEED_MAX;						// 最大落下速度
	const float GRAVITY_ACCELERATION;				// 重力加速度

	int groundCount = 0;
public:
	GameObject(int _modelHandle = -1 ,VECTOR _pos = VZero,Tag _tag = None);
	virtual ~GameObject();

protected:
	virtual void Start();

public:
	virtual void Update();
	virtual void Render();
	virtual void Setup();

	/*
	 * @brief モデルの削除
	 */
	virtual void DeleteModel();

	/*
	 * @brief トランスフォームの取得
	 */
	inline Transform* GetTransform() const { return pTransform.get(); }

	/*
	 * @brief コライダーの取得
	 */
	inline Collider* GetCollider() const { return pCollider.get(); }

	/*
	 * @brief 接地判定コライダーの取得
	 */
	inline Collider* GetGroundingCollider() const { return pGroundingCollider.get(); }

	/*
	 * @brief 座標の取得
	 */
	inline VECTOR GetPosition() const { return pTransform->GetPosition(); }

	/*
	 * @brief 回転の取得
	 */
	inline VECTOR GetRotation() const { return pTransform->GetLocalRotation(); }

	/*
	 * @brief 拡縮の取得
	 */
	inline VECTOR GetScale() const { return pTransform->GetScale(); }

	/*
	 * @brief 有効か
	 */
	inline bool IsActive() const { return isActive; }

	/*
	 * @brief 有効フラグ
	 */
	inline void SetActive(bool _isActive) { isActive = _isActive; }

	/*
	 * @brief モデルハンドルのセット
	 */
	inline void SetModelHandle(int _mHandle) { modelHandle = _mHandle; }

	/*
	 * @brief モデルハンドルの取得
	 */
	inline int GetModelHandle() const { return modelHandle; }

	/*
	 * @brief タグの取得
	 */
	inline Tag GetTag() const { return tag; }

	/*
	 * @brief 当たった時
	 */
	virtual void OnTriggerEnter(Collider* _pSelf, Collider* _pOther);

	/*
	 * @brief 当たってるとき
	 */
	virtual void OnTriggerStay(Collider* _pSelf, Collider* _pOther);

	/*
	 * @brief 離れた時
	 */
	virtual void OnTriggerExit(Collider* _pSelf, Collider* _pOther);

private:
	/*
	 *	重力による落下処理
	 *	@author Riku
	 */
	void GravityFall();
	public:
	/*
 *	落下速度に指定した値を加える
 *  @param	float	addValue	増加値
 *	@author Riku
 */
	void AddFallSpeed(float addValue) {
		fallSpeed += addValue;
	}
	/*
	 *	重力の有効無効変更
	 *  @author Riku
	 */
	void SetIsGravity(bool setValue) {
		isGravity = setValue;
	}
};
#endif // !_GAMEOBJECT_