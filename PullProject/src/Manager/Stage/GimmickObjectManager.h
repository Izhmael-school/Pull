/*
 *	@file	GimmickObjectManager.h
 *  @author oorui
 */

#ifndef _GIMMICKOBJECTMANAGER_H_
#define _GIMMICKOBJECTMANAGER_H_

#include"../../Component/Singleton.h"

#include <vector>
#include <memory>

class GimmickObject;

class GimmickObjectManager :public Singleton<GimmickObjectManager> {
	// フレンド宣言
	friend class Singleton<GimmickObjectManager>;
private:
	/*
	 * @brief 管理中のギミックオブジェクト
	 */
	std::vector<GimmickObject*> objects;
private:
	GimmickObjectManager() = default;
	~GimmickObjectManager() = default;


public:
	/*
	 * @brief オブジェクト登録
	 */
	void Register(GimmickObject* object);

	/*
	 * @brief オブジェクト解除
	 */
	void Unregister(GimmickObject* object);

	/*
	 * @brief 全更新
	 */
	void Update();

	/*
	 * @brief 全描画
	 */
	void Render();

	/*
	 * @brief 全削除
	 */
	void Clear();
};

#endif // !_GIMMICKOBJECTMANAGER_H_
