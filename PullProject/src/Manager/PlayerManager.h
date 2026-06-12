/*
 *	@file	PlayerManager.h
 *	@author	Riku
 */

#ifndef _PLAYERMANAGER_H_
#define _PLAYERMANAGER_H_

#include "../Component/Singleton.h"
#include "ManagerBase.h"
#include "../GameObject/Character/Player/PlayerCharacter.h"
#include <memory>

/*
 *	プレイヤー管理クラス
 */
class PlayerManager : public ManagerBase, public Singleton<PlayerManager> {
private:
	PlayerCharacterPtr player;

public:
	PlayerManager();
	~PlayerManager() = default;

public:
	void Update() override{}

public:
	/*
	 *	プレイヤー生成
	 */
	void CreatePlayer();

public:
	/*
	 *	プレイヤー取得
	 */
	inline PlayerCharacterPtr GetPlayer() { return player; }

};

#endif // !_PLA
