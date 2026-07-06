/*
 *	@file	InputSystemManager.h
 *	@author Riku
 */

#ifndef _INPUTSYSTEMMANAGER_H_
#define _INPUTSYSTEMMANAGER_H_

#include "../Component/Singleton.h"
#include "ManagerBase.h"

/*
 *	入力システムの管理クラス
 */
class InputSystemManager : public ManagerBase, public Singleton<InputSystemManager> {
private:

public:
	InputSystemManager();
	~InputSystemManager();

public:
	// 初期化処理
	void Start() override;
	// 更新処理
	void Update() override;

public:
	/*
	 *	ボタンが押されたかどうか
	 *  @param[in]	ActionMapBase::InputType _type	種類
	 *  @param[in]	int _button						入力
	 *	@return		bool
	 */
	bool IsInputDown(ActionMapBase::InputType _type, int _button);
	/*
	 *	ボタンが押されているかどうか
	 *  @param[in]	ActionMapBase::InputType _type	種類
	 *  @param[in]	int _button						入力
	 *	@return		bool
	 */
	bool IsInput(ActionMapBase::InputType _type, int _button);
	/*
	 *	ボタンが押されたかどうか
	 *  @param[in]	ActionMapBase::InputType _type	種類
	 *  @param[in]	int _button						入力
	 *	@return		bool
	 */
	bool IsInputUp(ActionMapBase::InputType _type, int _button);

};
#endif // !_INPUTSYSTEMMANAGER_H_
