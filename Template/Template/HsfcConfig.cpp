// HsfcConfig.cpp : implementation file
//

#include "StdAfx.h"
#include "HsfcConfig.h"


int yLineStartHC=10;
int xLineStartHC=100;
static int yLineEnd=400;

static int DASH_STANDARD_WIDTH = 10;
static int DASH_WIDE_WIDTH = 20;



/**
* CLASS HsfcConfig
*
* @brief HsfcConfig Back Flush to clean
*
* @version 1.0 
*
* @author David Libby
*
*/
HsfcConfig::HsfcConfig(void):
m_bModal (false)
	,m_OnTop(false)	
{
}

HsfcConfig::~HsfcConfig(void)
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
INT_PTR HsfcConfig::DoModal()
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
BOOL HsfcConfig::IsVisible()
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
void HsfcConfig::DoModeless(HWND hWndParent)
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
void HsfcConfig::SetOnTop(BOOL NewTop)
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
void HsfcConfig::Close()
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
LRESULT HsfcConfig::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	//std::string Start_mL="1.0";
	//m_EditBoxExample=GetDlgItem(IDC_EDIT_AMOUNT_ML);
	//m_EditBoxExample.SetWindowTextW(CA2W(Start_mL.c_str()).m_szBuffer);

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
LRESULT HsfcConfig::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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
LRESULT HsfcConfig::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);
	return 0;
}







LRESULT HsfcConfig::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PAINTSTRUCT ps; 
	HDC hdc; 
	hdc = BeginPaint( &ps); 	

	int LIneLength=yLineEnd-yLineStartHC;


	DrawVerticalDecadeLogScale(hdc, 111,70 ,280,4);
	DrawVerticalDecadeLogScale(hdc, 290,70 ,280,4);

	DrawHoroztallDecadeLogScale(hdc,190,415,358,4);
	EndPaint( &ps);



	return 0;
}


void HsfcConfig::DrawVerticalDecadeLogScale(HDC &hdc, int xLineStart, int yLineStart,int yLineEnd,int DecadeCount)
{
	LPPOINT lpPoint(0);
	int LineLength=yLineEnd-yLineStart;
	int DecadeLength=(int)(LineLength/DecadeCount);
	int LastLineEnd=yLineStart;



	for (int ct=1;ct<=DecadeCount;ct++)
	{
		DrawVerticalLogScale(hdc, LastLineEnd,LastLineEnd+DecadeLength,xLineStart);

		MoveToEx (hdc,xLineStart+DASH_WIDE_WIDTH,LastLineEnd,lpPoint);
		LineTo (hdc,xLineStart-DASH_WIDE_WIDTH,LastLineEnd);

		RECT DrawArea;
		DrawArea.top=LastLineEnd-7;
		DrawArea.bottom=DrawArea.top+15;
		DrawArea.left=xLineStart-DASH_WIDE_WIDTH/2+3;
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

	RECT DrawArea;
	DrawArea.top=yLineEnd-10;
	DrawArea.bottom=DrawArea.top+15;
	DrawArea.left=xLineStart-DASH_WIDE_WIDTH/2+3;
	DrawArea.right=DrawArea.left+15;

	DrawDecadeText(hdc ,DrawArea,14,"10");

	char str[20];
	_itoa_s(0, str, 10);
	std::string DecadeString= str;
	RECT LittleDrawArea;
	LittleDrawArea.bottom=DrawArea.top+2;
	LittleDrawArea.left=DrawArea.right;
	LittleDrawArea.top = LittleDrawArea.bottom - 8 ;
	LittleDrawArea.right=LittleDrawArea.left + 8;
	DrawDecadeText(hdc ,LittleDrawArea,10,DecadeString);

}

void HsfcConfig::DrawVerticalLogScale(HDC &hdc, int yLineStart,int yLineEnd, int xLineStart)
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
			MoveToEx (hdc,xLineStart-DASH_STANDARD_WIDTH,yLineEnd-(int)YPos,lpPoint);
			LineTo (hdc,xLineStart+DASH_STANDARD_WIDTH,yLineEnd-(int)YPos);
			PreviousPix=yLineEnd-(int)YPos;


		}
	}


}

void HsfcConfig::DrawHoroztallLogScale(HDC &hdc, int xLineStart, int yLineStart,int xLineEnd)
{
	LPPOINT lpPoint(0);

	int LineLength=xLineEnd-xLineStart;

	//DrawCenterLine
	MoveToEx (hdc,xLineStart,yLineStart,lpPoint);
	LineTo(hdc,xLineEnd,yLineStart);

	double DecadeValue = 10;//Decade Value Must Be 10
	double CurLineLength=LineLength;
	int PreviousPix=0;
	while(DecadeValue >= 1)
	{
		double logI=log10((double) DecadeValue );
		double XPos =xLineStart +( LineLength * log10((double) DecadeValue ));
		DecadeValue --;
		if(XPos>=xLineStart)
		{
			MoveToEx (hdc,(int)XPos,yLineStart-DASH_STANDARD_WIDTH,lpPoint);
			LineTo (hdc,(int)XPos,yLineStart);
			PreviousPix=(int)XPos;


		}
	}


}

void HsfcConfig::DrawHoroztallDecadeLogScale(HDC &hdc, int xLineStart, int yLineStart,int xLineEnd,int DecadeCount)
{
	LPPOINT lpPoint(0);
	int LineLength=xLineEnd-xLineStart;
	int DecadeLength=(int)(LineLength/DecadeCount);
	int LastLineEnd=xLineStart;



	for (int ct=0;ct<DecadeCount;ct++)
	{
		RECT DrawArea;
		DrawArea.top=yLineStart-33;
		DrawArea.bottom=DrawArea.top+15;
		DrawArea.left=LastLineEnd-DASH_WIDE_WIDTH/2+3;
		DrawArea.right=DrawArea.left+15;

		DrawDecadeText(hdc ,DrawArea,14,"10");

		char str[20];
		_itoa_s(ct, str, 10);
		std::string DecadeString= str;
		RECT LittleDrawArea;
		LittleDrawArea.bottom=DrawArea.top+2;
		LittleDrawArea.left=DrawArea.right;
		LittleDrawArea.top = LittleDrawArea.bottom - 8 ;
		LittleDrawArea.right=LittleDrawArea.left + 8;

		DrawDecadeText(hdc ,LittleDrawArea,10,DecadeString);



		DrawHoroztallLogScale(hdc, LastLineEnd,yLineStart,LastLineEnd+DecadeLength);

		MoveToEx (hdc,LastLineEnd,yLineStart-DASH_WIDE_WIDTH,lpPoint);
		LineTo (hdc,LastLineEnd,yLineStart);

		LastLineEnd+=DecadeLength;

	}
			MoveToEx (hdc,LastLineEnd,yLineStart-DASH_WIDE_WIDTH,lpPoint);
		LineTo (hdc,LastLineEnd,yLineStart);

	RECT DrawArea;
	DrawArea.top=yLineStart-33;
	DrawArea.bottom=DrawArea.top+15;
	DrawArea.left=LastLineEnd-DASH_WIDE_WIDTH/2+3;
	DrawArea.right=DrawArea.left+15;

	DrawDecadeText(hdc ,DrawArea,14,"10");

	char str[20];
	_itoa_s(DecadeCount, str, 10);
	std::string DecadeString= str;
	RECT LittleDrawArea;
	LittleDrawArea.bottom=DrawArea.top+2;
	LittleDrawArea.left=DrawArea.right;
	LittleDrawArea.top = LittleDrawArea.bottom - 8 ;
	LittleDrawArea.right=LittleDrawArea.left + 8;

	DrawDecadeText(hdc ,LittleDrawArea,10,DecadeString);


}
void HsfcConfig::DrawDecadeText(HDC &hdc,RECT DrawArea,int size,std::string TextString)
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
