/*
 *	@file	StageManager.h
 *  @author oorui
 */

#ifndef _STAGEMANAGER_H_
#define _STAGEMANAGER_H_

#include "../../Component/Singleton.h"
#include "../../GameObject/Stage/Stage.h"
#include "../../GameObject/Stage/StageState.h"
#include "../../Manager/ModelManager.h"
#include "../../Data/Gimmick/StageLoader.h"
#include "../../GameObject/Stage/CharacterSpawnPos.h"

#include <memory>
#include <string>
#include <unordered_map>


/*
 *	ステージ全体の管理
 */
class StageManager :public Singleton<StageManager> {
	// フレンド宣言
	friend class Singleton<StageManager>;
	
private:
	std::unique_ptr<StageBase> loadedStage;		// 読み込み済みのステージデータ
	StageState stageState;						// ステージの状態保持
	CharacterSpawnPos characterSpawnPos;		// キャラクターの生成位置

	std::string stageFile;
	int stageModel;
private:
	/*
	 *	コンストラクタ
	 */
	StageManager();
	/*
	 *	デストラクタ
	 */ 
	~StageManager() = default;

private:

	/*
	 *	簡易的に現在のステージにアクセス
	 *  @tips	Update,Render,StageCollider,を引数に持たせる
	 */
	template<typename Func>
	void WithCurrentStage(Func&& func) {
		// ステージがあるか見る
		if (!loadedStage)return;

		// 渡された関数を実行
		func(*loadedStage);
	}

public:
	/*
	 *	初期化
	 */
	void Initialize();

	/*
	 *	ステージの読み込み
	 *  @param[in]	const std::vector<int> モデルハンドルの配列
	 */
	void LoadStage(int stageID);

	/*
	 *	ステージの切り替え
	 */
	void ChangeStage();

	/*
	 * @brief ステージのIDを切り替える
	 */
	void TransitionStage(int setID);

	/*
	 *  更新
	 */
	void Update();

	/*
	 *  描画
	 */
	void Render();

	/*
	 *  終了
	 */
	void Execute();

public:
	/*
	 *	現在ステージハンドルの取得
	 */
	int GetCurrentStageHandle() const { return stageState.GetCurrentStageHandle(); }

	/*
	 *  現在のステージの実態の取得
	 *  @tips StageManager::GetInstance().GetCurrentStage()->Stage.csの処理を呼ぶ;
	 */
	StageBase* GetCurrentStage() const { return loadedStage.get(); }

	/*
	 *	敵の生成位置配列の取得
	 */
	const std::vector<VECTOR>& GetEnemySpawnPositions() const { return characterSpawnPos.GetEnemyPositions(); }

	/*
	 *	プレイヤーの生成位置の取得
	 */
	const VECTOR& GetPlayerSpawnPosition() const { return characterSpawnPos.GetPlayerPositions(); }
};



#endif // !_STAGEMANAGER_H_
