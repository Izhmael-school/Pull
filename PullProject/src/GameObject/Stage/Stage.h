/*
 *	@file	Stage.h
 *  @author oorui
 */

#ifndef _STAGE_H_
#define _STAGE_H_

#include "StageBase.h"
#include <string>
#include <vector>
#include "../GameObject.h"
#include "Generator/StageCollisionGenerator.h"

class Stage :public StageBase {
private:
	std::string modelPath;	// モデルのパス

public:
	/*
	 *	コンストラクタ
	 */
	Stage();

	/*
	 *	デストラクタ
	 */
	virtual ~Stage() override;

public:
	/*
	 *	ステージデータの読み込み
	 *  @param[in]	const int モデルハンドル
	 */
	void SetModelHandle(const int modelHandleBase)override;

	/*
	 *	更新
	 */
	void Update() override;

	/*
	 *	描画
	 */
	void Render() override;

	/*
	 *	終了処理
	 */
	void Execute() override;



};


#endif // !_STAGE_H_
