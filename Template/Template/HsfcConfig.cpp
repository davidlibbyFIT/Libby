// HsfcConfig.cpp : implementation file
//

#include "StdAfx.h"
#include "HsfcConfig.h"

//String Crap
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
//String Crap

//Dashes for lines
static int DASH_STANDARD_WIDTH = 10;
static int DASH_WIDE_WIDTH = 20;


static int START_DECADE_NUMBER = 3;

static double EDIT_THRESHOLD_MAX = 6.99;
static double EDIT_THRESHOLD_MIN = 3.0;
static double EDIT_THRESHOLD_SIZE = EDIT_THRESHOLD_MAX - EDIT_THRESHOLD_MIN;

//We use 0-1000 for scale on the Slide control to let us increment by .01
static double THRESHOLD_SCALE_MULTILPLIER = 1000;

static int SLIDE_THRESHOLD_TOP_SCALE = 1000;
static int SLIDE_THRESHOLD_BOTTOM_SCALE = 0;
static int SLIDE_THRESHOLD_SCALE_SIZE = SLIDE_THRESHOLD_TOP_SCALE - SLIDE_THRESHOLD_BOTTOM_SCALE;
static int SLIDE_THRESHOLD_PAGE_INC = 50;
static int SLIDE_THRESHOLD_LINE_INC = 2;

static int SLIDE_GAIN_TOP_SCALE = 1024 ;
static int SLIDE_GAIN_BOTTOM_SCALE = 0;
static int SLIDE_GAIN_MID_SCALE = SLIDE_GAIN_BOTTOM_SCALE + (SLIDE_GAIN_TOP_SCALE-SLIDE_GAIN_BOTTOM_SCALE)/2;

static int EDIT_WIDTH_MAX = 48;
static int EDIT_WIDTH_MIN = 4095;
static int EDIT_WIDTH_DEFAULT = 3200;

static int SLIDE_GAIN_PAGE_INC = 50;
static int SLIDE_GAIN_LINE_INC = 2;

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
	,m_Min_Ch1(EDIT_THRESHOLD_MIN)
	,m_Max_Ch1(EDIT_THRESHOLD_MAX)
	,m_Min_Ch2(EDIT_THRESHOLD_MIN)
	,m_Max_Ch2(EDIT_THRESHOLD_MAX)
	,m_Ret(HSFC_RET_NULL)
	,m_IsDirty(false)
	,m_CheckEnableCh1(false)
	,m_CheckEnableCh2(false)
	,m_Gain(SLIDE_GAIN_MID_SCALE)
	,m_CheckSmallParticle(false)

{
}

HsfcConfig::~HsfcConfig(void)
{
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


	m_EditMinCh1=GetDlgItem(IDC_EDIT_MIN_CH1);
	m_EditMaxCh1=GetDlgItem(IDC_EDIT_MAX_CH1);
	m_EditMinCh2=GetDlgItem(IDC_EDIT_MIN_CH2);
	m_EditMaxCh2=GetDlgItem(IDC_EDIT_MAX_CH2);

	m_ButtonApply=GetDlgItem(ID_APPILY);

	m_SliderMinCh1=GetDlgItem(IDC_SLIDER_MIN_CH1);
	m_SliderMaxCh1=GetDlgItem(IDC_SLIDER_MAX_CH1);
	m_SliderMinCh2=GetDlgItem(IDC_SLIDER_MIN_CH2);
	m_SliderMaxCh2=GetDlgItem(IDC_SLIDER_MAX_CH2);

	m_CheckCh1=GetDlgItem(IDC_CHECK_CH1_TRIGG);
	m_CheckCh2=GetDlgItem(IDC_CHECK_CH2_TRIGG);
	m_CheckSmallPartBox=GetDlgItem(IDC_CHECK_SMALL_PART);
	m_SliderGain=GetDlgItem(IDC_SLIDER_GAIN);


	//set range to 1000 so when we divide we will good decade numbers
	SendMessage(m_SliderMinCh1, TBM_SETRANGE, 0, (LPARAM)MAKELONG(SLIDE_THRESHOLD_BOTTOM_SCALE, SLIDE_THRESHOLD_TOP_SCALE));
	SendMessage(m_SliderMaxCh1, TBM_SETRANGE, 0, (LPARAM)MAKELONG(SLIDE_THRESHOLD_BOTTOM_SCALE, SLIDE_THRESHOLD_TOP_SCALE));
	SendMessage(m_SliderMinCh2, TBM_SETRANGE, 0, (LPARAM)MAKELONG(SLIDE_THRESHOLD_BOTTOM_SCALE, SLIDE_THRESHOLD_TOP_SCALE));
	SendMessage(m_SliderMaxCh2, TBM_SETRANGE, 0, (LPARAM)MAKELONG(SLIDE_THRESHOLD_BOTTOM_SCALE, SLIDE_THRESHOLD_TOP_SCALE));
	SendMessage(m_SliderGain, TBM_SETRANGE, 0, (LPARAM)MAKELONG(SLIDE_GAIN_BOTTOM_SCALE, SLIDE_GAIN_TOP_SCALE));

	//Set Page Increment
	SendMessage(m_SliderMinCh1, TBM_SETPAGESIZE , 0, SLIDE_THRESHOLD_PAGE_INC);
	SendMessage(m_SliderMaxCh1, TBM_SETPAGESIZE , 0, SLIDE_THRESHOLD_PAGE_INC);
	SendMessage(m_SliderMinCh2, TBM_SETPAGESIZE , 0, SLIDE_THRESHOLD_PAGE_INC);
	SendMessage(m_SliderMaxCh2, TBM_SETPAGESIZE , 0, SLIDE_THRESHOLD_PAGE_INC);
	SendMessage(m_SliderGain, TBM_SETPAGESIZE , 0, SLIDE_GAIN_PAGE_INC);

	//Set Single Line increment.
	SendMessage(m_SliderMinCh1, TBM_SETLINESIZE  , 0, SLIDE_THRESHOLD_LINE_INC);
	SendMessage(m_SliderMaxCh1, TBM_SETLINESIZE  , 0, SLIDE_THRESHOLD_LINE_INC);
	SendMessage(m_SliderMinCh2, TBM_SETLINESIZE  , 0, SLIDE_THRESHOLD_LINE_INC);
	SendMessage(m_SliderMaxCh2, TBM_SETLINESIZE  , 0, SLIDE_THRESHOLD_LINE_INC);
	SendMessage(m_SliderGain, TBM_SETLINESIZE  , 0, SLIDE_GAIN_LINE_INC);




	if(m_OnTop)
	{
		SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}else
	{		
		SetWindowPos(HWND_BOTTOM,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	readFromContext();
	m_IsDirty=false;
	RedrawControls();

	return 1;
};


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

	int response = MessageBoxW(L"Are you sure you want to exit without saving changes?",L"Unsaved changes",MB_YESNO|MB_ICONWARNING);
	if(response!=IDYES)
		return 0;
	
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);

	m_Ret=HSFC_RET_CANCEL;

	return HSFC_RET_CANCEL;
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

	m_Ret = HSFC_RET_OK;
	return HSFC_RET_OK;
}


/**
* FUNCTION OnPaint
*
* @brief 
*
* @author DAVID.LIBBY
* @date 5/29/2015 12:24:17 PM
*
* @param uMsg 
* @param wParam 
* @param lParam 
* @param bHandled 
*
* @return LRESULT 
*/
LRESULT HsfcConfig::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PAINTSTRUCT ps; 
	HDC hdc; 
	hdc = BeginPaint( &ps); 	



	DrawVerticalDecadeLogScale(hdc, 111,70 ,281,4);
	DrawVerticalDecadeLogScale(hdc, 290,70 ,280,4);
	DrawHoroztallDecadeLogScale(hdc,190,415,358,3);
	EndPaint( &ps);



	return 0;
}


/**
* FUNCTION DrawVerticalDecadeLogScale
*
* @brief Draws the Vertical Log Scale with Multipal Decades
*
* @author DAVID.LIBBY
* @date 5/29/2015 12:24:24 PM
*
* @param hdc 
* @param xLineStart 
* @param yLineStart 
* @param yLineEnd 
* @param DecadeCount 
*/
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
		_itoa_s(DecadeCount-ct+1+START_DECADE_NUMBER, str, 10);
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
	_itoa_s(START_DECADE_NUMBER, str, 10);
	std::string DecadeString= str;
	RECT LittleDrawArea;
	LittleDrawArea.bottom=DrawArea.top+2;
	LittleDrawArea.left=DrawArea.right;
	LittleDrawArea.top = LittleDrawArea.bottom - 8 ;
	LittleDrawArea.right=LittleDrawArea.left + 8;
	DrawDecadeText(hdc ,LittleDrawArea,10,DecadeString);

}

/**
* FUNCTION DrawVerticalLogScale
*
* @brief 
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:48:31 AM
*
* @param hdc 
* @param yLineStart 
* @param yLineEnd 
* @param xLineStart 
*/
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

/**
* FUNCTION DrawHoroztallLogScale
*
* @brief 
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:48:35 AM
*
* @param hdc 
* @param xLineStart 
* @param yLineStart 
* @param xLineEnd 
*/
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

/**
* FUNCTION DrawHoroztallDecadeLogScale
*
* @brief 
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:48:46 AM
*
* @param hdc 
* @param xLineStart 
* @param yLineStart 
* @param xLineEnd 
* @param DecadeCount 
*/
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
/**
* FUNCTION DrawDecadeText
*
* @brief 
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:48:53 AM
*
* @param hdc 
* @param DrawArea 
* @param size 
* @param TextString 
*/
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

	int gray=220;
	COLORREF OldCOlor = SetTextColor(hdc, RGB(gray,gray,gray));

	hOldFont = (HFONT)SelectObject(hdc, hCustomLargeFont);

	//Build a 1 pix Gray border by drawing the text sihfted in all 4 directions 
	int oldCkgroundMode = SetBkMode(hdc, TRANSPARENT);
	int pixAtound =3;
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


/**
* FUNCTION GetSliderPos
*
* @brief Gets the current slider position.
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:49:02 AM
*
* @param CurWindow 
*
* @return int 
*/
int HsfcConfig::GetSliderPos(CWindow &CurWindow)
{
	return SendMessageW(CurWindow,TBM_GETPOS,0,0);
}
/**
* FUNCTION SetThresholdSliderPos
*
* @brief Sets the Slider to the correct position given a double number.
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:49:19 AM
*
* @param CurWindow 
* @param NewPos 
*/
void HsfcConfig::SetThresholdSliderPos(CWindow &CurWindow,double NewPos)
{
	
	int NewSlidePos=SLIDE_THRESHOLD_TOP_SCALE;
	//We use 0-1000 for scale on the Slide control to let us increment by .01
	if(NewPos -EDIT_THRESHOLD_MIN>0.0)
	{
		NewSlidePos = int(((NewPos -EDIT_THRESHOLD_MIN )/ EDIT_THRESHOLD_SIZE ) * THRESHOLD_SCALE_MULTILPLIER);	
		
		//Reverse the scape for the Control.
		NewSlidePos= SLIDE_THRESHOLD_TOP_SCALE - NewSlidePos +SLIDE_THRESHOLD_BOTTOM_SCALE;
	}

	SendMessageW(CurWindow,TBM_SETPOS,true, NewSlidePos);
}
/**
* FUNCTION IntToString
*
* @brief Convert int to string.
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:49:55 AM
*
* @param Value 
*
* @return std::string 
*/
std::string HsfcConfig::intToString(int Value)
{
	const int i = 3;
	std::ostringstream s;
	s << Value;
	return s.str();
}

/**
* FUNCTION DoubleToString
*
* @brief convert double to string.
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:50:08 AM
*
* @param Value 
* @param decPlaces 
*
* @return std::string 
*/
std::string HsfcConfig::DoubleToString(double Value,int decPlaces/*=2*/)
{
	std::stringstream s;
	// print it into stream using maximum precision
	s << std::fixed << std::setprecision(std::numeric_limits<double>::digits10) << Value;
	std::string res = s.str();
	// Now the res contains something like 1.234567899000000
	// so truncate 9000000000 by hand
	size_t dotIndex = res.find(".");
	std::string final_res = res.substr(0, dotIndex + decPlaces+1);
	return final_res;
}


/**
* FUNCTION SetEditWindowValue
*
* @brief Sets the edit window text
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:50:21 AM
*
* @param CurWindow 
* @param NewValue 
*/
void HsfcConfig::SetEditWindowValue(CWindow &CurWindow,double NewValue)
{
	CurWindow.SetWindowTextW(CA2W(DoubleToString(NewValue).c_str()).m_szBuffer);
}
/**
* FUNCTION SetEditWindowValue
*
* @brief Sets the edit window text
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:50:21 AM
*
* @param CurWindow 
* @param NewValue 
*/
void HsfcConfig::SetEditWindowValue(CWindow &CurWindow,int NewValue)
{
	CurWindow.SetWindowTextW(CA2W(intToString(NewValue).c_str()).m_szBuffer);
}

/**
* FUNCTION GetEditWindowValue
*
* @brief Gets the double value of a given window.
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:50:41 AM
*
* @param CurWindow 
*
* @return double 
*/
double HsfcConfig::GetEditWindowValue(CWindow &CurWindow)
{
	std::string RetWindowString=RetriveStdStringFromCWindow(CurWindow);
	if(RetWindowString.empty())
		return 0.0;
	return (double)atof(RetWindowString.c_str());
}

/**
* FUNCTION SetCh1Min
*
* @brief Sets the channel one minimum.
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:51:01 AM
*
* @param newMin 
* @param refreshEdit 
*
* @return bool 
*/
bool HsfcConfig::setCh1Min(double newMin,bool refreshEdit/*=true*/)
{
	//if it is the same skip
	if(newMin==m_Min_Ch1)
		return true;
	//If it is too small then set to lowest value.
	if(newMin < EDIT_THRESHOLD_MIN)
	{
		newMin = EDIT_THRESHOLD_MIN;
		refreshEdit=true;
	}

	//If it is too large then set to max value
	if(newMin > EDIT_THRESHOLD_MAX)
	{
		newMin = EDIT_THRESHOLD_MAX;
		refreshEdit=true;
	}
	
	//If it is larger than the max then move the max up.
	if(newMin > m_Max_Ch1)
		m_Max_Ch1=newMin;

	//Set the new value
	m_Min_Ch1=newMin;
	
	m_IsDirty = true;
	//If the dialog is visible then redraw the screen
	if(IsVisible())
		RedrawControls(refreshEdit);


	
	
	return true;
}

/**
* FUNCTION SetCh1Max
*
* @brief Set channel one max 
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:53:14 AM
*
* @param newMax 
* @param refreshEdit 
*
* @return bool 
*/
bool HsfcConfig::setCh1Max(double newMax,bool refreshEdit/*=true*/)
{
	if(newMax==m_Max_Ch1)
		return true;

	if(newMax > EDIT_THRESHOLD_MAX)
	{
		newMax = EDIT_THRESHOLD_MAX;
		refreshEdit=true;
	}

	if(newMax < EDIT_THRESHOLD_MIN)
	{
		newMax = EDIT_THRESHOLD_MIN;
		refreshEdit=true;
	}


	if(newMax < m_Min_Ch1)
		m_Min_Ch1=newMax;

	m_Max_Ch1=newMax;
	m_IsDirty = true;
	if(IsVisible())
		RedrawControls(refreshEdit);

	return true;
}
/**
* FUNCTION SetCh2Min
*
* @brief set channel 2 min
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:53:33 AM
*
* @param newMin 
* @param refreshEdit 
*
* @return bool 
*/
bool HsfcConfig::setCh2Min(double newMin,bool refreshEdit/*=true*/)
{
	if(newMin==m_Min_Ch2)
		return true;

	if(newMin < EDIT_THRESHOLD_MIN)
	{
		newMin = EDIT_THRESHOLD_MIN;
		refreshEdit=true;
	}

	if(newMin > EDIT_THRESHOLD_MAX)
	{
		newMin = EDIT_THRESHOLD_MAX;
		refreshEdit=true;
	}


	if(newMin > m_Max_Ch2)
		m_Max_Ch2=newMin;

	m_Min_Ch2=newMin;
	m_IsDirty = true;
	if(IsVisible())
		RedrawControls(refreshEdit);
	
	return true;
}

/**
* FUNCTION SetCh2Max
*
* @brief set channel 2 max
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:53:44 AM
*
* @param newMax 
* @param refreshEdit 
*
* @return bool 
*/
bool HsfcConfig::setCh2Max(double newMax,bool refreshEdit/*=true*/)
{
	if(newMax==m_Max_Ch2)
		return true;

	if(newMax > EDIT_THRESHOLD_MAX)
	{
		newMax = EDIT_THRESHOLD_MAX;
		refreshEdit=true;
	}

	if(newMax < EDIT_THRESHOLD_MIN)
	{
		newMax = EDIT_THRESHOLD_MIN;
		refreshEdit=true;
	}
	
	if(newMax < m_Min_Ch2)
		m_Min_Ch2 = newMax;

	m_Max_Ch2=newMax;
	m_IsDirty = true;
	if(IsVisible())
		RedrawControls(refreshEdit);


	return true;
}



/**
* FUNCTION RetriveStdStringFromCWindow
*
* @brief Set the Std::string from a given edit box.
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:54:14 AM
*
* @param TempWindow 
*
* @return std::string 
*/
std::string HsfcConfig::RetriveStdStringFromCWindow(CWindow &TempWindow)
{
	int TestLength = TempWindow.GetWindowTextLengthW()+1;
	wchar_t* buffer = new wchar_t[TestLength+1];

	TempWindow.GetWindowTextW(buffer,TestLength);
	
	std::string SimpString=CW2A(buffer);

	delete[] buffer;
	return SimpString;
}


/**
* FUNCTION OnVScroll
*
* @brief Respond to windows scroll to recive the slide bar movment 
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:54:37 AM
*
* @param uMsg 
* @param wParam 
* @param lParam 
* @param bHandled 
*
* @return LRESULT 
*/
LRESULT HsfcConfig::OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int nScrollCode = (int) LOWORD(wParam); // scroll bar value
	int nPos = (short int) HIWORD(wParam); // scroll box position

	UINT CrtlUsed=0;

	//Determine what control is being changed.
	if(m_SliderMinCh1.m_hWnd==(HWND) lParam)
	{
		CrtlUsed=IDC_SLIDER_MIN_CH1;
	}else if(m_SliderMaxCh1.m_hWnd==(HWND) lParam)
	{
		CrtlUsed=IDC_SLIDER_MAX_CH1;
	}else if(m_SliderMinCh2.m_hWnd==(HWND) lParam)
	{
		CrtlUsed=IDC_SLIDER_MIN_CH2;
	}else if(m_SliderMaxCh2.m_hWnd==(HWND) lParam)
	{
		CrtlUsed=IDC_SLIDER_MAX_CH2;
	}
	else if(m_SliderGain.m_hWnd==(HWND) lParam)
	{
		CrtlUsed=IDC_SLIDER_GAIN;
	}

	//If we have an active control then respond.
	if(CrtlUsed)
	{
		//Respond to movement command.
		switch (nScrollCode)
		{
			case SB_LINEUP :
			case SB_LINEDOWN :
			case SB_PAGEDOWN :
			case SB_PAGEUP :
			case SB_BOTTOM:
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK :
			case SB_TOP  :
			case SB_ENDSCROLL :
			{
				//Get the windows handel.
				CWindow SlideWindowHandle=(HWND) lParam;

				//Reverse the controller and get the opposed position.
				int CtrlPos= SLIDE_THRESHOLD_TOP_SCALE - GetSliderPos(SlideWindowHandle)+SLIDE_THRESHOLD_BOTTOM_SCALE;
				double dblCtrlPos=EDIT_THRESHOLD_MIN;


				if(CtrlPos!=0)//Interpet the 0/1000 scale to Decimal THreshold value.					
					dblCtrlPos=EDIT_THRESHOLD_MIN+(((double)CtrlPos/THRESHOLD_SCALE_MULTILPLIER)*EDIT_THRESHOLD_SIZE);


				switch (CrtlUsed)
				{
				case IDC_SLIDER_MIN_CH1:
					setCh1Min(dblCtrlPos);
					break;
				case IDC_SLIDER_MAX_CH1:
					setCh1Max(dblCtrlPos);
					break;
				case IDC_SLIDER_MIN_CH2:
					setCh2Min(dblCtrlPos);
					break;
				case IDC_SLIDER_MAX_CH2:
					setCh2Max(dblCtrlPos);
					break;
				case IDC_SLIDER_GAIN:
					{
						int dd=1;
						m_IsDirty=true;
						RedrawControls();
						break;
					}
				}					


			}
			{
				break;
			}
		}

	}
	

	return 0;
}



/**
* FUNCTION RetriveIntFromCWindow
*
* @brief Get an int value from an edit window.
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:57:00 AM
*
* @param TempWindow 
*
* @return int 
*/
int HsfcConfig::RetriveIntFromCWindow(CWindow &TempWindow)
{
	std::string WindosString=RetriveStdStringFromCWindow(TempWindow);
	if(WindosString.empty())
		return -1;
	return atoi( WindosString.c_str() );
}
/**
* FUNCTION RetriveDoubleFromCWindow
*
* @brief Get a double value form an edit window.
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:57:21 AM
*
* @param TempWindow 
*
* @return double 
*/
double HsfcConfig::RetriveDoubleFromCWindow(CWindow &TempWindow)
{
	std::string WindosString=RetriveStdStringFromCWindow(TempWindow);
	if(WindosString.empty())
		return -1.0;
	return atof(WindosString.c_str());
}


/**
* FUNCTION OnEnKillfocusEditThreshold
*
* @brief AFter edit controll focus lost then check value.
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:57:57 AM
*
* @param wNotifyCode 
* @param wID 
* @param hWndCtl 
* @param bHandled 
*
* @return LRESULT 
*/
LRESULT HsfcConfig::OnEnKillfocusEditThreshold(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO: Add your control notification handler code here
	double newValue=RetriveDoubleFromCWindow((CWindow)hWndCtl);

	switch (wID)
	{
	case IDC_EDIT_MIN_CH1:
		{
			setCh1Min(newValue,false);
			break;
		}
	case IDC_EDIT_MAX_CH1:
		{
			setCh1Max(newValue,false);
			break;
		}
	case IDC_EDIT_MIN_CH2:
		{
			setCh2Min(newValue,false);
			break;
		}
	case IDC_EDIT_MAX_CH2:
		{
			setCh2Max(newValue,false);
			break;
		}
	}


	return 0;
}





/**
* FUNCTION RedrawControls
*
* @brief Redraw all controlls on the screen.
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:53:56 AM
*
* @param refreshEdit 
*/
void HsfcConfig::RedrawControls(bool refreshEdit/*=true*/)
{


	if(refreshEdit)//if we are in the edit mode then don't update the windows..
	{
		SetEditWindowValue(m_EditMinCh1,m_Min_Ch1);
		SetEditWindowValue(m_EditMaxCh1,m_Max_Ch1);

		SetEditWindowValue(m_EditMinCh2,m_Min_Ch2);
		SetEditWindowValue(m_EditMaxCh2,m_Max_Ch2);

		SetEditWindowValue(m_EditMinCh1,m_Min_Ch1);
		SetEditWindowValue(m_EditMaxCh1,m_Max_Ch1);

		SetEditWindowValue(m_EditMinCh2,m_Min_Ch2);
		SetEditWindowValue(m_EditMaxCh2,m_Max_Ch2);

	}

	m_ButtonApply.EnableWindow(m_IsDirty);
	SendMessage(m_CheckCh1, BM_SETCHECK,m_CheckEnableCh1, 0);
	SendMessage(m_CheckCh2, BM_SETCHECK,m_CheckEnableCh2, 0);
	SendMessage(m_CheckSmallPartBox, BM_SETCHECK,m_CheckSmallParticle, 0);

	SendMessageW(m_SliderGain,TBM_SETPOS,true, m_Gain);

	SetThresholdSliderPos(m_SliderMinCh1,m_Min_Ch1);
	SetThresholdSliderPos(m_SliderMaxCh1,m_Max_Ch1);

	SetThresholdSliderPos(m_SliderMinCh2,m_Min_Ch2);
	SetThresholdSliderPos(m_SliderMaxCh2,m_Max_Ch2);

}


LRESULT HsfcConfig::OnBnClickedCheckCh1Trigg(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	m_CheckEnableCh1=!m_CheckEnableCh1;
	m_IsDirty=true;
	RedrawControls();

	return 0;
}


LRESULT HsfcConfig::OnBnClickedCheckCh2Trigg(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	m_CheckEnableCh2=!m_CheckEnableCh2;
	m_IsDirty=true;
	RedrawControls();
	return 0;
}


LRESULT HsfcConfig::OnBnClickedCheckSmallPart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	m_CheckSmallParticle=!m_CheckSmallParticle;
	m_IsDirty=true;
	RedrawControls();
	return 0;
}


LRESULT HsfcConfig::OnBnClickedApply(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	m_Ret=HSFC_RET_APPILY;
	saveToContext();
	m_IsDirty=false;
	RedrawControls();
	return 0;

}


LRESULT HsfcConfig::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO: Add your message handler code here and/or call default
	int nScrollCode = (int) LOWORD(wParam); // scroll bar value
	int nPos = (short int) HIWORD(wParam); // scroll box position

	UINT CrtlUsed=0;

	 if(m_SliderGain.m_hWnd==(HWND) lParam)
	{
		CrtlUsed=IDC_SLIDER_GAIN;
	}

	//If we have an active control then respond.
	if(CrtlUsed)
	{
		//Respond to movement command.
		switch (nScrollCode)
		{
		case SB_ENDSCROLL :
			break;
		case SB_LINEUP :
		case SB_LINEDOWN :
		case SB_PAGEDOWN :
		case SB_PAGEUP :
		case SB_BOTTOM:
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK :
		case SB_TOP  :
			{
				switch (CrtlUsed)
				{
				case IDC_SLIDER_GAIN:
					{
						if(m_Gain!=nPos)
						{
							m_IsDirty=true;
							m_Gain=nPos;
							RedrawControls();
						}
						break;
					}
				}					


			}
			{
				break;
			}
		}

	}


	return 0;
}


void HsfcConfig::setCh1Enable(bool newValue)
{
	m_CheckEnableCh1=newValue;
	m_IsDirty=true;
	if(IsVisible())
		RedrawControls();
}
void HsfcConfig::setCh2Enable(bool newValue)
{
	m_CheckEnableCh2=newValue;
	m_IsDirty=true;
	if(IsVisible())
		RedrawControls();

}
void HsfcConfig::setGain(int NewGain)
{
	m_Gain=NewGain;
	m_IsDirty=true;
	if(IsVisible())
		RedrawControls();

}
void HsfcConfig::setSmallParticle(bool newValue)
{
	m_CheckSmallParticle=newValue;
	m_IsDirty=true;
	if(IsVisible())
		RedrawControls();

}

LRESULT HsfcConfig::OnBnClickedRestore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	readFromContext();
	m_IsDirty=false;
	RedrawControls();
	return 0;
}


/**
* FUNCTION saveToContext
*
* @brief Saves Current Dialog to context
*
* @author DAVID.LIBBY
* @date 6/2/2015 2:25:34 PM
*
*/
void HsfcConfig::saveToContext()
{
}
/**
* FUNCTION readFromContext
*
* @brief Reads Context and populates dialog settings
*
* @author DAVID.LIBBY
* @date 6/2/2015 2:25:50 PM
*
*/
void HsfcConfig::readFromContext()
{

	setCh1Min			(4.55);
	setCh1Max			(6.0);
	setCh2Min			(3.2);
	setCh2Max			(4.55);
	setCh1Enable		(true);
	setCh2Enable		(false);
	setGain				(512);
	setSmallParticle	(true);
}


