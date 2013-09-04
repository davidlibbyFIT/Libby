#pragma once
#include <string>
#include <map>



#define SetDebugTimer() Utils::DebugTimer SecretMacroDebugTimer(__FILE__,__FUNCTION__,__LINE__);


typedef struct DebugMapItem
{
	std::string File;
	std::string Line;
	std::string Function;
	std::string User;
	double Time;
	unsigned long Count;
	bool operator()  (const DebugMapItem &l, const DebugMapItem &r) 
	{ 
		if(l.File>r.File)
		{
			return true;
		}
		else if (l.Function>r.Function)
		{
			return true;
		}
		else if (l.Line>r.Line)
		{
			return true;
		}else  if (l.User.length() > 0 && l.User.length() > 0)
		{
			if (l.User>r.User)
			{
				return true;
			}
		}else
			return false;

	}


	DebugMapItem()
	{
		File.empty();
		Line.empty();
		Function.empty();
		User.empty();
		Time=0.0;
		Count=0;
	};
}DebugMapItem;


static std::map<std::string ,DebugMapItem> g_StaticDebugMap;


namespace Utils
{


	class DebugTimer
	{
	public:
		DebugTimer(char *psUserDefined=0);
		DebugTimer(char *pFile,char *pFunction,long pLine);
		~DebugTimer(void);
		void ReportToDebugTerm();
		void WriteToFile(char * pFilename);

	private:
		float GetSeconds();
		std::string getFileName(const std::string& strPath);
		DebugMapItem m_CurrentLoc;

	};


}

