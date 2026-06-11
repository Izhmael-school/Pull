/*
 *	@file	StageState.h
 *  @author oorui
 */

#ifndef _STAGESTATE_H_
#define _STAGESTATE_H_

#include <memory>
#include <vector>		// 後で必ず消す
#include "StageBase.h"

/*
 *	ステージの状態管理クラス
 */
class StageState {
private:
	std::vector<int> stageModelHandle;	// 読み込んだステージ全てを保持

	int currentStageID = -1;	//	現在のステージのID

public:
	/*
	 *	コンストラクタ
	 */
	StageState() = default;
	/*
	 *	デストラクタ
	 */
	~StageState() = default;

public:
	/*
	 *	ステージのモデルハンドルの追加
	 *  @pram[in]	int モデルのハンドル
	 */
	void AddStageModelHandle(int modelHandle) {
		// モデルがなければ実行しない
		if (modelHandle == -1)return;
		// モデルハンドルを追加
		stageModelHandle.push_back(modelHandle);
	}

	/*
	 *	次のステージを解放
	 *  @param[in]	int 現在のステージID+1
	 */
	void OpenStage(int setID) {
		// 次のステージを解放

	}


	/*
	 *	ステージの切り替え
	 *  @param[in]	int ステージのID
	 */
	void TransitionStage(int setID) {
		// 次のステージに移動
		currentStageID = setID;
	}


	/*
	 *	全ステージ削除
	 *  @fixd ステージのモデルを削除する
	 */
	void ResetStageModelHandle() {
		for (int handle : stageModelHandle) {
			if (handle != -1) MV1DeleteModel(handle);
		}
		stageModelHandle.clear();
		currentStageID = -1;
	}

public:

	/*
	 *	現在のステージインデックスを取得
	 */
	int GetCurrentStageIndex()const { return currentStageID; }

	/*
	 *	挑戦したステージの数を取得
	 */
	int GetClearStageCount() const { return static_cast<int>(stageModelHandle.size()); }

	/*
	 * @brief 現在のハンドルを取得
	 */
	int GetCurrentStageHandle() const {
		if (currentStageID >= 0 && currentStageID < (int)stageModelHandle.size())
			return stageModelHandle[currentStageID];
		return -1;
	}

};

#endif // !_STAGESTATE_H_
