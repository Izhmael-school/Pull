/*
 *	@file	BreakWall.h
 *  @author oorui
 */

#ifndef _BREALWALL_H_
#define _BREALWALL_H_



#include "GameObject/GameObject.h"
#include "GameObject/Stage/Gimmick/TriggerInterface.h"

 /*
  *	壊れる壁
  *  @param GameObject
  *  @param TriggerInterface		レバー対応ギミックのためインターフェース継承
  */
class BreakWall : public GameObject, public TriggerInterface {
private:
	int triggerID;	// 自身のID
	bool isBroken;	// 壊れたかどうか

	bool isFading;	// フェード中かどうか
	float opacity;	// アルファ値
	float fadeSpeed;// フェード速度
public:
	/*
	 *	コンストラクタ
	 */
	BreakWall(int id, int modelHandle, VECTOR pos);

private:
	/*
	 *	オブジェクトの不透明度を変更する
	 */
	void OpacityChange();

public:


	/*
	 *	使用前準備
	 */
	void Setup() override;

	/*
	 *	更新処理
	 */
	void Update()override;

	/*
	 *	描画処理
	 */
	void Render() override;

	/*
	 *	レバーから通知を受けた時の処理
	 */
	void OnTriggered() override;
public:
	/*
	 *	IDの取得
	 */
	int GetTriggerID() const override { return triggerID; };

};

#endif // !_BREALWALL_H_