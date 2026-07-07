/*
 *	@file	TitleScene.h
 *  @author oorui
 */

#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_

#include "Scene/SceneBase.h"
#include <string>
#include <functional>
#include <vector>


/*
 *	タイトルシーン
 */
class TitleScene : public SceneBase {
private:
	int currentScene;	// 現在のシーン

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

};





#endif // !_TITLE_SCENE_H_