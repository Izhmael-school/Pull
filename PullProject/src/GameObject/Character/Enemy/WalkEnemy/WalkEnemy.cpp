#include "WalkEnemy.h"
#include "DxLib.h"
#include "../../../../Definition/Const/EnemyConst.h"
#include "../../../../Definition/CommonModule/MyString.h"

int WalkEnemy::originModelHandle = -1;

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
	// 大本のモデルが無ければ読み込む
	if (originModelHandle == -1)
		originModelHandle = MV1LoadModel(MyString::MergeString(ENEMY_MODEL_FILEPATH, "WalkEnemy.mv1").c_str());

	modelHandle = MV1DuplicateModel(originModelHandle);

	// モデルが小さいから大きくする
	GetTransform()->SetScale(50);

	EnemyBase::Start();
}
