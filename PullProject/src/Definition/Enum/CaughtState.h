/*
 * @brief 掴めるオブジェクトの状態定義
 * @author Sekino
 */
#pragma once

enum CaughtState {
	NoneCaughtState,
	Catch,
	Catching,
	Throw,
	Throwing,
};