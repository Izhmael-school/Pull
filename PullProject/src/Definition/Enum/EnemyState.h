#pragma once

/*
 * @file EnemyState.h
 * @author Sekino
*/

enum EnemyActionState {
	Invalid = -1,
	NoneAction,		// 何もしてない(待機中)
	Wandering,	// 徘徊中
	Tracing,	// 追跡中
	Attack,		// 攻撃中
	OutofControl// 行動不能
};