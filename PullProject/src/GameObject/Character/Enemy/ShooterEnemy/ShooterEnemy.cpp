#include "ShooterEnemy.h"
#include "GameObject/Missile/Missile.h"
#include "Manager/GameObjectManager.h"

ShooterEnemy::ShooterEnemy(int _modelHandle, VECTOR _pos) 
	:EnemyBase(_modelHandle,_pos)
{
}

ShooterEnemy::~ShooterEnemy() {
}

void ShooterEnemy::Start() {
	EnemyBase::Start();
	// モデルの正面が反対だから180度追加
	MV1SetRotationXYZ(modelHandle, VScale(VUp, 180));
	type = Shooter;
}

void ShooterEnemy::Setup() {
	EnemyBase::Setup();
	// アニメーションに合わせてミサイルを出させる
	auto anim = pAnimator->GetAnimation("Attack");
	anim->SetEvent([this]() {GameObjectManager::GetInstance().CreateGameObject<Missile>("Missile", GetTransform()->GetForward(), GetPosition());}, 13);
}

void ShooterEnemy::WanderingAction() {

}

void ShooterEnemy::TracingAction() {
	ChangeNextState(Attack);
}
