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
	//int modelHandle = MV1LoadModel("res/Model/Enemy/WalkEnemy.mv1");
	player = std::make_shared<PlayerCharacter>(0, VGet(0, 500, 0));
}
