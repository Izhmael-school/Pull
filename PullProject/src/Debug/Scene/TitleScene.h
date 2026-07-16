/*
 *	@file	TitleScene.h
 *  @author oorui
 */

#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_

#include "Scene/SceneBase.h"
#include "Manager/EnemyManager.h"
#include "Manager/EffectManager.h"
#include "Manager/EffectResourceManager.h"
#include "Manager/AudioManager.h"
#include "Manager/AudioResourceManager.h"
#include "GameObject/GameObject.h"
#include "../../UI/UIManager.h"

#include <string>
#include <functional>
#include <vector>


/*
 *	タイトルシーン
 */
class TitleScene : public SceneBase {
private:
	int currentScene;	// 現在のシーン
	int SkyModel;

	EnemyManager enemyManager;						// エネミーの管理
	EffectManager effectManager;					// エフェクト管理
	EffectResourceManager effectResourceManager;	// エフェクトリソース管理
	AudioManager audioManager;						// オーディオ管理
	AudioResourceManager audioResourceManager;		// オーディオリソース管理
	UIManager m_uiManager;							// UI管理

public:
	/*
	 *	コンストラクタ
	 */
	TitleScene();
	/*
	 *	デストラクタ
	 */
	~TitleScene();

private:
	void Start() override;

public:
	void Update() override;

	void Render() override;

	void Setup() override;

	void Cleanup() override;
};





#endif // !_TITLE_SCENE_H_