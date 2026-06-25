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
	int playerModelHandle = MV1LoadModel("res/Model/Player/HandlessPlayer.mv1");
	player = std::make_shared<PlayerCharacter>(playerModelHandle, VGet(0, 200, 0));
	player->Start();
	int handsModelHandle = MV1LoadModel("res/Model/Player/PlayerHands.mv1");
	hands = std::make_shared<PlayerHands>(handsModelHandle, VZero);
	hands->GetTransform()->AttachParent(player->GetTransform());
	hands->Start();
}
