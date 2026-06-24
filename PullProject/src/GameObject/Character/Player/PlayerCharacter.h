/* 
 *	@file	PlayerCharacter.h
 *	@author	Riku
 */

#ifndef _PLAYERCHARACTER_H_
#define _PLAYERCHARACTER_H_

#include "../Character.h"
#include "../../../Component/Collider/Collider.h"
#include <memory>

/*
 *	プレイヤーキャラクタークラス
 */
class PlayerCharacter : public Character {
private:
	enum class PlayerState {
		Invalid = -1,
		Normal,
		ArmsExtended,
		GimmickCatch,
		EnemyCatch,

		Max
	};
	PlayerState playerState;	// プレイヤーの状態
	float speed;				// 移動速度
	float pullValue;			// 引っ張り値

	// 引っこ抜きライン
	const float PULL_VALUE_MAX;
	// 引っこ抜き時のカメラシェイクの大きさ
	const float PULL_CAMERA_SHAKE_POWER;
	// 引っこ抜き時のカメラシェイクの時間
	const float PULL_CAMERA_SHAKE_TIME;
	
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
	void OnTriggerEnter(Collider* _pOther) override;
	// 当たっているとき
	void OnTriggerStay(Collider* _pOther) override;
	// 離れた時
	void OnTriggerExit(Collider* _pOther) override;

private:
	/*
	 *	移動
	 */
	void Move();
	/*
	 *	引っこ抜き
	 *	@return bool	引き抜いたか否か
	 */
	bool Pull();

public:
	/*
	 *	引き抜きラインに対する引っ張り値の割合取得
	 *	@return	float
	 */
	float GetPullValueRatio();

public:
	/*
	 *　掴み状態か否か
	 *	@return bool
	 */
	inline bool IsGimmickCatch() { return playerState == PlayerState::GimmickCatch; }
};
// 別名定義
using PlayerCharacterPtr = std::shared_ptr<PlayerCharacter>;

#endif // !_PLAYERCHARACTER_H_
