/* 
 *	@file	PlayerCharacter.h
 *	@author	Riku
 */

#ifndef _PLAYERCHARACTER_H_
#define _PLAYERCHARACTER_H_

#include "../Character.h"
#include "PlayerHands.h"
#include "../../../Definition/CommonModule/ActionMapData.h"
#include <memory>

class Cliider;
class EnemyBase;
class ActionMapBase;

/*
 *	プレイヤーキャラクタークラス
 */
class PlayerCharacter : public Character {
private:
	enum class PlayerState {
		Invalid = -1,
		Idle,			// 待機
		Move,			// 移動
		Jump,			// ジャンプ
		LurchBackward,	// のけぞり
		
		Max
	};
	PlayerHandsPtr pHands;		// 手
	PlayerState playerState;	// プレイヤーの状態
	float speed;				// 移動速度
	float pullValue;			// 引っ張り値
	float lurchBackwardTime;	// のけぞり継続時間
	bool returnColor;			// 色戻しフラグ
	bool throwAnimation;		// 投げアニメーション中
	VECTOR lurchBackwardPos;	// のけぞり位置
	ActionState action;			// アクション状態


	// 引っこ抜きライン
	const float PULL_VALUE_MAX;
	// 引っこ抜き時のカメラシェイクの大きさ
	const float PULL_CAMERA_SHAKE_POWER;
	// 引っこ抜き時のカメラシェイクの時間
	const float PULL_CAMERA_SHAKE_TIME;
	// ジャンプ力
	const float JUMP_POWER;
	// 色戻し割合
	const float RETURN_COLOR_RATIO;
	// 引っ張り値戻し割合
	const float RETURN_PULL_VALUE_RATIO;
	// プレイヤーの振り向き割合
	const float TURN_RATIO;
	// のけぞり距離
	const float LURCH_BACKWARD_LENGTH;
	// のけぞり割合
	const float LURCH_BACKWARD_RATIO;
	// のけぞり切ったとみなす閾値
	const float LURCH_BACKWARD_THRESHOLD;
	// のけぞりの最大時間
	const float LURCH_BACKWARD_TIME_MAX;

public:
	PlayerCharacter(int _modelHandle, VECTOR _pos, Tag _tag = Player);
	~PlayerCharacter() = default;

public:
	// 初期化処理
	void Start() override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;
	// 当たった時
	void OnTriggerEnter(Collider* _pSelf, Collider* _pOther) override;
	// 当たっているとき
	void OnTriggerStay(Collider* _pSelf, Collider* _pOther) override;
	// 離れた時
	void OnTriggerExit(Collider* _pSelf, Collider* _pOther) override;

private:
	/*
	 *	移動
	 */
	void Move();
	/*
	 *	色を戻す
	 */
	void ReturnColor();
	/*
	 *  のけぞり
	 */
	void LurchBackward();

public:
	/*
	 *	手生成
	 *	@param	int モデルハンドル
	 *	@param	std::shared_ptr<PlayerCharacter> 所有者(プレイヤー)
	 */
	void CreateHands(std::shared_ptr<PlayerCharacter> owner, int modelHandle);
	/*
	 *	引っこ抜き
	 *	@return bool	引き抜いたか否か
	 */
	bool Pull();
	/*
	 *	引っこ抜き解除時処理
	 */
	void PullReset();
	/*
	 *	掴み移動の移動処理
	 *	@param	float 移動速度
	 */
	void CatchMovingMove(float moveSpeed);

public:
	/*
	 *	プレイヤーの手を取得
	 */
	inline PlayerHandsPtr GetHands() { return pHands; }
	/*
	 *	引き抜きラインに対する引っ張り値の割合取得
	 *	@return	float
	 */
	inline float GetPullValueRatio() { return pullValue / PULL_VALUE_MAX; }
	/*
	 *	プレイヤーの接地判定取得
	 */
	inline bool GetHitGroundingFrag() { return hitGroundingFrag; }
	/*
	 *	ジャンプ中か否か
	 */
	inline bool IsJump() { return playerState == PlayerState::Jump; }
	/*
	 *	入力アクション状態取得
	 */
	inline ActionState GetInputAction() { return action; }
	/*
	 *	投げアニメーション中かどうか
	 */
	inline bool IsThrowAnimation() { return throwAnimation; }
};
// 別名定義
using PlayerCharacterPtr = std::shared_ptr<PlayerCharacter>;

#endif // !_PLAYERCHARACTER_H_
