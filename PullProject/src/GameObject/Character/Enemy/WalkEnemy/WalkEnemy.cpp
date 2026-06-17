#include "WalkEnemy.h"
#include "DxLib.h"
#include "Definition/Const/EnemyConst.h"
#include "Definition/CommonModule/MyString.h"

WalkEnemy::WalkEnemy(int _modelHandle, VECTOR _pos)
	:EnemyBase(_modelHandle,_pos)
{
	Start();
}

WalkEnemy::~WalkEnemy(){
}

void WalkEnemy::Update() {
	EnemyBase::Update();

	// ステートの変更
	currentState = nextState;
	nextState = Wandering;

	switch (currentState) {
	case NoneAction:
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

void WalkEnemy::Start(){
	// モデルの正面が反対だから180度追加
	MV1SetRotationXYZ(modelHandle, VScale(VUp, 180));

	EnemyBase::Start();
}
