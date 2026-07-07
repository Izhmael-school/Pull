/*
 *	@file TitleScene.cpp
 *  @author oorui
 */

#include "TitleScene.h"
#include "Manager/SceneManager.h"
#include "Definition/Enum/SceneType.h"
#include "Manager/InputManager.h"
#include <algorithm>
#include <math.h>


 /*
  *	コンストラクタ
  */
TitleScene::TitleScene()
	:currentScene(0) {
	Start();
}

/*
 *	デストラクタ
 */
TitleScene::~TitleScene() {
}

/*
 *	開始処理
 */
void TitleScene::Start() {
}

/*
 *	更新処理
 */
void TitleScene::Update() {
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN)) {
		SceneManager::GetInstance().ChangeScene(SceneType::StageSelect);
	}
}

/*
 *	描画処理
 */
void TitleScene::Render() {
	DrawString(940, 600, "Start", 0xffff00);
}

void TitleScene::Setup() {
	currentScene = 0;
}
