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
		Pull,
		Event,

		Max
	};
	CameraMode mode;		// カメラのモード
	float speed;			// 移動速度
	float shakePower;		// シェイクの大きさ
	float shakeTime;		// シェイクの時間
	float shakeElapsedTime;	// シェイクの経過時間
	bool isShaking;			// シェイク中か否か

	// プレイヤーから離れる距離
	const float PLAYER_DISTANCE;
	// 引っ張り時のズーム割合の最大
	const float PULL_ZOOM_RATIO_MAX;
	// 引っ張り時のズーム割合の最小
	const float PULL_ZOOM_RATIO_MIN;
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
	 *	引っこ抜きカメラの更新処理
	 */
	void PullUpdate();
	/*
	 *	イベントカメラの更新処理
	 */
	void EventUpdate();
	/*
	 *	カメラのシェイク
	 */
	void CameraShake();

public:
	/*
	 *	カメラのシェイクを作動
	 *	@param	float setShakePowar	シェイクの大さ
	 *	@param	float setShakeTime	シェイクの時間
	 */
	void CameraShakeActivate(float setShakePower, float setShakeTime);

};
// 別名定義
using CameraObjectPtr = std::shared_ptr<CameraObject>;

#endif // !_CAMERAOBJECT_H_
