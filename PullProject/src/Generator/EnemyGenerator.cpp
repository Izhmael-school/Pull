#include "EnemyGenerator.h"
#include "GameObject/Character/Enemy/EnemyBase.h"
#include "GameObject/Character/Enemy/WalkEnemy/WalkEnemy.h"
#include "GameObject/Character/Enemy/BomberEnemy/BomberEnemy.h"
#include "Definition/Const/VECTORConst.h"
#include "Definition/Const/EnemyConst.h"
#include "Definition/CommonModule/MyString.h"
#include "Definition/CommonModule/MyJson.h"
#include <string>
#include <cassert>

EnemyGenerator::EnemyGenerator()
	:originEnemyModelHandle()
	,originEnemyAnimHandle()
{
	LoadModel();
	SetCreateEvent();
}

EnemyGenerator::~EnemyGenerator() {}

void EnemyGenerator::LoadModel() {
	originEnemyModelHandle.resize(EnemyType::MaxEnemyType);
	originEnemyAnimHandle.resize(EnemyType::MaxEnemyType);
	auto data = MyJson::LoadJsonFile("src/Data/EnemyData.json");

	// jsonが無ければ帰る
	if (data == nullptr) {
		assert(false && "Nothing EnemyData.json");
		return;
	}

	int index = 0;
	for (auto d : data) {
		// インデックスと同じIDでなければ次
		if (index != d["enemyTypeID"]) {
			index++;
			continue;
		}
		// モデルのロード
		std::string filePath = MyString::MergeString(ENEMY_MODEL_FILEPATH, d["modelFileName"], ENEMY_MODEL_EXTENSIION);
		originEnemyModelHandle[index] = MV1LoadModel(filePath.c_str());
		// アニメーションのロード
		filePath = MyString::MergeString(ENEMY_MODEL_FILEPATH, d["animationFileName"], ENEMY_MODEL_EXTENSIION);
		originEnemyAnimHandle[index] = MV1LoadModel(filePath.c_str());
		index++;
	}
}

void EnemyGenerator::SetCreateEvent(){
	CreateEnemyEvent.resize(MaxEnemyType);
	//　ラムダで包む
	CreateEnemyEvent[Walker] = [this](int _modelHandle, VECTOR _pos) {return CreateWalker(_modelHandle, _pos);};
	CreateEnemyEvent[Bomber] = [this](int _modelHandle, VECTOR _pos) {return CreateBomber(_modelHandle, _pos);};
}

std::unique_ptr<EnemyBase> EnemyGenerator::CreateEnemy(EnemyType _type, VECTOR _pos){
	int typeID = static_cast<int>(_type);
	int modelHandle = originEnemyModelHandle[typeID];
	// モデルが無ければ帰る
	if (modelHandle == -1) {
#if _DEBUG
		std::string errorText = MyString::MergeString(
			"EnemyTypeID:",
			std::to_string(typeID),
			"Model Nothing");
		assert(false && errorText.c_str());
#endif // _DEBUG
		return nullptr; 
	}
	// モデルの複製
	int dupHandle = MV1DuplicateModel(modelHandle);

	std::unique_ptr<EnemyBase> enemy = std::move(CreateEnemyEvent[typeID](dupHandle,_pos));
	// アニメーションの複製
	int animHandle = MV1DuplicateModel(originEnemyAnimHandle[typeID]);
	// アニメーションのロード
	enemy->GetAnimator()->Load(animHandle, false);
	enemy->LoopAnim("Walk");
	return std::move(enemy);
}

std::unique_ptr<EnemyBase> EnemyGenerator::CreateWalker(int _modelHandle,VECTOR _pos){
	return std::make_unique<WalkEnemy>(_modelHandle, _pos);
}

std::unique_ptr<EnemyBase> EnemyGenerator::CreateBomber(int _modelHandle, VECTOR _pos){
	return std::make_unique<BomberEnemy>(_modelHandle, _pos);
}
