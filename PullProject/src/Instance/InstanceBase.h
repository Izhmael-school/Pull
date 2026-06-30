/*
 * @brief 使用中のインスタンスを管理する基底クラス
 * @author Sekino
 */
#pragma once
#ifndef _INSTANCEBASE_H_
#define _INSTANCEBASE_H_

#include <memory>

class ResourceBase;

class InstanceBase {
protected:
	std::shared_ptr<ResourceBase> resource;	// 素材
	bool wantDelete;	// 削除してほしいか

public:
	InstanceBase(std::shared_ptr<ResourceBase> _resource);
	~InstanceBase() = default;

	// コピー禁止
	InstanceBase(const InstanceBase&) = delete;
	InstanceBase& operator=(const InstanceBase&) = delete;
	// 移動禁止
	InstanceBase(InstanceBase&&) = delete;
	InstanceBase& operator=(InstanceBase&&) = delete;

	/*
	 * @brief 更新
	 */
	virtual void Update() = 0;

	/*
	 * @brief 描画
	 */
	virtual void Render() = 0;

	/*
	 * @brief 削除要請
	 */
	inline bool WantDelete() const { return wantDelete; }

	/*
	 * @brief 削除可否
	 */
	inline void SetDelete(bool _fact) { wantDelete = _fact; }
};
#endif