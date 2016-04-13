
#include <Windows.h>
#include "Pet.h"

int WINAPI WinMain(HINSTANCE hPrevInstance, HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "NekoOnline";
	RegisterClass(&wc);

	HWND hwnd = CreateWindow("NekoOnline", NULL, WS_POPUP,
		0, 0, 0, 0, NULL, NULL, hInstance, NULL);
	hwnd = CreateWindowEx(WS_EX_LAYERED, "NekoOnline", NULL, WS_POPUP | WS_VISIBLE,
		0, 0, 0, 0, hwnd, NULL, hInstance, NULL);

	Pet pet;

	// get the primary monitor's info
	POINT ptZero = { 0 };
	HMONITOR hmonPrimary = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO monitorinfo = { 0 };
	monitorinfo.cbSize = sizeof(monitorinfo);
	GetMonitorInfo(hmonPrimary, &monitorinfo);

	// center the splash screen in the middle of the primary work area
	const RECT & rcWork = monitorinfo.rcWork;
	POINT ptOrigin;
	ptOrigin.x = rcWork.left + (rcWork.right - rcWork.left - pet.size.x) / 2;
	ptOrigin.y = rcWork.top + (rcWork.bottom - rcWork.top - pet.size.y) / 2;
	int i = 0;
	while (true) {
		i = (++i) % 1000;
		ptOrigin.x = i;Sleep(2);
		// create a memory DC holding the splash bitmap
		HDC hdcScreen = GetDC(NULL);
		HDC hdcMem = CreateCompatibleDC(hdcScreen);
		HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcMem, pet.bitmap.get());

		// use the source image's alpha channel for blending
		BLENDFUNCTION blend = { 0 };
		blend.BlendOp = AC_SRC_OVER;
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;

		SIZE size;
		size.cx = pet.size.x;
		size.cy = pet.size.y;

		// paint the window (in the right location) with the alpha-blended bitmap
		UpdateLayeredWindow(hwnd, hdcScreen, &ptOrigin, &size,
			hdcMem, &ptZero, RGB(0, 0, 0), &blend, ULW_ALPHA);

		// delete temporary objects
		SelectObject(hdcMem, hbmpOld);
		DeleteDC(hdcMem);
		ReleaseDC(NULL, hdcScreen);
	}
}

/*
RECT rcSize;
HDC hdcBackBuffer, hdcSprite;
HBITMAP hbmBackBuffer, hbmSprite;
int spriteX = 175, spriteY = 175;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_CREATE:
	{
		HDC hdcWindow = GetDC(hWnd);

		// make back buffer
		GetClientRect(hWnd, &rcSize);
		hdcBackBuffer = CreateCompatibleDC(hdcWindow);
		hbmBackBuffer = CreateCompatibleBitmap(hdcBackBuffer, rcSize.right - rcSize.left, rcSize.bottom - rcSize.top);
		SelectObject(hdcBackBuffer, hbmBackBuffer);  // SHOULD SAVE PREVIOUS...

													 // make sprite
		hdcSprite = CreateCompatibleDC(hdcWindow);
		hbmSprite = CreateCompatibleBitmap(hdcSprite, 50, 50);
		SelectObject(hdcSprite, hbmSprite);  // SHOULD SAVE PREVIOUS...
		RECT rcSprite;
		SetRect(&rcSprite, 0, 0, 50, 50);
		FillRect(hdcSprite, &rcSprite, (HBRUSH)GetStockObject(WHITE_BRUSH));

		ReleaseDC(hWnd, hdcWindow);
		return 0;
	}
	case WM_KEYDOWN:
	{
		// SHOULD REALLY USE GetAsyncKeyState for game, but simplified here
		switch (wParam)
		{
		case VK_LEFT:
			spriteX--;
			break;
		case VK_RIGHT:
			spriteX++;
			break;
		case VK_UP:
			spriteY--;
			break;
		case VK_DOWN:
			spriteY++;
			break;
		}
		return 0;
	}
	case WM_ERASEBKGND:
	{
		return 1; // INDICATE THAT WE ERASED THE BACKGROUND OURSELVES
	}
	case WM_PAINT:
	{
		HBRUSH hbrush;
		auto hdc = BeginPaint(hWnd, &ps);
		RECT rect;
		GetClientRect(hWnd, &rect);
		auto compatDC = CreateCompatibleDC(hdc);
		auto compatBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		auto oldBitmap = SelectObject(compatDC, compatBitmap);
		static int i = 0;
		i++;
		if (i % 2 == 0) /// "i" exists elsewhere in the code, in a for loop - this code reduced for simplicity.
			hbrush = CreateSolidBrush(RGB(245, 245, 245));
		else
			hbrush = CreateSolidBrush(RGB(240, 245, 245));
		FillRect(compatDC, &rect, hbrush);
		DeleteObject(hbrush);
		//monitors[i]->Paint(compatDC);
		BitBlt(hdc, 0, 0, rect.right, rect.bottom, compatDC, 0, 0, SRCCOPY);
		SelectObject(compatDC, oldBitmap);
		DeleteObject(compatBitmap);
		DeleteDC(compatDC);
		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_DESTROY:
	{
		// TODO - DESTROY ALL BITMAPS AND DEVICE CONTEXTS
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	}
}

int WINAPI WinMain(HINSTANCE hPrevInstance, HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd)
{
	static TCHAR className[] = TEXT("GameClass");
	static TCHAR windowName[] = TEXT("A Game");

	WNDCLASSEX wcex;

	wcex.cbClsExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = NULL;
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hIconSm = NULL;
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = className;
	wcex.lpszMenuName = NULL;
	wcex.style = 0;

	if (!RegisterClassEx(&wcex))
		return 0;

	HWND hWnd = CreateWindow(className, windowName, WS_CAPTION | WS_BORDER | WS_SYSMENU, 0, 0, 400, 400, NULL, NULL, hInstance, NULL);
	if (!hWnd)
		return 0;

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	for (;;)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		InvalidateRect(hWnd, NULL, FALSE);
	}

	return msg.wParam;
}
*/