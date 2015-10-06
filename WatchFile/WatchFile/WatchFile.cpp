// WatchFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include "VersaCom.h"
#include <iostream>       // std::cout

VersaCom g_Versa;

int _tmain(int argc, _TCHAR* argv[])
{
	
	g_Versa.setFileName("c:\\work\\time.txt");
	VersaReturn ret = g_Versa.startWorkingThread();

	if(ret!=VERSA_OK)
		return 1;

	int ct=0;
	while(1)
	{
		int aa=1;
		Sleep (1000);
		ct++;
		//std::cout << "_tmain Ticks = " << ct << "\n";	
	}

	return 0;
}

