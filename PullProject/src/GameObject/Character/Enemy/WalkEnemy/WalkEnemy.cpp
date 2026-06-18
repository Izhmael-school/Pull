#include "WalkEnemy.h"
#include "DxLib.h"
#include "Definition/Const/EnemyConst.h"
#include "Definition/CommonModule/MyString.h"
#include "Component/Collider/Collider.h"
#include "Manager/ColliderObjectManager.h"

WalkEnemy::WalkEnemy(int _modelHandle, VECTOR _pos)
	:EnemyBase(_modelHandle,_pos)
{
	Start();
}

WalkEnemy::~WalkEnemy(){
}

void WalkEnemy::Update() {
	EnemyBase::Update();

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
		break;
	default:
		break;
	}
}

void WalkEnemy::Setup(){
	EnemyBase::Setup();
}

bool WalkEnemy::VisionFan(VECTOR target){
	EnemyBase::VisionFan(target);
	// レイに入っていて攻撃中じゃない時に追跡行動に移る
	if (rayAnswer && !isAttacking)
		nextState = Tracing;

	return rayAnswer;
}

void WalkEnemy::AttackAction(){
	if (!isAttacking) {
		ColliderObjectManager::GetInstance().CreateSphere(GetPosition(), 200, Enemy);
	}

	EnemyBase::AttackAction();
}

void WalkEnemy::Start(){
	// モデルの正面が反対だから180度追加
	MV1SetRotationXYZ(modelHandle, VScale(VUp, 180));

	// ラディウスの計算
	VECTOR size = VSub(MV1GetMeshMaxPosition(modelHandle,0), MV1GetMeshMinPosition(modelHandle, 0));
	float r = (VSize(size) * 100) / 2;

	// 当たり判定
	pCollider = std::make_unique<SphereCollider>(this,VZero,r);

	EnemyBase::Start();
}
