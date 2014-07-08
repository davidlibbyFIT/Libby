// StrokeCheck.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int GetMsDelayForSelenoidPump(double MlPerMinute);


int _tmain(int argc, _TCHAR* argv[])
{
	

	int DelayMs=0;

	double MlPerMinute=2.8;
	
	DelayMs = GetMsDelayForSelenoidPump(MlPerMinute);
	DelayMs = GetMsDelayForSelenoidPump(3);
	DelayMs = GetMsDelayForSelenoidPump(2);
	DelayMs = GetMsDelayForSelenoidPump(1);
	DelayMs = GetMsDelayForSelenoidPump(2.5);
	DelayMs = GetMsDelayForSelenoidPump(1.5);
	return 0;
}

int GetMsDelayForSelenoidPump(double MlPerMinute)
{
	const int msPerStroke=1030;//Magic time it takes to complete one stroke thru Distribution board;
	const int uLPerStroke=50;//Size of the Pump

	int ulVolume=(int)(MlPerMinute*1000.00);//convert to uL
	int StrokeCount=ulVolume/uLPerStroke;//Get Stroke Count
	int strokesPerMs=60000/StrokeCount;//60000 represents # of ms per minute

	int MsDelay= strokesPerMs-msPerStroke;
	if(MsDelay<0)
		MsDelay=0;
	
	return MsDelay;//Remove Delay in the Distribution board.
}

