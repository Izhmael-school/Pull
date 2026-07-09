#include <DxLib.h>
#include <Application.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	Application::GetInstance().Run();

	return 0;
}