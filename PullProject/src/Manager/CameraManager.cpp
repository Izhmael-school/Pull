#include "CameraManager.h"

CameraManager::CameraManager()
	: camera(nullptr)
{}

/*
 *	カメラ生成
 */
void CameraManager::CreateCamera() {
	camera = std::make_shared<CameraObject>();
}

/*
 *	カメラのシェイク
 *	@param	float setShakePowar	シェイクの大さ
 *	@param	float setShakeTime	シェイクの時間
 */
void CameraManager::CameraShake(float setShakePower, float setShakeTime) {
	camera->CameraShakeActivate(setShakePower, setShakeTime);
}
