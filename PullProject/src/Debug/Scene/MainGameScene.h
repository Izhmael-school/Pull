/*
 *	@file	MainGameScene.h
 *  @author oorui
 */

#ifndef _MAINGAMESCENE_H_
#define _MAINGAMESCENE_H_

#include "Scene/SceneBase.h"
#include "GameObject/GameObject.h"
#include "Manager/EnemyManager.h"
#include "Manager/EffectManager.h"
#include "Manager/EffectResourceManager.h"
#include "Manager/AudioManager.h"
#include "Manager/AudioResourceManager.h"
#include "../../UI/UIManager.h"
#include "Resource/Shadow/ShadowMap.h"

#include <memory>

class MainGameScene :public SceneBase {
private:
	EnemyManager enemyManager;						// エネミーの管理
	//EffectManager effectManager;					// エフェクト管理
	//EffectResourceManager effectResourceManager;	// エフェクトリソース管理
	//AudioManager audioManager;						// オーディオ管理
	//AudioResourceManager audioResourceManager;		// オーディオリソース管理
	UIManager m_UIManager;
	ShadowMap shadowMap;								// シャドウマップ

	bool isStageClear = false;						// ステージクリア判定
	
	int SkyModel;

	VECTOR playerPos;
public:
	MainGameScene();
	~MainGameScene() = default;

private:
	void Start() override;

public:
	void Setup() override;

	void Update() override;

	void Render() override;

	void Cleanup() override;

	void Reset();

};


#endif // !_MAINGAMESCENE_H_
