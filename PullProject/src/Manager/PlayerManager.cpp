/*
 *	@file	PlayerManager.cpp
 *	@author Riku
 */

#include "PlayerManager.h"

/*
 *	プレイヤー生成
 */
PlayerManager::PlayerManager()
	: player(nullptr)
{}
void PlayerManager::CreatePlayer() {
	player = std::make_shared<PlayerCharacter>(0, VGet(0, 10, 0));
}
