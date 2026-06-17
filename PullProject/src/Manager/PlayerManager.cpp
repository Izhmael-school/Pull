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
	int modelHandle = MV1LoadModel("res/Model/Player/Simple Player.mv1");
	player = std::make_shared<PlayerCharacter>(modelHandle, VGet(0, 400, 0));
}
