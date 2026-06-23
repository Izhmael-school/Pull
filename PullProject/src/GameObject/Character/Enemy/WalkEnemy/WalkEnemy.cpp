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

void WalkEnemy::Setup(){
	EnemyBase::Setup();

	// アニメーションに合わせて攻撃する
	auto anim = pAnimator->GetAnimation("Attack");
	anim->SetEvent([this]() {ColliderObjectManager::GetInstance().CreateSphere(VAdd(GetPosition(), VScale(GetTransform()->GetForward(), 100)),200);}, 22);
}

void WalkEnemy::Start(){
	// モデルの正面が反対だから180度追加
	MV1SetRotationXYZ(modelHandle, VScale(VUp, 180));

	type = Walker;

	EnemyBase::Start();
}
