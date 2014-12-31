// NumberCrush.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <map>
#include <locale> //Toupper
#include <climits> //unsigned long limit ULONG_MAX
#include <math.h>       /* pow */

#include "FeatureKeyHandler.h"


////////////////////Generic FIle IO should compile in windows and linux

#include <sys/types.h>
#include <sys/stat.h>

FeatureKeyHandler m_myFeatHandler;

#define _WINSOCKAPI_

#include <Shlobj.h>

void reg()
{
	

	// Roaming AppData - Vista and greater
	PWSTR wszPath;
	// http://blogs.msdn.com/b/patricka/archive/2010/03/18/where-should-i-store-my-data-and-configuration-files-if-i-target-multiple-os-versions.aspx

	if(SHGetKnownFolderPath ( FOLDERID_ProgramData, KF_FLAG_CREATE,NULL, &wszPath )!=E_FAIL)

	{

		printf("\nSHGetKnownFolderPath FOLDERID_RoamingAppData    =%S\n", wszPath);


	}

}



/**
* FUNCTION file_directory_permission
*
* @brief Checks Permission on Files and Directories.
*
* @author DAVID.LIBBY
* @date 12/5/2014 2:41:15 PM
*
* @param path file\path location
* @param Permission 
*
*		S_IFMT		File type mask
*		S_IFDIR		is Directory
*		S_IFCHR		Character special (indicates a device if set)
*		S_IFREG		Regular
*		S_IREAD		Permission to read
*		S_IWRITE	Permission to write
*		S_IEXEC		Execute/search permission, owner
*
*		S_IREAD & S_IWRITE == Read Write.
*
*
* @return bool 
*/
bool file_directory_permission(const std::string path,unsigned short Permission )
{
	struct stat info;

	if(stat( path.c_str(), &info ) == 0)
	{

		if(info.st_mode & Permission)
		{
			return true;
		}
	}

	return false;

}


bool file_directory_exists(const std::string &path )
{
	return file_directory_permission(path,S_IFDIR);
}


//End ////////////////////Generic FIle IO should compile in windows and linux



//Intentional removal of o to keep it from getting confused with 0
//This contains 34 chars the should not changes these are used for Key generation.
const char LookupTable[]=	{'1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H',
	'I','J','K','L','M','N','P','Q','R','S','T','U','V','W','X','Y','Z'};

#define TABLE_COUNT  ( sizeof(LookupTable) / sizeof(LookupTable[ 0 ]) )
#define MAX_CHAR 6 //We will limit to 6 chars so the largest number is ZZZZZZ ( 1,544,804,415 )


bool StdStringToInt(const std::string &inString,unsigned long &retInt);
bool GetIntValueFromItem(char SingleChar, unsigned long &retvalue);
bool GetCharValueFromInt(unsigned long number,char &retChar);
bool IntToStdString(const unsigned long &inInt,std::string &retString);


//Mac Address crap
#include <vector>
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "IPHLPAPI.lib")



void getdMacAddresses(std::vector<std::string> &vMacAddresses)
{
	vMacAddresses.clear();
	IP_ADAPTER_INFO AdapterInfo[32];       // Allocate information for up to 32 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer
	DWORD dwStatus = GetAdaptersInfo(      // Call GetAdapterInfo
		AdapterInfo,                 // [out] buffer to receive data
		&dwBufLen);                  // [in] size of receive data buffer

	//No network card? Other error?
	if(dwStatus != ERROR_SUCCESS)
		return;

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
	char szBuffer[512];
	while(pAdapterInfo)
	{
		if(pAdapterInfo->Type == MIB_IF_TYPE_ETHERNET)
		{
			sprintf_s(szBuffer, sizeof(szBuffer), "%.2x-%.2x-%.2x-%.2x-%.2x-%.2x"
				, pAdapterInfo->Address[0]
			, pAdapterInfo->Address[1]
			, pAdapterInfo->Address[2]
			, pAdapterInfo->Address[3]
			, pAdapterInfo->Address[4]
			, pAdapterInfo->Address[5]
			);
			vMacAddresses.push_back(szBuffer);
		}
		pAdapterInfo = pAdapterInfo->Next;

	}
}

#include <stdio.h>

void addKeyFromFileName(std::string &justFileName, std::string &fileAndDirName);

bool loadKeysFromDir(const std::string keyDir)
{
	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;
	wchar_t  sPath[2048];

	std::wstring wkeyDir(keyDir.begin(), keyDir.end());

	//Specify a file mask. *.* = We want everything!
	wsprintf (sPath, L"%s\\*.key", wkeyDir.c_str());

	if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
	{
		printf("Path not found: [%s]\n", wkeyDir.c_str());
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
			wsprintf(sPath, L"%s\\%s", wkeyDir.c_str(), fdFile.cFileName);

			//Is the entity a File or Folder?
			if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				printf("Directory: %s\n", sPath);
				std::wstring widepath( sPath );
				const std::string newpath( widepath.begin(), widepath.end() );
				loadKeysFromDir(newpath); //Recursion, I love it!
			}
			else
			{
				std::wstring ws( fdFile.cFileName );
				std::string justFileName( ws.begin(), ws.end() );
				ws= sPath ;
				std::string fileAndDirName( ws.begin(), ws.end() );				
				addKeyFromFileName(justFileName, fileAndDirName);

			}
		}
	}
	while(FindNextFile(hFind, &fdFile)); //Find the next file.

	FindClose(hFind); //Always, Always, clean things up!

	return true;
}

/**
* FUNCTION findFilesInDir
*
* @brief Look for files in directory
*
* @author DAVID.LIBBY
* @date 12/11/2014 1:56:19 PM
*
* @param startingDir Directory to search don't put the final \ just keep it clean like c:\work
* @param SearchMask *.* *.reg sec..
* @param filelistString Vector to fill with file names.
*
* @return bool 
*/
bool findFilesInDir(const std::string startingDir,const std::string &SearchMask,std::vector <std::string> &filelistString)
{
	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;
	wchar_t  sPath[2048];

	//Do we have a valid search string
	if(startingDir.length()<1||SearchMask.length()<1)
		return false;
	//Convert std::string to wide string.
	std::wstring wkeyDir(startingDir.begin(), startingDir.end());
	std::wstring wMask(SearchMask.begin(), SearchMask.end());

	//Specify a file mask. *.* = We want everything!
	wsprintf (sPath, L"%s\\%s", wkeyDir.c_str(),wMask.c_str());

	if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
	{
		printf("Path not found: [%s]\n", wkeyDir.c_str());
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
			wsprintf(sPath, L"%s\\%s", wkeyDir.c_str(), fdFile.cFileName);

			//Is the entity a File or Folder?
			if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				std::wstring widepath( sPath );
				const std::string newpath( widepath.begin(), widepath.end() );
				findFilesInDir(newpath,SearchMask,filelistString); //Recursion, I love it!
			}
			else
			{
				std::wstring ws( fdFile.cFileName );
				std::string justFileName( ws.begin(), ws.end() );
				ws= sPath ;
				std::string fileAndDirName( ws.begin(), ws.end() );				
				filelistString.push_back(fileAndDirName);

			}
		}
	}
	while(FindNextFile(hFind, &fdFile)); //Find the next file.

	FindClose(hFind); //Always, Always, clean things up!

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector <std::string> myvec;
	findFilesInDir("C:\\work\\Libby","*.*",myvec);
	reg();

	std::vector<std::string> vMacAddresses;
	getdMacAddresses(vMacAddresses);

	int aa=1;

	std::string code;


	//C:\work\keys

	loadKeysFromDir("c:/work/keys");



	FeatureKeyItem ff;

	if(ff.setFeature("LIB")!=FEATURE_KEY_OK)
	{
		int fail=1;
	}
	if(ff.setExpireDate("000000")!=FEATURE_KEY_OK)
	{
		int fail=1;
	}
	if(ff.setSerial("DAVID")!=FEATURE_KEY_OK)	
	{
		int fail=1;
	}

	FeatuerKeyStatus ret;
	if(ff.vaildateFeatureKey()==FEATURE_KEY_OK)
	{
		if(ff.getEncodeString(code)==FEATURE_KEY_OK)
		{
			ret = m_myFeatHandler.addKeyItem(ff);
			if(ret != FEATURE_KEY_OK)
			{
				int DeleteTHeKeyFIle=1;
			}

		}else
		{
			int fail=1;

		}
	}else
	{
		int fail=1;

	}

	if(ff.setExpireDate("120914")!=FEATURE_KEY_OK)
	{
		int fail=1;
	}
	if(ff.vaildateFeatureKey()==FEATURE_KEY_OK)
	{
		if(ff.getEncodeString(code)==FEATURE_KEY_OK)
		{
			ret = m_myFeatHandler.addKeyItem(ff);
			if(ret !=FEATURE_KEY_OK)
			{
				int DeleteTHeKeyFIle=1;
			}

		}else
		{
			int fail=1;

		}
	}else
	{
		int fail=1;

	}

	FeatureKeyItem gg;

	ret= gg.getEncodeString(code);

	if(gg.DecodeString("EP6M-QYBZ-J4DC-BAZQ")!=FEATURE_KEY_OK)
	{
		int somthingFailedToDecode=1;
	}

	if(gg.DecodeString("NUL0-0000-0000-00FF")!=FEATURE_KEY_OK)
	{
		int aa=1;
	}




	return 0;
}
bool IntToStdString(const unsigned long &inInt,std::string &retString)
{
	retString.clear();

	unsigned long CurrentinInt=inInt;
	//!is it too big?
	if(inInt > ULONG_MAX)
		return false;
	for(int col=MAX_CHAR;col>=0;col--)
	{
		unsigned long MaxSize=(unsigned long)pow((long double)TABLE_COUNT-1,(long double)col)-1;
		if(col == 0 || inInt == MaxSize || inInt / MaxSize >=1)
		{
			for (int charLoc=TABLE_COUNT-1;charLoc>=0;charLoc--)
			{
				char newChar = LookupTable[charLoc];
				unsigned long CharValue=(unsigned long)pow((long double)charLoc,(long double)col+1);

				if( CurrentinInt/CharValue >= 1)
				{
					CurrentinInt-=CharValue;
					char newChar = LookupTable[charLoc];
				}

			}	
		}
		int aa=1;

	}


	return true;
}

bool StdStringToInt(const std::string &inString,unsigned long &retInt)
{

	retInt=0;
	size_t strlength=inString.length();

	//! do we have a string
	if (strlength < 1 || strlength > MAX_CHAR)
		return false;

	//! are all the charters valid
	for(std::string::size_type i = 0; i < inString.size(); ++i) {
		unsigned long dummyInt;
		if(!GetIntValueFromItem(inString[i],dummyInt))
			return false;
	}

	for(size_t index=0;index<strlength;index++)
	{
		unsigned long PosValue=0;
		if(GetIntValueFromItem(inString[index],PosValue))
		{
			unsigned long powerMultiplier=(unsigned long)pow((long double)TABLE_COUNT-1,(long double)strlength-index-1);
			retInt+=powerMultiplier * PosValue;
		}
	}

	return true;
}




bool GetIntValueFromItem(char SingleChar, unsigned long &retvalue)
{
	char upperchar=toupper(SingleChar);
	retvalue = 0;

	for (unsigned long index=0;index < TABLE_COUNT;index++)
	{

		if(upperchar == LookupTable[index] )
		{
			retvalue =  index;
			return true;
		}
	}

	return false;

}
bool GetCharValueFromInt(unsigned long number,char &retChar)
{

	//! bail out we have gone too far.
	if(number >= TABLE_COUNT )
		return false;

	retChar = LookupTable[number];
	return true;

}

void addKeyFromFileName(std::string &justFileName, std::string &fileAndDirName)
{
	//Remove.key from file name.
	justFileName.erase(justFileName.length()-4,4);


	FeatureKeyItem singleKey;

	FeatuerKeyStatus ret = singleKey.DecodeString(justFileName);
	if(ret != FEATURE_KEY_OK)
	{
		if( remove( fileAndDirName.c_str() ) != 0 )
		{
			return;
		}
	}else
	{					
		ret = m_myFeatHandler.addKeyItem(singleKey);
		if(ret != FEATURE_KEY_OK)
		{
			if(ret == FEATURE_KEY_EXPIRED)
			{
				if( remove( fileAndDirName.c_str() ) != 0 )
				{
					return;
				}
			}

		}
	}
}
