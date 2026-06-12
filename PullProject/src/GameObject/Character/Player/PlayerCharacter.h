/* 
 *	@file	PlayerCharacter.h
 *	@author	Riku
 */

#ifndef _PLAYERCHARACTER_H_
#define _PLAYERCHARACTER_H_

#include "../Character.h"

/*
 *	プレイヤーキャラクタークラス
 */
class PlayerCharacter : public Character {
private:
	float speed;
	
public:
	PlayerCharacter(int _modelHandle, VECTOR _pos, Tag _tag = None);
	~PlayerCharacter() = default;

public:
	// 初期化処理
	void Start() override;
	// 更新処理
	void Update() override;
};

#endif // !_PLAYERCHARACTER_H_
