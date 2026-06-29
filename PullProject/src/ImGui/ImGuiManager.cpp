#include "ImGuiManager.h"

#include <ImGui/imgui.h>

#include <ImGui/backends/imgui_impl_dx11.h>
#include <ImGui/backends/imgui_impl_win32.h>

#include <DxLib.h>
#include <d3d11.h>

void ImGuiManager::Init()
{

	// ImGui本体を生成
	ImGui::CreateContext();

	// ダーク設定
	ImGui::StyleColorsDark();

	HWND hwnd = GetMainWindowHandle();

	// Windowsの入力を取得
	ImGui_ImplWin32_Init(hwnd);

	// DirectX11を初期化
	auto* device = (ID3D11Device *)GetUseDirect3D11Device();
	auto* context = (ID3D11DeviceContext*)GetUseDirect3D11DeviceContext();

	ImGui_ImplDX11_Init( device, context);
}


void ImGuiManager::BeginFrame()
{

	UpdateMouseInput();

	ImGui_ImplDX11_NewFrame();

	ImGui_ImplWin32_NewFrame();

	// UI生成を通知
	ImGui::NewFrame();

}

void ImGuiManager::EndFrame()
{

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData());

}

void ImGuiManager::Release()
{

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}

void ImGuiManager::UpdateMouseInput()
{

	ImGuiIO& io = ImGui::GetIO();

	// DXライブラリからマウス情報を取得
	int mouse = DxLib::GetMouseInput();
	int wheel = DxLib::GetMouseWheelRotVol();

	// 取得した情報をInGuiへ渡す
	io.AddMouseButtonEvent(0, (mouse & MOUSE_INPUT_LEFT) != 0);
	io.AddMouseButtonEvent(1, (mouse & MOUSE_INPUT_RIGHT) != 0);
	io.AddMouseButtonEvent(2, (mouse & MOUSE_INPUT_MIDDLE) != 0);
	io.AddMouseWheelEvent( 0.0f, static_cast<float>(wheel) / 120.0f );
}
