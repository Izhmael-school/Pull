#include "DebugCamera.h"
#include "../Manager/InputManager.h"
#include "../Definition/Const/VECTORConst.h"
#include "../Definition/Const/ColorConst.h"
#include "../Definition/CommonModule/MyMath.h"
#include <DxLib.h>

/*
 * @file DebugCamera.cpp
 * @author Sekino
 */

void DebugCamera::Update()
{
	transform.Update();

	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_LSHIFT))
		moveStop = !moveStop;

	if (!moveStop) {

		if (InputManager::GetInstance().IsKey(KEY_INPUT_UP)) transform.AddRotation(VLeft, 2);
		if (InputManager::GetInstance().IsKey(KEY_INPUT_DOWN)) transform.AddRotation(VRight, 2);
		if (InputManager::GetInstance().IsKey(KEY_INPUT_RIGHT)) transform.AddRotation(VUp, 2);
		if (InputManager::GetInstance().IsKey(KEY_INPUT_LEFT)) transform.AddRotation(VDown, 2);


		float speed = 10.0f;

		if (InputManager::GetInstance().IsKey(KEY_INPUT_W))
			transform.AddPosition(transform.GetForward(), speed);

		if (InputManager::GetInstance().IsKey(KEY_INPUT_S))
			transform.AddPosition(transform.GetForward(), -speed);

		if (InputManager::GetInstance().IsKey(KEY_INPUT_D))
			transform.AddPosition(transform.GetRight(), speed);

		if (InputManager::GetInstance().IsKey(KEY_INPUT_A))
			transform.AddPosition(transform.GetRight(), -speed);

		if (InputManager::GetInstance().IsKey(KEY_INPUT_Q))
			transform.AddPosition(transform.GetUp(), -speed);

		if (InputManager::GetInstance().IsKey(KEY_INPUT_E))
			transform.AddPosition(transform.GetUp(), speed);

		VECTOR pos = transform.GetPosition();
		VECTOR rot = transform.GetLocalRotation();

		SetCameraPositionAndAngle(pos, MyMath::Deg2Rad(rot.x), MyMath::Deg2Rad(rot.y), MyMath::Deg2Rad(rot.z));

		Set3DSoundListenerPosAndFrontPos_UpVecY(pos, VAdd(pos, VForward));
	}
}

void DebugCamera::Render(){

}
