/*
 *	@file	GimmickManager.cpp
 *  @author oorui
 */

#include "GimmickManager.h"
#include "GameObject/Stage/Gimmick/TriggerInterface.h"

 /*
  *  レバー対応ギミックの登録
  *  @param[in] int	対応したレバーのID
  *  @param[in] TriggerInterface* インターフェース
  */
void GimmickManager::RegisterLeverReceiver(int leverId, TriggerInterface* gimmick) {
	if (gimmick == nullptr) return;

	// 指定レバーIDのリスト取得
	auto& receivers = triggerGimmickMaps[leverId];

	// 二重登録防止
	auto it = std::find(
		receivers.begin(),
		receivers.end(),
		gimmick);

	// 末尾に登録
	if (it == receivers.end()) {
		receivers.push_back(gimmick);
	}
}

/*
 *	レバー対応ギミック破棄
 *  @param[in] int 対応したレバーのID
 *  @param[in] TriggerInterface* 削除するギミック
 */
void GimmickManager::UnregisterLeverReceiver(int leverId, TriggerInterface* gimmick) {
	// レバーID検索
	auto mapIt = triggerGimmickMaps.find(leverId);
	// 登録されたギミックが無ければ抜ける
	if (mapIt == triggerGimmickMaps.end()) return;

	// レバーに対応するギミック一覧取得
	auto& receivers = mapIt->second;

	// 指定ギミック削除
	receivers.erase(
		std::remove(
			receivers.begin(),
			receivers.end(),
			gimmick),
		receivers.end());

	// 登録が無くなったらMapごと削除
	if (receivers.empty()) {
		triggerGimmickMaps.erase(mapIt);
	}
}

/*
 *	レバー起動
 */
void GimmickManager::ActivateLever(int leverId) {
	// レバーID検索
	auto mapIt = triggerGimmickMaps.find(leverId);

	// 指定のギミックが登録されていなければ抜ける
	if (mapIt == triggerGimmickMaps.end())return;

	// 受け取ったIDのギミックに通知を送る
	for (auto receiver : mapIt->second) {
		if (receiver == nullptr) {
			continue;
		}

		// ギミック起動
		receiver->OnTriggered();
	}
}

/*
 *	全登録情報削除
 */
void GimmickManager::Clear() {
	triggerGimmickMaps.clear();
}