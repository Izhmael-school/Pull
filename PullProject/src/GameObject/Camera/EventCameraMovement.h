#pragma once

// Sekino
#include "../../Definition/CommonModule/MyJson.h"
#include "../../Manager/Stage/StageManager.h"
#include "../../Manager/TimeManager.h"
#include <vector>
//

class CameraObject;

// カメラの特殊行動のタイプ
enum CameraActionType {
	StopCamera
};

// カメラの特殊行動
struct CameraAction {
	CameraActionType type;
	float actionStartTime = -1.0f;
	float actionEndTime = -1.0f;
	float elapsedTime = 0.0f;
};

struct EventCameraMove {
	int eventID = -1;
	VECTOR lookAtPos = VZero;
	VECTOR startPos = VZero;
	VECTOR goalPos = VZero;
	float elapsedTime = 0.0f;
	std::vector<CameraAction> cameraAction;
};

class EventCameraMovement {
private:
	static EventCameraMove eventCameraMove;

	static bool isEventEnd;
public:
	/*
	 * @brief イベントカメラの起動
	 * @author Sekino
	 */
	static void StartEventCamera(CameraObject* _camera, std::string _eventName);

	static void Update(CameraObject* _camera);

	static bool IsEventEnd() { return isEventEnd; }
private:
	static void InitEventCamera_StartStage(CameraObject* _camera,nlohmann::json_abi_v3_12_0::json _json);

	static void UpdateEventCamera_StartStage(CameraObject* _camera);

	static void InitEventCamera_Action(nlohmann::json_abi_v3_12_0::json _json);

	static bool UpdateEventCamera_Action();
};

