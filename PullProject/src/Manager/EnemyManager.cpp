#include "EnemyManager.h"
#include "GameObject/Character/Enemy/EnemyBase.h"
#include "PlayerManager.h"
#include "Stage/StageManager.h"
#include "ColliderObjectManager.h"
#include "GameObjectManager.h"
#include "EffectManager.h" 
#include "AudioManager.h"
#include "GameObject/Missile/Missile.h"
#include "Definition/CommonModule/MyJson.h"
#include "ImGui/ImGui.h"

EnemyManager::EnemyManager(EnemyNeedManager _need)
	:effectManager(_need.effectManager)
	, audioManager(_need.audioManager)
	, gameObjectManager(GameObjectManager::GetInstance())
	, colliderObjectManager(ColliderObjectManager::GetInstance()) {
	Start();
}

EnemyManager::~EnemyManager() {
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
		ImGui::Begin("EnemyPos");
		VECTOR p = enemy->GetPosition();
		ImGui::Text("%f,%f,%f", p.x,p.y,p.z);
		ImGui::End();
	}

	// 配列からの除外
	std::erase_if(useEnemyArray, [this](EnemyPtr& enemy) {
		if (!enemy) return false;
		// 未使用化を希望してなければ次
		if (!enemy->IsWantUnuse()) return false;
		// 未使用配列に移す
		UnuseEnemy(enemy);
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
	// イベントをセット
	enemy->SetEvent(std::bind(&EnemyManager::PlayAnimEvent_Effect, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
		std::bind(&EnemyManager::PlayAnimEvent_Audio, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5),
		std::bind(&EnemyManager::PlayAnimEvent_Missile, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
		std::bind(&EnemyManager::PlayAnimEvent_Sphere, this, std::placeholders::_1, std::placeholders::_2),
		std::bind(&EnemyManager::PlayAnimEvent_AABB, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	enemy->GetTransform()->SetPosition(_pos);


	// 使用準備
	enemy->Setup();
	useEnemyArray.push_back(std::move(enemy));
}

void EnemyManager::UnuseAllEnemy() {
	for (auto& enemy : useEnemyArray) {
		enemy->Cleanup();
		// 未使用配列に入れる
		EnemyType type = enemy->GetType();
		unuseEnemyArray[type].push_back(std::move(enemy));
	}

	useEnemyArray.clear();
}

void EnemyManager::SpawnStageFramePoint(EnemyType _type, StageManager& _stageManager) {
	// フレームのワールド座標を取得
	std::vector<VECTOR> spawnPositions = _stageManager.GetEnemySpawnPositions();
	int count = static_cast<int>(spawnPositions.size());

	// 今は敵の種類を選べないからWalkerで固定
	for (int i = 0; i < count; i++) {
		UseEnemy(_type, spawnPositions[i]);
	}
}

void EnemyManager::SpawnStageFramePoint(int _stageID, StageManager& _stageManager) {
	// ステージのデータを取得
	std::string filePath = std::format(STAGE_ENEMY_DATA_FILEPATH, _stageID);
	auto data = MyJson::LoadJsonFile(filePath);
	// データが無ければ帰る
	if (data.empty()) return;
	// フレームのワールド座標を取得
	std::vector<VECTOR> spawnPositions = _stageManager.GetEnemySpawnPositions();
	int count = static_cast<int>(spawnPositions.size());
	int index = 0;
	for (auto d : data["data"]) {
		if (spawnPositions.size() <= index) break;

		int typeNum = d["enemyType"];
		EnemyType type = static_cast<EnemyType>(typeNum);
		UseEnemy(type, spawnPositions[index]);
		index++;
	}
}

void EnemyManager::PlayAnimEvent_Sphere(VECTOR _pos, float _radius) {
	colliderObjectManager.CreateSphere(_pos, _radius, None, 0.2f);
}

void EnemyManager::PlayAnimEvent_AABB(VECTOR _pos, VECTOR _min, VECTOR _max) {
	colliderObjectManager.CreateAABB(_pos, _min, _max, None, 0.2f);
}

void EnemyManager::PlayAnimEvent_Effect(const std::string& _effectName, VECTOR _pos, float _scale, VECTOR rot) {
	effectManager.Play(_effectName, _pos, _scale, rot);
}

void EnemyManager::PlayAnimEvent_Audio(const std::string& _audioName, float _volume, bool _isLoop, VECTOR _pos, float distance) {
	audioManager.Play(_audioName, _volume, _isLoop, _pos, distance);
}
void EnemyManager::PlayAnimEvent_Missile(std::string _modelName, GameObject* _pOwner, VECTOR _dir, VECTOR _pos) {
	gameObjectManager.CreateGameObject<Missile>(_modelName, _pOwner, &effectManager, _dir, _pos);
}

void EnemyManager::UnuseEnemy(EnemyPtr& _unuseEnemy) {
	auto itr = std::ranges::find(useEnemyArray, _unuseEnemy);
	// 使用中配列になければ帰る
	if (itr == useEnemyArray.end()) return;
	// 後処理
	_unuseEnemy->Cleanup();
	// 未使用配列に入れる
	EnemyType type = _unuseEnemy->GetType();
	unuseEnemyArray[type].push_back(std::move(_unuseEnemy));
}