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
