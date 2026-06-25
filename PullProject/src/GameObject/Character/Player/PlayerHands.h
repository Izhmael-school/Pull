/*
 *	PlayerHands.h
 *	@author Riku
 */

#ifndef _PLAYERHANDS_H_
#define _PLAYERHANDS_H_

#include "../Character.h"
#include "../../../Component/Collider/Collider.h"
#include <memory>

/*
 *	プレイヤーの手クラス
 */
class PlayerHands : public Character {
private:
	float speed;

public:
	PlayerHands(int _modelHandle, VECTOR _pos, Tag _tag = Player);
	~PlayerHands() = default;

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

};
// 別名定義
using PlayerHandsPtr = std::shared_ptr<PlayerHands>;

#endif // !_PLAYERHANDS_H_
