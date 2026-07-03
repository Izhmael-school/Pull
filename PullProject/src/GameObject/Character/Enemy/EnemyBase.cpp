#include "EnemyBase.h"
#include "Definition/Enum/EnemyState.h"
#include "Definition/Const/EnemyConst.h"
#include "Definition/CommonModule/MyMath.h"
#include "Definition/Const/ColorConst.h"
#include "Manager/TimeManager.h"
#include "Manager/InputManager.h"
#include "Manager/CollisionManager.h"
#include "Manager/EffectManager.h"
#include "Component/Collider/Collider.h"

EnemyBase::EnemyBase(int _modelHandle, VECTOR _pos)
	:Character(_modelHandle, _pos, Enemy)
	, prevState(NoneAction)
	, currentState(NoneAction)
	, nextState(NoneAction)
	, isAttacking(false)
	, point()
	, rayAnswer(false)
	, spawnPoint()
	, standbyTime(1.0f)
	, wanderingGoalPos(VGet(static_cast<float>(INT_MAX), 0, 0))
	, tracingTargetPos(VZero)
	, type()
	, wantUnuse(false)
	, endAttack(true)
	, canAttack(true)
	, standbyElapsedTime(0.0f) 
	, thrownDir(VZero)
{
}

EnemyBase::~EnemyBase() {}

void EnemyBase::Start() {
	Character::Start();

	isGravity = false;

	// モデルの最小点と最大点を取得
	VECTOR maxBodyPos = VScale(MV1GetMeshMaxPosition(modelHandle, 0), 100);
	VECTOR minBodyPos = VScale(MV1GetMeshMinPosition(modelHandle, 0), 100);
	VECTOR maxLeg1 = VScale(MV1GetMeshMaxPosition(modelHandle, 1), 100);
	VECTOR minLeg1 = VScale(MV1GetMeshMinPosition(modelHandle, 1), 100);
	VECTOR maxLeg2 = VScale(MV1GetMeshMaxPosition(modelHandle, 2), 100);
	VECTOR minLeg2 = VScale(MV1GetMeshMinPosition(modelHandle, 2), 100);

	VECTOR min;
	min.x = std::min(minBodyPos.x, std::min(minLeg1.x, minLeg2.x));
	min.y = std::min(minBodyPos.y, std::min(minLeg1.y, minLeg2.y));
	min.z = std::min(minBodyPos.z, std::min(minLeg1.z, minLeg2.z));

	VECTOR max;
	max.x = std::max(maxBodyPos.x, std::max(maxLeg1.x, maxLeg2.x));
	max.y = std::max(maxBodyPos.y, std::max(maxLeg1.y, maxLeg2.y));
	max.z = std::max(maxBodyPos.z, std::max(maxLeg1.z, maxLeg2.z));

	// 当たり判定
	pCollider = std::make_unique<AABBCollider>(this, min, max);
	pCollider->SetResolve(false);
}

void EnemyBase::Update() {
	Character::Update();

	// ステートの変更
	prevState = currentState;
	currentState = nextState;
	ChangeNextState(Wandering);

	switch (currentState) {
	case NoneAction:
		Wait();
		break;
	case Wandering:
		WanderingAction();
		break;
	case Tracing:
		TracingAction();
		break;
	case Attack:
		AttackAction();
		break;
	case OutofControl:
		CaughtUpdate();
		break;
	case Die:
		Dead();
		break;
	default:
		break;
	}

	// 見失ったら何もしない
	if (prevState == Tracing && currentState == Wandering)
		ChangeNextState(NoneAction);

}

void EnemyBase::Render() {
	Character::Render();

#if _DEBUG
	DrawVisionFanDebug();
	DrawCone3D(spawnPoint, VAdd(spawnPoint, VUp), WANDERING_RADIUS, 16, green, green, false);
	DrawSphere3D(wanderingGoalPos, 10, 16, red, red, true);
#endif
}

void EnemyBase::Setup() {
	spawnPoint = GetPosition();
	wanderingGoalPos = VGet(static_cast<float>(INT_MAX), 0, 0);

	if (pAnimator) {
		// 攻撃終了時処理を持たせる
		SetAnimEvent("Attack", pAnimator->GetTotalTime("Attack"), [this]() {EndAttack();});
		SetAnimEvent("Die", pAnimator->GetTotalTime("Die"), [this]() {Death();});
	}

	// 再度更新できるように
	if (pCollider)
		pCollider->SetEnable(true);

	wantUnuse = false;
}

void EnemyBase::Cleanup() {
	// イベントを全部消す
	if (pAnimator)
		pAnimator->ResetEvents();
	// 当たり判定の更新をしないように
	if (pCollider)
		pCollider->SetEnable(false);
}

void EnemyBase::Move(VECTOR targetPos) {
	VECTOR dir = VSub(targetPos, GetPosition());
	// 上方向は使わない
	dir.y = 0.0f;
	VECTOR nDir = VNorm(dir);
	// 時間 
	float d = TimeManager::GetInstance().GetDeltaTime();
	// 移動倍率
	float move = moveSpeed * d;
	VECTOR pos = VScale(nDir, move);

	// y軸は移動しないように
	pos.y = 0.0f;
	// 移動
	GetTransform()->AddPosition(pos);
	// ゴールを向く
	GetTransform()->LookAtY(targetPos);
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

	moveSpeed = 200;
	// 目的地に向かう
	Move(wanderingGoalPos);

	ChangeNextState(Wandering);

	// ゴール判定
	VECTOR pos = GetPosition();
	if (wanderingGoalPos.x - GOAL_JODGMENT > pos.x || wanderingGoalPos.x + GOAL_JODGMENT < pos.x) return;
	if (wanderingGoalPos.z - GOAL_JODGMENT > pos.z || wanderingGoalPos.z + GOAL_JODGMENT < pos.z) return;

	// 待機にする
	wanderingGoalPos.x = static_cast<float>(INT_MAX);
	ChangeNextState(NoneAction);
}

void EnemyBase::TracingAction() {
	if (!rayAnswer) return;

	moveSpeed = 300;
	// 追跡
	Move(tracingTargetPos);
}

void EnemyBase::AttackAction() {
	if (!isAttacking) {
		// 攻撃中にする
		isAttacking = true;
		canAttack = false;
		// アニメーション再生
		pAnimator->Play("Attack");

	}

	if (isAttacking)
		ChangeNextState(Attack);
}

void EnemyBase::Wait() {
	// 前の行動が待機では無ければ時間を初期化
	if (prevState != NoneAction) {
		standbyElapsedTime = 0.0f;
		pAnimator->Play("Idle");
	}

	if (standbyElapsedTime >= standbyTime) {
		standbyElapsedTime = 0.0f;

		canAttack = true;
	}
	else {
		// 時間の加算
		standbyElapsedTime += TimeManager::GetInstance().GetDeltaTime();
		ChangeNextState(NoneAction);
	}
}

void EnemyBase::Dead() {
	pAnimator->Play("Die");

	ChangeNextState(Die);
}

bool EnemyBase::VisionFan(VECTOR target) {
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

	tracingTargetPos = target;
	// レイに入っていて攻撃中じゃない時に追跡行動に移る
	if (rayAnswer && nextState != Attack)
		ChangeNextState(Tracing);

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

void EnemyBase::Death() {
	wantUnuse = true;
	effectEvent("EnemyDeath", GetPosition(), 50.0f, VZero);
}

void EnemyBase::EndAttack() {
	isAttacking = false;
	ChangeNextState(NoneAction);
}

void EnemyBase::ChangeNextState(EnemyActionState _state) {
	// 死亡ならそのまま変える
	if (_state == Die) {
		nextState = _state;
		return;
	}
	// 今か次の行動がDieなら変えない
	if (nextState == Die || currentState == Die) return;

	// 次の行動が行動不能なら変えない
	if (nextState == OutofControl) return;
	nextState = _state;
}

void EnemyBase::LoopAnim(std::string _animName) {
	GetAnimator()->GetAnimation(_animName)->isLoop = true;
}

void EnemyBase::SetEvent(EffectEvent _effectEvent, AudioEvent _audioEvent, MissileEvent _createEvent, SphereEvent _sphereEvent, AABBEvent _aabbEvent) {
	effectEvent = _effectEvent;
	audioEvent = _audioEvent;
	createEvent = _createEvent;
	sphereEvent = _sphereEvent;
	aabbEvent = _aabbEvent;
}

void EnemyBase::SetAnimEvent(std::string _animName, int _frameCount, std::function<void()> _animEvent) {
	auto anim = pAnimator->GetAnimation(_animName);
	if (anim == nullptr) return;
	if (_frameCount < 0) _frameCount = pAnimator->GetTotalTime(_animName);
	anim->SetEvent(_animEvent, _frameCount);
}

void EnemyBase::OnTriggerEnter(Collider* _pSelf, Collider* _pOther) {
	if (GetCurrentCaughtState() != CaughtState::Throwing) return;
	// 何かしらに当たったら
	HitObject();
}

void EnemyBase::OnTriggerStay(Collider* _pSelf, Collider* _pOther) {
	Tag tag = _pOther->GetGameObject()->GetTag();
	if (tag == Player) {
		if (rayAnswer)
			ChangeNextState(Attack);
	}
}

void EnemyBase::CaughtAction() {
	// 行動不能状態にする
	ChangeNextState(OutofControl);
	// 捕まった状態にする
	ChangeCaughtState(Catch);
}

void EnemyBase::ThrownAction(VECTOR _dir) {
	// 投げられた状態にする
	ChangeCaughtState(Throw);

	// 投げられた方向を保存
	thrownDir = _dir;
}

void EnemyBase::CatchStart() {
	CaughtObject::CatchStart();

	ChangeNextState(OutofControl);
}

void EnemyBase::Catching() {
	pAnimator->Play("Walk", 2.0f);
	ChangeNextState(OutofControl);
}

void EnemyBase::ThrowStart() {
	CaughtObject::ThrowStart();
	ChangeNextState(OutofControl);
}

void EnemyBase::Throwing() {
	GetTransform()->AddRotation(VScale(VUp, 10));
	GetTransform()->AddPosition(VScale(thrownDir, -10));
	ChangeNextState(OutofControl);
}

void EnemyBase::HitObject() {
	ChangeNextState(Die);
}