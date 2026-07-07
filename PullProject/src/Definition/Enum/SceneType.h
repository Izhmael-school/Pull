#pragma once

/*
 * @file SceneType.h
 * @author Sekino
 */

enum class SceneType {
	Invalid = -1,
	Debug,
	DebugSceneSelect,
	PlayerDebug,
	EnemyDebug,
	StageDebug,
	CollisionDebug,
	Title,				// タイトルシーン
	GameModeSelect,		// デバッグか本編か選択※後で削除
	StageSelect,		// ステージ選択シーン
	Game,				// ゲームプレイシーン
	End,				// リザルト、終了シーン
	Max
};