#include <Windows.h>
#include <CommCtrl.h>
#include "tchar.h"
#include "resource.h"
#include "gogo.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>

/*

Disclaimer from you old Pal David Libby, here I sit 6/5/2015, I was tasked with making a dialog and property sheet for setting
the HSFC context settings. after making the Dialog (a thing of beauty) I I looked at how to attach it to a property sheet. 
Well lo and behold the property sheet is not a dialog it needs a DialogProc, So I struggles for a while and I have finally
Settled on this solution, This is not ad Winston Churchill would say my "Finest Hour", so suck it up, buckle up and ripple 
with the c version of the HSFCControlDialog. Glory at my use of Globals in the Cpp file, Revel in Message handler, and prepare 
to be astounded that I was able to translate a CDialogImpl Dialog to this privative format.

THank you and goodnight.

David S Libby ESQ.

*/


typedef enum HSFC_ConfigReturn
{
	HSFC_RET_OK
	, HSFC_RET_CANCEL
	, HSFC_RET_APPLY
	, HSFC_RET_NULL
};


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
static int SLIDE_THRESHOLD_BOTTOg_SCALE = 0;
static int SLIDE_THRESHOLD_SCALE_SIZE = SLIDE_THRESHOLD_TOP_SCALE - SLIDE_THRESHOLD_BOTTOg_SCALE;
static int SLIDE_THRESHOLD_PAGE_INC = 50;
static int SLIDE_THRESHOLD_LINE_INC = 2;

static int SLIDE_GAIN_TOP_SCALE = 3 ;
static int SLIDE_GAIN_BOTTOg_SCALE = 0;
static int SLIDE_GAIN_MID_SCALE = 0;

int static GAIN_0 = 0;
int static GAIN_1 = 342;
int static GAIN_2 = 682;
int static GAIN_3 = 1024;

static int EDIT_WIDTH_MAX = 48;
static int EDIT_WIDTH_MIN = 4095;
static int EDIT_WIDTH_DEFAULT = 3200;

static int SLIDE_GAIN_PAGE_INC = 1;
static int SLIDE_GAIN_LINE_INC = 1;


//Not a huge fan of using these global variables here but I need to support this dialog in both style sheet and 
//a dialog app

//! Track Modal State
static BOOL g_bModal=false;
//! Always On Top
static BOOL g_OnTop=false;
//! User Cancel variable.
static bool g_UserCancel=false;

static HSFC_ConfigReturn g_Ret=HSFC_RET_NULL;
static bool g_IsDirty=false;

static HWND g_EditMinCh1=NULL;
static HWND g_EditMaxCh1=NULL;
static HWND g_EditMinCh2=NULL;
static HWND g_EditMaxCh2=NULL;

static HWND g_SliderMinCh1=NULL;
static HWND g_SliderMaxCh1=NULL;
static HWND g_SliderMinCh2=NULL;
static HWND g_SliderMaxCh2=NULL;

static HWND g_CheckCh1=NULL;
static HWND g_CheckCh2=NULL;
static HWND g_CheckSmallPartBox=NULL;
static HWND g_SliderGain=NULL;
static HWND g_ButtonApply=NULL;

static bool g_CheckEnableCh1=false;
static bool g_CheckEnableCh2=false;
static bool g_CheckSmallParticle=false;

static INT g_Gain=SLIDE_GAIN_MID_SCALE;

static double g_Min_Ch1=EDIT_THRESHOLD_MIN;
static double g_Max_Ch1=EDIT_THRESHOLD_MAX;
static double g_Min_Ch2=EDIT_THRESHOLD_MIN;
static double g_Max_Ch2=EDIT_THRESHOLD_MAX;

static HWND g_hDialog=NULL;

/**
* FUNCTION initVar
*
* @brief Init all the variables to default values..
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:54:18 PM
*
*/
void initVar()
{
	//! Track Modal State
	g_bModal=false;
	//! Always On Top
	g_OnTop=false;
	//! User Cancel variable.
	g_UserCancel=false;

	g_Ret=HSFC_RET_NULL;
	g_IsDirty=false;

	g_EditMinCh1=NULL;
	g_EditMaxCh1=NULL;
	g_EditMinCh2=NULL;
	g_EditMaxCh2=NULL;

	g_SliderMinCh1=NULL;
	g_SliderMaxCh1=NULL;
	g_SliderMinCh2=NULL;
	g_SliderMaxCh2=NULL;

	g_CheckCh1=NULL;
	g_CheckCh2=NULL;
	g_CheckSmallPartBox=NULL;
	g_SliderGain=NULL;
	g_ButtonApply=NULL;

	g_CheckEnableCh1=false;
	g_CheckEnableCh2=false;
	g_CheckSmallParticle=false;

	g_Gain=SLIDE_GAIN_MID_SCALE;

	g_Min_Ch1=EDIT_THRESHOLD_MIN;
	g_Max_Ch1=EDIT_THRESHOLD_MAX;
	g_Min_Ch2=EDIT_THRESHOLD_MIN;
	g_Max_Ch2=EDIT_THRESHOLD_MAX;
}


/**
* FUNCTION HSFCControlProc
*
* @brief Process Messages
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:38:42 PM
*
* @param hDlg 
* @param msg 
* @param wParam 
* @param lParam 
*
* @return INT_PTR 
*/
INT_PTR CALLBACK HSFCControlProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch(msg)
	{
		case WM_INITDIALOG:
			{
				OnInitDialog(hDlg);
				break;
			}
		case WM_PAINT:
			{
				OnPaint(hDlg);
				break;
			}
		case WM_VSCROLL:
			{
				OnVScroll(wParam, lParam);
				break;
			}	
		case WM_HSCROLL:
			{
				OnHScroll(wParam, lParam);
				break;
			}	
		case WM_COMMAND:
			{
				return WmCommands(wParam);
				break;
			}
		case WM_CLOSE:
			{
				return OnCancel();
				break;
			}
		case WM_DESTROY:
			break;

		case WM_TIMER:
			break;

	}

	return FALSE;
}

/**
* FUNCTION WmCommands
*
* @brief Command Message handler
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:59:32 PM
*
* @param wParam 
*
* @return INT_PTR 
*/
INT_PTR CALLBACK WmCommands(WPARAM wParam)
{
	switch(LOWORD(wParam))
	{

	case IDCANCEL:
		{
			return OnCancel();
			break;
		}

	case IDOK:
		{
			return OnOk();

			break;
		}

	case IDC_EDIT_MIN_CH1:
		{
			return KillFocus(wParam, g_EditMinCh1);
			break;
		}

	case IDC_EDIT_MAX_CH1:
		{
			return KillFocus(wParam, g_EditMaxCh1);
			break;
		}
	case IDC_EDIT_MIN_CH2:
		{
			return KillFocus(wParam, g_EditMinCh2);
			break;
		}
	case IDC_EDIT_MAX_CH2:
		{
			return KillFocus(wParam, g_EditMaxCh2);
			break;
		}
	case IDC_CHECK_CH1_TRIGG:
		{
			return OnCheckCh1(wParam);

			break;
		}
	case IDC_CHECK_CH2_TRIGG:
		{
			return OnCheckCh2(wParam);

			break;
		}
	case IDC_CHECK_SMALL_PART:
		{
			return OnCheckSmallPart(wParam);

			break;
		}
	case ID_APPLY_HSFC:
		{
			return OnButtonClickAppily(wParam);

			break;
		}
	case ID_RESTORE:
		{
			return OnButtonClickRestore(wParam);
			break;
		}

	}
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
void saveToContext()
{
	//Save Crap to context

	g_IsDirty=false;
	RedrawControls();
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
void readFromContext()
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
bool setCh1Min(double newMin,bool refreshEdit/*=true*/)
{
	//if it is the same skip
	if(newMin==g_Min_Ch1)
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
	if(newMin > g_Max_Ch1)
		g_Max_Ch1=newMin;

	//Set the new value
	g_Min_Ch1=newMin;

	g_IsDirty = true;
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
bool setCh1Max(double newMax,bool refreshEdit/*=true*/)
{
	if(newMax==g_Max_Ch1)
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


	if(newMax < g_Min_Ch1)
		g_Min_Ch1=newMax;

	g_Max_Ch1=newMax;
	g_IsDirty = true;
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
bool setCh2Min(double newMin,bool refreshEdit/*=true*/)
{
	if(newMin==g_Min_Ch2)
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


	if(newMin > g_Max_Ch2)
		g_Max_Ch2=newMin;

	g_Min_Ch2=newMin;
	g_IsDirty = true;
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
bool setCh2Max(double newMax,bool refreshEdit/*=true*/)
{
	if(newMax==g_Max_Ch2)
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

	if(newMax < g_Min_Ch2)
		g_Min_Ch2 = newMax;

	g_Max_Ch2=newMax;
	g_IsDirty = true;
	if(IsVisible())
		RedrawControls(refreshEdit);


	return true;
}



/**
* FUNCTION RetriveStdStringFromHWND
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
std::string RetriveStdStringFromHWND(HWND &TempWindow)
{
	int TestLength = GetWindowTextLength(TempWindow)+1;
	wchar_t* buffer = new wchar_t[TestLength+1];

	GetWindowText(TempWindow,buffer,TestLength);
	std::wstring wideString(buffer);

	std::string SimpString( wideString.begin(), wideString.end());

	delete[] buffer;
	return SimpString;
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
BOOL IsVisible()
{
	
	g_hDialog;
	BOOL ret =FALSE;
	if(IsWindow(g_hDialog))
	{
		ret = IsWindowVisible(g_hDialog);
	}
	return ret;

}

/**
* FUNCTION RedrawControls
*
* @brief Redraw all controls on the screen.
*
* @author DAVID.LIBBY
* @date 6/1/2015 9:53:56 AM
*
* @param refreshEdit 
*/
void RedrawControls(bool refreshEdit/*=true*/)
{


	if(refreshEdit)//if we are in the edit mode then don't update the windows..
	{
		SetEditWindowValue(g_EditMinCh1,g_Min_Ch1);
		SetEditWindowValue(g_EditMaxCh1,g_Max_Ch1);

		SetEditWindowValue(g_EditMinCh2,g_Min_Ch2);
		SetEditWindowValue(g_EditMaxCh2,g_Max_Ch2);

		SetEditWindowValue(g_EditMinCh1,g_Min_Ch1);
		SetEditWindowValue(g_EditMaxCh1,g_Max_Ch1);

		SetEditWindowValue(g_EditMinCh2,g_Min_Ch2);
		SetEditWindowValue(g_EditMaxCh2,g_Max_Ch2);

	}

	
	EnableWindow(g_ButtonApply,g_IsDirty);
	SendMessage(g_CheckCh1, BM_SETCHECK,g_CheckEnableCh1, 0);
	SendMessage(g_CheckCh2, BM_SETCHECK,g_CheckEnableCh2, 0);
	SendMessage(g_CheckSmallPartBox, BM_SETCHECK,g_CheckSmallParticle, 0);

	setGainSliderPos(g_Gain);

	SetThresholdSliderPos(g_SliderMinCh1,g_Min_Ch1);
	SetThresholdSliderPos(g_SliderMaxCh1,g_Max_Ch1);

	SetThresholdSliderPos(g_SliderMinCh2,g_Min_Ch2);
	SetThresholdSliderPos(g_SliderMaxCh2,g_Max_Ch2);

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
void SetEditWindowValue(HWND &CurWindow,double NewValue)
{
	std::string standardString=HSFCDoubleToString(NewValue);
	std::wstring ff=strToStrWide(HSFCDoubleToString(NewValue)).c_str();
	LPCTSTR lwstirng=ff.c_str();
	SetWindowText(CurWindow,lwstirng);
}

std::wstring strToStrWide(std::string &NewString)
{
	return std::wstring (NewString.begin(),NewString.end());
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
std::string HSFCDoubleToString(double Value,int decPlaces/*=2*/)
{
	std::stringstream s;
	// print it into stream using maximum precision
	s << std::fixed << std::setprecision(std::numeric_limits<double>::digits10) << Value;
	std::string res = s.str();
	// Now the res contains something like 1.234567899000000
	// Now the res contains something like 1.234567899000000
	// so truncate 9000000000 by hand
	size_t dotIndex = res.find(".");
	std::string final_res = res.substr(0, dotIndex + decPlaces+1);
	return final_res;
}

/**
* FUNCTION setGainSliderPos
*
* @brief Gets Sets the slider pos to one of 4 positions given a Gain Number.
*
* @author DAVID.LIBBY
* @date 6/3/2015 9:24:52 AM
*
* @param newGain 
*/
void setGainSliderPos(int newGain)
{
	int pos=GAIN_0;
	if( newGain <=GAIN_0 )
	{
		pos=0;
	}else if(newGain>=GAIN_1 && newGain <GAIN_2 )	
	{
		pos=1;
	}else if(newGain>=GAIN_2 && newGain <GAIN_3 )
	{
		pos=2;
	}else if(newGain>=GAIN_3  )
	{
		pos=3;
	}

	SendMessageW(g_SliderGain,TBM_SETPOS,true, pos);;
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
void SetThresholdSliderPos(HWND &CurWindow,double NewPos)
{

	int NewSlidePos=SLIDE_THRESHOLD_TOP_SCALE;
	//We use 0-1000 for scale on the Slide control to let us increment by .01
	if(NewPos -EDIT_THRESHOLD_MIN>0.0)
	{
		NewSlidePos = int(((NewPos -EDIT_THRESHOLD_MIN )/ EDIT_THRESHOLD_SIZE ) * THRESHOLD_SCALE_MULTILPLIER);	

		//Reverse the scape for the Control.
		NewSlidePos= SLIDE_THRESHOLD_TOP_SCALE - NewSlidePos +SLIDE_THRESHOLD_BOTTOg_SCALE;
	}

	SendMessageW(CurWindow,TBM_SETPOS,true, NewSlidePos);
}

/**
* FUNCTION setGain
*
* @brief Set Gain
*
* @author DAVID.LIBBY
* @date 6/2/2015 2:58:18 PM
*
* @param NewGain 
*/
void setGain(int NewGain)
{
	g_Gain=NewGain;
	g_IsDirty=true;
	if(IsVisible())
		RedrawControls();

}
/**
* FUNCTION setCh1Enable
*
* @brief Set state of enable Chanel one
*
* @author DAVID.LIBBY
* @date 6/2/2015 2:57:44 PM
*
* @param newValue 
*/
void setCh1Enable(bool newValue)
{
	g_CheckEnableCh1=newValue;
	g_IsDirty=true;
	if(IsVisible())
		RedrawControls();
}

/**
* FUNCTION setCh2Enable
*
* @brief Set state of enable Chanel two
*
* @author DAVID.LIBBY
* @date 6/2/2015 2:58:02 PM
*
* @param newValue 
*/
void setCh2Enable(bool newValue)
{
	g_CheckEnableCh2=newValue;
	g_IsDirty=true;
	if(IsVisible())
		RedrawControls();

}

/**
* FUNCTION setSmallParticle
*
* @brief On if the small particle "Filter" is enabled.
*
* @author DAVID.LIBBY
* @date 6/2/2015 2:58:27 PM
*
* @param newValue 
*/
void setSmallParticle(bool newValue)
{
	g_CheckSmallParticle=newValue;
	g_IsDirty=true;
	if(IsVisible())
		RedrawControls();

}

/**
* FUNCTION OnInitDialog
*
* @brief Init Dialog Message Handler
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:55:31 PM
*
* @param hDlg 
*/
void OnInitDialog(HWND hDlg)
{
	
	g_hDialog=hDlg;
	g_EditMinCh1=GetDlgItem(hDlg,IDC_EDIT_MIN_CH1);
	g_EditMaxCh1=GetDlgItem(hDlg,IDC_EDIT_MAX_CH1);
	g_EditMinCh2=GetDlgItem(hDlg,IDC_EDIT_MIN_CH2);
	g_EditMaxCh2=GetDlgItem(hDlg,IDC_EDIT_MAX_CH2);

	g_ButtonApply=GetDlgItem(hDlg,ID_APPLY_HSFC);

	g_SliderMinCh1=GetDlgItem(hDlg,IDC_SLIDER_MIN_CH1);
	g_SliderMaxCh1=GetDlgItem(hDlg,IDC_SLIDER_MAX_CH1);
	g_SliderMinCh2=GetDlgItem(hDlg,IDC_SLIDER_MIN_CH2);
	g_SliderMaxCh2=GetDlgItem(hDlg,IDC_SLIDER_MAX_CH2);

	g_CheckCh1=GetDlgItem(hDlg,IDC_CHECK_CH1_TRIGG);
	g_CheckCh2=GetDlgItem(hDlg,IDC_CHECK_CH2_TRIGG);
	g_CheckSmallPartBox=GetDlgItem(hDlg,IDC_CHECK_SMALL_PART);
	g_SliderGain=GetDlgItem(hDlg,IDC_SLIDER_GAIN);


	//set range to 1000 so when we divide we will good decade numbers
	SendMessage(g_SliderMinCh1, TBM_SETRANGE, 0, (LPARAM)MAKELONG(SLIDE_THRESHOLD_BOTTOg_SCALE, SLIDE_THRESHOLD_TOP_SCALE));
	SendMessage(g_SliderMaxCh1, TBM_SETRANGE, 0, (LPARAM)MAKELONG(SLIDE_THRESHOLD_BOTTOg_SCALE, SLIDE_THRESHOLD_TOP_SCALE));
	SendMessage(g_SliderMinCh2, TBM_SETRANGE, 0, (LPARAM)MAKELONG(SLIDE_THRESHOLD_BOTTOg_SCALE, SLIDE_THRESHOLD_TOP_SCALE));
	SendMessage(g_SliderMaxCh2, TBM_SETRANGE, 0, (LPARAM)MAKELONG(SLIDE_THRESHOLD_BOTTOg_SCALE, SLIDE_THRESHOLD_TOP_SCALE));
	SendMessage(g_SliderGain, TBM_SETRANGE, 0, (LPARAM)MAKELONG(SLIDE_GAIN_BOTTOg_SCALE, SLIDE_GAIN_TOP_SCALE));

	//Set Page Increment
	SendMessage(g_SliderMinCh1, TBM_SETPAGESIZE , 0, SLIDE_THRESHOLD_PAGE_INC);
	SendMessage(g_SliderMaxCh1, TBM_SETPAGESIZE , 0, SLIDE_THRESHOLD_PAGE_INC);
	SendMessage(g_SliderMinCh2, TBM_SETPAGESIZE , 0, SLIDE_THRESHOLD_PAGE_INC);
	SendMessage(g_SliderMaxCh2, TBM_SETPAGESIZE , 0, SLIDE_THRESHOLD_PAGE_INC);
	SendMessage(g_SliderGain, TBM_SETPAGESIZE , 0, SLIDE_GAIN_PAGE_INC);

	//Set Single Line increment.
	SendMessage(g_SliderMinCh1, TBM_SETLINESIZE  , 0, SLIDE_THRESHOLD_LINE_INC);
	SendMessage(g_SliderMaxCh1, TBM_SETLINESIZE  , 0, SLIDE_THRESHOLD_LINE_INC);
	SendMessage(g_SliderMinCh2, TBM_SETLINESIZE  , 0, SLIDE_THRESHOLD_LINE_INC);
	SendMessage(g_SliderMaxCh2, TBM_SETLINESIZE  , 0, SLIDE_THRESHOLD_LINE_INC);
	SendMessage(g_SliderGain, TBM_SETLINESIZE  , 0, SLIDE_GAIN_LINE_INC);




	if(g_OnTop)
	{
		SetWindowPos(hDlg,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}else
	{		
		SetWindowPos(hDlg,HWND_BOTTOM,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	readFromContext();
	g_IsDirty=false;
	RedrawControls();
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
void DrawVerticalDecadeLogScale(HDC &hdc, int xLineStart, int yLineStart,int yLineEnd,int DecadeCount)
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
void DrawVerticalLogScale(HDC &hdc, int yLineStart,int yLineEnd, int xLineStart)
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
void DrawHoroztallLogScale(HDC &hdc, int xLineStart, int yLineStart,int xLineEnd)
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
void DrawHoroztallDecadeLogScale(HDC &hdc, int xLineStart, int yLineStart,int xLineEnd,int DecadeCount)
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
void DrawDecadeText(HDC &hdc,RECT DrawArea,int size,std::string TextString)
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
	
	std::wstring ff=strToStrWide(TextString).c_str();
	LPCTSTR lwstirng=ff.c_str();
	
	DrawText(hdc,lwstirng,TextString.size(),&DrawArea,DT_CENTER);
	DrawArea.top+=pixAtound;
	DrawArea.left-=pixAtound;
	DrawText(hdc,lwstirng,TextString.size(),&DrawArea,DT_CENTER);
	DrawArea.bottom+=pixAtound;
	DrawArea.left+=pixAtound;
	DrawText(hdc,lwstirng,TextString.size(),&DrawArea,DT_CENTER);
	DrawArea.bottom-=pixAtound;
	DrawArea.right+=pixAtound;
	DrawText(hdc,lwstirng,TextString.size(),&DrawArea,DT_CENTER);	
	DrawArea.right-=pixAtound;

	//Now draw the font black Centered.
	SetTextColor(hdc, RGB(0,0,0));
	SelectObject(hdc, hCustomSmallFont);
	DrawText(hdc,lwstirng,TextString.size(),&DrawArea,DT_CENTER);

	SetBkMode(hdc, oldCkgroundMode  );

	// Restore the original font.        
	SelectObject(hdc, hOldFont); 
	// Restore the original Color.        
	SetTextColor(hdc, OldCOlor);



}

/**
* FUNCTION OnPaint
*
* @brief On paint Message Handler
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:56:00 PM
*
* @param hDlg 
*/
void OnPaint(HWND hDlg)
{
	PAINTSTRUCT ps; 
	HDC hdc; 
	hdc = BeginPaint(hDlg, &ps); 	



	DrawVerticalDecadeLogScale(hdc, 111,70 ,281,4);
	DrawVerticalDecadeLogScale(hdc, 290,70 ,280,4);
	DrawHoroztallDecadeLogScale(hdc,190,415,358,3);
	EndPaint(hDlg, &ps);
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
int getSliderPos(HWND &CurWindow)
{
	return SendMessageW(CurWindow,TBM_GETPOS,0,0);
}

/**
* FUNCTION OnVScroll
*
* @brief Vertical Scroll Message Handler
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:56:17 PM
*
* @param wParam 
* @param lParam 
*/
void OnVScroll(WPARAM wParam, LPARAM lParam)
{
	int nScrollCode = (int) LOWORD(wParam); // scroll bar value
	int nPos = (short int) HIWORD(wParam); // scroll box position

	UINT CrtlUsed=0;

	//Determine what control is being changed.
	if(g_SliderMinCh1==(HWND) lParam)
	{
		CrtlUsed=IDC_SLIDER_MIN_CH1;
	}else if(g_SliderMaxCh1==(HWND) lParam)
	{
		CrtlUsed=IDC_SLIDER_MAX_CH1;
	}else if(g_SliderMinCh2==(HWND) lParam)
	{
		CrtlUsed=IDC_SLIDER_MIN_CH2;
	}else if(g_SliderMaxCh2==(HWND) lParam)
	{
		CrtlUsed=IDC_SLIDER_MAX_CH2;
	}
	else if(g_SliderGain==(HWND) lParam)
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
				HWND SlideWindowHandle=(HWND) lParam;

				//Reverse the controller and get the opposed position.
				int CtrlPos= SLIDE_THRESHOLD_TOP_SCALE - getSliderPos(SlideWindowHandle)+SLIDE_THRESHOLD_BOTTOg_SCALE;
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
						g_IsDirty=true;
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
}


/**
* FUNCTION getGainSliderPos
*
* @brief Returns a number for one of 4 positions 0-1024
*
* @author DAVID.LIBBY
* @date 6/3/2015 9:25:31 AM
*
*
* @return int 
*/
int getGainSliderPos()
{
	int newPos = SendMessageW(g_SliderGain,TBM_GETPOS,0,0);
	int Gain=g_Gain;
	switch (newPos)
	{
	case 0:
		Gain = GAIN_0;
		break;
	case 1:
		Gain = GAIN_1;
		break;
	case 2:
		Gain = GAIN_2;
		break;
	case 3:
		Gain = GAIN_3;
		break;
	}

	return Gain;

}

/**
* FUNCTION OnHScroll
*
* @brief Horozontal Scroll Message Handler
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:56:33 PM
*
* @param wParam 
* @param lParam 
*/
void OnHScroll(WPARAM wParam, LPARAM lParam)
{
	int nScrollCode = (int) LOWORD(wParam); // scroll bar value
	int nPos = (short int) HIWORD(wParam); // scroll box position

	UINT CrtlUsed=0;

	if(g_SliderGain==(HWND) lParam)
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
						int newGain=getGainSliderPos();
						if(g_Gain!=newGain)
						{
							g_IsDirty=true;							
							g_Gain=newGain;
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
}

/**
* FUNCTION OnCancel
*
* @brief Button Handler
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:56:52 PM
*
*
* @return INT_PTR 
*/
INT_PTR CALLBACK OnCancel()
{
	if(g_IsDirty)
	{
		int response = MessageBox(g_hDialog,L"Are you sure you want to exit without saving changes?",L"Unsaved changes",MB_YESNO|MB_ICONWARNING);
		if(response!=IDYES)
			return 0;

	}

	if (g_bModal)
		EndDialog(g_hDialog,IDCANCEL);
	else
		::DestroyWindow(g_hDialog);

	g_Ret=HSFC_RET_CANCEL;

	return HSFC_RET_CANCEL;
}

INT_PTR CALLBACK OnOk()
{
	saveToContext();

	g_Ret = HSFC_RET_OK;

	if (g_bModal)
		EndDialog(g_hDialog,IDCANCEL);
	else
		::DestroyWindow(g_hDialog);

	return g_Ret;
}
/**
* FUNCTION RetriveDoubleFromWinHandel
*
* @brief Double from Window Handle
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:57:12 PM
*
* @param TempWindow 
*
* @return double 
*/
double RetriveDoubleFromWinHandel(HWND &TempWindow)
{
	std::string WindosString=RetriveStdStringFromWinHandel(TempWindow);
	if(WindosString.empty())
		return -1.0;
	return atof(WindosString.c_str());
}
/**
* FUNCTION RetriveStdStringFromWinHandel
*
* @brief Get String from Window Handle
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:57:44 PM
*
* @param TempWindow 
*
* @return std::string 
*/
std::string RetriveStdStringFromWinHandel(HWND &TempWindow)
{
	int TestLength = GetWindowTextLengthW(TempWindow)+1;
	wchar_t* buffer = new wchar_t[TestLength+1];

	GetWindowTextW(TempWindow,buffer,TestLength);

	std::wstring ws(buffer);

	std::string SimpString(ws.begin(),ws.end());

	delete[] buffer;
	return SimpString;
}

/**
* FUNCTION KillFocus
*
* @brief Focus Lost Message Handler
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:57:58 PM
*
* @param wParam 
* @param hCommandWindowHandel 
*
* @return INT_PTR 
*/
INT_PTR CALLBACK KillFocus(WPARAM wParam, HWND hCommandWindowHandel)
{
	if (HIWORD(wParam) != EN_KILLFOCUS)
		return false;
	// TODO: Add your control notification handler code here
	if(!hCommandWindowHandel)
		return 0;

	double newValue=RetriveDoubleFromWinHandel(hCommandWindowHandel);

	switch (LOWORD(wParam))
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
	return true;
}

/**
* FUNCTION OnCheckCh1
*
* @brief Check box clicked Message Handler
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:58:33 PM
*
* @param wParam 
*
* @return INT_PTR 
*/
INT_PTR CALLBACK OnCheckCh1(WPARAM wParam)
{
	if (HIWORD(wParam) != BN_CLICKED)
		return false;
	// TODO: Add your control notification handler code here
	g_CheckEnableCh1=!g_CheckEnableCh1;
	g_IsDirty=true;
	RedrawControls();
	return true;
}

/**
* FUNCTION OnCheckCh2
*
* @brief Check bos Clicked Message Handler
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:58:47 PM
*
* @param wParam 
*
* @return INT_PTR 
*/
INT_PTR CALLBACK OnCheckCh2(WPARAM wParam)
{
	if (HIWORD(wParam) != BN_CLICKED)
		return false;
	// TODO: Add your control notification handler code here
	g_CheckEnableCh2=!g_CheckEnableCh2;
	g_IsDirty=true;
	RedrawControls();
	return true;
}

/**
* FUNCTION OnCheckSmallPart
*
* @brief Checkbox Clicked Message Handler
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:58:59 PM
*
* @param wParam 
*
* @return INT_PTR 
*/
INT_PTR CALLBACK OnCheckSmallPart(WPARAM wParam)
{
	if (HIWORD(wParam) != BN_CLICKED)
		return false;

	g_CheckSmallParticle=!g_CheckSmallParticle;
	g_IsDirty=true;
	RedrawControls();
	return true;
}

/**
* FUNCTION OnButtonClickAppily
*
* @brief 
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:59:25 PM
*
* @param wParam 
*
* @return INT_PTR 
*/
INT_PTR CALLBACK OnButtonClickAppily(WPARAM wParam)
{
	if (HIWORD(wParam) != BN_CLICKED)
		return false;
	g_Ret=HSFC_RET_APPLY;
	saveToContext();
	g_IsDirty=false;
	RedrawControls();
	return true;
}

/**
* FUNCTION OnButtonClickRestore
*
* @brief 
*
* @author DAVID.LIBBY
* @date 6/5/2015 1:59:28 PM
*
* @param wParam 
*
* @return INT_PTR 
*/
INT_PTR CALLBACK OnButtonClickRestore(WPARAM wParam)
{
	if (HIWORD(wParam) != BN_CLICKED)
		return false;
	// TODO: Add your control notification handler code here
	readFromContext();
	g_IsDirty=false;
	RedrawControls();
	return true;
}

