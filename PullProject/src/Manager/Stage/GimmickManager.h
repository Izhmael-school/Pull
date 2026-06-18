/*
 *	@file	GimmickManager.h
 *  @author oorui
 */
#ifndef _GIMMICKMANAGER_H_
#define _GIMMICKMANAGER_H_



#include "../../Component/Singleton.h"
#include "../../Definition/CommonModule/MyMath.h"
#include <unordered_map>
#include <vector>

class TriggerInterface;

class GimmickManager :public Singleton<GimmickManager> {
	// フレンド宣言
	friend class Singleton<GimmickManager>;	
private:
	// レバーIDと対応ギミック
	std::unordered_map<int, std::vector<TriggerInterface*>> triggerGimmickMaps;

	/*
	 *	コンストラクタ
	 */
	GimmickManager();
	/*
	 *	デストラクタ
	 */
	~GimmickManager() = default;


public:

	/*
	 *  レバー対応ギミックの登録
	 *  @param[in] int	対応したレバーのID
	 *  @param[in] TriggerInterface* インターフェース
	 */
	void RegisterLeverReceiver(int leverId, TriggerInterface* gimmick);

	/*
	 *	レバー対応ギミック破棄
	 *  @param[in] int 対応したレバーのID
	 *  @param[in] TriggerInterface* 削除するギミック
	 */
	void UnregisterLeverReceiver(int leverId, TriggerInterface* gimmick);


	/*
	 *	レバー起動
	 */
	void ActivateLever(int leverId);

	/*
	 * 全登録情報削除
	 */
	void Clear();
};


#endif // !_GIMMICKMANAGER_H_