#include "ShooterEnemy.h"
#include "GameObject/Missile/Missile.h"
#include "Manager/GameObjectManager.h"

ShooterEnemy::ShooterEnemy(int _modelHandle, VECTOR _pos) 
	:EnemyBase(_modelHandle, _pos)
{
	Start();
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
	SetAnimEvent("Attack",13,[this]() {createEvent("Missile",GetTransform()->GetForward(), GetPosition());});
}

void ShooterEnemy::WanderingAction() {

}

void ShooterEnemy::TracingAction() {
	ChangeNextState(Attack);
}
