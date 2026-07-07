/*
 *	@file	TriggerInterface.h
 *  @author oorui
 */

#ifndef _TRIGGERINTERFACE_H_
#define _TRIGGERINTERFACE_H_

#include <DxLib.h>
/*
 *	レバーに対応したギミックのインターフェース
 */
class TriggerInterface {
public:
	/*
	 *	デストラクタ
	 */
	virtual ~TriggerInterface() = default;

	/*
	 *	通知を受けた時の処理
	 */
	virtual void OnTriggered() = 0;

	/*
	 *	自身のIDの取得
	 */
	virtual int GetTriggerID() const = 0;

	/*
	 *  レバー配置位置取得可否判定
	 *  @param[in]	VECTOR& 生成位置を返す用
	 * 
	 */
	virtual bool GetLeverSpawnPosition(VECTOR& pos) const = 0;
};


#endif // !_TRIGGERINTERFACE_H_
