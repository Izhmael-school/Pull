#pragma once

// ImGui C++用の軽量なグラフィカルユーザーインターフェース
// https://github.com/ocornut/imgui

/// <summary>
/// ImGuiを管理するクラス
/// </summary>
class ImGuiManager
{

public:

	/// <summary>
	/// 初期設定
	/// プログラム起動時に実行
	/// </summary>
	/// <returns></returns>
	void Init();

	/// <summary>
	/// 毎フレームの最初に呼ぶ
	/// </summary>
	void BeginFrame();

	/// <summary>
	/// 毎フレームの最後に呼ぶ
	/// 描画も担当
	/// </summary>
	void EndFrame();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Release();

private:

	/// <summary>
	/// DXライブラリのマウス入力をImGuiへ渡す
	/// DXライブラリが動作する環境ではImGuiのマウス入力が正常に取得できない
	/// そのためDXライブラリから入力してImGuiIOへ渡してる
	/// </summary>
	void UpdateMouseInput();

};

