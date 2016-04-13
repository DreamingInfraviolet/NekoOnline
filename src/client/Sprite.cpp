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
	: bitmap(path), size(bitmap.w, bitmap.h)
{
	sInit();
	spriteDC = CreateCompatibleDC(sDesktopDC);
}


Sprite::~Sprite()
{
}

void Sprite::draw()
{
	//SelectObject(spriteDC, spriteHBM);
	//RECT rcSprite;
	//SetRect(&rcSprite, 0, 0, size.x, size.y);
	//FillRect(spriteDC, &rcSprite, (HBRUSH)GetStockObject(GRAY_BRUSH));
	//ReleaseDC(NULL, spriteDC);

	//BitBlt(sDesktopDC, 0, 0, 50, 50, spriteDC, 0, 0, SRCCOPY);
	//BitBlt(sDesktopDC, pos.x, pos.y, pos.x+size.x, pos.y+size.y, spriteDC, 0, 0, SRCCOPY);
}
