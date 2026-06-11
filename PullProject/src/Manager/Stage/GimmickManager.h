/*
 *	@file	GimmickManager.h
 *  @author oorui
 */

#include <unordered_map>
#include <vector>

class TriggerInterface;

class GimmickManager {
private:
	// レバー対応ギミックのインターフェース
	std::unordered_map<int, std::vector<TriggerInterface*>> pTriggerInterface;

public:

	/*
	 *  レバー対応ギミックの登録
	 *  @param[in] int	対応したレバーのID
	 *  @param[in] TriggerInterface* インターフェース
	 */
	void RegisterLeverReceiver(int leverId, TriggerInterface* receiver) {
		// レバー対応ギミックの末尾に追加
		pTriggerInterface[leverId].push_back(receiver);
	}

	/*
	 *	レバー起動
	 */
	void ActivateLever(int leverId) {
		auto it = pTriggerInterface.find(leverId);

		if (it == pTriggerInterface.end())
		{
			return;
		}

		//for (auto receiver : it->second)
		//{
		//	receiver->OnTriggered();
		//}
	}
};
