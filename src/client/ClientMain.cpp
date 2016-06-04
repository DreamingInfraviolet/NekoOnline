#include <thread>
#include <Windows.h>
#include "Pet.h"
#include "animation.h"
#include "client_settings.h"

#include <io.h>
#include <fcntl.h>


int WINAPI WinMain(HINSTANCE hPrevInstance, HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd)
{
	std::cout << "Running...\n";

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "NekoOnline";
	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW, "NekoOnline", NULL, WS_POPUP | WS_VISIBLE,
		0, 0, 0, 0, NULL, NULL, hInstance, NULL);


	// get the current window size and position
	RECT rect;
	GetWindowRect(hwnd, &rect);

	// now change the size, position, and Z order 
	// of the window.
	SetWindowPos(hwnd,       // handle to window
		HWND_TOPMOST,  // placement-order handle
		rect.left,     // horizontal position
		rect.top,      // vertical position
		rect.right-rect.left,  // width
		rect.bottom-rect.top, // height
		SWP_SHOWWINDOW // window-positioning options
		);


	FileSystem::IniParser iniParser;
	if (!iniParser.parseFile("config.ini"))
		throw std::runtime_error("Failed parsing config.ini");

	Config::ClientSettings settings;
	if (!settings.attachParser(&iniParser, "GLOBAL"))
		throw std::runtime_error("Failed attaching parser.");
	settings.initialise();

	Pet pet(settings.imageSource);
	AnimationComponent animation;
	
	math::vec2 screenSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	
	MSG msg;

	bool quit = false;
	while (!quit)
	{
		while ((PeekMessage(&msg, hwnd, NULL, 0, PM_REMOVE) > 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Sleep(100);

		pet.update(screenSize);
		auto currentTargetDelta = pet.getTargetDelta();
		animation.setCurrentSequence(animation.getAnimationFromDirection(currentTargetDelta));
		animation.draw(hwnd, &pet);
	}
}