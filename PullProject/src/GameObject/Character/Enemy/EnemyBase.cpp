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
#include <vector>

EnemyBase::EnemyBase(int _modelHandle, VECTOR _pos)
	:Character(_modelHandle, _pos, Enemy)
	, prevState(NoneAction)
	, currentState(NoneAction)
	, nextState(NoneAction)
	, isAttacking(false)
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
	, footPos(0.0f) {
}

EnemyBase::~EnemyBase() {}

void EnemyBase::Start() {
	Character::Start();

	isGravity = true;

	int num = MV1GetMeshNum(modelHandle);

	std::vector<VECTOR> mins;
	std::vector<VECTOR> maxs;
	for (int i = 0;i < num;i++) {
		// 各メッシュの最大点と最小点を保持
		maxs.push_back(VScale(MV1GetMeshMaxPosition(modelHandle, i), 50));
		mins.push_back(VScale(MV1GetMeshMinPosition(modelHandle, i), 50));
	}

	VECTOR min = VGet(INT_MAX, INT_MAX, INT_MAX);
	VECTOR max = VGet(-INT_MAX, -INT_MAX, -INT_MAX);

	for (int i = 0;i < num;i++) {
		// 最小点配列の中のさらに最小を保持
		min.x = std::min(min.x, mins[i].x);
		min.y = std::min(min.y, mins[i].y);
		min.z = std::min(min.z, mins[i].z);
		// 最大点配列の中のさらに最大を保持
		max.x = std::max(max.x, maxs[i].x);
		max.y = std::max(max.y, maxs[i].y);
		max.z = std::max(max.z, maxs[i].z);
	}

	// 地面に埋まらないように現在の座標を変える
// footPoint取得
	int footFrame = MV1SearchFrame(modelHandle, "FootPoint");
	// footPointとスポーン位置の差だけ上昇させる
	VECTOR footPos = MV1GetFramePosition(modelHandle, footFrame);
	VECTOR enemyPos = GetPosition();
	float up = (enemyPos.y - footPos.y);
	// 上げる
	GetTransform()->SetPosition(VGet(enemyPos.x, up, enemyPos.z));

	// 当たり判定
	pCollider = std::make_unique<AABBCollider>(this, min, max);
	pCollider->SetResolve(true);

	pCollider->SetLayer(ColliderLayer::Enemy);

	// 接地判定用当たり判定
	pGroundingCollider = std::make_unique<SphereCollider>(this, VScale(VUp, footPos.y), 10);
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

	if (pGroundingCollider)
		pGroundingCollider->SetEnable(true);

	// 足元まで当たり判定を伸ばす
	auto aabb = dynamic_cast<AABBCollider*>(pCollider.get());
	if (aabb) {
		VECTOR min = aabb->GetMin();
		aabb->SetMin(VGet(min.x, min.y + footPos, min.z));
	}


	wantUnuse = false;
}

void EnemyBase::Cleanup() {
	// イベントを全部消す
	if (pAnimator)
		pAnimator->ResetEvents();
	// 当たり判定の更新をしないように
	if (pCollider)
		pCollider->SetEnable(false);
	if (pGroundingCollider)
		pGroundingCollider->SetEnable(false);
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
	Character::OnTriggerEnter(_pSelf, _pOther);

	if (_pOther->GetLayer() == ColliderLayer::Wall)
		wanderingGoalPos = GetPosition();

	if (GetCurrentCaughtState() == CaughtState::Throwing) {
		// 何かしらに当たったら
		HitObject();
	}
}

void EnemyBase::OnTriggerStay(Collider* _pSelf, Collider* _pOther) {
	Character::OnTriggerStay(_pSelf, _pOther);
	Tag tag = _pOther->GetGameObject()->GetTag();
	if (tag == Player) {
		if (rayAnswer)
			ChangeNextState(Attack);
	}
}

void EnemyBase::OnTriggerExit(Collider* _pSelf, Collider* _pOther) {
	Character::OnTriggerExit(_pSelf, _pOther);


}

void EnemyBase::CaughtAction(VECTOR _rot, VECTOR _pos) {
	// 行動不能状態にする
	ChangeNextState(OutofControl);
	// 捕まった状態にする
	ChangeCaughtState(Catch);

	// プレイヤーの手の子になるため、座標と回転を手に合わせる
	GetTransform()->SetPosition(_pos);
	GetTransform()->SetRotation(_rot);
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
	AddFallSpeed(-20);
	hitGroundingFrag = false;
}

void EnemyBase::Throwing() {
	GetTransform()->AddRotation(VScale(VUp, 10));
	GetTransform()->AddPosition(VScale(thrownDir, -10));
	ChangeNextState(OutofControl);
}

void EnemyBase::HitObject() {
	ChangeNextState(Die);
}