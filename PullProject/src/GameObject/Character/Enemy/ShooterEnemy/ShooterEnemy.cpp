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

	addScore = 1000;
	vision.rayLenght = 3000;
}

void ShooterEnemy::Setup() {
	EnemyBase::Setup();
	// アニメーションに合わせてミサイルを出させる
	int frameCount = MV1SearchFrame(modelHandle, "ShotPoint");
	SetAnimEvent("Attack",13,[this,frameCount]() {createEvent("Missile",this,GetTransform()->GetForward(), MV1GetFramePosition(modelHandle, frameCount));});
}

void ShooterEnemy::WanderingAction() {

}

void ShooterEnemy::TracingAction() {
	ChangeNextState(Attack);
	GetTransform()->LookAtY(tracingTargetPos);
}
