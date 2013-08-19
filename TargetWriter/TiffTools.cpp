
#include "stdafx.h"

#include "TiffTools.h"

#include "Preconditions.h"
#include "ScopedArray.h"
#include "ScopeGuard.h"

#include "tiffio.h"

HDC * CreateImage(long width, long height)
{


	BITMAPINFO bmInfo = {0};
	memset(&bmInfo,0,sizeof(bmInfo));
	bmInfo.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth       = width;
	bmInfo.bmiHeader.biHeight      = height * -1; // Negative gives us a top-down image.
	bmInfo.bmiHeader.biPlanes      = 1;
	bmInfo.bmiHeader.biBitCount    = 24;
	bmInfo.bmiHeader.biCompression = BI_RGB;

	HDC hdc = ::CreateCompatibleDC(NULL);
	ScopeGuard guardDC = MakeGuard(::DeleteDC, hdc);

	unsigned char *pBits = NULL;
	HBITMAP hbm = ::CreateDIBSection(hdc, &bmInfo, DIB_RGB_COLORS, (void **)&pBits, NULL, NULL);

	writeTIFF("C:\\Flake.tif", hdc);
	
	//DeleteObject(SelectObject(hdc, hbm));


	return &hdc;
}



void writeTIFF(const char *pszFile, HDC hdc)
{
	precondition_throw(pszFile != NULL, "Path to TIFF file cannot be null.");
	precondition_throw(hdc != NULL, "Source device context cannot be null.");

	HBITMAP hbm = (HBITMAP) ::GetCurrentObject(hdc, OBJ_BITMAP);
	DIBSECTION ds = {255};
	memset(&ds,255,sizeof(ds));
	::GetObject(hbm, sizeof(DIBSECTION), &ds);

	LONG buffWidth    = ds.dsBmih.biWidth;
	LONG buffHeight   = (ds.dsBmih.biHeight < 0) ? ds.dsBmih.biHeight * -1 : ds.dsBmih.biHeight;
	WORD planes       = ds.dsBmih.biPlanes;
	WORD bitsPerPixel = ds.dsBmih.biBitCount;

	if (planes != 1)
		throw std::runtime_error("Unsupported image format.");

	long padding = 0;
	while ((buffWidth * 3 + padding) % sizeof(DWORD) != 0)
		++padding;

	long pitchBytes  = buffWidth * 3 + padding;
	long neededBytes = pitchBytes * buffHeight;

	ScopedArray<BYTE> spBuffer(new BYTE[neededBytes]);
	memset(spBuffer.get(), 0, neededBytes);

	BITMAPINFO bmi = {0};
	bmi.bmiHeader.biSize        = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 24;
	bmi.bmiHeader.biCompression = BI_RGB;		
	bmi.bmiHeader.biWidth       = buffWidth;
	bmi.bmiHeader.biHeight      = buffHeight * -1;

	::GetDIBits(hdc, hbm, 0, buffHeight, spBuffer.get(), &bmi, DIB_RGB_COLORS);

	TIFF *pTIFF = TIFFOpen(pszFile, "w");
	if (!pTIFF)
		throw std::runtime_error("Couldn't create specified TIFF file.");

	ScopeGuard guardTIFF = MakeGuard(TIFFClose, pTIFF);

	TIFFSetField(pTIFF, TIFFTAG_IMAGEWIDTH, buffWidth);
	TIFFSetField(pTIFF, TIFFTAG_IMAGELENGTH, buffHeight);
	TIFFSetField(pTIFF, TIFFTAG_SAMPLESPERPIXEL, 3);
	TIFFSetField(pTIFF, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(pTIFF, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(pTIFF, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(pTIFF, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	long packedSize = buffWidth * buffHeight * 3;
	ScopedArray<BYTE> spPackedBuffer(new BYTE[packedSize]);

	BYTE *pDest = spPackedBuffer.get();
	BYTE *pSrc = spBuffer.get();
	for (long y = 0; y < buffHeight; ++y)
	{
		for (long x = 0; x < buffWidth; ++x)
		{
			// Swapping bytes so the byte order is as we expect
			pDest[0] = pSrc[2];
			pDest[1] = pSrc[1];
			pDest[2] = pSrc[0];
			pDest += 3;
			pSrc  += 3;
		}
		pSrc += padding;
	}

	if (TIFFWriteEncodedStrip(pTIFF, 0, spPackedBuffer.get(), buffWidth * buffHeight * 3) == 0)
		throw std::runtime_error("Couldn't write image.");
}
