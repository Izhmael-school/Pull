/*
 *	@file CameraObject.h
 *	@author Riku
 */

#ifndef _CAMERAOBJECT_H_
#define _CAMERAOBJECT_H_

#include "../GameObject.h"
#include <unordered_map>
#include <functional>

/*
 *	カメラオブジェクトクラス
 */
class CameraObject : public GameObject {
private:
	enum class CameraMode {
		Invalid = -1,
		Player,
		Event,
		Debug,

		Max
	};
	CameraMode mode;	// カメラのモード
	float speed;		// 移動速度

public:
	CameraObject();
	~CameraObject() = default;

public:
	void Start() override;
	void Update() override;

private:
	/*
	 *	プレイヤーカメラの更新処理
	 */
	void PlayerUpdate();
	/*
	 *	イベントカメラの更新処理
	 */
	void EventUpdate();
	/*
	 *	デバッグカメラの更新処理
	 */
	void DebugUpdate();
};

#endif // !_CAMERAOBJECT_H_
