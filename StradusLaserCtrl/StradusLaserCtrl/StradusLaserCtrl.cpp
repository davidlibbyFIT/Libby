// StradusLaserCtrl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <CommCtrl.h>
#include "resource.h"

#include "LaserStatusDialog.h"
#include "StradusUSB.h"



int _tmain(int argc, _TCHAR* argv[])
{
	
	InitCommonControls();

	LaserStatusDialog myLaser;
	myLaser.DoModal();

	return 0;
}




