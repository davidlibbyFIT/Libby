// StradusLaserStatus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <CommCtrl.h>
#include "resource.h"
#include <memory>
#include "StradusUSB.h"
#include "LaserStatusDialog.h"

LaserStatusDialog g_LaserDialog;


std::shared_ptr <StradusObserver> g_pStratusObserver;

int CallBackFunction(StradusObserver::StradusData recivedData)
{
	
	bool on=recivedData.laserOn;

	//Scott is using Digital Modulation to control the laser so it is always "On" and when idle power is set to 0
	//THis allows for faster startup times.. From the Manual "A maximum digital modulation bandwidth of 50 KHz"
	// 	Digital Modulation
	// 		The Vortran Stradus™ is available with optional digital modulation. By inputting
	// 		standard TTL voltage levels into the SMB connector, the laser output can be
	// 		modulated from DC to 200MHz. Computer control is required to set the laser in
	// 		“Pulse Mode”. Please refer to section 9 for instructions on computer controlled
	// 		operation.
	// 		8.6.1 Digital Modulation Specifications for Standard Single Mode Lasers
	// 									Min		Typical		Max
	// 		Optical Rise Time (nsec)  	1.0		1.3  		2.0
	// 		Optical Fall Time (nsec)  	1.0		1.6			2.0
	// 		Laser Off Input Voltage  						0.8
	// 		Laser On Input Voltage  	3.5					5.0
	// 		-3dB Bandwidth (MHz)		200		250 
	if(recivedData.powermW==0)
	{
		if(recivedData.faultCode & STRADUS_STANDBY)
		{
			g_LaserDialog.SetLaserState(LASER_DIALOG_LASER_STANDBY);
		}
		else 
		{
			g_LaserDialog.SetLaserState(LASER_DIALOG_LASER_OFF);
		}

	}else
	{
		if(recivedData.faultCode==STRADUS_LASER_EMISSION_ACTIVE)
			g_LaserDialog.SetLaserState(LASER_DIALOG_LASER_ON);
	}

	g_LaserDialog.SetCurrentTempC(recivedData.baseTempCelsius);
	g_LaserDialog.SetPower(recivedData.powermW);
	g_LaserDialog.SetWaveLength(recivedData.laserWavelength);
	if(recivedData.powermW==0)
	{
		g_LaserDialog.SetStatusText("Laser Idle");
	}else
	{
		g_LaserDialog.SetStatusText(recivedData.statusText);
	}


	return 88;
}
int _tmain(int argc, _TCHAR* argv[])
{
	
	InitCommonControls();
	g_pStratusObserver.reset(new StradusObserver);
	g_pStratusObserver->StradusObserverRegisterCallback(&CallBackFunction);
	
	g_LaserDialog.DoModeless();
	//g_LaserDialog.ShowWindow(SW_HIDE);
	//g_LaserDialog.EnableWindow(false);
	
	



	
	while (1)
	{
	
		if(g_LaserDialog.IsVisible())
		{
			BOOL ret;
			MSG msg;
			ret = GetMessage(&msg, 0, 0, 0);
			if(ret != -1)
			{
				if(!IsDialogMessage(g_LaserDialog.m_hWnd, &msg)) 
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

			}
			if(g_LaserDialog.getEmergencyStop())
			{
				g_pStratusObserver->emergencyStopLaser();
			}
		}
	}
		

	return 0;
}




