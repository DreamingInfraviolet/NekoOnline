#include "Sprite.h"
#include <Windows.h>

bool Sprite::sInitialised = false;
HDC Sprite::sDesktopDC;

void Sprite::sInit()
{
	if (!sInitialised)
	{
		sDesktopDC = GetDC(NULL);
		sInitialised = true;
	}
}

Sprite::Sprite(const std::string& path)
	: bitmap(path), size(bitmap.size)
{
	sInit();
	spriteDC = CreateCompatibleDC(sDesktopDC);
}


Sprite::~Sprite()
{
	DeleteDC(spriteDC);
	ReleaseDC(NULL, sDesktopDC);
}

void Sprite::draw(HWND hwnd, int startX, int startY, int endX, int endY)
{
	POINT ptSource = { 0 };
	ptSource.x = startX;
	ptSource.y = startY;

	SIZE wsize;
	wsize.cx = (endX < 0 ? size.x : endX) - ptSource.x;
	wsize.cy = (endY < 0 ? size.y : endY) - ptSource.y;

	POINT ptTarget;
	ptTarget.x = pos.x;
	ptTarget.y = pos.y;

	HBITMAP hbmpOld = (HBITMAP)SelectObject(spriteDC, bitmap.get());

	// use the source image's alpha channel for blending
	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	// paint the window (in the right location) with the alpha-blended bitmap
	UpdateLayeredWindow(hwnd, sDesktopDC, &ptTarget, &wsize,
		spriteDC, &ptSource, RGB(0, 0, 0), &blend, ULW_ALPHA);

	// delete temporary objects
	SelectObject(spriteDC, hbmpOld);
}
