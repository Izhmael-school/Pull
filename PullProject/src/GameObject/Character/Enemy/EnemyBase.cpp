#include "EnemyBase.h"
#include "../../../Definition/Enum/EnemyState.h"
#include "../../../Definition/Const/EnemyConst.h"
#include "../../../Definition/CommonModule/MyMath.h"
#include "../../../Definition/Const/ColorConst.h"

EnemyBase::EnemyBase(int _modelHandle, VECTOR _pos)
	:Character(_modelHandle,_pos,None)
	,currentState(NoneAction)
	,nextState(NoneAction)
	,isAttacking(false)
	,point()
	,rayAnswer(false)
	,spawnPoint()
	,standbyTime(0.0f)
	,wanderingGoalPos(VGet(static_cast<float>(INT_MAX), 0, 0))
{}

EnemyBase::~EnemyBase(){}

void EnemyBase::Start() {
	Character::Start();
	std::string file = "res/Model/Enemy/WalkEnemyAnim.mv1";
	pAnimator->Load(file, false, false);
	pAnimator->GetAnimation("Walk")->isLoop = true;
}

void EnemyBase::Update(){
	Character::Update();

	// レイの更新
	VisionFan(VZero);
}

void EnemyBase::Render()
{
	Character::Render();

	DrawCone3D(spawnPoint, VAdd(spawnPoint,VUp), WANDERING_RADIUS, 16, green, green, false);
	DrawSphere3D(wanderingGoalPos, 10, 16, red, red, true);
}

void EnemyBase::Setup() {
	spawnPoint = GetPosition();
	wanderingGoalPos = VGet(static_cast<float>(INT_MAX), 0, 0);
}

void EnemyBase::Move(VECTOR targetPos) {
	VECTOR dir = VSub(targetPos, GetPosition());
	VECTOR nDir = VNorm(dir);
	// 時間 今は決め打ち
	float d = 0.001f;
	// 移動倍率
	float move = moveSpeed * d;
	VECTOR pos = VScale(nDir, move);
	// y軸は移動しないように
	pos.y = 0.0f;
	// 移動
	GetTransform()->AddPosition(pos);
	// ゴールを向く
	GetTransform()->LookAtY(wanderingGoalPos);
	// アニメーションの再生
	pAnimator->Play("Walk");
}

void EnemyBase::WanderingAction() {
	// ゴールを設定する
	if (wanderingGoalPos.x == static_cast<float>(INT_MAX)) {
		int x = static_cast<int>(spawnPoint.x);
		int z = static_cast<int>(spawnPoint.z);
		wanderingGoalPos.x = MyMath::RandomF(x - WANDERING_RADIUS, x + WANDERING_RADIUS);
		wanderingGoalPos.z = MyMath::RandomF(z - WANDERING_RADIUS, z + WANDERING_RADIUS);
	}

	// 目的地に向かう
	Move(wanderingGoalPos);

	// ゴール判定
	VECTOR pos = GetPosition();
	if (wanderingGoalPos.x - GOAL_JODGMENT > pos.x || wanderingGoalPos.x + GOAL_JODGMENT < pos.x) return;
	if (wanderingGoalPos.z - GOAL_JODGMENT > pos.z || wanderingGoalPos.z + GOAL_JODGMENT < pos.z) return;

	// 待機にする
	wanderingGoalPos.x = static_cast<float>(INT_MAX);
	nextState = EnemyActionState::NoneAction;
}

void EnemyBase::TracingAction(){
	if (!rayAnswer) return;
}

void EnemyBase::AttackAction()
{
}

bool EnemyBase::VisionFan(VECTOR target) {
	point.position = target;

	Fan fan;
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

void EnemyBase::DrawVisionFanDebug() {
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