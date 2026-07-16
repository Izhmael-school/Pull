#include "WalkEnemy.h"
#include "DxLib.h"
#include "Definition/Const/EnemyConst.h"
#include "Definition/CommonModule/MyString.h"
#include "Component/Collider/Collider.h"

WalkEnemy::WalkEnemy(int _modelHandle, VECTOR _pos)
	:EnemyBase(_modelHandle,_pos)
{
	Start();
}

WalkEnemy::~WalkEnemy(){
}

void WalkEnemy::Setup(){
	EnemyBase::Setup();

	// アニメーションに合わせて攻撃する
	SetAnimEvent("Attack", 22, [this]() {
		VECTOR pos = VAdd(GetPosition(), VScale(GetTransform()->GetForward(), 300));
		sphereEvent(pos, 200.0f,Tag::EnemyAttack,0.1f);
		effectEvent("EnemyAttack", pos,100.0f,GetTransform()->GetForward());
		audioEvent("Tackle", 255.0f, false, GetPosition(), 1000.0f);
		});
}

void WalkEnemy::Start(){
	// モデルの正面が反対だから180度追加
	MV1SetRotationXYZ(modelHandle, VScale(VUp, 180));

	type = Walker;

	addScore = 200;

	EnemyBase::Start();
}
