/*
 * @file GameObject.h
 * @author oorui
 */

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Component/Transform.h"
#include "Definition/Enum/TagEnum.h"
#include "Definition/Const/VECTORConst.h"
#include "../../GameObject.h"
#include <memory>

class Collider;

class GimmickObject : public GameObject {

public:
	GimmickObject(int _modelHandle = -1, VECTOR _pos = VZero, VECTOR _rota = VZero, Tag _tag = None);
	virtual ~GimmickObject();

protected:
	virtual void Start();

	/*
	 *	モデルのローカル座標からAABBを求める
	 *  @param[in]	VECTOR& 最小値
	 *  @param[in]	VECTOR& 最大値
	 */
	void CalculateLocalAABB(VECTOR& outMin, VECTOR& outMax, VECTOR scale, VECTOR rotation) const;
public:
	virtual void Update();
	virtual void Render();
	virtual void Setup();
	virtual void Execute();
	/*
	 * @brief モデルの削除
	 */
	virtual void DeleteModel();

	/*
	 * @brief トランスフォームの取得
	 */
	inline Transform* GetTransform() const { return pTransform.get(); }

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
	 * @brief 更新可否取得
	 */
	inline bool IsActive() const { return isActive; }

	/*
	 * @brief 更新可否
	 */
	inline void SetActive(bool _isActive) { isActive = _isActive; }

	/*
	 * @brief モデルハンドルの代入
	 */
	inline void SetModelHandle(int _mHandle) { modelHandle = _mHandle; }

	/*
	 * @brief モデルハンドルの取得
	 */
	inline int GetModelHandle() const { return modelHandle; }

	/*
	 * @brief 当たった時
	 */
	virtual void OnTriggerEnter(Collider* _pSelf, Collider* _pOther);

	/*
	 * @brief 当たってる時
	 */
	virtual void OnTriggerStay(Collider* _pSelf, Collider* _pOther);

	/*
	 * @brief 離れた時
	 */
	virtual void OnTriggerExit(Collider* _pSelf, Collider* _pOther);

};

#endif // !_GAMEOBJECT_H_
