#pragma once
#include "../../GameObject/GameObject.h"

/*
 * @file Collider.h
 * @author Tanaka
 */

class Collider {
protected:
	bool isEnable;              //  当たり判定の有効/無効
    GameObject* pGameObject;    //  当たり判定を付けるオブジェクト

public:
    //  コンストラクタ
    Collider(GameObject* _pObj);
	//  デストラクタ
    virtual ~Collider();

public:
    //  更新処理
    virtual void Update() = 0;
	//  描画処理
    virtual void Render() = 0; 

public:
	//  当たり判定の有効/無効
    bool IsEnable() const { return isEnable; }
    void SetEnable(bool _v) { isEnable = _v; }

	//  当たり判定を付けるオブジェクト
    GameObject* GetGameObject() const { return pGameObject; }
    void SetGameObject(GameObject* _pObj) { pGameObject = _pObj; }
};

/*
 *	@file Collider.h
 *	@brief AABB
 *	@author Tanaka
 */

class AABBCollider : public Collider {
private:
	VECTOR localMin;
	VECTOR localMax;

	VECTOR worldMin;
	VECTOR worldMax;

public:
	AABBCollider(GameObject* obj, VECTOR min, VECTOR max);

	void Update() override;
	void Render() override;

	VECTOR GetMin() const { return worldMin; }
	VECTOR GetMax() const { return worldMax; }
};

/*
 *	@file Collider.h
 *	@brief 当たり判定 Sphere
 *	@author Tanaka
 */
class SphereCollider : public Collider {
private:
	VECTOR localCenter;	//  球のローカル中心点
	VECTOR worldCenter;	//  球のワールド中心点
	float radius;		//  球の半径

public:
	//  コンストラクタ
	SphereCollider(GameObject* _pObj, VECTOR _localCenter, float _radius);
	//  デストラクタ
	~SphereCollider() = default;

public:	//	オーバーライドしたメンバ関数
	//  更新処理
	void Update() override;
	//  描画処理
	void Render() override;
	
public:	//	ゲッターとセッター
	inline float GetRadius() const { return radius; }
	inline void SetRadius(float _radius) { radius = _radius; }
	inline VECTOR GetLocalCenter() const { return localCenter; }
	inline VECTOR GetWorldCenter() const { return worldCenter; }
};

class CapsuleCollider : public Collider {
private:
	VECTOR localStart;	//  カプセルのローカル開始点
	VECTOR localEnd;	//  カプセルのローカル終了点
	VECTOR worldStart;	//  カプセルのワールド開始点
	VECTOR worldEnd;	//  カプセルのワールド終了点

	float radius;		//  カプセルの半径

public:
	CapsuleCollider(GameObject* _pObj, VECTOR _localStart, VECTOR _localEnd, float _radius, VECTOR _offset);
	~CapsuleCollider() = default;

public:
	void Update() override;
	void Render() override;

public:
	inline float GetRadius() const { return radius; }
	inline void SetRadius(float _radius) { radius = _radius; }
	inline VECTOR GetLocalStart() const { return localStart; }
	inline VECTOR GetLocalEnd() const { return localEnd; }
	inline VECTOR GetWorldStart() const { return worldStart; }
	inline VECTOR GetWorldEnd() const { return worldEnd; }
};