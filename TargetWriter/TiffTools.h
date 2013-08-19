#pragma once

#include <windef.h>
#include "wingdi.h"

HDC * CreateImage(long width, long height);
void writeTIFF(const char *pszFile, HDC hdc);
