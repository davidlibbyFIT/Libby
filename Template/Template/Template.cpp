// Template.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include <CommCtrl.h>
#include "tchar.h"
#include "resource.h"
#include <iostream>
#include "AutoFocusAssistant.h"
#include "ProgressDialog.h"
#include "LaserStatusDialog.h"
#include "BackFlushDlg.h"
#include "PMTDlgCtrl.h"
#include "AutoCamera.h"


#include "Polygon.h"
#include "Duity.h"
#include "HsfcPMT.h"
#include "FeatureRegDlg.h"
#include "HsfcConfig.h"
#include "GoGo.h"
#include "VersaDlg.h"

//#include "DebugTimer.h"


HINSTANCE hInst;
ProgressDialog g_MyProgress;
LaserStatusDialog g_MyLaser;
BackFlushDlg g_MyBackFlush;

HWND g_hProgressDialog = NULL;
PmtDlgCtrl g_PmtCtrl;


AutoCamera g_CameraAuto;
VersaDlg g_Versa;

void LaunchPmtDialog();

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//SetDebugTimer();

	stAFAssistData stDataStruct;

	{
		CPolygon myPolly;
		myPolly.create(4);


		myPolly.setVertex(0,0,0);
		myPolly.setVertex(1,9,0);
		myPolly.setVertex(2,9,9);
		myPolly.setVertex(3,0,9);

		myPolly.setVertex(0,0,0);
		myPolly.setVertex(1,8,0);
		myPolly.setVertex(2,8,8);
		myPolly.setVertex(3,0,8);



		myPolly.setVertex(0,531.00000000000000,403.00000000000000);
		myPolly.setVertex(1,492.00000000000000,403.00000000000000);
		myPolly.setVertex(2,492.00000000000000,364.00000000000000);
		myPolly.setVertex(3,531.00000000000000,364.00000000000000);


		double area = myPolly.area();

		double per = myPolly.perimeter();

		CPolygon::vertex_t Min,Max;
		myPolly.GetMbr(Min,Max);

		int minAngle;
		int maxAngle;

		double maxSize,minSize;
		myPolly.GetFeret(minSize,maxSize,minAngle,maxAngle);


		int gg=1;

	}




	stDataStruct.szFlowCel=_T("FC100");
	stDataStruct.szObjective=_T("10X");
	stDataStruct.SyringeSizeml=12.5;
	stDataStruct.RequiredVolume=5.44;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			
			
			//std::cout << "Function : " << __FUNCTION__ << " : " << __FUNCSIG__ <<std::endl;
			std::cout << __FILE__ << " " << __LINE__<< " " << __FUNCTION__ << std::endl;

			
			//g_MyBackFlush.DoModeless(hDlg);
			//g_MyBackFlush.DoModal();
			//DestroyWindow(hDlg);

			g_MyLaser.SetOnTop(true);
			double NewTempature=22;
			g_MyLaser.SetCurrentTempC(NewTempature);
			//g_MyLaser.DoModeless();

			//DestroyWindow(hDlg);
			break;

		}

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON1:
			stDataStruct.ObjFlowSizeOk=true;
			stDataStruct.ObjFlowVolumeOk=true;
			stDataStruct.ObjEverythingOk=true;

		case IDC_BUTTON2:
			{

				//AutoFocusDialog ggg(stDataStruct);
				//AFAssistant0 ggg(stDataStruct);
				AutoFocusAssistant MyAss(&stDataStruct);
				int ret = MyAss.DoModal();

				switch (ret)
				{
				case AF_WIZ_NEXT:
					{
						std::cout << "AF_WIZ_NEXT" << std::endl;
						break;
					}
				case AF_WIZ_BACK:
					{
						std::cout << "AF_WIZ_BACK" << std::endl;
						break;
					}
				case AF_WIZ_FINISH:
					{
						std::cout << "AF_WIZ_FINISH" << std::endl;
						break;
					}
				case AF_WIZ_CANCEL:
					{
						std::cout << "AF_WIZ_CANCEL" << std::endl;
						break;
					}

				}



				//AutoFocusAssistant MyAss;
				//MyAss.AddDialog(ggg);

				break;
			}
		case IDCANCEL:
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			return TRUE;
			break;
		case IDC_BUTTON_PROGRESS:
			{
				//g_MyProgress.DoModal();
				//g_MyProgress.DoModeless(NULL);
				g_MyProgress.SetRange(0,100);
				g_MyProgress.SetOnTop(TRUE);
				g_MyProgress.DoModeless(hDlg);



				//g_hProgressDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR),hwnd, ToolDlgProc);
				int a=1;
				break;
			}
		case IDC_BUTTON_PMT_LAUNCH:			
			{
				
				//LaunchPmtDialog();
				//g_PmtCtrl.DoModeless(hDlg);

				break;
			}
		case IDC_BUTTON_LASER:
			{
				//g_MyProgress.DoModal();
				//g_MyProgress.DoModeless(NULL);
				g_MyLaser.SetOnTop(false);
				g_MyLaser.DoModeless(hDlg);
				//g_hProgressDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR),hwnd, ToolDlgProc);
				int a=1;
				break;
			}

		}
		break;

	case WM_CLOSE:
		{
			DestroyWindow(hDlg);
		}
		return TRUE;

	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;

	case WM_TIMER:
		{ 
			if ( wParam == 0 ) {
				int aa=1;

				if(g_MyProgress.UserCancel())
				{
					int a=1;
				}
				
					
				

				if(g_MyProgress.IsVisible())
				{
					int pp=g_MyProgress.GetPosition();
					if(g_MyProgress.GetPosition()>=100)
						g_MyProgress.SetPosition(0);

					g_MyProgress.SetPosition(g_MyProgress.GetPosition()+1);
				}
			}
			if ( wParam == 1 ) 
			{
				//int pos = SendMessage(This->progress_handle_2, PBM_GETPOS, 0, 0 ) + 1;
				//SendMessage(This->progress_handle_2, PBM_SETPOS, pos, 0 );
				//if ( pos == 600 ) {
				//    SendMessage(This->progress_handle_2, PBM_SETPOS, 0, 0 );
				// }
			}
			if ( wParam == 2 ) 
			{
				if(!g_MyLaser.IsLaserRunning())
					return 0;

				double curTemp=g_MyLaser.GetCurrentTempC();
				if(curTemp>60.00)
					g_MyLaser.SetCurrentTempC(0.0);
				else
					g_MyLaser.SetCurrentTempC(curTemp+0.1);
			}

			return 0;
		}

	}

	return FALSE;
}


void myFun(char c)
{
	char jp[10];
	for (int i=0; i<10; i++)
	{
		jp[i] = c + i;
	}
	jp[9] = '\0';
	printf("%s\n", jp);
}



int _tmain(int argc, _TCHAR* argv[])
{

	char r[1];
	r[0]='a';
	myFun('a');
	for (int i=0; i<200; i++)
	{
		printf("%d ", r[i]);
	}

	HWND hDlg=NULL;
	HWND hDlg2=NULL;
	//MSG msg;
	//BOOL ret;


	InitCommonControls();
	//hDlg = CreateDialogParam(NULL, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc, 0);
	//SetTimer(hDlg, 0, 200, NULL);
	//SetTimer(hDlg, 1, 100, NULL);
	//SetTimer(hDlg, 2, 500, NULL);

	//DuityDlg MyDytDlg;
	//MyDytDlg.DoModal();

	//FeatureRegDlg gg;
	//gg.DoModal();
	
	//hDlg2 = CreateDialogParam(NULL, MAKEINTRESOURCE(IDD_DIALOG_HSFC_FLOUR), 0, HSFCControlProc, 0);

	AutoCameraData FPS;
	FPS.current =50;
	FPS.min =1;
	FPS.max =100;

	AutoCameraData Exposure;
	Exposure.current = 40;
	Exposure.min = 16;
	Exposure.max = 40;

	AutoCameraData FlashA;
	FlashA.current = 99;
	FlashA.min = 20;
	FlashA.max = 100;

	g_CameraAuto.setVars(FPS,FlashA,Exposure);

	g_CameraAuto.DoModal();
	
	//g_Versa.DoModal();

	/*
	HsfcConfig HSFCCfg;
	int retdls = HSFCCfg.DoModal();
	switch (HSFCCfg.getExitCode())
	{
	case HsfcConfig::HSFC_RET_APPLY:
		{
			int aa=1;
			break;	
		}
	case HsfcConfig::HSFC_RET_OK:
		{
			int aa=1;
			break;	
		}
	case HsfcConfig::HSFC_RET_CANCEL:
		{
			int aa=1;
			break;	
		}
	case HsfcConfig::HSFC_RET_NULL:
		{
			int aa=1;
			break;	
		}

	}*/

	//HsfcPmtDlg MyDytDlg;
	//MyDytDlg.DoModal();


	//LaunchPmtDialog();
	//return 0;


	if(hDlg2)
		ShowWindow(hDlg2, SW_SHOW);

	/*
	while((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
		if(ret == -1)
			return -1;

		if(!IsDialogMessage(hDlg, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	*/

	//Utils::DebugTimer MyTim;
	//MyTim.ReportToDebugTerm();
	//MyTim.WriteToFile("C:\\libby.txt");


	return 0;
}

void LaunchPmtDialog()
{
	{
		PmtDlgComboItem Single;
		PmtDlgComboVector ChListVector;
		Single.UserVisibleName="x1";
		Single.Data=1;
		Single.SelectedItem=false;
		ChListVector.push_back(Single);

		Single.UserVisibleName="x10";
		Single.Data=10;
		Single.SelectedItem=true;
		ChListVector.push_back(Single);

		Single.UserVisibleName="x100";
		Single.Data=100;
		Single.SelectedItem=false;
		ChListVector.push_back(Single);
		g_PmtCtrl.AddComboListCh1(ChListVector);
		g_PmtCtrl.AddComboListCh2(ChListVector);
	}
	{
		PmtDlgComboItem Single;
		PmtDlgComboVector ChListVector;
		Single.UserVisibleName="10%";
		Single.Data=10;
		Single.SelectedItem=true;
		ChListVector.push_back(Single);

		Single.UserVisibleName="100%";
		Single.Data=100;
		Single.SelectedItem=false;
		ChListVector.push_back(Single);

		g_PmtCtrl.AddComboListLaser(ChListVector);
	}


	g_PmtCtrl.SetCh1WaveLengthString("532nm");
	g_PmtCtrl.SetCh2WaveLengthString("700nm");
	g_PmtCtrl.DoModal();

}

