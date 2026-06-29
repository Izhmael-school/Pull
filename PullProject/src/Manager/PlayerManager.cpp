/*
 *	@file	PlayerManager.cpp
 *	@author Riku
 */

#include "PlayerManager.h"

/*
 *	プレイヤー生成
 */
PlayerManager::PlayerManager()
	: pPlayer(nullptr)
{}
void PlayerManager::CreatePlayer() {
	// プレイヤーの生成
	int playerModelHandle = MV1LoadModel("res/Model/Player/HandlessPlayer.mv1");
	pPlayer = std::make_shared<PlayerCharacter>(playerModelHandle, VGet(500, 600, 300));
	pPlayer->Start();

	// プレイヤーの手生成
	int handsModelHandle = MV1LoadModel("res/Model/Player/PlayerHands.mv1");
	pPlayer->CreateHands(pPlayer, handsModelHandle);

}
