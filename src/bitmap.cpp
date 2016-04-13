#include "bitmap.h"
#include <Windows.h>
#include <Shlwapi.h>
#include <wincodec.h>
#include <stdexcept>
#include <codecvt>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "windowscodecs.lib")

Bitmap::Bitmap()
{
	static bool firstInit = true;
	if (firstInit)
	{
		CoInitialize(NULL);
		firstInit = false;
	}
	w = h = 0;
	
}

Bitmap::~Bitmap()
{
	if (hbitmap)
		DeleteObject(hbitmap);
}

Bitmap::Bitmap(const std::string& path)
	: Bitmap()
{
	loadFromFile(path);
}

void Bitmap::loadFromFile(const std::string& path)
{
	// The factory pointer
	IWICImagingFactory *pFactory = NULL;

	// Create the COM imaging factory
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pFactory)
		);
	
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide = converter.from_bytes(path.c_str());

	IStream* stream;
	if (FAILED(SHCreateStreamOnFileEx(wide.c_str(), STGM_READ, FILE_ATTRIBUTE_NORMAL, FALSE, nullptr, &stream)))
		throw std::runtime_error("Unable to create image file stream");
		try
	{
		IWICBitmapSource* ipBitmap = Bitmap::loadBitmapFromStream(stream);
		try
		{
			hbitmap = createHBITMAP(ipBitmap);
			BITMAP bm;
			GetObject(hbitmap, sizeof(bm), &bm);
			w = bm.bmWidth;
			h = bm.bmHeight;
		}
		catch (...)
		{
			ipBitmap->Release();
			throw;
		}
		ipBitmap->Release();
	}
	catch (...)
	{
		stream->Release();
		throw;
	}
	stream->Release();
}

HBITMAP Bitmap::get()
{
	return hbitmap;
}


// Loads a PNG image from the specified stream (using Windows Imaging Component).
IWICBitmapSource* Bitmap::loadBitmapFromStream(IStream* ipImageStream)
{
	using namespace std;

	// initialize return value
	IWICBitmapSource * ipBitmap = NULL;

	// load WIC's PNG decoder
	IWICBitmapDecoder * ipDecoder = NULL;

	// load the first frame (i.e., the image)
	IWICBitmapFrameDecode * ipFrame = NULL;
	
	try
	{
		if (FAILED(CoCreateInstance(CLSID_WICPngDecoder, NULL, CLSCTX_INPROC_SERVER, __uuidof(ipDecoder), reinterpret_cast<void**>(&ipDecoder))))
			throw runtime_error("Unable to create image decoder");

		// load the PNG
		if (FAILED(ipDecoder->Initialize(ipImageStream, WICDecodeMetadataCacheOnLoad)))
			throw runtime_error("Unable to load PNG image");

		// check for the presence of the first frame in the bitmap
		UINT nFrameCount = 0;
		if (FAILED(ipDecoder->GetFrameCount(&nFrameCount)) || nFrameCount != 1)
			throw runtime_error("Unable to get frame count for image");

		if (FAILED(ipDecoder->GetFrame(0, &ipFrame)))
			throw runtime_error("Unable to get frame for image");

		// convert the image to 32bpp BGRA format with pre-multiplied alpha
		//   (it may not be stored in that format natively in the PNG resource,
		//   but we need this format to create the DIB to use on-screen)
		WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, ipFrame, &ipBitmap);
	}
	catch (...)
	{
		ipFrame->Release();
		throw;
	}

	
	ipDecoder->Release();
	return ipBitmap;
}

// Creates a 32-bit DIB from the specified WIC bitmap.
HBITMAP Bitmap::createHBITMAP(IWICBitmapSource * ipBitmap)
{
	// initialize return value
	HBITMAP hbmp = NULL;

	// get image attributes and check for valid image
	UINT width = 0;
	UINT height = 0;
	if (FAILED(ipBitmap->GetSize(&width, &height)) || width == 0 || height == 0)
		throw std::runtime_error("Unable to get bitmap size");

	// prepare structure giving bitmap information (negative height indicates a top-down DIB)
	BITMAPINFO bminfo;
	ZeroMemory(&bminfo, sizeof(bminfo));
	bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bminfo.bmiHeader.biWidth = width;
	bminfo.bmiHeader.biHeight = -((LONG)height);
	bminfo.bmiHeader.biPlanes = 1;
	bminfo.bmiHeader.biBitCount = 32;
	bminfo.bmiHeader.biCompression = BI_RGB;

	// create a DIB section that can hold the image
	void * pvImageBits = NULL;
	HDC hdcScreen = GetDC(NULL);
	hbmp = CreateDIBSection(hdcScreen, &bminfo, DIB_RGB_COLORS, &pvImageBits, NULL, 0);
	ReleaseDC(NULL, hdcScreen);
	if (hbmp == NULL)
		throw std::runtime_error("Unable to create DIB section for image");

	// extract the image into the HBITMAP
	const UINT cbStride = width * 4;
	const UINT cbImage = cbStride * height;
	if (FAILED(ipBitmap->CopyPixels(NULL, cbStride, cbImage, static_cast<BYTE *>(pvImageBits))))
	{
		// couldn't extract image; delete HBITMAP
		DeleteObject(hbmp);
		hbmp = NULL;
	}

Return:
	return hbmp;
}