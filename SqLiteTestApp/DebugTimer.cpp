#include "DebugTimer.h"
#include <windows.h>
#include <sstream>
#include <iostream>
#include <fstream>

namespace Utils
{
	LARGE_INTEGER m_Timer;

	DebugTimer::DebugTimer(char *pFile,char *pFunction,long pLine)
	{
		if(pFile)
		{
			m_CurrentLoc.File = getFileName(std::string(pFile));
		}
		if(pFunction)
		{
			m_CurrentLoc.Function = std::string(pFunction);
		}
		if(pLine>0)
		{
			std::ostringstream ss;
			ss << pLine;
			std::string LineNumber = ss.str();			
			m_CurrentLoc.Line =  LineNumber;
		}
		QueryPerformanceCounter(&m_Timer);


	}

	/**
	* FUNCTION DebugTimer*
	* @brief Class to Setup Debug Timmers for the purpose of looking at how long things are in "Functions"
	*
	* @version 1.0 
	*
	* @author David Libby
	* @date 9/4/2013 3:18:35 PM
	*
	* @param psUserDefine 
	*/
	DebugTimer::DebugTimer(char *psUserDefine)
	{
		QueryPerformanceCounter(&m_Timer);
		if(psUserDefine)
		{
			m_CurrentLoc.User = std::string(psUserDefine);
		}

	}


	/**
	* FUNCTION GetSeconds*
	* @brief Gets the seconds since the class was created.
	*
	* @version 1.0 
	*
	* @author David Libby
	* @date 9/4/2013 3:19:22 PM
	*
	*
	* @return float 
	*/
	float DebugTimer::GetSeconds()
	{
		LARGE_INTEGER now;
		LARGE_INTEGER freq;

		QueryPerformanceCounter(&now);
		QueryPerformanceFrequency(&freq);

		return (now.QuadPart - m_Timer.QuadPart) / static_cast<float>(freq.QuadPart);
	};

	DebugTimer::~DebugTimer(void)
	{

		std::string MyLocation=m_CurrentLoc.File + m_CurrentLoc.Function + m_CurrentLoc.Line + m_CurrentLoc.User;		

		m_CurrentLoc.Time=g_StaticDebugMap[MyLocation].Time+GetSeconds();
		m_CurrentLoc.Count=g_StaticDebugMap[MyLocation].Count+1;

		if(g_StaticDebugMap.size()==0)
		{
			int aa=1;
		}
		if(g_StaticDebugMap[MyLocation].Count<1)
		{
			int a=1;
		}

		g_StaticDebugMap[MyLocation]=m_CurrentLoc;
	}
	/**
	* FUNCTION ReportToDebugTerm*
	* @brief Report to Output window in debug mode.
	*
	* @version 1.0 
	*
	* @author David Libby
	* @date 9/4/2013 3:17:25 PM
	*
	*/
	void DebugTimer::ReportToDebugTerm()
	{

#ifdef _DEBUG
		if(g_StaticDebugMap.size()<1)
			return;

		typedef std::map<std::string ,DebugMapItem>::iterator it_type;
		OutputDebugStringA(("\n"));
		OutputDebugStringA(("CompileTimeDate,File,Function,Line,UserDefined,Count,Time\n"));
		for(it_type iterator = g_StaticDebugMap.begin(); iterator != g_StaticDebugMap.end(); iterator++) {
			//iterator->first = key
			// iterator->second = value
			DebugMapItem Single = iterator->second;

			std::ostringstream ss;
			ss << iterator->second.Time;
			std::string Time = ss.str();

			std::ostringstream ss2;
			ss2 << iterator->second.Count;
			std::string Count = ss2.str();


			std::string timedate = __TIMESTAMP__;

			std::string OutSting = timedate + "," + iterator->second.File + "," + iterator->second.Function + "," + iterator->second.Line + "," + iterator->second.User + "," + Count + "," + Time + "\n";
			LPSTR lOutString = const_cast<char *>(OutSting.c_str());

			OutputDebugStringA(lOutString);
		}
		OutputDebugStringA(("\n"));
#endif // _DEBUG
	}
	/**
	* FUNCTION getFileName*
	* @brief Strips the Filename from a full path
	*
	* @version 1.0 
	*
	* @author David Libby
	* @date 9/4/2013 3:18:01 PM
	*
	* @param strPath 
	*
	* @return std::string 
	*/
	std::string DebugTimer::getFileName(const std::string& strPath)
	{
		size_t iLastSeparator = 0;
		return strPath.substr((iLastSeparator = strPath.find_last_of("\\")) != std::string::npos ? iLastSeparator + 1 : 0, strPath.size() );
	}


	/**
	* FUNCTION WriteToFile*
	* @brief Write Output to file.
	*
	* @version 1.0 
	*
	* @author David Libby
	* @date 9/4/2013 3:48:28 PM
	*
	* @param pFilename 
	*/
	void DebugTimer::WriteToFile(char * pFilename)
	{
		if(!pFilename)
			return;

		if(g_StaticDebugMap.size()<1)
			return;
		std::string filename(pFilename);
		std::ofstream out;
		out.open(filename);
		//out << input;
		typedef std::map<std::string ,DebugMapItem>::iterator it_type;
		out << "CompileTimeDate,File,Function,Line,UserDefined,Count,Time\n";
		for(it_type iterator = g_StaticDebugMap.begin(); iterator != g_StaticDebugMap.end(); iterator++) {
			//iterator->first = key
			// iterator->second = value
			DebugMapItem Single = iterator->second;

			std::ostringstream ss;
			ss << iterator->second.Time;
			std::string Time = ss.str();

			std::ostringstream ss2;
			ss2 << iterator->second.Count;
			std::string Count = ss2.str();


			std::string timedate = __TIMESTAMP__;

			std::string OutSting = timedate + "," + iterator->second.File + "," + iterator->second.Function + "," + iterator->second.Line + "," + iterator->second.User + "," + Count + "," + Time + "\n";
			LPSTR lOutString = const_cast<char *>(OutSting.c_str());

			out << lOutString;

		}

		out.close();

	}


}

