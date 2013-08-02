// Template.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include <CommCtrl.h>
#include "tchar.h"
#include "resource.h"
#include <iostream>
#include "AutoFocusAssistant.h"
#include "ProgressDialog.h"

HINSTANCE hInst;
ProgressDialog g_MyProgress;

HWND g_hProgressDialog = NULL;

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	stAFAssistData stDataStruct;
	



	stDataStruct.szFlowCel=_T("FC100");
	stDataStruct.szObjective=_T("10X");
	stDataStruct.SyringeSizeml=12.5;
	stDataStruct.RequiredVolume=5.44;

	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
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
			case IDC_BUTTON_PROGRESS:
				{
					//g_MyProgress.DoModal();
					//g_MyProgress.DoModeless(NULL);
					g_MyProgress.SetRange(0,100);
					g_MyProgress.SetOnTop(TRUE);
					g_MyProgress.DoModeless(hDlg);
					

					
					//g_hProgressDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR),hwnd, ToolDlgProc);
					int a=1;
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
                    if ( wParam == 1 ) {
                        //int pos = SendMessage(This->progress_handle_2, PBM_GETPOS, 0, 0 ) + 1;
                        //SendMessage(This->progress_handle_2, PBM_SETPOS, pos, 0 );
                        //if ( pos == 600 ) {
                        //    SendMessage(This->progress_handle_2, PBM_SETPOS, 0, 0 );
                       // }
                    }
                    return 0;
                }

	}

	return FALSE;
}

int _tmain(int argc, _TCHAR* argv[])
{

	HWND hDlg;
	MSG msg;
	BOOL ret;

	InitCommonControls();
	hDlg = CreateDialogParam(NULL, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc, 0);
	SetTimer(hDlg, 0, 200, NULL);
	SetTimer(hDlg, 1, 100, NULL);

	ShowWindow(hDlg, SW_SHOW);

	while((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
		if(ret == -1)
			return -1;

		if(!IsDialogMessage(hDlg, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	return 0;
}
