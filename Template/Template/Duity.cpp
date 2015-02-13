#include "StdAfx.h"
#include "Duity.h"
#include <math.h>       /* log */


int yLineStart=10;
int xLineStart=100;
static int yLineEnd=400;

static int DASH_STANDARD_WIDTH = 10;
static int DASH_WIDE_WIDTH = 20;

/**


* CLASS DuityDlg
*
* @brief DuityDlg Back Flush to clean
*
* @version 1.0 
*
* @author David Libby
*
*/
DuityDlg::DuityDlg(void):
	m_bModal (false)
	,m_OnTop(false)	
{
}



DuityDlg::~DuityDlg(void)
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
INT_PTR DuityDlg::DoModal()
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
BOOL DuityDlg::IsVisible()
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
void DuityDlg::DoModeless(HWND hWndParent)
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
void DuityDlg::SetOnTop(BOOL NewTop)
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
void DuityDlg::Close()
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
LRESULT DuityDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	//std::string Start_mL="1.0";
	//m_EditBoxExample=GetDlgItem(IDC_EDIT_AMOUNT_ML);
	//m_EditBoxExample.SetWindowTextW(CA2W(Start_mL.c_str()).m_szBuffer);
	CWindow m_SlideWindowHandle=GetDlgItem(IDC_SLIDER1);
	RECT Rectangle;
	m_SlideWindowHandle.GetClientRect(&Rectangle);

	if(m_OnTop)
	{
		SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}else
	{		
		SetWindowPos(HWND_BOTTOM,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

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
LRESULT DuityDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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
LRESULT DuityDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);
	return 0;
}

LRESULT DuityDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PAINTSTRUCT ps; 
	HDC hdc; 
	hdc = BeginPaint( &ps); 	

	int LIneLength=yLineEnd-yLineStart;
	//DrawVerticalLogScale(hdc, yLineStart,yLineEnd,  400);
	//DrawVerticalLogScale(hdc, yLineStart+LIneLength+2,yLineEnd+LIneLength*2,  xLineStart);
	//DrawVerticalLogScale(hdc,10,110,   100);
	//DrawVerticalLogScale(hdc, 10,210,  200);
	//DrawVerticalLogScale(hdc, 10,50 , 300);

	//DrawSecadeLogScale(hdc, 20,yLineEnd , 50,1);
	//DrawSecadeLogScale(hdc, 20,yLineEnd , 100,2);
	//DrawSecadeLogScale(hdc, 20,yLineEnd , 150,3);
	//DrawSecadeLogScale(hdc, 20,yLineEnd , 200,4);
	//DrawSecadeLogScale(hdc, 20,yLineEnd , 250,5);
	//DrawSecadeLogScale(hdc, 20,yLineEnd , 300,6);
	//DrawSecadeLogScale(hdc, 20,yLineEnd , 350,7);
	//DrawSecadeLogScale(hdc, 20,yLineEnd , 400,8);
	//DrawSecadeLogScale(hdc, 20,yLineEnd , 450,9);
	//DrawSecadeLogScale(hdc, 20,yLineEnd , 500,10);

	EndPaint( &ps);



	return 0;
}
void DuityDlg::DrawSecadeLogScale(HDC &hdc, int yLineStart,int yLineEnd, int xLineStart,int DecadeCount)
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
		DrawArea.right=DrawArea.left+10;

		DrawDecadeText(hdc ,DrawArea,15,"10");

		char str[20];
		_itoa_s(DecadeCount-ct+1, str, 10);
		std::string DecadeString= str;
		RECT LittleDrawArea;
		LittleDrawArea.bottom=DrawArea.top+2;
		LittleDrawArea.left=DrawArea.right+4;
		LittleDrawArea.top = LittleDrawArea.bottom - 8 ;
		LittleDrawArea.right=LittleDrawArea.left + 8;



		DrawDecadeText(hdc ,LittleDrawArea,12,DecadeString);



		LastLineEnd+=DecadeLength;

	}
}
void DuityDlg::DrawDecadeText(HDC &hdc,RECT DrawArea,int size,std::string TextString)
{

	//DrawArea.top-=19;

	HFONT hFont, hOldFont; 

	// Retrieve a handle to the variable stock font.  
	hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT); 

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

void DuityDlg::DrawVerticalLogScale(HDC &hdc, int yLineStart,int yLineEnd, int xLineStart)
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



LRESULT DuityDlg::OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here



	yLineEnd-=20;

	if(yLineEnd<20)
		yLineEnd=400;


	
	char str[20];
	_itoa_s(yLineEnd, str, 10);
	std::string intNumberString= str;

	std::string Display="Height:"+ intNumberString +"\n";

	OutputDebugStringA(Display.c_str());
	//.this->InvalidateRect();
	::InvalidateRect(this->m_hWnd, 0, true);
	::UpdateWindow (this->m_hWnd);
	Sleep(250);




	return 0;
}



LRESULT DuityDlg::OnNMCustomdrawSlider1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.

	pNMCD->hdc;
	pNMCD->rc.top;
	CWindow SlideWindowHandle=GetDlgItem(pNMHDR->idFrom);
	RECT Rectangle;
	SlideWindowHandle.GetClientRect(&Rectangle);
	
	HBRUSH brush = CreateSolidBrush(RGB(200,200,200));
	FillRect(pNMCD->hdc, &Rectangle, brush);
	DeleteObject(brush);

	DrawSecadeLogScale(pNMCD->hdc, Rectangle.top + 13,Rectangle.bottom-14,Rectangle.left+15,4);

	return 0;
}


LRESULT DuityDlg::OnNMReleasedcaptureSlider1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	return 0;
}
