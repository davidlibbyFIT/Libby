// WatchFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Windows.h"
#include <string>


class FileInformation
{
public:
	FileInformation(const std::string &fileName);
	~FileInformation();
	void refresh();
	SYSTEMTIME FileInformation::getCreationTime();
	SYSTEMTIME FileInformation::getLastAccessTime();
	SYSTEMTIME FileInformation::getLastWriteTime();
	std::string FileInformation::getStringCreationTime();
	std::string FileInformation::getStringLastAccessTime();
	std::string FileInformation::getStringLastWriteTime();
	std::string FileInformation::ConvertFileTimeDateToString(const SYSTEMTIME &FileTime);
	std::string FileInformation::GetStringAttributes(WIN32_FILE_ATTRIBUTE_DATA &fileInfo);
	void FileInformation::waitForFileToChange(int msSleep=100);

protected:
private:
	SYSTEMTIME m_creationTime;
	SYSTEMTIME m_lastAccessTime;
	SYSTEMTIME m_lastWriteTime;
	std::string m_fileAttrib;
	std::string m_fileName;
	unsigned long m_filesize;

};

FileInformation::FileInformation(const std::string &fileName):
	m_fileName (fileName),
	m_filesize (0)
{
	refresh();
}

FileInformation::~FileInformation()
{
}
void FileInformation::refresh()
{
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
	if (!GetFileAttributesEx(m_fileName.c_str(), GetFileExInfoStandard, &fileInfo)) {
		return;
	}
	m_fileAttrib = GetStringAttributes(fileInfo);
	FileTimeToSystemTime (&fileInfo.ftCreationTime, &m_creationTime);
	FileTimeToSystemTime (&fileInfo.ftLastAccessTime, &m_lastAccessTime);
	FileTimeToSystemTime (&fileInfo.ftLastWriteTime, &m_lastWriteTime);	
	unsigned long MaxWord=MAXDWORD;//Set this up to avoid int const warning on complation.
	m_filesize = (fileInfo.nFileSizeHigh * (MaxWord + 1)) + fileInfo.nFileSizeLow;
}
SYSTEMTIME FileInformation::getCreationTime()
{
	return m_creationTime;
}
SYSTEMTIME FileInformation::getLastAccessTime()
{
	return m_lastAccessTime;
}
SYSTEMTIME FileInformation::getLastWriteTime()
{
	return m_lastWriteTime;
}

std::string FileInformation::getStringCreationTime()
{
	return ConvertFileTimeDateToString(m_creationTime);
}
std::string FileInformation::getStringLastAccessTime()
{
	return ConvertFileTimeDateToString(m_lastAccessTime);
}
std::string FileInformation::getStringLastWriteTime()
{
	return ConvertFileTimeDateToString(m_lastWriteTime);
}
void FileInformation::waitForFileToChange(int msSleep/*=100*/)
{	
	std::string holdLaseWrite=getStringLastWriteTime();
	std::string currentLaseWrite = holdLaseWrite;
	while(currentLaseWrite == holdLaseWrite)
	{
		Sleep(msSleep);
		refresh();
		currentLaseWrite = getStringLastWriteTime();
	}
}


std::string FileInformation::GetStringAttributes(WIN32_FILE_ATTRIBUTE_DATA &fileInfo)
{
	std::string Fa;
	if (fileInfo.dwFileAttributes &FILE_SHARE_READ){ Fa += "FILE_SHARE_READ ";}
	if (fileInfo.dwFileAttributes &FILE_SHARE_WRITE){ Fa += "FILE_SHARE_WRITE ";}
	if (fileInfo.dwFileAttributes &FILE_SHARE_DELETE){ Fa += "FILE_SHARE_DELETE ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_READONLY){ Fa += "FILE_ATTRIBUTE_READONLY ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_HIDDEN){ Fa += "FILE_ATTRIBUTE_HIDDEN ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_SYSTEM){ Fa += "FILE_ATTRIBUTE_SYSTEM ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY){ Fa += "FILE_ATTRIBUTE_DIRECTORY ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_ARCHIVE){ Fa += "FILE_ATTRIBUTE_ARCHIVE ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_DEVICE){ Fa += "FILE_ATTRIBUTE_DEVICE ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_NORMAL){ Fa += "FILE_ATTRIBUTE_NORMAL ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_TEMPORARY){ Fa += "FILE_ATTRIBUTE_TEMPORARY ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_SPARSE_FILE){ Fa += "FILE_ATTRIBUTE_SPARSE_FILE ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_REPARSE_POINT){ Fa += "FILE_ATTRIBUTE_REPARSE_POINT ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_COMPRESSED){ Fa += "FILE_ATTRIBUTE_COMPRESSED ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_OFFLINE){ Fa += "FILE_ATTRIBUTE_OFFLINE ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_NOT_CONTENT_INDEXED){ Fa += "FILE_ATTRIBUTE_NOT_CONTENT_INDEXED ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_ENCRYPTED){ Fa += "FILE_ATTRIBUTE_ENCRYPTED ";}
	if (fileInfo.dwFileAttributes &FILE_ATTRIBUTE_VIRTUAL){ Fa += "FILE_ATTRIBUTE_VIRTUAL ";}
	return Fa;
}

std::string FileInformation::ConvertFileTimeDateToString(const SYSTEMTIME &FileTime)
{
	char szDate[200];
	sprintf_s(szDate,sizeof(szDate),"%02d-%02d-%02d %02d:%02d:%02d",
		FileTime.wMonth,
		FileTime.wDay,
		FileTime.wYear,
		FileTime.wHour,
		FileTime.wMinute,
		FileTime.wSecond);
	return szDate;
}




int _tmain(int argc, _TCHAR* argv[])
{
	//directory_exists("c:\\work\\time.txt");
	
	
	FileInformation my("c:\\work\\time.txt");
	my.waitForFileToChange();
	int aa=1;

	return 0;
}

