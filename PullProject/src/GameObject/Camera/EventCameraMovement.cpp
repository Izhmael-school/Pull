#include "EventCameraMovement.h"
#include "GameObject/Camera/CameraObject.h"

EventCameraMove EventCameraMovement::eventCameraMove;
bool EventCameraMovement::isEventEnd = true;

void EventCameraMovement::StartEventCamera(CameraObject* _camera, std::string _eventName) {
	auto data = MyJson::LoadJsonFile("src/Data/EventCamera.json");
	if (data.is_null()) return;
	auto events = data["events"];
	if (events.is_null()) return;
	auto event = events[_eventName];
	if (event.is_null()) return;
	if (event["action"].is_null()) return;
	auto action = event["action"];
	eventCameraMove.eventID = event["eventID"];
	// IDに沿って処理を変える
	switch (eventCameraMove.eventID) {
	case 0:
		InitEventCamera_StartStage(_camera,event);
		break;
	}

	InitEventCamera_Action(action);

	isEventEnd = false;
}

void EventCameraMovement::Update(CameraObject* _camera) {
	// 先にアクションの確認更新をする
	if (!UpdateEventCamera_Action()) return;

	switch (eventCameraMove.eventID) {
	case 0:
		UpdateEventCamera_StartStage(_camera);
		break;
	default:
		break;
	}
}

void EventCameraMovement::InitEventCamera_StartStage(CameraObject* _camera,nlohmann::json_abi_v3_12_0::json _json) {
	if (_json == nullptr) return;
	int modelHandle = StageManager::GetInstance().GetCurrentStage()->GetStageModelHandle();
	if(modelHandle == -1) return;
	// フレーム名からワールド座標を算出
	std::string frameName = _json["lookAtStageFrameName"];
	eventCameraMove.lookAtPos = MV1GetFramePosition(modelHandle, MV1SearchFrame(modelHandle, frameName.c_str()));
	frameName = _json["startPosFrameName"];
	eventCameraMove.startPos = MV1GetFramePosition(modelHandle, MV1SearchFrame(modelHandle, frameName.c_str()));
	frameName = _json["goalPosFrameName"];
	eventCameraMove.goalPos = MV1GetFramePosition(modelHandle, MV1SearchFrame(modelHandle, frameName.c_str()));
	// スタート位置から少し離れた場所から開始
	VECTOR dir = VSub(eventCameraMove.goalPos, eventCameraMove.startPos);
	VECTOR nDir = VNorm(dir);
	VECTOR addPos = VScale(nDir, 1000);
	eventCameraMove.startPos = VAdd(eventCameraMove.startPos, addPos);
	eventCameraMove.goalPos = VAdd(eventCameraMove.goalPos, addPos);

	eventCameraMove.startPos.y += 1000;
	eventCameraMove.goalPos.y += 1000;
	eventCameraMove.elapsedTime = 0.0f;

	Transform* transform = _camera->GetTransform();
	transform->SetPosition(eventCameraMove.startPos);
	transform->LookAt(eventCameraMove.lookAtPos);
	VECTOR rot = _camera->GetRotation();
}

void EventCameraMovement::UpdateEventCamera_StartStage(CameraObject* _camera) {
	if (eventCameraMove.elapsedTime >= 1.0f) {
		// カメラモードを変える
		_camera->ChangeCameraMode(1);
		isEventEnd = true;
		// 終了
		return;
	}

	Transform* transform = _camera->GetTransform();
	// 一点を見続ける
	transform->LookAt(eventCameraMove.lookAtPos);
	VECTOR rot = _camera->GetRotation();
	eventCameraMove.elapsedTime += TimeManager::GetInstance().GetDeltaTime() / 2;
	// 移動
	VECTOR pos;
	pos.x = std::lerp(eventCameraMove.startPos.x, eventCameraMove.goalPos.x, eventCameraMove.elapsedTime);
	pos.y = std::lerp(eventCameraMove.startPos.y, eventCameraMove.goalPos.y, eventCameraMove.elapsedTime);
	pos.z = std::lerp(eventCameraMove.startPos.z, eventCameraMove.goalPos.z, eventCameraMove.elapsedTime);
	transform->SetPosition(pos);
}

void EventCameraMovement::InitEventCamera_Action(nlohmann::json_abi_v3_12_0::json _json) {
	if (_json == nullptr) return;
	// 初期化
	eventCameraMove.cameraAction.clear();

	for (auto j : _json) {
		CameraAction cameraAction;
		cameraAction.type = static_cast<CameraActionType>(j["actionTypeID"]);
		cameraAction.actionStartTime = j["actionStartTime"];
		float end = j["actionEndTime"];
		cameraAction.actionEndTime = end + cameraAction.actionStartTime;
		// イベントを入れる
		eventCameraMove.cameraAction.push_back(cameraAction);
	}
}

bool EventCameraMovement::UpdateEventCamera_Action() {
	float t = TimeManager::GetInstance().GetDeltaTime();
	for (auto& action : eventCameraMove.cameraAction) {
		action.elapsedTime += t;

		// アクションの再生時間内なら
		if (action.elapsedTime >= action.actionStartTime &&
			action.actionEndTime >= action.elapsedTime) {

			switch (action.type) {
			case StopCamera:
				return false;
			default:
				break;
			}
		}
	}
	return true;
}
