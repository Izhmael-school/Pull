/*
 *	@file	BomBreakWall.h
 *  @author oorui
 */

#ifndef _BOMBREAKWALL_H_
#define _BOMBREAKWALL_H_

#include "GameObject/GameObject.h"
#include "GameObject/Stage/Gimmick/GimmickObject.h"

#include "../../../Definition/CommonModule/MyMath.h"

 /*
  *	爆弾で壊れる壁
  *  @param	GameObject
  */
class BomBreakWall : public GimmickObject {
private:
	bool isBroken;		// 壊れたかどうか
	bool isFading;		// フェード中かどうか
	bool isActiv;		// ギミック起動チェック

	float opacity;		// アルファ値
	float fadeSpeed;	// フェード速度

public:
	/*
	 *	コンストラクタ
	 */
	BomBreakWall(int modelHandle, VECTOR pos, VECTOR rota);


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


public:

	/*
	 *	ギミック起動
	 *  @param[in]	bool	起動するかどうか
	 */
	void ActivGimmick(bool flag) { isActiv == flag; }
};





#endif // !_BOMBREAKWALL_H_