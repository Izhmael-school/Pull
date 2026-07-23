/*
 *	@file	PlayerCharacter.h
 *	@author	Riku
 */

#ifndef _PLAYERCHARACTER_H_
#define _PLAYERCHARACTER_H_

#include "../Character.h"
#include "PlayerHands.h"
#include "../../../Component/Collider/Collider.h"
#include "../../../Definition/CommonModule/ActionMapData.h"
#include <memory>

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
		UseHands,		// 手使用

		Max
	};
	PlayerHandsPtr pHands;		// 手
	PlayerState playerState;	// プレイヤーの状態
	float speed;				// 移動速度
	float pullValue;			// 引っ張り値
	float lurchBackwardTime;	// のけぞり継続時間
	bool returnColor;			// 色戻しフラグ
	bool throwAnimation;		// 投げアニメーション中
	bool dieAnimation;			// 死亡アニメーション中
	bool isDead;				// 死亡しているか
	bool lockOn;				// ロックオン
	VECTOR lockOnTarget;		// ロックオン中のターゲット位置
	VECTOR lurchBackwardPos;	// のけぞり位置
	ActionState action;			// アクション状態
	// ロックオン用の視界
	std::unique_ptr<RayCollider> pLockOnVision;

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
	// 視界の射程
	const float VISION_LENGTH;
	// 視界の上下範囲
	const float VISION_HEIGHT;
	// 視界の扇形角度(度数)
	const float VISION_ANGLE;
	// 距離をジャンプパワーに変換する割合
	const float CHANGE_LENGTH_TO_POWER_RATIO;
	// 掴み移動時のジャンプ制限距離
	const float CATCH_MOVING_JAMP_LENGTH;

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
	 *	掴み解除時処理
	 */
	void CatchReset();
	/*
	 *	掴み移動の移動処理
	 *	@param	float 移動速度
	 */
	void CatchMovingMove(float moveSpeed);
	/*
	 *	視界内にオブジェクトがあるかどうか
	 *	@param[out]	VECTOR	視界内の一番近いオブジェクトの位置
	 */
	bool GetVisionObject(VECTOR& targetObject);
	/*
	 *	掴み移動時のジャンプ
	 */
	void CatchMovingJamp();

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
	/*
	 *	死亡アニメーション中かどうか
	 */
	inline bool IsDieAnimation() { return dieAnimation; }
	/*
	 *	死亡しているか
	 */
	inline bool IsDead() { return isDead; }
	/*
	 *	ロックオン中かどうか
	 */
	inline bool GetLockOn() { return lockOn; }
	/*
	 *	ロックオン中のターゲット位置取得
	 */
	inline VECTOR GetLockOnTarget() { return lockOnTarget; }

	/*
	 *	死亡判定の切り替え
	 *  @author oorui
	 */
	inline void SetIsDead(bool frag) { isDead = frag; }
};
// 別名定義
using PlayerCharacterPtr = std::shared_ptr<PlayerCharacter>;

#endif // !_PLAYERCHARACTER_H_
