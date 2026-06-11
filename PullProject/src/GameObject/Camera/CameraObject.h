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
		Debug,
		Player,
		Event,

		Max
	};
	CameraMode mode;	// カメラのモード
	float speed;		// 移動速度

public:
	CameraObject();
	~CameraObject() = default;

public:
	// 初期化処理
	void Start() override;
	// 更新処理
	void Update() override;

private:
	/*
	 *	デバッグカメラの更新処理
	 */
	void DebugUpdate();
	/*
	 *	プレイヤーカメラの更新処理
	 */
	void PlayerUpdate();
	/*
	 *	イベントカメラの更新処理
	 */
	void EventUpdate();
};
// 別名定義
using CameraObjectPtr = std::shared_ptr<CameraObject>;

#endif // !_CAMERAOBJECT_H_
