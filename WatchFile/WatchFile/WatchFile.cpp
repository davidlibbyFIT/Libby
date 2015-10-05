// WatchFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Windows.h"


bool directory_exists(const char *dirname)
{
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;

	if (dirname && *dirname) {
		if (!GetFileAttributesEx(dirname, GetFileExInfoStandard, &fileInfo)) {
			return false;
		}

		if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			return true;
		}
	}

	return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
	directory_exists("c:\\work\\time.txt");
	return 0;
}

