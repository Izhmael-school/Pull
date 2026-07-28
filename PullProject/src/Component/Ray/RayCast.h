#pragma once

#include <vector>

#include "RayHit.h"
#include "RayCastSetting.h"
#include "Ray.h"

class RayCast {
public:

    static bool Cast(
        const RayCastSetting& setting,
        RayHit& hit);

    static bool FanCast(
        const RayCastSetting& setting,
        std::vector<RayHit>& hits);

    static VECTOR GetDirection(
        const RayCastSetting& setting);
};

/*
 	RayCastSetting setting;

	setting.origin = GetPosition();

	// プレイヤーのヨー角
	float yawRad =
		MyMath::Deg2Rad(
			GetRotation().y);

	// 上向き角度
	float pitchRad =
		MyMath::Deg2Rad(30.0f);

	VECTOR dir;

	// XZ平面の向き
	dir.x =
		sinf(yawRad) *
		cosf(pitchRad);

	dir.y =
		sinf(pitchRad);

	dir.z =
		cosf(yawRad) *
		cosf(pitchRad);

	setting.direction =
		VNorm(dir);

	setting.maxDistance = 1000.0f;

	setting.targetLayers.push_back(
		ColliderLayer::Wall);

	setting.ignoreLayers.push_back(
		ColliderLayer::Player);

	setting.ignoreLayers.push_back(
		ColliderLayer::PlayerArm);

	RayCast::Cast(
		setting,
		rayHit);

	DrawLine3D(
	setting.origin,
	VAdd(
		setting.origin,
		VScale(
			setting.direction,
			setting.maxDistance)),
	GetColor(255, 255, 0));

#if _DEBUG

	ImGui::Begin("Ray Debug");

	ImGui::Text(
		"Hit : %s",
		rayHit.hit ? "True" : "False");

	ImGui::Text(
	"Origin : %.2f %.2f %.2f",
	setting.origin.x,
	setting.origin.y,
	setting.origin.z);

	ImGui::Text(
		"Direction : %.2f %.2f %.2f",
		dir.x,
		dir.y,
		dir.z);

	ImGui::Text(
		"Distance : %.2f",
		rayHit.distance);

	ImGui::Text(
		"Point : %.2f %.2f %.2f",
		rayHit.point.x,
		rayHit.point.y,
		rayHit.point.z);

	if (rayHit.collider) {
		ImGui::Text(
			"Type : %s",
			rayHit.collider->GetTypeName());

		ImGui::Text(
			"Layer : %d",
			(int)rayHit.collider->GetLayer());
	}

	ImGui::End();

 */