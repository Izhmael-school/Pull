#include "EnemyGenerator.h"
#include "../GameObject/Character/Enemy/EnemyBase.h"
#include "../GameObject/Character/Enemy/WalkEnemy/WalkEnemy.h"
#include "../Definition/Const/VECTORConst.h"
#include "../Definition/Const/EnemyConst.h"
#include "../Definition/CommonModule/MyString.h"

int EnemyGenerator::originWalkEnemyModel = -1;

void EnemyGenerator::LoadModel(){
	originWalkEnemyModel = MV1LoadModel(MyString::MergeString(ENEMY_MODEL_FILEPATH,"WalkEnemy.mv1").c_str());
}

std::unique_ptr<EnemyBase> EnemyGenerator::CreateWalkEnemy(){
	if (originWalkEnemyModel == -1) 
		LoadModel();

	// モデルの複製
	int modelHandle = MV1DuplicateModel(originWalkEnemyModel);

	if (modelHandle == -1) return nullptr;

	std::unique_ptr<WalkEnemy> walkEnemy = std::make_unique<WalkEnemy>(modelHandle, VZero);
	return std::move(walkEnemy);
}
