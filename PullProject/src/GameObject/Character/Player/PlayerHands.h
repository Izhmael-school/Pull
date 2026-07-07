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
		Idle,
		ArmsExtending,
		ArmsReturning,
		Catch,

		Max,
	};
	enum class CatchState {
		Invalid = -1,
		None,
		LeverCatch,
		EnemyCatch,
		PillerCatch,

		Max,
	};
	HandsState handsState;					// 手の状態
	CatchState catchState;					// 所持状態
	std::shared_ptr<PlayerCharacter> pOwner;// 所有者(プレイヤー)
	float extendSpeed;						// 伸ばすスピード
	float returnSpeedRatio;					// 戻ってくる速度割合
	ActionState action;						// アクション状態

	const float RETURN_THRESHOLD;			// 戻ってきたとみなす閾値
	
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
	inline bool IsArmExtended() { return handsState == HandsState::ArmsExtending; }

};
// 別名定義
using PlayerHandsPtr = std::shared_ptr<PlayerHands>;

#endif // !_PLAYERHANDS_H_
