#include "EnemyManager.h"
#include "GameObject/Character/Enemy/EnemyBase.h"

#include "Manager/PlayerManager.h"

EnemyManager::EnemyManager()
{
	Start();
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Start() {
	unuseEnemyArray.resize(EnemyType::MaxEnemyType);
}

void EnemyManager::Update() {
	// 更新

	// プレイヤーの座標をもらう
	auto player = PlayerManager::GetInstance().GetPlayer();
	VECTOR playerPos = player != nullptr ? player->GetPosition() : VGet(INT_MAX, INT_MAX, INT_MAX);
	for (auto& enemy : useEnemyArray) {
		enemy->VisionFan(playerPos);
		enemy->Update();
	}

	// 配列からの除外
	std::erase_if(useEnemyArray, [this](std::unique_ptr<EnemyBase>& enemy) {
		// 未使用化を希望してなければ次
		if (!enemy->IsWantUnuse()) return false;
		// 未使用配列に移す
		UnuseEnemy(std::move(enemy));
		return true;
		});
}

void EnemyManager::Render() {
	// 更新
	for (auto& enemy : useEnemyArray) {
		enemy->Render();
	}
}

void EnemyManager::UseEnemy(EnemyType _type, VECTOR _pos) {
	std::unique_ptr<EnemyBase> enemy;
	// 未使用状態がいなければつくる
	if (unuseEnemyArray[_type].size() == 0)
		unuseEnemyArray[_type].push_back(generator.CreateEnemy(_type, _pos));
	// 末尾から取得
	enemy = std::move(unuseEnemyArray[_type].back());
	// 末尾削除
	unuseEnemyArray[_type].pop_back();
	// 使用準備
	enemy->Setup();
	useEnemyArray.push_back(std::move(enemy));
}

void EnemyManager::UnuseAllEnemy(){
	for (auto& enemy : useEnemyArray) {
		enemy->Cleanup();
		// 未使用配列に入れる
		EnemyType type = enemy->GetType();
		unuseEnemyArray[type].push_back(std::move(enemy));
	}

	useEnemyArray.clear();
}

void EnemyManager::UnuseEnemy(EnemyPtr _unuseEnemy) {
	auto itr = std::ranges::find(useEnemyArray, _unuseEnemy);
	// 使用中配列になければ帰る
	if (itr == useEnemyArray.end()) return;
	// 使用中配列から削除
	useEnemyArray.erase(itr);
	// 後処理
	_unuseEnemy->Cleanup();
	// 未使用配列に入れる
	EnemyType type = _unuseEnemy->GetType();
	unuseEnemyArray[type].push_back(std::move(_unuseEnemy));
}