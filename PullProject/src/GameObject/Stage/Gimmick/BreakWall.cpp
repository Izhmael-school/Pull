/*
 *	@file	BreakWall.cpp
 *  @author oorui
 */
#include "BreakWall.h"
#include "Manager/Stage/GimmickManager.h"


 /*
  *	コンストラクタ
  */
BreakWall::BreakWall(int id, int modelHandle, VECTOR pos)
	:GameObject(modelHandle, pos)
	, triggerID(id)
	, isBroken(false)
	, isFading(false)
	, opacity(1.0f)
	, fadeSpeed(0.5f) {
}

/*
 *	使用前準備
 */
void BreakWall::Setup() {
	// レバー対応ギミックの末尾に追加
	GimmickManager::GetInstance().RegisterLeverReceiver(triggerID, this);
	
}

/*
 *	更新処理
 */
void BreakWall::Update(){
	// ギミックが起動したら
	if (isFading) {
		// 徐々に透明にしていく
		opacityChange();
	};
}

/*
 *	描画処理
 */
void BreakWall::Render() {
	GameObject::Render();
	// モデルハンドルなければ処理を抜ける
	if (modelHandle <= 0) return;
	// 壊れていたら描画しない
	if (isBroken) return;


	// モデルの透明度を設定
	MV1SetOpacityRate(modelHandle, opacity);

	// 描画
	//MV1DrawModel(modelHandle);
	
}

/*
 *	通知を受けた時の処理
 */
void BreakWall::OnTriggered() {
	// フェード中、フェード修了していたら抜ける
	if (isFading || isBroken)return;

	// エフェクト　※なんかでっかい煙とか

	// フェード処理を開始判定にする
	isFading = true;
}

/*
 *	オブジェクトの不透明度を変更する
 */
void BreakWall::opacityChange() {
	// α値を徐々に減らしていく
	opacity -= fadeSpeed;

	// 0以下対策
	if (opacity <= 0.0f) {
		opacity = 0.0f;

		// 消えた判定にする
		isFading = false;
		isBroken = true;
	}
}