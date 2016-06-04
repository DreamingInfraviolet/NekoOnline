#pragma once
#include <string>
#include <Shlwapi.h>
#include <wincodec.h>
#include <Windows.h>
#include "matrixd.h"

class Bitmap
{
private:
	static IWICBitmapSource * loadBitmapFromStream(IStream * ipImageStream);
	static HBITMAP createHBITMAP(IWICBitmapSource * ipBitmap);

	HBITMAP hbitmap = nullptr;

	Bitmap(const Bitmap&) = delete;

public:
    Bitmap();
	~Bitmap();
    Bitmap(const std::string& path);
    void loadFromFile(const std::string& path);
	math::vec2i size;

	HBITMAP get();
};