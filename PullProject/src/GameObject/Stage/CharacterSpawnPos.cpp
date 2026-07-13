/*
 *	@file	CharacterSpawnPos.cpp
 *  @author oorui
 */

#include "CharacterSpawnPos.h"
#include <format>


 /*
  * まとめてロードする
  */
void CharacterSpawnPos::Load(int modelHandle) {
	LoadEnemyPosition(modelHandle);
	LoadPlayerPosition(modelHandle);
}


/*
 * 敵の生成位置取得
 */
void CharacterSpawnPos::LoadEnemyPosition(int modelHandle) {
	// クリア
	enemyPositions.clear();

	int index = 0;

	while (true) {
		// 番号からフレーム名を取得
		std::string frameName = std::format("Enemy_SP{}", index);
		// フレーム検索
		int frameIndex = MV1SearchFrame(modelHandle, frameName.c_str());

		// 見つからなければ終了
		if (frameIndex <= 0) {
			break;
		}

		// 座標取得
		enemyPositions.push_back(
			MV1GetFramePosition(modelHandle, frameIndex));

		index++;
	}
}

/*
 * 敵生成位置一覧取得
 */
const std::vector<VECTOR>& CharacterSpawnPos::GetEnemyPositions() const {
	return enemyPositions;
}

/*
 *	プレイヤーの生成位置を探す
 */
void CharacterSpawnPos::LoadPlayerPosition(int modelHandle) {
	// 生成位置のフレームを探す
	int frameIndex = MV1SearchFrame(modelHandle, "PlayerSP");

	// あれば
	if (frameIndex != -1) {
		// 座標を設定
		playerPosition = MV1GetFramePosition(modelHandle, frameIndex);
	}

}

/*
 *	プレイヤーの生成位置を取得
 */
const VECTOR& CharacterSpawnPos::GetPlayerPositions() const {
	return playerPosition;
}

/*
 *	カメラの生成位置を探す
 */
void CharacterSpawnPos::LoadCameraPosition(int modelHandle) {
	// 生成位置のフレームを探す
	int frameIndex = MV1SearchFrame(modelHandle, "CameraPos");

	// あれば
	if (frameIndex != -1) {
		// 座標を設定
		playerPosition = MV1GetFramePosition(modelHandle, frameIndex);
	}
}

/*
 *	カメラの生成位置を取得
 */
const VECTOR& CharacterSpawnPos::GetCameraPositions() const{
	return cameraPosition;
}
