#include "StdAfx.h"
#include "HsfcPMT.h"

//String Crap
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
//String Crap

static int DASH_STANDARD_WIDTH = 10;
static int DASH_WIDE_WIDTH = 20;
static double EDIT_SENSITIVITY_MAX = 4.0;
static double EDIT_SENSITIVITY_MIN = 0.0;

static int SLIDE_TOP_SCALE = (int)(EDIT_SENSITIVITY_MAX*1000.0);
static int SLIDE_BOTTOM_SCALE = (int)(EDIT_SENSITIVITY_MIN*1000);



std::string HsfcPmtDlg::IntToString(int Value)
{
	const int i = 3;
	std::ostringstream s;
	s << Value;
	return s.str();
}


std::string HsfcPmtDlg::DoubleToString(double Value,int decPlaces/*=2*/)
{
	std::stringstream s;
	// print it into sstream using maximum precision
	s << std::fixed << std::setprecision(std::numeric_limits<double>::digits10) << Value;
	std::string res = s.str();

	// Now the res contains something like 1.234567899000000
	// so truncate 9000000000 by hand

	size_t dotIndex = res.find(".");

	std::string final_res = res.substr(0, dotIndex + decPlaces+1);

	return final_res;


}


/**
* CLASS HsfcPmtDlg
*
* @brief HsfcPmtDlg Back Flush to clean
*
* @version 1.0 
*
* @author David Libby
*
*/
HsfcPmtDlg::HsfcPmtDlg(void):
	m_bModal (false)
	,m_OnTop(false)	
	,m_Ch1Sensitivity(0.0)
	,m_Ch2Sensitivity(0.0)

{
}

HsfcPmtDlg::~HsfcPmtDlg(void)
{
}


/**
* FUNCTION DoModal
*
* @brief Launches the Model version of the Dialog.
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:39:53 AM
*
*
* @return INT_PTR 
*/
INT_PTR HsfcPmtDlg::DoModal()
{   
	m_bModal = TRUE;
    const INT_PTR rval = CDialogImpl::DoModal();
    m_bModal = FALSE;
    return rval;
}



/**
* FUNCTION IsVisible
*
* @brief Is the Dialog Currently Visible?
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:34:55 AM
*
*
* @return BOOL True if Visible.
*/
BOOL HsfcPmtDlg::IsVisible()
{
	BOOL ret =FALSE;
	if(this->IsWindow())
	{
		ret = this->IsWindowVisible();
	}
	return ret;

}

/**
* FUNCTION DoModeless
*
* @brief Launches Modeless version of Dialog
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:35:21 AM
*
* @param hWndParent By Default is NULL but can contain a parent Window handle if Parent provided it will stay above this window.
*/
void HsfcPmtDlg::DoModeless(HWND hWndParent)
{
	if(!this->IsWindow())
		this->Create(hWndParent);
	if(!IsVisible())
	{
		this->ShowWindow(SW_SHOWNORMAL);
	}
}

/**
* FUNCTION SetOnTop
*
* @brief Experimental It will force this window to the top.
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:36:43 AM
*
* @param NewTop 
*/
void HsfcPmtDlg::SetOnTop(BOOL NewTop)
{
	m_OnTop=NewTop;
}



/**
* FUNCTION Close
*
* @brief Closes the current Dialog.
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:38:19 AM
*
*/
void HsfcPmtDlg::Close()
{
	if (m_bModal)
		EndDialog(0);
	else
		DestroyWindow();
}


/**
* FUNCTION OnInitDialog
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:38:34 AM
*
* @param uMsg 
* @param wParam 
* @param lParam 
* @param bHandled 
*
* @return LRESULT 
*/
LRESULT HsfcPmtDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	//std::string Start_mL="1.0";
	//m_EditBoxExample=GetDlgItem(IDC_EDIT_AMOUNT_ML);
	//m_EditBoxExample.SetWindowTextW(CA2W(Start_mL.c_str()).m_szBuffer);
	m_EditCh1Sensitivity=GetDlgItem(IDC_EDIT_CH1_SENSITIVITY);
	m_EditCh2Sensitivity=GetDlgItem(IDC_EDIT_CH2_SENSITIVITY);

	m_SliderCh1Sensitivity=GetDlgItem(IDC_SLIDER_CH1);
	m_SliderCh2Sensitivity=GetDlgItem(IDC_SLIDER_CH2);


	SendMessage(m_SliderCh1Sensitivity, TBM_SETRANGE, 0, (LPARAM)MAKELONG(SLIDE_BOTTOM_SCALE, SLIDE_TOP_SCALE));
	SendMessage(m_SliderCh2Sensitivity, TBM_SETRANGE, 0, (LPARAM)MAKELONG(SLIDE_BOTTOM_SCALE, SLIDE_TOP_SCALE));

	if(m_OnTop)
	{
		SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}else
	{		
		SetWindowPos(HWND_BOTTOM,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

	RedrawControls();
	return 1;
};

/**
* FUNCTION OnBnClickedCancel
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:38:41 AM
*
* @param  
* @param  
* @param  
* @param  
*
* @return LRESULT 
*/
LRESULT HsfcPmtDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);

	return 1;
}

/**
* FUNCTION OnBnClickedCancel
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:38:41 AM
*
* @param  
* @param  
* @param  
* @param  
*
* @return LRESULT 
*/
LRESULT HsfcPmtDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);
	return 0;
}

LRESULT HsfcPmtDlg::OnBnClickedApply(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	return 0;
}


LRESULT HsfcPmtDlg::OnNMCustomdrawSlider(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here

	int aa=0;
	switch (pNMCD->dwDrawStage)
	{
	case CDDS_PREERASE:
		{
			aa=1;
			break;
		}
	case CDDS_POSTERASE:
		{
			aa=2;
			break;
		}
	case CDDS_PREPAINT:
		{
			aa=3;
			break;
		}
	case CDDS_ITEM | CDDS_PREPAINT:
		{
			aa=4;
			break;
		}
	case CDDS_SUBITEM | CDDS_PREPAINT:
		{
			aa=5;
			break;
		}
	case CDDS_SUBITEM | CDDS_POSTPAINT:
		{
			aa=6;
			break;
		}
	case CDDS_ITEM | CDDS_POSTPAINT:
		{
			aa=7;
			break;
		}
	case CDDS_POSTPAINT:
		{
			aa=8;
			break;
		}
	default:
		{
			aa=9;
			break;
		}
		break;
	}

	pNMCD->hdc;
	pNMCD->rc.top;
	CWindow SlideWindowHandle=GetDlgItem(pNMHDR->idFrom);
	RECT Rectangle;
	SlideWindowHandle.GetClientRect(&Rectangle);

	HBRUSH brush = CreateSolidBrush(RGB(200,200,200));
	FillRect(pNMCD->hdc, &Rectangle, brush);
	DeleteObject(brush);

	DrawSecadeLogScale(pNMCD->hdc, Rectangle.top + 13,Rectangle.bottom-14,Rectangle.left+18,4);

	return CDRF_DODEFAULT ;


	return 0;
}


void HsfcPmtDlg::DrawSecadeLogScale(HDC &hdc, int yLineStart,int yLineEnd, int xLineStart,int DecadeCount)
{
	LPPOINT lpPoint(0);
	int LineLength=yLineEnd-yLineStart;
	int DecadeLength=(int)(LineLength/DecadeCount);
	int LastLineEnd=yLineStart;
	for (int ct=1;ct<=DecadeCount;ct++)
	{
		DrawVerticalLogScale(hdc, LastLineEnd,LastLineEnd+DecadeLength,xLineStart);

		MoveToEx (hdc,xLineStart+DASH_WIDE_WIDTH,LastLineEnd,lpPoint);
		LineTo (hdc,xLineStart,LastLineEnd);

		RECT DrawArea;
		DrawArea.top=LastLineEnd-7;
		DrawArea.bottom=DrawArea.top+15;
		DrawArea.left=xLineStart+DASH_WIDE_WIDTH+3;
		DrawArea.right=DrawArea.left+15;

		DrawDecadeText(hdc ,DrawArea,14,"10");

		char str[20];
		_itoa_s(DecadeCount-ct+1, str, 10);
		std::string DecadeString= str;
		RECT LittleDrawArea;
		LittleDrawArea.bottom=DrawArea.top+2;
		LittleDrawArea.left=DrawArea.right;
		LittleDrawArea.top = LittleDrawArea.bottom - 8 ;
		LittleDrawArea.right=LittleDrawArea.left + 8;



		DrawDecadeText(hdc ,LittleDrawArea,10,DecadeString);



		LastLineEnd+=DecadeLength;

	}
}
void HsfcPmtDlg::DrawDecadeText(HDC &hdc,RECT DrawArea,int size,std::string TextString)
{

	//DrawArea.top-=19;

	HFONT hFont, hOldFont; 

	// Retrieve a handle to the variable stock font.  
	hFont = (HFONT)GetStockObject(SYSTEM_FONT); 

	LOGFONT LargeFont,SmallFont;
	GetObject ( hFont, sizeof(LOGFONT), &LargeFont );
	LargeFont.lfHeight = size ;                    // request a 12-pixel-height font
	_tcsncpy_s(LargeFont.lfFaceName, LF_FACESIZE,  _T("Arial"), 7);
	SmallFont=LargeFont;

	HFONT hCustomLargeFont = CreateFontIndirect ( &LargeFont );
	HFONT hCustomSmallFont = CreateFontIndirect ( &SmallFont );

	int gray=110;
	COLORREF OldCOlor = SetTextColor(hdc, RGB(gray,gray,gray));

	hOldFont = (HFONT)SelectObject(hdc, hCustomLargeFont);

	//Build a 1 pix Gray border by drawing the text sihfted in all 4 directions 
	int oldCkgroundMode = SetBkMode(hdc, TRANSPARENT);
	int pixAtound =1;
	DrawArea.top-=pixAtound;
	DrawText(hdc,CA2W(TextString.c_str()).m_szBuffer,TextString.size(),&DrawArea,DT_CENTER);
	DrawArea.top+=pixAtound;
	DrawArea.left-=pixAtound;
	DrawText(hdc,CA2W(TextString.c_str()).m_szBuffer,TextString.size(),&DrawArea,DT_CENTER);
	DrawArea.bottom+=pixAtound;
	DrawArea.left+=pixAtound;
	DrawText(hdc,CA2W(TextString.c_str()).m_szBuffer,TextString.size(),&DrawArea,DT_CENTER);
	DrawArea.bottom-=pixAtound;
	DrawArea.right+=pixAtound;
	DrawText(hdc,CA2W(TextString.c_str()).m_szBuffer,TextString.size(),&DrawArea,DT_CENTER);	
	DrawArea.right-=pixAtound;

	//Now draw the font black Centered.
	SetTextColor(hdc, RGB(0,0,0));
	SelectObject(hdc, hCustomSmallFont);
	DrawText(hdc,CA2W(TextString.c_str()).m_szBuffer,TextString.size(),&DrawArea,DT_CENTER);

	SetBkMode(hdc, oldCkgroundMode  );

	// Restore the original font.        
	SelectObject(hdc, hOldFont); 
	// Restore the original Color.        
	SetTextColor(hdc, OldCOlor);



}

void HsfcPmtDlg::DrawVerticalLogScale(HDC &hdc, int yLineStart,int yLineEnd, int xLineStart)
{
	LPPOINT lpPoint(0);

	int LineLength=yLineEnd-yLineStart;

	//DrawCenterLine
	MoveToEx (hdc,xLineStart,yLineStart,lpPoint);
	LineTo(hdc,xLineStart,yLineEnd);

	double DecadeValue = 10;//Decade Value Must Be 10
	double CurLineLength=LineLength;
	int PreviousPix=0;
	while(DecadeValue >= 1)
	{
		double logI=log10((double) DecadeValue );
		double YPos = LineLength * log10((double) DecadeValue );
		DecadeValue --;
		if((yLineEnd-(int)YPos)-PreviousPix>1)
		{
			MoveToEx (hdc,xLineStart+DASH_STANDARD_WIDTH,yLineEnd-(int)YPos,lpPoint);
			LineTo (hdc,xLineStart,yLineEnd-(int)YPos);
			PreviousPix=yLineEnd-(int)YPos;


		}
	}


}

std::string HsfcPmtDlg::RetriveStdStringFromCWindow(CWindow &TempWindow)
{
	int TestLength = TempWindow.GetWindowTextLengthW()+1;
	wchar_t* buffer = new wchar_t[TestLength+1];

	TempWindow.GetWindowTextW(buffer,TestLength);
	std::string SimpString=CW2A(buffer);

	delete[] buffer;
	return SimpString;
}

void HsfcPmtDlg::SetEditWindowValue(CWindow &CurWindow,double NewValue)
{
	CurWindow.SetWindowTextW(CA2W(DoubleToString(NewValue).c_str()).m_szBuffer);
}
double HsfcPmtDlg::GetEditWindowValue(CWindow &CurWindow)
{
	std::string RetWindowString=RetriveStdStringFromCWindow(CurWindow);
	if(RetWindowString.empty())
		return 0.0;
	return (double)atof(RetWindowString.c_str());
}

double HsfcPmtDlg::GetSliderPos(CWindow &CurWindow)
{
	return SendMessageW(CurWindow,TBM_GETPOS,0,0);
}
void HsfcPmtDlg::SetSliderPos(CWindow &CurWindow,int NewPos)
{
	if(NewPos > SLIDE_TOP_SCALE || NewPos < SLIDE_BOTTOM_SCALE)
		return;

	SendMessageW(CurWindow,TBM_SETPOS,true,NewPos);
}

LRESULT HsfcPmtDlg::OnNMReleasedcaptureSlider(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled)
{
	// TODO: Add your control notification handler code here
	CWindow SlideWindowHandle=GetDlgItem(pNMHDR->idFrom);
	RECT Rectangle;
	SlideWindowHandle.GetClientRect(&Rectangle);
	int CtrlPos= (int)(SLIDE_TOP_SCALE - GetSliderPos(SlideWindowHandle));
	double dblCtrlPos=(double)CtrlPos/1000.0;
	if(CtrlPos==0)
		dblCtrlPos=0.0;

	std::string DebugOUtput="Ctrl Raw Location = " +IntToString(CtrlPos);
	DebugOUtput+="\t Log10=" + DoubleToString(dblCtrlPos,2) +="\n";


	OutputDebugStringA(DebugOUtput.c_str());

	std::string PosDblString=DoubleToString(dblCtrlPos,2);
	switch (idCtrl)
	{
		case IDC_SLIDER_CH1:
			{		
				int aa=1;
					SetCh1Sensitivity(dblCtrlPos);
					RedrawControls();			

				break;
			}
		case IDC_SLIDER_CH2:
			{		
					SetCh2Sensitivity(dblCtrlPos);
					RedrawControls();
				break;
			}
		default:
			{
				break;
			}
		}

		return 0;
}
void HsfcPmtDlg::RedrawControls()
{
	//SetEditWindowValue(m_EditCh1Sensitivity,dblCtrlPos);
	//m_EditCh1Sensitivity.SetWindowTextW(CA2W(DoubleToString(dblCtrlPos).c_str()).m_szBuffer);

	int SliderPosCh1=SLIDE_TOP_SCALE-(int)(m_Ch1Sensitivity*1000.0);
	int SliderPosCh2=SLIDE_TOP_SCALE-(int)(m_Ch2Sensitivity*1000.0);


	SetSliderPos(m_SliderCh1Sensitivity,SliderPosCh1);
	SetSliderPos(m_SliderCh2Sensitivity,SliderPosCh2);
	m_EditCh1Sensitivity.SetWindowTextW(CA2W(DoubleToString(m_Ch1Sensitivity).c_str()).m_szBuffer);
	m_EditCh2Sensitivity.SetWindowTextW(CA2W(DoubleToString(m_Ch2Sensitivity).c_str()).m_szBuffer);
}






LRESULT HsfcPmtDlg::OnEnChangeEditSensitivity(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogImpl::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here



	return 0;
}


LRESULT HsfcPmtDlg::OnEnKillfocusEditSensitivity(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO: Add your control notification handler code here

	CWindow EditWindowHandle=GetDlgItem(wID);

	double EditWIndowDblValue=GetEditWindowValue(EditWindowHandle);
	if(EditWIndowDblValue>EDIT_SENSITIVITY_MAX)
		EditWIndowDblValue=EDIT_SENSITIVITY_MAX;

	if(EditWIndowDblValue<EDIT_SENSITIVITY_MIN)
		EditWIndowDblValue=EDIT_SENSITIVITY_MIN;




	switch (wID)
	{
	case IDC_EDIT_CH1_SENSITIVITY:
		{		
			if(EditWIndowDblValue!=GetCh1Sensitivity())
			{
				SetCh1Sensitivity(EditWIndowDblValue);
				RedrawControls();
			}
			break;
		}
	case IDC_EDIT_CH2_SENSITIVITY:
		{		
			if(EditWIndowDblValue!=GetCh2Sensitivity())
			{
				SetCh2Sensitivity(EditWIndowDblValue);
				RedrawControls();
			}
			break;
		}
	default:
		{
			break;
		}
	}

	return 0;
}
