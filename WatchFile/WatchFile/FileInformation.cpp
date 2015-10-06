#include "FileInformation.h"


FileInformation::FileInformation(const std::string &fileName):
	m_fileName (fileName)
	,m_filesize (0)
{
	refresh();
}

FileInformation::~FileInformation()
{
}
/**
* FUNCTION refresh
*
* @brief Refresh the File info from the os.
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 3:49:43 PM
*
*/
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
/**
* FUNCTION getCreationTime
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 3:50:07 PM
*
*
* @return SYSTEMTIME 
*/
SYSTEMTIME FileInformation::getCreationTime()
{
	return m_creationTime;
}
/**
* FUNCTION getLastAccessTime
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 3:50:13 PM
*
*
* @return SYSTEMTIME 
*/
SYSTEMTIME FileInformation::getLastAccessTime()
{
	return m_lastAccessTime;
}
/**
* FUNCTION getLastWriteTime
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 3:50:16 PM
*
*
* @return SYSTEMTIME 
*/
SYSTEMTIME FileInformation::getLastWriteTime()
{
	return m_lastWriteTime;
}

/**
* FUNCTION getStringCreationTime
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 3:50:20 PM
*
*
* @return std::string 
*/
std::string FileInformation::getStringCreationTime()
{
	return ConvertFileTimeDateToString(m_creationTime);
}
/**
* FUNCTION getStringLastAccessTime
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 3:50:24 PM
*
*
* @return std::string 
*/
std::string FileInformation::getStringLastAccessTime()
{
	return ConvertFileTimeDateToString(m_lastAccessTime);
}
/**
* FUNCTION getStringLastWriteTime
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 3:50:28 PM
*
*
* @return std::string 
*/
std::string FileInformation::getStringLastWriteTime()
{
	return ConvertFileTimeDateToString(m_lastWriteTime);
}


/**
* FUNCTION waitForFileToChange
*
* @brief THis function will wait until the file has changed. Please be caerfull as this can lock up the UI 
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 3:50:32 PM
*
* @param msSleep 
*/
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


/**
* FUNCTION GetStringAttributes
*
* @brief Returned a friendly String with the file attributes.
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 4:12:57 PM
*
* @param fileInfo 
*
* @return std::string 
*/
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

/**
* FUNCTION ConvertFileTimeDateToString
*
* @brief Convert time tate to string.
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 4:13:27 PM
*
* @param FileTime 
*
* @return std::string 
*/
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

