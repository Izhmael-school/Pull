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
#include "../../UI/Scene/PauseScreen.h"
#include "../../Definition/CommonModule/ActionMapData.h"

#include <memory>

class MainGameScene :public SceneBase {
private:
	EnemyManager enemyManager;		// 敵の管理
	UIManager m_UIManager;			// UIの管理
	ShadowMap shadowMap;			// シャドウマップ

	bool isStageClear = false;		// ステージクリア判定
	bool useEventCamera = false;	// イベントカメラ使用判定
	bool m_isPause = false;			// ポーズ中かどうか

	int SkyModel;	// スカイボックス用のモデル

	VECTOR playerPos;	// プレイヤーの座標
	ActionState action;								// アクション状態
	UIInput uiInput;
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

	/*
	 * @brief ステージを開始したときに入る
	 * @author Sekino
	 */
	void StageStartSetup();

	/*
	 * UI用入力処理
	 */
	void UIInputSetings();

	/*
	 * ポーズ中の処理
	 */
	void PauseRound();
};


#endif // !_MAINGAMESCENE_H_
