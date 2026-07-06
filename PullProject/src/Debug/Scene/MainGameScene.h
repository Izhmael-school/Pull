/*
 *	@file	MainGameScene.h
 *  @author oorui
 */

#ifndef _MAINGAMESCENE_H_
#define _MAINGAMESCENE_H_

#include "Scene/SceneBase.h"
#include "GameObject/GameObject.h"

#include <memory>

class MainGameScene :public SceneBase {
private:

public:
	MainGameScene();
	~MainGameScene() = default;

private:
	void Start() override;

public:
	void Setup() override;

	void Update() override;

	void Render() override;
};


#endif // !_MAINGAMESCENE_H_
