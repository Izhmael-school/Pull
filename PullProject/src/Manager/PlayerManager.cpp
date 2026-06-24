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
	int playerModelHandle = MV1LoadModel("res/Model/Player/Simple Player.mv1");
	player = std::make_shared<PlayerCharacter>(playerModelHandle, VGet(0, 200, 0));
	player->Start();
}
