/*
 *	PlayerHands.h
 *	@author Riku
 */

#ifndef _PLAYERHANDS_H_
#define _PLAYERHANDS_H_

#include "../Character.h"
#include "../../../Definition/CommonModule/ActionMapData.h"
#include <memory>

class Collider;
class PlayerCharacter;

/*
 *	プレイヤーの手クラス
 */
class PlayerHands : public Character {
private:
	enum class HandsState {
		Invalid = -1,
		Idle,			// 待機
		ArmsExtending,	// ウデ伸ばし
		ArmsReturning,  // ウデ戻し
		Catch,			// 掴み

		Max,
	};
	enum class CatchState {
		Invalid = -1,
		None,			// 何も掴んでいない
		LeverCatch,		// レバーを掴んでいる
		EnemyCatch,		// 敵を掴んでいる
		PillerCatch,	// 柱を掴んでいる

		Max,
	};
	HandsState handsState;						// 手の状態
	CatchState catchState;						// 所持状態
	std::shared_ptr<PlayerCharacter> pOwner;	// 所有者(プレイヤー)
	Collider* pCatchCollider;					// 掴んだオブジェクト
	float extendSpeed;							// 伸ばすスピード
	ActionState action;							// アクション状態
	
	// 戻ってくる速度割合
	float RETURN_SPEED_RATIO;
	// 戻ってきたとみなす閾値
	const float RETURN_THRESHOLD;
	// ウデ伸ばし上限
	const float ARM_LENGTH_MAX;
	// 持ち上げる位置
	const VECTOR CARRY_POSITION;
	// 掴み変色割合
	const float CATCH_COLOR_CHANGE_RATIO;
	// 手の当たり判定の半径
	const float HANDS_COLLIDER_RADIUS;
	
public:
	PlayerHands(std::shared_ptr<PlayerCharacter> _owner, int _modelHandle, VECTOR _pos, Tag _tag = Player);
	~PlayerHands() = default;

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
	 *	手の移動処理
	 */
	void HandsMove();
	/*
	 *	掴み移動処理
	 */
	void CatchMoving();
	/*
	 *	掴み中の更新処理
	 */
	void CatchUpdate();
	/*
	 *	掴み時の反応
	 */
	void CatchReaction();

public:
	/*
	 *	何かを掴んでいるか否か
	 *  @return bool
	 */
	inline bool IsCatch() { return handsState == HandsState::Catch; }
	/*
	 *　レバーを掴んでいるか否か
	 *	@return bool
	 */
	inline bool IsLeverCatch() { return catchState == CatchState::LeverCatch; }
	/*
	 *	ウデ伸ばし中か否か
	 *	@return bool
	 */
	inline bool IsArmExtending() { return handsState == HandsState::ArmsExtending; }
	/*
	 *	ウデ戻し中か否か
	 *	@return bool
	 */
	inline bool IsArmReturning() { return handsState == HandsState::ArmsReturning; }
	/*
	 *	手は待機状態か否か
	 *	@return bool
	 */
	inline bool IsHandIdle() { return handsState == HandsState::Idle; }
	/*
	 *	敵を掴んでいるか否か
	 *	@return bool
	 */
	inline bool IsEnemyCatch() { return catchState == CatchState::EnemyCatch; }

};
// 別名定義
using PlayerHandsPtr = std::shared_ptr<PlayerHands>;

#endif // !_PLAYERHANDS_H_
