#ifndef _COLLISIONMANAGER_H_
#define _COLLISIONMANAGER_H_

#include <vector>
#include "Component/Singleton.h"
#include "Component/Collider/Collider.h"
#include <unordered_map>

/*
 *  CollisionManager.h
 *  Tanaka
 */

 // 判定関数の型定義
using HitFunc = bool(*)(Collider*, Collider*);
// 押し出し関数の型定義
using ResolveFunc = void(*)(Collider*, Collider*);

class CollisionManager : public Singleton<CollisionManager> {
private:
	// コライダーの配列
    std::vector<class Collider*> pColliderArray;

	//  前回のフレームの当たり判定結果
    std::vector<std::vector<bool>> prevs;
	//  今回のフレームの当たり判定結果
    std::vector<std::vector<bool>> currents;

	//  当たり判定関数のテーブル
    std::unordered_map<std::string, HitFunc> hitFuncTable;
	//  押し出し関数のテーブル
    std::unordered_map<std::string, ResolveFunc> resolveFuncTable;

    int index = 0;

public:
	// コンストラクタ・デストラクタ
    CollisionManager();
    ~CollisionManager();

	// 登録・更新・描画・クリア
    void Register(Collider* col);
    void Update();
    void Render();
    void Clear();

	// 登録チェック・削除
    void CheckRegister(Collider* _pCol);
    void UnRegister(Collider* _pCol);
    void UnRegisterAll();

	// コライダー配列の取得
    const std::vector<Collider*>& GetColliders() const { return pColliderArray; }

	// 判定・押し出し
    void Resolve(Collider* a, Collider* b);

	// JSONから当たり判定ルールを読み込む
    void LoadCollisionRules(const std::string& path);

private:
	// 判定関数の呼び出し
    bool CheckHit(Collider* a, Collider* b);

public:
    // 判定
    static bool SphereVsSphere(Collider* a, Collider* b);
    static bool SphereVsAABB(Collider* a, Collider* b);
    static bool AABBvsAABB(Collider* a, Collider* b);
    static bool CapsuleVsSphere(Collider* a, Collider* b);
    static bool CapsuleVsAABB(Collider* a, Collider* b);
    static bool CapsuleVsCapsule(Collider* a, Collider* b);
    static bool RayVsSphere(Collider* a, Collider* b);
    static bool RayVsAABB(Collider* a, Collider* b);
    static bool RayVsCapsule(Collider* a, Collider* b);

    // 押し出し
    static void ResolveSphereSphere(Collider* a, Collider* b);
    static void ResolveSphereAABB(Collider* s, Collider* box);
    static void ResolveCapsuleAABB(Collider* cap, Collider* box);
};

#endif