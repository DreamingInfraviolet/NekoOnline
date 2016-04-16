#pragma once
#include <Windows.h>
#include "matrixd.h"
#include "bitmap.h"

class Sprite
{
	static bool sInitialised;
	static void sInit();

	static HDC sDesktopDC;
	HDC spriteDC;
public:
	Bitmap bitmap;
	Sprite(const std::string& bitmapPath);
	~Sprite();
	math::vec2 pos;
	const math::vec2i& size;

	void draw(HWND hwnd, int startX=0, int startY=0, int endX=-1, int endY=-1);
};

