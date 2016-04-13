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
	math::vec2i pos;
	const math::vec2i size;

	void draw();
};

