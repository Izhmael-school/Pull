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
	PlayerCharacter();
	~PlayerCharacter() = default;

public:
	void Start() override;
	void Update() override;

};

#endif // !_PLAYERCHARACTER_H_
