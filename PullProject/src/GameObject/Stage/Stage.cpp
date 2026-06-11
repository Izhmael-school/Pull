/*
 *  @file Stage.cpp
 *  @author oorui
 */

#include "Stage.h"

 /*
  *  コンストラクタ
  */
Stage::Stage()
	: StageBase() {
	modelHandle = -1;
}

/*
 *  デストラクタ
 */
Stage::~Stage() {
	Execute();
}


/*
 *	ステージデータの読み込み
 *  @pram const int modelHandle
 */
void Stage::SetModelHandle(const int modelHandleBase) {
	modelHandle = modelHandleBase;

	// 当たり判定クラスの生成

#if _DEBUG
	// メモリログに書き込む
#endif
}

/*
 *	更新
 */
void Stage::Update() {
}


/*
 *	描画
 */
void Stage::Render() {
	// モデルハンドルがあれば描画処理を行う
	if (modelHandle >= 0) MV1DrawModel(modelHandle);

#if _DEBUG
	// ステージの当たり判定の描画を行う(デバッグ)

#endif
}


/*
 *	終了処理
 */
void Stage::Execute() {
	if (modelHandle >= 0) {
		// モデル非表示
		Clean(modelHandle);
		// モデルの片付け
		MV1DeleteModel(modelHandle);
		// モデルをnull
		modelHandle = -1;
	}
}

