// NetFileSpy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <string>
#include <lm.h>
#include "comutil.h"
#include <atlbase.h>
#include <atlconv.h>
#include <queue>
#include <set>
#include <fstream>      // std::ifstream
#include <sstream>      // std::stringstream
#include <time.h>
#include "../../SqLite/SqLite/sqlite3.h"

#pragma comment(lib, "mpr.lib")

HANDLE hOut;
CONSOLE_SCREEN_BUFFER_INFO bInfo;
char charProgress[80] = 
{"............................................................................"};
//{"================================================================"};
char spaceProgress = ' ';




struct FileInfo
{
	std::string CreationTime ;
	std::string LastAccessTime ;
	std::string LastWriteTime ;
	unsigned long FileSize ;
	std::string FileName;
	std::string AlternateFileName;
	std::string FileAttributes;
	std::string FileDir;
};

typedef std::queue<FileInfo> FileInfo_queue;

typedef std::queue<std::string> string_queue;
typedef std::set<std::string> string_set;


string_queue g_ShareList;
string_set g_ShareListSet;
HANDLE g_NetworkThreadHandle;
HANDLE g_DriveSearchThreadHandle;
sqlite3 *g_pSqlDB;
FileInfo_queue g_InfoQue;
std::string g_RunTimeDate;

#define DB_FILENAME "files.db"


BOOL WINAPI EnumerateFunc(LPNETRESOURCE lpnr);
void AddShare( std::string ShareToAdd );
bool IsOkDir( std::string &fullpath );
void FindHiddenSharesOnServer( std::string ServerName);
std::string GetNextItem();
void CloseDriveSearchThread();
int ProgressBar(char *task, int row, int progress);

unsigned _stdcall DriveSearchThreadProc(void *data);
unsigned _stdcall FindSharesThreadProc(void *data);
void CloseNetworkThread();
void CloseThread( HANDLE &hThreadToClose );
bool ListDirectoryContents(const wchar_t *sDir);

std::string ConvertFileTImeDate(const FILETIME FileTime);
std::string ConvertFileTImeDateSqLite(const FILETIME FileTime);
void StdOut( FileInfo &CurrentFileInfo );
void WriteToFile(FileInfo &CurrentFileInfo );
void PopulateFileInfo( FileInfo &CurrentFileInfo, WIN32_FIND_DATA &fdFile, std::string LastDir );
void AtomicCommitQue();
int InitDb();
void SetRunTimeDate();

std::string FloatToStdString(float number);
std::string unsignedLongToStdString(unsigned long number);
bool fexists(const char *filename);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);



int _tmain(int argc, _TCHAR* argv[])
{
	g_pSqlDB= NULL;	
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hOut, &bInfo);
	char *zErrMsg = 0;

	bool NewFile = ! fexists(DB_FILENAME);

	int  rc;
	rc = sqlite3_open(DB_FILENAME, &g_pSqlDB);
	if(NewFile)
	{
		rc = InitDb();
	}

	SetRunTimeDate();



	std::cout << "\n";

	//ListDirectoryContents(L"\\\\tsclient\\K\\Employee_Dropboxes\\1.Archive\\Laura_Aurilio\\Laura's laptop archive - sept 19 2011\\Sales Meeting 2011 DVD");

	LPNETRESOURCE lpnr = NULL;
	g_NetworkThreadHandle =  (HANDLE)_beginthreadex(NULL, 0,FindSharesThreadProc, NULL, 0, NULL);
	g_DriveSearchThreadHandle =  (HANDLE)_beginthreadex(NULL, 0,DriveSearchThreadProc, NULL, 0, NULL);
	int bar=0;
	while(g_NetworkThreadHandle||g_DriveSearchThreadHandle)

	{
			ProgressBar("Working", 0, bar);
			Sleep(100);
			bar++;
			if (bar>100)
				bar=0;

	}

	 AtomicCommitQue();

	return 0;
}
std::string GetNextItem()
{
	std::string NextDir = g_ShareList.front();
	g_ShareList.pop();
	return NextDir;

}

void CloseDriveSearchThread()
{
	CloseThread(g_DriveSearchThreadHandle);

}


bool ListDirectoryContents(const wchar_t *sDir)
{
	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;
	wchar_t  sPath[4096];
	std::string LastDir=CW2A(sDir);

	int aa = MAX_PATH;
	int bb= wcslen(sPath);

	//Specify a file mask. *.* = We want everything!
	wsprintf (sPath, L"%s\\*.*", sDir);

	if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
	{
		//printf("Path not found: [%s]\n", sDir);
		return false;
	}
	do
	{
		//Find first file will always return "."
		//    and ".." as the first two directories.
		if(wcscmp(fdFile.cFileName, L".") != 0
			&& wcscmp(fdFile.cFileName, L"..") != 0)
		{
			//Build up our file path using the passed in
			//  [sDir] and the file/folder name we just found:
			wsprintf(sPath, L"%s\\%s", sDir, fdFile.cFileName);




			FileInfo CurrentFileInfo;

			PopulateFileInfo(CurrentFileInfo, fdFile, LastDir);


			//Is the entity a File or Folder?
			if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				//printf("Directory: %s\n",  ATL::CT2A(sPath));
				ListDirectoryContents(sPath); //Recursion, I love it!
			}
			else{

				//printf("File: %s", ATL::CT2A(sPath));

				//StdOut(CurrentFileInfo);

				g_InfoQue.push(CurrentFileInfo);



				if(g_InfoQue.size()>1000)
				{
					AtomicCommitQue();

				}


			}
		}
	}
	while(FindNextFile(hFind, &fdFile)); //Find the next file.

	FindClose(hFind); //Always, Always, clean things up!

	return true;
}



unsigned _stdcall DriveSearchThreadProc(void *data)
{
	unsigned int result = 0;


	int count= g_ShareList.size();

	while (g_NetworkThreadHandle!=NULL || count>0)
	{
		if(count>0)
		{
			std::string NextDir = GetNextItem();
			BSTR b = _com_util::ConvertStringToBSTR(NextDir.c_str());
			LPWSTR lp = b;
			//pCEnumNetworkDlg->SetCurrentSearchDir(lp);
			ListDirectoryContents(lp);
			SysFreeString(b);
			//pCEnumNetworkDlg->AddCompleteDir(lp);
			printf("Dir Complete: %s\n",ATL::CT2A(lp));
		}
		count=g_ShareList.size();

	}



	CloseDriveSearchThread();


	return result ;
}



unsigned _stdcall FindSharesThreadProc(void *data)
{
	unsigned int result = 0;
	//int x =*(int*)data;

	LPNETRESOURCE lpnr = NULL;
	if (EnumerateFunc(lpnr) == FALSE) 
	{
		printf("Call to EnumerateFunc failed\n");
		result = false;
	}
	CloseNetworkThread();


	return result ;
}

void CloseNetworkThread()
{
	CloseThread(g_NetworkThreadHandle);
}


void CloseThread( HANDLE &hThreadToClose )
{
	if(hThreadToClose)
	{
		CloseHandle(hThreadToClose);
		hThreadToClose = NULL;
	}
}


BOOL WINAPI EnumerateFunc(LPNETRESOURCE lpnr)
{
	DWORD dwResult, dwResultEnum;
	HANDLE hEnum;
	DWORD cbBuffer = 16384;     // 16K is a good size
	DWORD cEntries = -1;        // enumerate all possible entries
	LPNETRESOURCE lpnrLocal;    // pointer to enumerated structures
	DWORD i;
	//
	// Call the WNetOpenEnum function to begin the enumeration.
	//
	dwResult = WNetOpenEnum(RESOURCE_GLOBALNET, // all network resources
		RESOURCETYPE_DISK,   // all resources
		0,  // enumerate all resources
		lpnr,       // NULL first time the function is called
		&hEnum);    // handle to the resource

	if (dwResult != NO_ERROR) {
		//printf("WnetOpenEnum failed with error %d\n", dwResult);
		return FALSE;
	}
	//
	// Call the GlobalAlloc function to allocate resources.
	//
	lpnrLocal = (LPNETRESOURCE) GlobalAlloc(GPTR, cbBuffer);
	if (lpnrLocal == NULL) {
		//printf("WnetOpenEnum failed with error %d\n", dwResult);
		//      NetErrorHandler(hwnd, dwResult, (LPSTR)"WNetOpenEnum");
		return FALSE;
	}

	do {

		//
		// Initialize the buffer.
		//
		ZeroMemory(lpnrLocal, cbBuffer);
		//
		// Call the WNetEnumResource function to continue
		//  the enumeration.
		//
		dwResultEnum = WNetEnumResource(hEnum,  // resource handle
			&cEntries,      // defined locally as -1
			lpnrLocal,      // LPNETRESOURCE
			&cbBuffer);     // buffer size
		//
		// If the call succeeds, loop through the structures.
		//
		if (dwResultEnum == NO_ERROR) {
			for (i = 0; i < cEntries; i++) {
				std::string fullpath=ATL::CT2A(lpnrLocal[i].lpRemoteName);

				if(lpnrLocal[i].dwDisplayType==RESOURCEDISPLAYTYPE_SHARE)
				{


					AddShare(fullpath);
				}

				//also check for Hidden Shares.
				FindHiddenSharesOnServer(fullpath);


				// If the NETRESOURCE structure represents a container resource, 
				//  call the EnumerateFunc function recursively.

				if (RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage
					& RESOURCEUSAGE_CONTAINER))
					//          if(!EnumerateFunc(hwnd, hdc, &lpnrLocal[i]))
					if (!EnumerateFunc(&lpnrLocal[i]))
					{
						//printf("EnumerateFunc returned FALSE\n");
					}
					//            TextOut(hdc, 10, 10, "EnumerateFunc returned FALSE.", 29);
			}
		}
		// Process errors.
		//
		else if (dwResultEnum != ERROR_NO_MORE_ITEMS) {
			//printf("WNetEnumResource failed with error %d\n", dwResultEnum);

			//      NetErrorHandler(hwnd, dwResultEnum, (LPSTR)"WNetEnumResource");
			break;
		}
	}
	//
	// End do.
	//
	while (dwResultEnum != ERROR_NO_MORE_ITEMS);
	//
	// Call the GlobalFree function to free the memory.
	//
	GlobalFree((HGLOBAL) lpnrLocal);
	//
	// Call WNetCloseEnum to end the enumeration.
	//
	dwResult = WNetCloseEnum(hEnum);

	if (dwResult != NO_ERROR) {
		//
		// Process errors.
		//
		//printf("WNetCloseEnum failed with error %d\n", dwResult);
		//    NetErrorHandler(hwnd, dwResult, (LPSTR)"WNetCloseEnum");
		return FALSE;
	}

	return TRUE;
}

void AddShare( std::string ShareToAdd )
{

	if(!IsOkDir(ShareToAdd))
		return;

	if(g_ShareListSet.find(ShareToAdd)==g_ShareListSet.end())
	{
		g_ShareList.push(ShareToAdd);
		g_ShareListSet.insert(ShareToAdd);

		BSTR b = _com_util::ConvertStringToBSTR(ShareToAdd.c_str());
		LPWSTR lp = b;

		//m_ListBoxShareList.AddString(lp);

		SysFreeString(b);



		std::ofstream file;
		file.open("DriveShares.txt", std::fstream::out | std::fstream::app);
		if(file.is_open())
		{
			file << ShareToAdd;
			file << "\n";
			file.close();
		}			

	}


}

void FindHiddenSharesOnServer( std::string ServerName)
{
	// TODO: Add your control notification handler code here

	PSHARE_INFO_502 BufPtr,p;

	NET_API_STATUS res;
	DWORD er=0,tr=0,resume=0, i;

	while( ServerName[0] == '\\')
	{	
		ServerName.erase(0,1);
	}
	//
	// Print a report header.
	//
	//printf("Share:              Local Path:                   Uses:   Descriptor:\n");
	//printf("---------------------------------------------------------------------\n");
	//
	// Call the NetShareEnum function; specify level 502.
	//
	do // begin do
	{


		BSTR b = _com_util::ConvertStringToBSTR(ServerName.c_str());
		LPWSTR lp = b;

		res = NetShareEnum (b, 502, (LPBYTE *) &BufPtr, MAX_PREFERRED_LENGTH, &er, &tr, &resume);

		SysFreeString(b);

		//
		// If the call succeeds,
		//
		if(res == ERROR_SUCCESS || res == ERROR_MORE_DATA)
		{
			p=BufPtr;
			//
			// Loop through the entries;
			//  print retrieved data.
			//
			for(i=1;i<=er;i++)
			{
				//printf("%-20S%-30S%-8u",p->shi502_netname, p->shi502_path, p->shi502_current_uses);
				std::string computer= ServerName;
				std::string share=ATL::CT2A(p->shi502_netname);
				computer = "\\\\" + computer + "\\";
				std::string fullpath=computer+share;

				AddShare(fullpath);

				//
				// Validate the value of the 
				//  shi502_security_descriptor member.
				//
				if (IsValidSecurityDescriptor(p->shi502_security_descriptor))
				{
					//printf("Yes\n");
				}
				else
				{
					//printf("No\n");
				}
				p++;
			}
			//
			// Free the allocated buffer.
			//
			NetApiBufferFree(BufPtr);
		}
		//else 
		//	printf("Error: %ld\n",res);
	}
	// Continue to call NetShareEnum while 
	//  there are more entries. 
	// 
	while (res==ERROR_MORE_DATA); // end do
	return;
}


bool IsOkDir( std::string &fullpath )
{
	//List of dir's we want to avoid.
	// \\FIT\ADMIN$
	// \\FIT\IPC$
	// \\DC01\NETLOGON
	// \\DC01\SYSVOL


	//Check for Dir's we don't want.
	int stringpos =	fullpath.find("IPC$");
	if(stringpos<0)
		stringpos =	fullpath.find("ADMIN$");
	if(stringpos<0)
		stringpos =	fullpath.find("NETLOGON");
	if(stringpos<0)
		stringpos =	fullpath.find("SYSVOL");

	return (stringpos==-1);
}

std::string ConvertFileTImeDate(const FILETIME FileTime)
{

	SYSTEMTIME CreationTime;
	FileTimeToSystemTime (&FileTime, &CreationTime);

	char szDate[200];

	sprintf_s(szDate,sizeof(szDate),"%02d:%02d:%02d %02d-%02d-%02d",CreationTime.wHour,CreationTime.wMinute,CreationTime.wSecond,CreationTime.wMonth,CreationTime.wDay,CreationTime.wYear);


	return szDate;
}

std::string ConvertFileTImeDateSqLite(const FILETIME FileTime)
{

	SYSTEMTIME CreationTime;
	FileTimeToSystemTime (&FileTime, &CreationTime);

	char szDate[200];

	sprintf_s(szDate,sizeof(szDate),"%02d-%02d-%02d %02d:%02d:%02d",
		CreationTime.wYear,
		CreationTime.wMonth,
		CreationTime.wDay,
		CreationTime.wHour,
		CreationTime.wMinute,
		CreationTime.wSecond);
	
	return szDate;
}

void StdOut( FileInfo &CurrentFileInfo )
{
	std::cout << CurrentFileInfo.FileDir;
	std::cout << ",";
	std::cout << CurrentFileInfo.FileName;
	std::cout << ",";
	std::cout << CurrentFileInfo.FileSize;
	std::cout << ",";
	std::cout << CurrentFileInfo.FileAttributes;
	std::cout << ",";
	std::cout << CurrentFileInfo.CreationTime;
	std::cout << ",";
	std::cout << CurrentFileInfo.LastAccessTime;
	std::cout << ",";
	std::cout << CurrentFileInfo.LastWriteTime;
	std::cout << ",";
	std::cout << CurrentFileInfo.AlternateFileName;
	std::cout << "\n";
}

void WriteToFile( FileInfo &CurrentFileInfo )
{
	std::ofstream file;

	std::string filename = g_RunTimeDate;

	file.open(g_RunTimeDate.c_str(), std::fstream::out | std::fstream::app);
	if(file.is_open())
	{

		std::string TempPath ;
		file << CurrentFileInfo.FileDir;
		file << ",";
		file << CurrentFileInfo.FileName;
		file << ",";
		file << CurrentFileInfo.FileSize;
		file << ",";
		file << CurrentFileInfo.FileAttributes;
		file << ",";
		file << CurrentFileInfo.CreationTime;
		file << ",";
		file << CurrentFileInfo.LastAccessTime;
		file << ",";
		file << CurrentFileInfo.LastWriteTime;
		file << ",";
		file << CurrentFileInfo.AlternateFileName;
		file << "\n";
		file.close();
	}
}




/*
* show a progress in the [row] line
* row start from 0 to the end
*/
int ProgressBar(char *task, int row, int progress)
{
	char str[100];
	int len, barLen,progressLen;
	COORD crStart, crCurr;
	GetConsoleScreenBufferInfo(hOut, &bInfo);
	crCurr = bInfo.dwCursorPosition; //the old position
	len = bInfo.dwMaximumWindowSize.X;
	barLen = len - 17;//minus the extra char
	progressLen = (int)((progress/100.0)*barLen);
	crStart.X = 0;
	crStart.Y = row;

	sprintf_s(str,sizeof(str),"%-10s[%-.*s>%*c]", task,progressLen,charProgress, barLen-progressLen,spaceProgress,50);
	SetConsoleCursorPosition(hOut, crStart);
	printf("%s\n", str);
	SetConsoleCursorPosition(hOut, crCurr);
	return 0;
}

/**
* FUNCTION PopulateFileInfo*
* @brief Fill the 
*
* @version 1.0 
*
* @author David Libby
* @date 1/13/2014 8:39:20 AM
*
* @param CurrentFileInfo 
* @param fdFile 
* @param LastDir 
*/
void PopulateFileInfo( FileInfo &CurrentFileInfo, WIN32_FIND_DATA &fdFile, std::string LastDir )
{
	CurrentFileInfo.CreationTime = ConvertFileTImeDateSqLite(fdFile.ftCreationTime );
	CurrentFileInfo.LastAccessTime = ConvertFileTImeDateSqLite(fdFile.ftLastAccessTime );
	CurrentFileInfo.LastWriteTime = ConvertFileTImeDateSqLite(fdFile.ftLastWriteTime );
	CurrentFileInfo.FileSize= (fdFile.nFileSizeHigh * (MAXDWORD+1)) + fdFile.nFileSizeLow;
	CurrentFileInfo.FileName = ATL::CT2A(fdFile.cFileName);
	CurrentFileInfo.AlternateFileName = ATL::CT2A(fdFile.cAlternateFileName);
	CurrentFileInfo.FileDir=LastDir;

	char rep = 39;

	std::replace(CurrentFileInfo.FileDir.begin(), CurrentFileInfo.FileDir.end(), rep, '`');
	std::replace(CurrentFileInfo.FileName.begin(), CurrentFileInfo.FileName.end(), rep, '`');
	std::replace(CurrentFileInfo.AlternateFileName.begin(), CurrentFileInfo.AlternateFileName.end(), rep, '`');

	//std::replace(CurrentFileInfo.FileDir.begin(), CurrentFileInfo.FileDir.end(), 39, '`');

	//CurrentFileInfo.FileDir.replace(CurrentFileInfo.FileDir.find("'"), 1, "");
	//CurrentFileInfo.FileName.replace(CurrentFileInfo.FileName.find("'"), 1, "");
	//CurrentFileInfo.AlternateFileName.replace(CurrentFileInfo.AlternateFileName.find("'"), 1, "");

	if (fdFile.dwFileAttributes &FILE_SHARE_READ){ CurrentFileInfo.FileAttributes += "FILE_SHARE_READ ";}
	if (fdFile.dwFileAttributes &FILE_SHARE_WRITE){ CurrentFileInfo.FileAttributes += "FILE_SHARE_WRITE ";}
	if (fdFile.dwFileAttributes &FILE_SHARE_DELETE){ CurrentFileInfo.FileAttributes += "FILE_SHARE_DELETE ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_READONLY){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_READONLY ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_HIDDEN){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_HIDDEN ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_SYSTEM){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_SYSTEM ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_DIRECTORY ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_ARCHIVE){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_ARCHIVE ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_DEVICE){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_DEVICE ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_NORMAL){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_NORMAL ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_TEMPORARY){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_TEMPORARY ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_SPARSE_FILE){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_SPARSE_FILE ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_REPARSE_POINT){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_REPARSE_POINT ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_COMPRESSED){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_COMPRESSED ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_OFFLINE){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_OFFLINE ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_NOT_CONTENT_INDEXED){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_NOT_CONTENT_INDEXED ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_ENCRYPTED){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_ENCRYPTED ";}
	if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_VIRTUAL){ CurrentFileInfo.FileAttributes += "FILE_ATTRIBUTE_VIRTUAL ";}
}

int InitDb()
{
	char * zErrMsg;
	int rc;
	std::string InitDb = "CREATE TABLE 'SearchResults' ('Run' DATETIME, 'CreationTime' DATETIME, 'LastAccessTime' DATETIME, 'LastWriteTime' DATETIME, 'FileSizekB' FLOAT, 'FileName' TEXT, 'AlternateFileName' TEXT, 'FileAttributes' TEXT, 'FileDir' TEXT);";
	rc = sqlite3_exec(g_pSqlDB, InitDb.c_str(), callback, 0, &zErrMsg);

	InitDb = "CREATE  VIEW 'main'.'Directory Size KBytes' AS SELECT Run ,FileDir,sum(FileSizeKb) as Kbytes  FROM SearchResults group by Run,FileDir order by run,Kbytes desc;";
	sqlite3_exec(g_pSqlDB, InitDb.c_str(), callback, 0, &zErrMsg);

	InitDb = "CREATE  VIEW 'main'.'Directory Size KBytes Round' AS SELECT Run ,FileDir,ROUND(sum(FileSizeKb),2) as Kbytes  FROM SearchResults group by Run,FileDir order by run,Kbytes desc;";
	sqlite3_exec(g_pSqlDB, InitDb.c_str(), callback, 0, &zErrMsg);

	InitDb = "CREATE  VIEW 'main'.'Directory Size MBytes' AS SELECT Run ,FileDir,sum(FileSizeKb)/1024 as Mbytes  FROM SearchResults group by Run,FileDir order by run,Mbytes desc;";
	sqlite3_exec(g_pSqlDB, InitDb.c_str(), callback, 0, &zErrMsg);

	InitDb = "CREATE  VIEW 'main'.'Directory Size MBytes Round' AS SELECT Run ,FileDir,ROUND(sum(FileSizeKb)/1024,2) as Mbytes  FROM SearchResults group by Run,FileDir order by run,Mbytes desc;";
	sqlite3_exec(g_pSqlDB, InitDb.c_str(), callback, 0, &zErrMsg);



	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(g_pSqlDB));
		exit(0);
	}else{
		//fprintf(stdout, "Opened database successfully\n");
	}
	return rc;
}

void AtomicCommitQue()
{
	if(g_InfoQue.size()<1)
		return;

	 //begin the Atomic commit into the db.
	 sqlite3_exec(g_pSqlDB, "BEGIN", 0, 0, 0);

	while (g_InfoQue.size()>0)
	{
		FileInfo tempItem = g_InfoQue.front();
		g_InfoQue.pop();

		char * zErrMsg;
		int rc;
		std::string InitDb = "INSERT INTO 'SearchResults' ('Run','CreationTime','LastAccessTime','LastWriteTime','FileSizekB','FileName','AlternateFileName','FileAttributes','FileDir') VALUES (";

		//YYYY-MM-DD HH:MM:SS
		InitDb +="'"+g_RunTimeDate+"',";//RunTimDate..


		float filesizekB=(float)tempItem.FileSize/(float)1024;


		InitDb +="'"+tempItem.CreationTime+"',";
		InitDb +="'"+tempItem.LastAccessTime+"',";
		InitDb +="'"+tempItem.LastWriteTime+"',";
		InitDb +="'"+FloatToStdString(filesizekB)+"',";
		InitDb +="'"+tempItem.FileName+"',";
		InitDb +="'"+tempItem.AlternateFileName+"',";
		InitDb +="'"+tempItem.FileAttributes+"',";
		InitDb +="'"+tempItem.FileDir+"');";

		

		int aa=1;
		rc = sqlite3_exec(g_pSqlDB, InitDb.c_str(), callback, 0, &zErrMsg);
		if( rc!=SQLITE_OK ) 
		{
			int problem=1;
		}
	}
	sqlite3_exec(g_pSqlDB, "COMMIT", 0, 0, 0);

}

void SetRunTimeDate()
{
	time_t     now = time(0);
	struct tm  tstruct;
	localtime_s(&tstruct,  &now);

	char szDate[200];

	sprintf_s(szDate,sizeof(szDate),"%02d-%02d-%02d %02d:%02d:%02d",
		tstruct.tm_year+ 1900,
		tstruct.tm_mon+1,
		tstruct.tm_mday,
		tstruct.tm_hour,
		tstruct.tm_min,
		tstruct.tm_sec);
	g_RunTimeDate=szDate;
}
std::string FloatToStdString(float number)
{

	std::stringstream ss;
	ss << number;
	return ss.str() ;
}

std::string unsignedLongToStdString(unsigned long number)
{

	std::stringstream ss;
	ss << number;
	return ss.str() ;
}


bool fexists(const char *filename)
{
	std::ifstream ifile(filename);
	return ifile.good();
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	return 0;
}


