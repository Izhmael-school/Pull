/*
 *	@file	StageSelectScene.h
 *  @author oorui
 */

#ifndef _STAGESELECTSCENE_H_
#define _STAGESELECTSCENE_H_

#include "Scene/SceneBase.h"
#include "GameObject/GameObject.h"
#include "../../Definition/CommonModule/ActionMapData.h"


#include <memory>
#include <string>
#include <functional>
#include <vector>


struct StageSelectInfo {
	std::string sceneName;
	std::function<void()> SceneChangeFunc;
};

 /*
  *	ステージ選択シーン
  */
class StageSelectScene :public SceneBase {
private:
	std::vector<StageSelectInfo> selectInfoArray;

	class AABBConllider* AABB;
	class CapsuleCollider* capsule;
	int currentScene;
	ActionState action;			// アクション状態

	int SkyModel;
public:
	/*
	 *	コンストラクタ
	 */
	StageSelectScene();
	/*
	 *	デストラクタ
	 */
	~StageSelectScene() = default;
	
private:
	void Start() override;

public:
	void Update() override;
	void Setup()override;
	void Render() override;

	void Cleanup();

};

#endif // !_STAGESELECTSCENE_H_
