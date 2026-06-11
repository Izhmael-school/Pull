/*
 *	@file	TriggerInterface.h
 *  @author oorui
 */

#ifndef _TRIGGERINTERFACE_H_
#define _TRIGGERINTERFACE_H_

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
};


#endif // !_TRIGGERINTERFACE_H_
