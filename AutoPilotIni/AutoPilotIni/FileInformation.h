#ifndef FILE_INFO_CLASS
#define FILE_INFO_CLASS

#include <string>
#include "Windows.h"

//! Return types in the wait function.
enum FileInfoWaitResult
{
	FILE_INFO_WAIT_PAUSE
	,FILE_INFO_WAIT_DIFFRENCE

};
/*!
* @brief FileInformation THis class is meant to collect metadata information abbout a file 
*
* The FileInformation class provides methods to collect metadata about a file ir the change dates and times
* and watch a file for a change of state.
*
*/

class FileInformation
{
public:
	//! constructor requires a valid filename.
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
	FileInfoWaitResult FileInformation::waitForFileToChange(int msSleep=100);

protected:
private:
	SYSTEMTIME m_creationTime;
	SYSTEMTIME m_lastAccessTime;
	SYSTEMTIME m_lastWriteTime;
	std::string m_fileAttrib;
	std::string m_fileName;
	unsigned long m_filesize;
	std::string m_strLastWrite;

};

#endif //FILE_INFO_CLASS