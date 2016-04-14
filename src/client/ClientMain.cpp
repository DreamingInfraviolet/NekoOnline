#include <thread>
#include <Windows.h>
#include "Pet.h"
#include "animation.h"

int WINAPI WinMain(HINSTANCE hPrevInstance, HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "NekoOnline";
	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW, "NekoOnline", NULL, WS_POPUP | WS_VISIBLE,
		0, 0, 0, 0, NULL, NULL, hInstance, NULL);

	/*
		HWND hwnd = CreateWindow("NekoOnline", NULL, WS_POPUP,
		0, 0, 0, 0, NULL, NULL, hInstance, NULL);
	hwnd = CreateWindowEx(WS_EX_LAYERED, "NekoOnline", NULL, WS_POPUP | WS_VISIBLE,
		0, 0, 0, 0, hwnd, NULL, hInstance, NULL);
	*/

	RECT rect;

	// get the current window size and position
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


	Pet pet;
	AnimationComponent animation;
	animation.setCurrentSequence(AnimationComponent::SEQUENCE_FALLING_ASLEEP);

	MSG msg;

	bool quit = false;
	while (!quit)
	{
		while ((PeekMessage(&msg, hwnd, NULL, 0, PM_REMOVE) > 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Sleep(300);

		pet.update();
		animation.draw(hwnd, &pet);
	}
}