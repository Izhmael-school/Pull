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
	VECTOR target;			// 追従カメラのターゲット
	float speed;			// 移動速度
	float shakePower;		// シェイクの大きさ
	float shakeTime;		// シェイクの時間
	float shakeElapsedTime;	// シェイクの経過時間
	float chasePlayerPosY;	// プレイヤーを追うためのY座標
	bool isShaking;			// シェイク中か否か
	bool isChaseXZ;			// 追うか否か(XZ平面)
	bool isChaseY;			// 追うか否か(Y軸)

	bool isEvent;
	
	// Y軸移動の上限
	const float POSITION_Y_LIMIT_UP;
	// Y軸移動の下限
	const float POSITION_Y_LIMIT_DOWN;
	// プレイヤーから離れる距離
	const float PLAYER_DISTANCE;
	// 引っ張り時のズーム割合の最大
	const float PULL_ZOOM_RATIO_MAX;
	// 引っ張り時のズーム割合の最小
	const float PULL_ZOOM_RATIO_MIN;
	// ターゲットがプレイヤーを追う時の補間割合
	const float TARGET_MOVE_RATIO;
	// ターゲットがプレイヤーと離れられる最大距離
	const float TARGET_DISTANCE_MAX;
	// ターゲットとプレイヤーが重なったとみなす閾値
	const float TARGET_THRESHOLD;

public:
	CameraObject();
	~CameraObject() = default;

public:
	// 初期化処理
	void Start() override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;
	void OnTriggerExit(Collider* _pSelf, Collider* _pOther);

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
	/*
	 *	ターゲットのXZ平面上の移動
	 */
	void TargetMoveXZ();
	/*
	 *	ターゲットのY軸移動
	 */
	void TargetMoveY();

public:
	/*
	 *	カメラのシェイクを作動
	 *	@param	float setShakePowar	シェイクの大さ
	 *	@param	float setShakeTime	シェイクの時間
	 */
	void CameraShakeActivate(float setShakePower, float setShakeTime);

	/*
	 * @brief カメラモードの変更
	 * @author Sekino
	 */
	inline void ChangeCameraMode(int _mode) { mode = static_cast<CameraMode>(_mode); }
};
// 別名定義
using CameraObjectPtr = std::shared_ptr<CameraObject>;

#endif // !_CAMERAOBJECT_H_
