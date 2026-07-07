/*
 *	@file	InputSystemManager.h
 *	@author Riku
 */

#ifndef _INPUTSYSTEMMANAGER_H_
#define _INPUTSYSTEMMANAGER_H_

#include "../Component/Singleton.h"
#include "ManagerBase.h"
#include "../Definition/CommonModule/ActionMapData.h"
#include "../Input/ActionMapBase.h"
#include <memory>
#include <DxLib.h>

/*
 *	入力システムの管理クラス
 */
class InputSystemManager : public ManagerBase, public Singleton<InputSystemManager> {
private:
	// 各アクションマップ
	std::unordered_map<ActionMap, std::shared_ptr<ActionMapBase>> actionMaps;
public:
	InputSystemManager();
	~InputSystemManager();

private:
	// 初期化処理
	void Start() override;

public:
	// 更新処理
	void Update() override;

public:
	/*
	 *	ボタンが押されたかどうか
	 *  @param[in]	InputType _type	種類
	 *  @param[in]	int _button						入力
	 *	@return		bool
	 */
	bool IsInputDown(InputType _type, int _button);
	/*
	 *	ボタンが押されているかどうか
	 *  @param[in]	InputType _type	種類
	 *  @param[in]	int _button						入力
	 *	@return		bool
	 */
	bool IsInput(InputType _type, int _button);
	/*
	 *	ボタンが押されたかどうか
	 *  @param[in]	InputType _type	種類
	 *  @param[in]	int _button						入力
	 *	@return		bool
	 */
	bool IsInputUp(InputType _type, int _button);

	/*
	 *	2軸入力の値取得
	 *  @param[in]	InputType _type	種類
	 *	@return		bool
	 */
	VECTOR GetAxisValue(InputType _type);

public: // アクションマップ関連
	/*
	 *	各アクションマップの有効状態切り替え
	 *	@param[in]	ActionMap map	切り替えるアクションマップ
	 *  @param[in]	bool setActive	切り替え値
	 */
	inline void SetActionMapIsActive(ActionMap map, bool setActive) {
		actionMaps[map]->isActive = setActive;
	}
	/*
	 *	各アクションマップの入力状態取得
	 *	@param[in]	ActionMap map				取得するアクションマップ
	 */
	inline ActionState GetInputState(ActionMap map) {
		return actionMaps[map]->state;
	}
};
#endif // !_INPUTSYSTEMMANAGER_H_
