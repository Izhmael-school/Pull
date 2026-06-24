/*
 * @file GimmickObjectManager.cpp
 * @author oprui
 */

#include "GimmickObjectManager.h"
#include "../../GameObject/Stage/Gimmick/GimmickObject.h"

 /*
  *	オブジェクト登録
  */
void GimmickObjectManager::Register(GimmickObject* object) {
	if (object == nullptr) {
		return;
	}
	// 二重登録防止
	auto it = std::find(
		objects.begin(),
		objects.end(),
		object);

	if (it != objects.end()) {
		return;
	}

	objects.push_back(object);

	// 登録後に初期設定
	object->Setup();
}

/*
 * オブジェクト解除
 */
void GimmickObjectManager::Unregister(GimmickObject* object) {
	if (object == nullptr) {
		return;
	}

	objects.erase(
		std::remove(
			objects.begin(),
			objects.end(),
			object),
		objects.end());
}

void GimmickObjectManager::Setup() {
	for (auto object : objects) {
		if (object == nullptr) {
			continue;
		}

		if (!object->IsActive()) {
			continue;
		}

		object->Setup();
	}
}

/*
 * 全更新
 */
void GimmickObjectManager::Update() {
	for (auto object : objects) {
		if (object == nullptr) {
			continue;
		}

		if (!object->IsActive()) {
			continue;
		}

		object->Update();
	}
}

/*
 * 全描画
 */
void GimmickObjectManager::Render() {
	for (auto object : objects) {
		if (object == nullptr) {
			continue;
		}

		object->Render();
	}
}

/*
 * 全削除
 */
void GimmickObjectManager::Clear() {
	objects.clear();
}
