#include "StdAfx.h"
#include "LaserStatusDialog.h"

#define SHRINK_SIZE 140


/**
* CLASS LaserDialog
*
* @brief LaserDialog to give update Progress information.
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:39:11 AM
*
*/
LaserStatusDialog::LaserStatusDialog(void):
	m_bModal (false),
	m_OnTop(false)
{
}

LaserStatusDialog::~LaserStatusDialog(void)
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
INT_PTR LaserStatusDialog::DoModal()
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
BOOL LaserStatusDialog::IsVisible()
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
void LaserStatusDialog::DoModeless(HWND hWndParent)
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
void LaserStatusDialog::SetOnTop(BOOL NewTop)
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
void LaserStatusDialog::Close()
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
LRESULT LaserStatusDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	GetWindowRect(&m_LargeRect);

	m_SmallRect=m_LargeRect;
	m_SmallRect.right-=SHRINK_SIZE;
	SetWindowSmall();

	GetDlgItem(IDC_STATIC_TEMP).GetClientRect(&m_StaticTempRectangle);
	GetDlgItem(IDC_STATIC_TEMP).MapWindowPoints(this->m_hWnd,(LPPOINT)&m_StaticTempRectangle,2);
	
	std::string WindowText="Laser Dialog ( 488 nm )";
	this->SetWindowTextW(CA2W(WindowText.c_str()).m_szBuffer);



	if(m_OnTop)
	{
		SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}else
	{		
		SetWindowPos(HWND_BOTTOM,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
			//::SetForegroundWindow(GetDlgItem(IDD_DIALOG_PROGRESS));
			::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);

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
LRESULT LaserStatusDialog::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);


	return 0;
}

/**
* FUNCTION Sets the window size
*
* @brief Sets the Small Window Size
*
* @version 1.0 
*
* @author David Libby
* @date 6/10/2014 1:39:13 PM
*
* @param set 
*/
void LaserStatusDialog::SetWindowSmall(bool set/*=true*/)
{
	if(set)
	{
		SetWindowPos(NULL,m_SmallRect.left,m_SmallRect.top,m_SmallRect.right-m_SmallRect.left,m_SmallRect.bottom-m_SmallRect.top,SWP_NOZORDER );
		m_bIsSmall=true;
	}else
	{
		SetWindowPos(NULL,m_LargeRect.left,m_LargeRect.top,m_LargeRect.right-m_LargeRect.left,m_SmallRect.bottom-m_SmallRect.top,SWP_NOZORDER );
		m_bIsSmall=false;
	}
}
/**
* FUNCTION OnMouseMove
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 6/10/2014 1:39:48 PM
*
* @param uMsg 
* @param wParam 
* @param lParam 
* @param bHandled 
*
* @return LRESULT 
*/
LRESULT LaserStatusDialog::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!m_bMouseTracking) {
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = this->m_hWnd;

		if (::_TrackMouseEvent(&tme)) {
			m_bMouseTracking = TRUE;
			if(m_bIsSmall)
			{
				SetWindowSmall(false);
			}

		}
	}
	return 0;
}

/**
* FUNCTION OnMouseLeave
*
* @brief When the mouse leaves the current dialog.
*
* @version 1.0 
*
* @author David Libby
* @date 6/10/2014 1:39:53 PM
*
* @param uMsg 
* @param wParam 
* @param lParam 
* @param bHandled 
*
* @return LRESULT 
*/
LRESULT LaserStatusDialog::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO: Add your message handler code here and/or call default
	m_bMouseTracking = FALSE;
	SetWindowSmall(true);
	return 0;
}
/**
* FUNCTION OnWindowPosChanged
*
* @brief When the windows position changes.
*
* @version 1.0 
*
* @author David Libby
* @date 6/10/2014 1:40:06 PM
*
* @param uMsg 
* @param wParam 
* @param lParam 
* @param bHandled 
*
* @return LRESULT 
*/
LRESULT LaserStatusDialog::OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	//lParam  A pointer to a WINDOWPOS structure that contains information about the window's new size and position.
	WINDOWPOS* lpwndpos=(WINDOWPOS*)lParam;

	if(IsVisible())
	{
		int width,height;
		width=m_LargeRect.right-m_LargeRect.left;
		height=m_LargeRect.bottom-m_LargeRect.top;
		m_LargeRect.top=lpwndpos->y;
		m_LargeRect.left=lpwndpos->x;
		m_LargeRect.bottom=m_LargeRect.top+height;
		m_LargeRect.right=m_LargeRect.left+width;

	}

	if(IsVisible())
	{
		int width,height;
		width=m_SmallRect.right-m_SmallRect.left;
		height=m_LargeRect.bottom-m_LargeRect.top;
		m_SmallRect.top=lpwndpos->y;
		m_SmallRect.left=lpwndpos->x;
		m_SmallRect.bottom=m_SmallRect.top+height;
		m_SmallRect.right=m_SmallRect.left+width;

	}



	return 0;
}

/**
* FUNCTION OnPaint
*
* @brief Paint Routine.
*
* @version 1.0 
*
* @author David Libby
* @date 6/10/2014 10:05:16 AM
*
* @param uMsg 
* @param wParam 
* @param lParam 
* @param bHandled 
*
* @return LRESULT 
*/
LRESULT LaserStatusDialog::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DrawTempBackground();

	PAINTSTRUCT ps; 
	HDC hdc; 
	hdc = BeginPaint( &ps); 	


	
	EndPaint( &ps);

	return 0;
}

void LaserStatusDialog::DrawHorizontalGraidentRect(HDC &hdc, COLORREF Start, COLORREF End,const RECT &rect )
{
	int StartRed=GetRValue(Start),StartGreen=GetGValue(Start),StartBlue=GetBValue(Start);
	int EndRed=GetRValue(End),EndGreen=GetGValue(End),EndBlue=GetBValue(End);

	int recWidth=rect.left-rect.right;
	int recHeight=rect.bottom-rect.top;

	for(int i=0;i<recWidth;i++)
	{ 
		int r,g,b;
		r = StartRed + (i * (EndRed-StartRed) / recWidth);
		g = StartGreen + (i * (EndGreen-StartGreen) / recWidth);
		b = StartBlue + (i * (EndBlue-StartBlue) / recWidth);
		RECT RectPlus=rect;
		RectPlus.left+=i;
		FillRect(hdc, &RectPlus, CreateSolidBrush(RGB(r,g,b)));
	}

}

void LaserStatusDialog::DrawVerticalGraidentRect(HDC &hdc, COLORREF Start, COLORREF End,const RECT &rect )
{
	int StartRed=GetRValue(Start),StartGreen=GetGValue(Start),StartBlue=GetBValue(Start);
	int EndRed=GetRValue(End),EndGreen=GetGValue(End),EndBlue=GetBValue(End);

	int recWidth=rect.left-rect.right;
	int recHeight=rect.bottom-rect.top;

	for(int i=0;i<recHeight;i++)
	{ 
		int r,g,b;
		r = StartRed + (i * (EndRed-StartRed) / recHeight);
		g = StartGreen + (i * (EndGreen-StartGreen) / recHeight);
		b = StartBlue + (i * (EndBlue-StartBlue) / recHeight);

		RECT RectPlus=rect;
		RectPlus.top+=i;
		FillRect(hdc, &RectPlus, CreateSolidBrush(RGB(r,g,b)));

	}
}

void LaserStatusDialog::DrawTempBackground()
{
	PAINTSTRUCT ps; 
	HDC hdc; 
	hdc = BeginPaint( &ps); 	


	COLORREF StartGreen = RGB(157,227,66);
	COLORREF EndGreen = RGB(103,195,36);

	COLORREF Start = RGB(157,227,66);
	COLORREF End = RGB(103,195,36);

	COLORREF StartRed = RGB(224,49,45);
	COLORREF EndRed = RGB(161,8,10);

	COLORREF StartYellow = RGB(252,227,127);
	COLORREF EndYellow = RGB(253,205,7);

	int recWidth=m_StaticTempRectangle.left-m_StaticTempRectangle.right;
	int recHeight=m_StaticTempRectangle.bottom-m_StaticTempRectangle.top;


	//RECT GreenZone(0,60,recWidth,recHeight);
	RECT GreenZone(m_StaticTempRectangle);
	//GreenZone=m_StaticTempRectangle;
	GreenZone.top+=60;

	RECT YellowZone(GreenZone);
	YellowZone.top-=30;
	YellowZone.bottom=GreenZone.top;

	RECT RedZone(YellowZone);
	RedZone.top-=30;	
	RedZone.bottom=YellowZone.top;

	RECT NullZone(m_StaticTempRectangle);
	NullZone.top=NullZone.bottom;
	NullZone.bottom=NullZone.top+20;


	DrawVerticalGraidentRect(hdc,StartGreen, EndGreen, GreenZone );
	DrawVerticalGraidentRect(hdc,StartYellow, EndYellow, YellowZone );
	DrawVerticalGraidentRect(hdc,StartRed, EndRed, RedZone );

	DrawTempatureScaleText(hdc,RedZone,"60°C");
	DrawTempatureScaleText(hdc,YellowZone,"50°C");
	DrawTempatureScaleText(hdc,GreenZone,"40°C");
	DrawTempatureScaleText(hdc,NullZone,"0°C");

	
	
	POINT StartLoc;
	StartLoc.x=GreenZone.top - 20;
	StartLoc.y=GreenZone.right+3;


	int PointerHeight=20;
	int PointerWidth=50;
	int PointerOffset=5;

	POINT ptArray[6];
	ptArray[0]=StartLoc;
	
	ptArray[1].x=StartLoc.x+PointerOffset;
	ptArray[1].y=StartLoc.y-PointerHeight/2;

	ptArray[2].x=ptArray[1].x+PointerWidth;
	ptArray[2].y=ptArray[1].y;

	ptArray[3].x=ptArray[2].x;
	ptArray[3].y=StartLoc.y+PointerHeight/2;

	ptArray[4].x=ptArray[1].x;
	ptArray[4].y=ptArray[3].y;


	ptArray[5]=StartLoc;

	RECT PolyTextZone;
	PolyTextZone.top=ptArray[1].y+2;
	PolyTextZone.left=ptArray[1].x;
	PolyTextZone.bottom=ptArray[3].y;
	PolyTextZone.right=ptArray[3].x;




	int OldPolyFillMode=SetPolyFillMode(hdc,WINDING);

	//Polyline (hdc, ptArray,6 );
	//PolyDraw(hdc,ptArray,PT_CLOSEFIGURE,6);
	Polygon(hdc,ptArray,6);
	SetPolyFillMode(hdc,ALTERNATE);

	DrawTempatureScaleText(hdc,PolyTextZone,"155°C");

	EndPaint( &ps);
	

}

void LaserStatusDialog::DrawTempatureScaleText(HDC &hdc,RECT DrawArea,std::string TempString)
{
	
	//DrawArea.top-=19;

	HFONT hFont, hOldFont; 

	// Retrieve a handle to the variable stock font.  
	hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT); 

	LOGFONT LargeFont,SmallFont;
	GetObject ( hFont, sizeof(LOGFONT), &LargeFont );
	LargeFont.lfHeight = 20 ;                    // request a 12-pixel-height font
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
	DrawText(hdc,CA2W(TempString.c_str()).m_szBuffer,TempString.size(),&DrawArea,DT_CENTER);
	DrawArea.top+=pixAtound;
	DrawArea.left-=pixAtound;
	DrawText(hdc,CA2W(TempString.c_str()).m_szBuffer,TempString.size(),&DrawArea,DT_CENTER);
	DrawArea.bottom+=pixAtound;
	DrawArea.left+=pixAtound;
	DrawText(hdc,CA2W(TempString.c_str()).m_szBuffer,TempString.size(),&DrawArea,DT_CENTER);
	DrawArea.bottom-=pixAtound;
	DrawArea.right+=pixAtound;
	DrawText(hdc,CA2W(TempString.c_str()).m_szBuffer,TempString.size(),&DrawArea,DT_CENTER);	
	DrawArea.right-=pixAtound;
	
	//Now draw the font black Centered.
	SetTextColor(hdc, RGB(0,0,0));
	SelectObject(hdc, hCustomSmallFont);
	DrawText(hdc,CA2W(TempString.c_str()).m_szBuffer,TempString.size(),&DrawArea,DT_CENTER);

	SetBkMode(hdc, oldCkgroundMode  );

	// Restore the original font.        
	SelectObject(hdc, hOldFont); 
	// Restore the original Color.        
	SetTextColor(hdc, OldCOlor);



}
