/*
 *	@file	CameraManager.h
 *	@author	Riku
 */

#ifndef _CAMERAMANAGER_H_
#define _CAMERAMANAGER_H_

#include "../Component/Singleton.h"
#include "ManagerBase.h"
#include "../GameObject/Camera/CameraObject.h"
#include <memory>

/*
 *	カメラ管理クラス
 */
class CameraManager : public ManagerBase, public Singleton<CameraManager> {
private:
	CameraObjectPtr camera;

public:
	CameraManager();
	~CameraManager() = default;

public:
	void Update() override {}

public:
	/*
	 *	カメラ生成
	 */
	void CreateCamera();

public:
	/*
	 *	カメラ取得
	 */
	inline CameraObjectPtr GetCamera() { return camera; }

};


#endif // !_CAMERAMANAGER_H_
