/*
 *	@file	StageBase.h
 *  @author oorui
 */

#ifndef _STAGEBASE_H_
#define _STAGEBASE_H_

#include <DxLib.h>
#include <string>

class  GameObject;

/*
 *	ステージの基礎
 */
class StageBase {
protected:
	// モデルハンドル
	int modelHandle;

public:
	// コンストラクタ
	StageBase() :modelHandle(-1) {}
	virtual ~StageBase() = default;

protected:
	/*
	 *	片付け処理
	 */
	void Clean(int m_handle) {
		if (m_handle >= 0) MV1SetVisible(m_handle, false);
	}

public:
	/*
	 *	更新処理
	 */
	virtual void Update() = 0;

	/*
	 *	描画処理
	 */
	virtual void Render() = 0;

	/*
	 *	終了処理
	 */
	virtual void Execute() = 0;

public:
	/*
	 *	ステージモデルハンドルの設定
	 *  @param	int modelHandle 設定するステージモデルハンドル
	 */
	virtual void SetModelHandle(const int modelHandleBase) = 0;

	/*
	 *	ステージのモデルハンドルの取得
	 *  @return modelHandle
	 */
	virtual int GetStageModelHandle() const { return modelHandle; }

};





#endif // !_STAGEBASE_H_
