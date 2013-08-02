#include "AutoFocusAssistant.h"
#include "WinUser.h"


#define MAXDLG 7

AutoFocusAssistant::AutoFocusAssistant(stAFAssistData *pinDataStructure):
m_pAssistData(pinDataStructure)
{
}
AutoFocusAssistant::~AutoFocusAssistant()
{
}
int AutoFocusAssistant::DoModal()
{

	return AutoFocusAssistant::DisplayDialog(1);

}

int AutoFocusAssistant::DisplayDialog(int DialogNumber)
{
	if(DialogNumber > MAXDLG || DialogNumber < 1)
		return AF_WIZ_CANCEL;

	int ret = AF_WIZ_CANCEL;
	switch (DialogNumber)
	{	
	case 0 :
		{
			AFAssistant0 Wiz0(m_pAssistData);
			ret = Wiz0.DoModal();
			break;
		}
	case 1 :
		{
			AFAssistant1 Wiz(m_pAssistData);
			ret = Wiz.DoModal();
			break;
		}
	case 2 :
		{
			AFAssistant2 Wiz(m_pAssistData);
			ret = Wiz.DoModal();
			break;
		}
	case 3 :
		{
			AFAssistant3 Wiz(m_pAssistData);
			ret = Wiz.DoModal();
			break;
		}
	case 4 :
		{
			AFAssistant4 Wiz(m_pAssistData);
			ret = Wiz.DoModal();
			break;
		}
	case 5 :
		{
			AFAssistant5 Wiz(m_pAssistData);
			ret = Wiz.DoModal();
			break;
		}
	case 6 :
		{
			AFAssistant6 Wiz(m_pAssistData);
			ret = Wiz.DoModal();
			break;
		}
	case 7 :
		{
			AFAssistant7 Wiz(m_pAssistData);
			ret = Wiz.DoModal();
			break;
		}

	default:
		{

			ret = AF_WIZ_CANCEL;
			break;
		}

	}

	int newDlg = DialogNumber;
	switch (ret)
	{
	case AF_WIZ_NEXT:
		{
			newDlg++;
			return DisplayDialog(newDlg);
			break;
		}
	case AF_WIZ_BACK:
		{
			newDlg--;
			return DisplayDialog(newDlg);
			break;
		}
	case AF_WIZ_FINISH:
		{
			return AF_WIZ_FINISH;
			break;
		}

	}

	return AF_WIZ_CANCEL;

}



//////////////// 00000000000000000000000000
AFAssistant0::AFAssistant0 (stAFAssistData *pinDataStructure) :
m_pAFDataStructure (pinDataStructure) 
{	
}
LRESULT AFAssistant0::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Do some initialization code
	return 1;
};


LRESULT AFAssistant0::OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_BACK);

	return 0;
}

LRESULT AFAssistant0::OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_NEXT);

	return 0;
}
LRESULT AFAssistant0::OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_FINISH);
	return 0;
}


LRESULT AFAssistant0::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_CANCEL);

	return 0;
}


//////////////// 1
AFAssistant1::AFAssistant1 (stAFAssistData *pinDataStructure) :
m_pAFDataStructure (pinDataStructure) 
{	
}

LRESULT AFAssistant1::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Do some initialization code
	GetDlgItem(IDD_BUTTON_BACK).ShowWindow(SW_HIDE);
	GetDlgItem(IDD_BUTTON_FINISH).ShowWindow(SW_HIDE);

	HFONT MyOldTextFont = GetDlgItem(IDC_STATIC_AF_1_HEADER_TEXT).GetFont();
	LOGFONT MyNewFontFont;
	GetObject ( MyOldTextFont, sizeof(LOGFONT), &MyNewFontFont );
	MyNewFontFont.lfHeight = 24;                      // request a 12-pixel-height font
	_tcsncpy_s(MyNewFontFont.lfFaceName, LF_FACESIZE,  _T("Arial"), 7);
	HFONT hCustomFont = CreateFontIndirect ( &MyNewFontFont );
	GetDlgItem(IDC_STATIC_AF_1_HEADER_TEXT).SetFont(hCustomFont);



	return 1;
};

LRESULT AFAssistant1::OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_BACK);

	return 0;
}

LRESULT AFAssistant1::OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_NEXT);

	return 0;
}
LRESULT AFAssistant1::OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_FINISH);
	return 0;
}


LRESULT AFAssistant1::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_CANCEL);

	return 0;
}


//////////////// 2
AFAssistant2::AFAssistant2 (stAFAssistData *pinDataStructure) :
m_pAFDataStructure (pinDataStructure) 
{	
}
LRESULT AFAssistant2::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Do some initialization code
	//
	GetDlgItem(IDD_BUTTON_FINISH).ShowWindow(SW_HIDE);
	HFONT MyOldTextFont = GetDlgItem(IDC_STATIC_AF_2_HEADER_TEXT).GetFont();
	LOGFONT MyNewFontFont;
	GetObject ( MyOldTextFont, sizeof(LOGFONT), &MyNewFontFont );
	MyNewFontFont.lfHeight = 24;                      // request a 12-pixel-height font
	_tcsncpy_s(MyNewFontFont.lfFaceName, LF_FACESIZE,  _T("Arial"), 7);
	HFONT hCustomFont = CreateFontIndirect ( &MyNewFontFont );
	MyNewFontFont.lfWeight = FW_HEAVY;
	HFONT hCustomBoldFont = CreateFontIndirect ( &MyNewFontFont );

	GetDlgItem(IDC_STATIC_AF_2_HEADER_TEXT).SetFont(hCustomFont);

	GetDlgItem(IDC_STATIC_AF_HEADER_OBJECTIVE).SetFont(hCustomFont);	
	GetDlgItem(IDC_STATIC_AF_2_HEADER_FLOW).SetFont(hCustomFont);
	GetDlgItem(IDC_STATIC_AF_2_HEADER_SYRINGE).SetFont(hCustomFont);

	GetDlgItem(IDC_STATIC_AF_BODY_OBJECTIVE).SetFont(hCustomBoldFont);
	GetDlgItem(IDC_STATIC_AF_2_BODY_FLOW).SetFont(hCustomBoldFont);
	GetDlgItem(IDC_STATIC_AF_2_BODY_SYRINGE).SetFont(hCustomBoldFont);

	GetDlgItem(IDC_RADIO_AF_NO).SetFont(hCustomFont);
	GetDlgItem(IDC_RADIO_AF_YES).SetFont(hCustomFont);


	TCHAR Syringe[20];
	Syringe[0]='\0';


	_stprintf_s(Syringe, _countof(Syringe),_T( "%.1f ml"),m_pAFDataStructure->SyringeSizeml);		

	GetDlgItem(IDC_STATIC_AF_BODY_OBJECTIVE).SetWindowTextW(m_pAFDataStructure->szObjective);
	GetDlgItem(IDC_STATIC_AF_2_BODY_FLOW).SetWindowTextW(m_pAFDataStructure->szFlowCel);
	GetDlgItem(IDC_STATIC_AF_2_BODY_SYRINGE).SetWindowTextW(Syringe);


	if(m_pAFDataStructure->ObjFlowSizeOk)
	{
		SendMessage(GetDlgItem(IDC_RADIO_AF_NO), BM_SETCHECK,BST_UNCHECKED, 0);
		SendMessage(GetDlgItem(IDC_RADIO_AF_YES), BM_SETCHECK,BST_CHECKED, 0);

		GetDlgItem(IDD_BUTTON_BACK).EnableWindow(true);
		GetDlgItem(IDD_BUTTON_NEXT).EnableWindow(true);
		GetDlgItem(IDC_STATIC_AF_2_FOOTER_TEXT).ShowWindow(SW_HIDE);

	}else
	{

		SendMessage(GetDlgItem(IDC_RADIO_AF_NO), BM_SETCHECK,BST_CHECKED, 0);
		SendMessage(GetDlgItem(IDC_RADIO_AF_YES), BM_SETCHECK,BST_UNCHECKED, 0);

		GetDlgItem(IDD_BUTTON_BACK).EnableWindow(true);
		GetDlgItem(IDD_BUTTON_NEXT).EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_AF_2_FOOTER_TEXT).ShowWindow(SW_SHOW);
	}


	return 1;
};

LRESULT AFAssistant2::OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_BACK);

	return 0;
}

LRESULT AFAssistant2::OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_NEXT);

	return 0;
}
LRESULT AFAssistant2::OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_FINISH);
	return 0;
}


LRESULT AFAssistant2::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_CANCEL);

	return 0;
}


LRESULT AFAssistant2::OnBnClickedRadioAfNo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

	GetDlgItem(IDD_BUTTON_BACK).EnableWindow(true);
	GetDlgItem(IDD_BUTTON_NEXT).EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_AF_2_FOOTER_TEXT).ShowWindow(SW_SHOW);
	m_pAFDataStructure->ObjFlowSizeOk=false;

	return 0;
}


LRESULT AFAssistant2::OnBnClickedRadioAfYes(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	GetDlgItem(IDD_BUTTON_BACK).EnableWindow(true);
	GetDlgItem(IDD_BUTTON_NEXT).EnableWindow(true);
	GetDlgItem(IDC_STATIC_AF_2_FOOTER_TEXT).ShowWindow(SW_HIDE);
	m_pAFDataStructure->ObjFlowSizeOk=true;

	return 0;
}


//////////////// 3
AFAssistant3::AFAssistant3 (stAFAssistData *pinDataStructure) :
m_pAFDataStructure (pinDataStructure) 
{	
}
LRESULT AFAssistant3::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Do some initialization code
	//
	GetDlgItem(IDD_BUTTON_FINISH).ShowWindow(SW_HIDE);
	HFONT MyOldTextFont = GetDlgItem(IDC_STATIC_AF_3_HEADER_TEXT).GetFont();
	LOGFONT MyNewFontFont;
	GetObject ( MyOldTextFont, sizeof(LOGFONT), &MyNewFontFont );
	MyNewFontFont.lfHeight = 24;                      // request a 12-pixel-height font
	_tcsncpy_s(MyNewFontFont.lfFaceName, LF_FACESIZE,  _T("Arial"), 7);
	HFONT hCustomFont = CreateFontIndirect ( &MyNewFontFont );
	MyNewFontFont.lfWeight = FW_HEAVY;
	HFONT hCustomBoldFont = CreateFontIndirect ( &MyNewFontFont );

	GetDlgItem(IDC_STATIC_AF_3_HEADER_TEXT).SetFont(hCustomFont);

	GetDlgItem(IDC_STATIC_AF_3_HEADER_SYRINGE).SetFont(hCustomFont);
	GetDlgItem(IDC_STATIC_AF_3_HEADER_VOLUME).SetFont(hCustomFont);

	GetDlgItem(IDC_STATIC_AF_3_BODY_SYRINGE).SetFont(hCustomBoldFont);
	GetDlgItem(IDC_STATIC_AF_3_BODY_VOLUME).SetFont(hCustomBoldFont);

	GetDlgItem(IDC_RADIO_AF_NO).SetFont(hCustomFont);
	GetDlgItem(IDC_RADIO_AF_YES).SetFont(hCustomFont);


	TCHAR Syringe[20];
	Syringe[0]='\0';
	_stprintf_s(Syringe, _countof(Syringe),_T( "%.1f ml"),m_pAFDataStructure->SyringeSizeml);					

	TCHAR Volume[20];
	Volume[0]='\0';
	_stprintf_s(Volume, _countof(Syringe),_T( "%.1f ml"),m_pAFDataStructure->RequiredVolume);				

	GetDlgItem(IDC_STATIC_AF_3_BODY_SYRINGE).SetWindowTextW(Syringe);
	GetDlgItem(IDC_STATIC_AF_3_BODY_VOLUME).SetWindowTextW(Volume);


	if(m_pAFDataStructure->ObjFlowVolumeOk)
	{
		SendMessage(GetDlgItem(IDC_RADIO_AF_NO), BM_SETCHECK,BST_UNCHECKED, 0);
		SendMessage(GetDlgItem(IDC_RADIO_AF_YES), BM_SETCHECK,BST_CHECKED, 0);

		GetDlgItem(IDD_BUTTON_BACK).EnableWindow(true);
		GetDlgItem(IDD_BUTTON_NEXT).EnableWindow(true);
		GetDlgItem(IDC_STATIC_AF_3_FOOTER_TEXT).ShowWindow(SW_HIDE);

	}else
	{

		SendMessage(GetDlgItem(IDC_RADIO_AF_NO), BM_SETCHECK,BST_CHECKED, 0);
		SendMessage(GetDlgItem(IDC_RADIO_AF_YES), BM_SETCHECK,BST_UNCHECKED, 0);

		GetDlgItem(IDD_BUTTON_BACK).EnableWindow(true);
		GetDlgItem(IDD_BUTTON_NEXT).EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_AF_3_FOOTER_TEXT).ShowWindow(SW_SHOW);
	}


	return 1;
};

LRESULT AFAssistant3::OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_BACK);

	return 0;
}

LRESULT AFAssistant3::OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_NEXT);

	return 0;
}
LRESULT AFAssistant3::OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_FINISH);
	return 0;
}


LRESULT AFAssistant3::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_CANCEL);

	return 0;
}


LRESULT AFAssistant3::OnBnClickedRadioAfNo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

	GetDlgItem(IDD_BUTTON_BACK).EnableWindow(true);
	GetDlgItem(IDD_BUTTON_NEXT).EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_AF_3_FOOTER_TEXT).ShowWindow(SW_SHOW);
	m_pAFDataStructure->ObjFlowVolumeOk=false;

	return 0;
}


LRESULT AFAssistant3::OnBnClickedRadioAfYes(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	GetDlgItem(IDD_BUTTON_BACK).EnableWindow(true);
	GetDlgItem(IDD_BUTTON_NEXT).EnableWindow(true);
	GetDlgItem(IDC_STATIC_AF_3_FOOTER_TEXT).ShowWindow(SW_HIDE);
	m_pAFDataStructure->ObjFlowVolumeOk=true;

	return 0;
}


//////////////// 4
AFAssistant4::AFAssistant4 (stAFAssistData *pinDataStructure) :
m_pAFDataStructure (pinDataStructure) 
{	
}
LRESULT AFAssistant4::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	GetDlgItem(IDD_BUTTON_FINISH).ShowWindow(SW_HIDE);
	HFONT MyOldTextFont = GetDlgItem(IDC_STATIC_AF_4_HEADER_TEXT).GetFont();
	LOGFONT MyNewFontFont;
	GetObject ( MyOldTextFont, sizeof(LOGFONT), &MyNewFontFont );
	MyNewFontFont.lfHeight = 24;                      // request a 12-pixel-height font
	_tcsncpy_s(MyNewFontFont.lfFaceName, LF_FACESIZE,  _T("Arial"), 7);
	HFONT hCustomFont = CreateFontIndirect ( &MyNewFontFont );

	GetDlgItem(IDC_STATIC_AF_4_HEADER_TEXT).SetFont(hCustomFont);

	return 1;
};


LRESULT AFAssistant4::OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_BACK);

	return 0;
}

LRESULT AFAssistant4::OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_NEXT);

	return 0;
}
LRESULT AFAssistant4::OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_FINISH);
	return 0;
}


LRESULT AFAssistant4::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_CANCEL);

	return 0;
}


//////////////// 5
AFAssistant5::AFAssistant5 (stAFAssistData *pinDataStructure) :
m_pAFDataStructure (pinDataStructure) 
{	
}
LRESULT AFAssistant5::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	GetDlgItem(IDD_BUTTON_FINISH).ShowWindow(SW_HIDE);
	HFONT MyOldTextFont = GetDlgItem(IDC_STATIC_AF_5_HEADER_TEXT).GetFont();
	LOGFONT MyNewFontFont;
	GetObject ( MyOldTextFont, sizeof(LOGFONT), &MyNewFontFont );
	MyNewFontFont.lfHeight = 24;                      // request a 12-pixel-height font
	_tcsncpy_s(MyNewFontFont.lfFaceName, LF_FACESIZE,  _T("Arial"), 7);
	HFONT hCustomFont = CreateFontIndirect ( &MyNewFontFont );

	GetDlgItem(IDC_STATIC_AF_5_HEADER_TEXT).SetFont(hCustomFont);

	m_pAFDataStructure->ObjSolutionOk=true;

	return 1;
};


LRESULT AFAssistant5::OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_BACK);

	return 0;
}

LRESULT AFAssistant5::OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_NEXT);

	return 0;
}
LRESULT AFAssistant5::OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_FINISH);
	return 0;
}


LRESULT AFAssistant5::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_CANCEL);

	return 0;
}
//////////////// 6
AFAssistant6::AFAssistant6 (stAFAssistData *pinDataStructure) :
m_pAFDataStructure (pinDataStructure) 
{	
}
LRESULT AFAssistant6::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	GetDlgItem(IDD_BUTTON_FINISH).ShowWindow(SW_HIDE);
	HFONT MyOldTextFont = GetDlgItem(IDC_STATIC_AF_6_HEADER_TEXT).GetFont();
	LOGFONT MyNewFontFont;
	GetObject ( MyOldTextFont, sizeof(LOGFONT), &MyNewFontFont );
	MyNewFontFont.lfHeight = 24;                      // request a 12-pixel-height font
	_tcsncpy_s(MyNewFontFont.lfFaceName, LF_FACESIZE,  _T("Arial"), 7);
	HFONT hCustomFont = CreateFontIndirect ( &MyNewFontFont );

	GetDlgItem(IDC_STATIC_AF_6_HEADER_TEXT).SetFont(hCustomFont);

	m_pAFDataStructure->ObjFocusOk=true;


	return 1;
};


LRESULT AFAssistant6::OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_BACK);

	return 0;
}

LRESULT AFAssistant6::OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_NEXT);

	return 0;
}
LRESULT AFAssistant6::OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_FINISH);
	return 0;
}


LRESULT AFAssistant6::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_CANCEL);

	return 0;
}


//////////////// 7
AFAssistant7::AFAssistant7 (stAFAssistData *pinDataStructure) :
m_pAFDataStructure (pinDataStructure) 
{	
}
LRESULT AFAssistant7::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Do some initialization code
	//
	//GetDlgItem(IDD_BUTTON_FINISH).ShowWindow(SW_HIDE);
	GetDlgItem(IDD_BUTTON_NEXT).ShowWindow(SW_HIDE);
	HFONT MyOldTextFont = GetDlgItem(IDC_STATIC_AF_7_HEADER_TEXT).GetFont();
	LOGFONT MyNewFontFont;
	GetObject ( MyOldTextFont, sizeof(LOGFONT), &MyNewFontFont );
	MyNewFontFont.lfHeight = 21;                      // request a 12-pixel-height font
	_tcsncpy_s(MyNewFontFont.lfFaceName, LF_FACESIZE,  _T("Arial"), 7);
	HFONT hCustomFont = CreateFontIndirect ( &MyNewFontFont );
	MyNewFontFont.lfWeight = FW_HEAVY;
	HFONT hCustomBoldFont = CreateFontIndirect ( &MyNewFontFont );

	GetDlgItem(IDC_STATIC_AF_7_HEADER_TEXT).SetFont(hCustomFont);

	GetDlgItem(IDC_STATIC_AF_7_HEADER_OBJECTIVE).SetFont(hCustomFont);	
	GetDlgItem(IDC_STATIC_AF_7_HEADER_FLOW).SetFont(hCustomFont);
	GetDlgItem(IDC_STATIC_AF_7_HEADER_SYRINGE).SetFont(hCustomFont);
	GetDlgItem(IDC_STATIC_AF_7_HEADER_SYRINGE_VOLUME).SetFont(hCustomFont);
	GetDlgItem(IDC_STATIC_AF_7_HEADER_SOLUTION_ADDED).SetFont(hCustomFont);
	GetDlgItem(IDC_STATIC_AF_7_HEADER_COURSE_FOCUS).SetFont(hCustomFont);

	GetDlgItem(IDC_STATIC_AF_7_BODY_OBJECTIVE).SetFont(hCustomBoldFont);
	GetDlgItem(IDC_STATIC_AF_7_BODY_FLOW).SetFont(hCustomBoldFont);
	GetDlgItem(IDC_STATIC_AF_7_BODY_SYRINGE).SetFont(hCustomBoldFont);
	GetDlgItem(IDC_STATIC_AF_7_BODY_SYRINGE_VOLUME).SetFont(hCustomBoldFont);
	GetDlgItem(IDC_STATIC_AF_7_BODY_SOLUTION_ADDED).SetFont(hCustomBoldFont);
	GetDlgItem(IDC_STATIC_AF_7_BODY_COURSE_FOCUS).SetFont(hCustomBoldFont);

	GetDlgItem(IDC_RADIO_AF_NO).SetFont(hCustomFont);
	GetDlgItem(IDC_RADIO_AF_YES).SetFont(hCustomFont);

	TCHAR Syringe[20];
	Syringe[0]='\0';
	_stprintf_s(Syringe, _countof(Syringe),_T( "%.1f ml"),m_pAFDataStructure->SyringeSizeml);					

	TCHAR Volume[20];
	Volume[0]='\0';
	_stprintf_s(Volume, _countof(Syringe),_T( "%.1f ml"),m_pAFDataStructure->RequiredVolume);					

	GetDlgItem(IDC_STATIC_AF_7_BODY_OBJECTIVE).SetWindowTextW(m_pAFDataStructure->szObjective);
	GetDlgItem(IDC_STATIC_AF_7_BODY_FLOW).SetWindowTextW(m_pAFDataStructure->szFlowCel);
	GetDlgItem(IDC_STATIC_AF_7_BODY_SYRINGE).SetWindowTextW(Syringe);
	GetDlgItem(IDC_STATIC_AF_7_BODY_SYRINGE_VOLUME).SetWindowTextW(Volume);
	GetDlgItem(IDC_STATIC_AF_7_BODY_SOLUTION_ADDED).SetWindowTextW(m_pAFDataStructure->ObjSolutionOk ? _T("Yes") : _T("No"));
	GetDlgItem(IDC_STATIC_AF_7_BODY_COURSE_FOCUS).SetWindowTextW(m_pAFDataStructure->ObjFocusOk ? _T("Yes") : _T("No"));


	if(m_pAFDataStructure->ObjFlowSizeOk)
	{
		SendMessage(GetDlgItem(IDC_RADIO_AF_NO), BM_SETCHECK,BST_UNCHECKED, 0);
		SendMessage(GetDlgItem(IDC_RADIO_AF_YES), BM_SETCHECK,BST_CHECKED, 0);

		GetDlgItem(IDD_BUTTON_BACK).EnableWindow(true);
		GetDlgItem(IDD_BUTTON_FINISH).EnableWindow(true);
		GetDlgItem(IDC_STATIC_AF_7_FOOTER_TEXT).ShowWindow(SW_HIDE);

	}else
	{

		SendMessage(GetDlgItem(IDC_RADIO_AF_NO), BM_SETCHECK,BST_CHECKED, 0);
		SendMessage(GetDlgItem(IDC_RADIO_AF_YES), BM_SETCHECK,BST_UNCHECKED, 0);

		GetDlgItem(IDD_BUTTON_BACK).EnableWindow(true);
		GetDlgItem(IDD_BUTTON_FINISH).EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_AF_7_FOOTER_TEXT).ShowWindow(SW_SHOW);
	}

	m_pAFDataStructure->ObjFocusOk=true;

	return 1;
};

LRESULT AFAssistant7::OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_BACK);

	return 0;
}

LRESULT AFAssistant7::OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_NEXT);

	return 0;
}
LRESULT AFAssistant7::OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_FINISH);
	return 0;
}


LRESULT AFAssistant7::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(AF_WIZ_CANCEL);

	return 0;
}


LRESULT AFAssistant7::OnBnClickedRadioAfNo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

	GetDlgItem(IDD_BUTTON_BACK).EnableWindow(true);
	GetDlgItem(IDD_BUTTON_FINISH).EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_AF_7_FOOTER_TEXT).ShowWindow(SW_SHOW);
	m_pAFDataStructure->ObjFlowSizeOk=false;

	return 0;
}


LRESULT AFAssistant7::OnBnClickedRadioAfYes(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	GetDlgItem(IDD_BUTTON_BACK).EnableWindow(true);
	GetDlgItem(IDD_BUTTON_FINISH).EnableWindow(true);
	GetDlgItem(IDC_STATIC_AF_7_FOOTER_TEXT).ShowWindow(SW_HIDE);
	m_pAFDataStructure->ObjFlowSizeOk=true;

	return 0;
}
