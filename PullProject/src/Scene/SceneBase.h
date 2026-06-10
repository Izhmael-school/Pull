/*
 * @file SceneBase.h
 * @author Sekino
 */
#pragma once

/// <summary>
/// シーンの基底クラス
/// </summary>
class SceneBase {
public:
	SceneBase();
	virtual ~SceneBase() = default;

private:
	/// <summary>
	/// 作られたときに入る処理
	/// </summary>
	virtual void Start();

public:
	/// <summary>
	/// シーンの更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// シーンの描画
	/// </summary>
	virtual void Render() = 0;
	/// <summary>
	/// シーンに入るときの処理
	/// </summary>
	virtual void Setup();
	/// <summary>
	/// シーンを抜けるときの処理
	/// </summary>
	virtual void Cleanup();

};