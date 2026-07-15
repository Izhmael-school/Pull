#include "CameraManager.h"

CameraManager::CameraManager()
	: camera(nullptr)
{}

/*
 *	カメラ生成
 */
void CameraManager::CreateCamera(VECTOR position, VECTOR rotation) {
	camera = std::make_shared<CameraObject>(position, rotation);
	camera->Start();
}

/*
 *	カメラのシェイク
 *	@param	float setShakePowar	シェイクの大さ
 *	@param	float setShakeTime	シェイクの時間
 */
void CameraManager::CameraShake(float setShakePower, float setShakeTime) {
	camera->CameraShakeActivate(setShakePower, setShakeTime);
}
