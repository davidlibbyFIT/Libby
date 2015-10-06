#ifndef FILE_INFO_CLASS
#define FILE_INFO_CLASS

#include <string>
#include "Windows.h"

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

#endif //FILE_INFO_CLASS