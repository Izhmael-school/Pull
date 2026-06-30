/*
 * @file ManagerBase.h
 * @author Sekino
 */
#pragma once
#ifndef _MANAGERBASE_H_
#define _MANAGERBASE_H_

class ManagerBase{

public:
	ManagerBase() = default;
	virtual ~ManagerBase() = default;

protected:
	/// <summary>
	/// 作られたときに入る処理
	/// </summary>
	virtual void Start();

public:
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Render();
	/// <summary>
	/// 後処理等
	/// </summary>
	virtual void Cleanup();
};
#endif