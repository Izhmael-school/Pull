#include "Character.h"
#include "../../Component/Collider/Collider.h"
#include "ImGui/ImGui.h"
#include "../../Definition/CommonModule/MyMath.h"

Character::Character(int _modelHandle, VECTOR _pos, Tag _tag)
	:GameObject(_modelHandle, _pos, _tag)
	, point()
	, rayAnswer(false)

{
	Start();
}

Character::~Character(){}

void Character::Start() {
	GameObject::Start();

	pAnimator = std::make_unique<Animator>(modelHandle);
}

void Character::Update() {
	GameObject::Update();

	if (pAnimator != nullptr)
		pAnimator->Update();



	if (pGroundingCollider)
		pGroundingCollider->Update();
}

void Character::Render() {
	GameObject::Render();

#if _DEBUG
	/*
	 *	@author Riku
	 */
	if (pGroundingCollider)
		pGroundingCollider->Render();
#endif
}

void Character::Setup() {
	GameObject::Setup();
}

/*
 *	@author Riku
 */
void Character::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {
	if (_pSelf == pGroundingCollider.get() &&
		_pOther->GetLayer() == ColliderLayer::Ground)
		hitGroundingFrag = true;
}

/*
 *	@author Riku
 */
void Character::OnTriggerExit(Collider* _pSelf, Collider* _pOther) {
	if (_pSelf == pGroundingCollider.get() &&
		_pOther->GetLayer() == ColliderLayer::Ground)
		hitGroundingFrag = false;
}

void Character::DrawVisionFanDebug() {
	float halfAngle = MyMath::Deg2Rad(vision.rayAngle / 2.0f);
	float dirRad = MyMath::Deg2Rad(GetRotation().y);

	// 中心方向ベクトル
	VECTOR dir = VGet(sinf(dirRad), 0, cosf(dirRad));

	// 左右の端方向ベクトル
	VECTOR leftDir = VGet(sinf(dirRad - halfAngle), 0, cosf(dirRad - halfAngle));
	VECTOR rightDir = VGet(sinf(dirRad + halfAngle), 0, cosf(dirRad + halfAngle));

	// 先端座標
	VECTOR centerEnd = VAdd(GetPosition(), VScale(dir, vision.rayLenght));
	VECTOR leftEnd = VAdd(GetPosition(), VScale(leftDir, vision.rayLenght));
	VECTOR rightEnd = VAdd(GetPosition(), VScale(rightDir, vision.rayLenght));

	// 色
	unsigned int col = GetColor(255, 255, 0); // 黄色

	// 中心線
	DrawLine3D(GetPosition(), centerEnd, col);

	// 左右の扇端
	DrawLine3D(GetPosition(), leftEnd, col);
	DrawLine3D(GetPosition(), rightEnd, col);

	// 扇の外周（円弧）を描画
	const int div = 20; // 円弧の分割数
	for (int i = 0; i < div; i++) {
		float a1 = (float)i / div;
		float a2 = (float)(i + 1) / div;

		float rad01 = dirRad - halfAngle + vision.rayAngle * a1 * DX_PI_F / 180.0f;
		float rad02 = dirRad - halfAngle + vision.rayAngle * a2 * DX_PI_F / 180.0f;

		VECTOR p1 = VAdd(GetPosition(), VScale(VGet(sinf(rad01), 0, cosf(rad01)), vision.rayLenght));
		VECTOR p2 = VAdd(GetPosition(), VScale(VGet(sinf(rad02), 0, cosf(rad02)), vision.rayLenght));

		DrawLine3D(p1, p2, col);
	}
}

bool Character::VisionFan(VECTOR target) {
	point.position = target;

	Fan fan = {};
	fan.position = GetPosition();
	fan.directionDegree = GetRotation().y;
	fan.length = vision.rayLenght;
	fan.rangeDegree = vision.rayAngle;

	// 点と扇のベクトル
	VECTOR vecFanToPoint = {
		point.position.x - fan.position.x,
		0,
		point.position.z - fan.position.z,
	};

	// ベクトルの長さを算出
	float vecLength = sqrtf(powf(vecFanToPoint.x, 2.0f) + powf(vecFanToPoint.z, 2.0f));

	// ほぼほぼ重なっていたら当たってる判定にする
	if (vecLength <= 0.0001f)
		return rayAnswer = true;

	// ベクトルと扇の長さの比較
	if (fan.length < vecLength) return rayAnswer = false; // 当たってない

	// 扇を２等分する線のベクトルを求める
	float dirRad = MyMath::Deg2Rad(fan.directionDegree);
	VECTOR fanDir = VGet(sinf(dirRad), 0, cosf(dirRad));

	// 扇と点のベクトルを単位ベクトルにする
	VECTOR normalFanToPoint = {
		vecFanToPoint.x / vecLength,
		0,
		vecFanToPoint.z / vecLength
	};

	// 内積計算
	float dot = normalFanToPoint.x * fanDir.x + normalFanToPoint.z * fanDir.z;

	// 扇の範囲をcosにする
	float fanCos = cosf(MyMath::Deg2Rad(fan.rangeDegree / 2));

	// 点が扇の範囲内にあるか比較
	if (fanCos > dot)
		return rayAnswer = false; // 当たってない



	return rayAnswer = true;
}
