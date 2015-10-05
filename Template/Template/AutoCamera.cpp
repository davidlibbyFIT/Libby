#include "StdAfx.h"
#include "AutoCamera.h"

//String Crap
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>


/**
* CLASS AutoCamera
*
* @brief AutoCamera 
*
* @version 1.0 
*
* @author David Libby
*
*/
AutoCamera::AutoCamera():
	m_bModal (false),
	m_OnTop(false),
	m_ButtonSetLevel(NULL),
	m_ComboFPS(NULL),
	m_EditAverageBackground(NULL),
	m_EditExposureUs(NULL),
	m_EditFlashAmplitude(NULL),
	m_LableExposureUs(NULL),
	m_LableFPS(NULL),
	m_LableFlashAmplitude(NULL),
	m_SliderSetting(NULL),
	m_flashASize(0),
	m_exposure_uS_Size(0),
	m_overallSlideCtrlSize(0)


{
	//THis Map is the lookup table for the Different speeds available to the user..
	m_CameraSpeedMap_uS[0]=18;
	m_CameraSpeedMap_uS[1]=23;
	m_CameraSpeedMap_uS[2]=28;
	m_CameraSpeedMap_uS[3]=33;
	m_CameraSpeedMap_uS[4]=38;

}


AutoCamera::~AutoCamera(void)
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
LRESULT AutoCamera::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	m_ButtonSetLevel = GetDlgItem(IDC_BUTTON_SET_LEVEL);
	m_ComboFPS = GetDlgItem(IDC_COMBO_FPS);
	m_EditAverageBackground = GetDlgItem(IDC_EDIT_AVERAGE_BACKGROUND);
	m_EditExposureUs = GetDlgItem(IDC_EDIT_EXPOSURE_US);
	m_EditFlashAmplitude = GetDlgItem(IDC_EDIT_FLASH_AMPLITUDE);
	m_LableExposureUs = GetDlgItem(IDC_STATIC_LABLE_EXPOSURE_US);
	m_LableFPS = GetDlgItem(IDC_STATIC_LABEL_FPS);
	m_LableFlashAmplitude = GetDlgItem(IDC_STATIC_LABLE_FLASH_AMPLITUDE);
	m_SliderSetting = GetDlgItem(IDC_SLIDER_SETTING);

	//Add Camera Frames Per Second.
	
	for (int ct=m_CtxFPS.min;ct<=m_CtxFPS.max;ct++)
	{
		char buffer [33];
		_itoa_s (ct,buffer,10);
		SendMessage(m_ComboFPS, CB_ADDSTRING,0, (LPARAM) CA2W(buffer).m_szBuffer);	
	}

	//Populate the Edit Windows.
	SetEditWindowValue(m_EditAverageBackground,0);
	setSelectionByExposure_us(m_CurrentFlashA.current);
	SetEditWindowValue(m_EditFlashAmplitude,m_CurrentFlashA.current);
	SetEditWindowValue(m_EditExposureUs,m_CameraSpeedMap_uS[0]);


	m_flashASize=m_CtxFlashA.max-m_CtxFlashA.min;	
	m_exposure_uS_Size=m_CameraSpeedMap_uS.size();
	m_overallSlideCtrlSize=m_flashASize*m_exposure_uS_Size;

	SendMessage(m_SliderSetting, TBM_SETRANGE, 0, (LPARAM)MAKELONG(0, m_overallSlideCtrlSize));

	setSliderPos();

	if(m_OnTop)
	{
		SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}else
	{		
		SetWindowPos(HWND_BOTTOM,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	
	//SetTimer(NULL,100);
	return 1;
}


/**
* FUNCTION OnTimer
*
* @brief Timer for test purpose.
*
* @version 1.0 
*
* @author David Libby
* @date 9/29/2015 2:54:19 PM
*
* @param uMsg 
* @param wParam 
* @param lParam 
* @param bHandled 
*
* @return LRESULT 
*/
LRESULT AutoCamera::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//SetEditWindowValue(m_EditAverageBackground,getSliderPos());
	return 1;
}

/**
* FUNCTION setSliderPos
*
* @brief Sets the slider position for a given 
*
* @version 1.0 
*
* @author David Libby
* @date 9/29/2015 2:54:41 PM
*
*/
void AutoCamera::setSliderPos()
{
	int curFa= GetEditWindowValue(m_EditFlashAmplitude) - m_CurrentFlashA.min ; //IE 0-80
	int cur_us= GetEditWindowValue(m_EditExposureUs)  ;	
	
	int mapKeyPos=findCameraSpeedKey(cur_us);

	//If we did not find the position take the first one...
	if(mapKeyPos<=0)
		mapKeyPos=1;
	
	int curpos = curFa * mapKeyPos;
	
	if(curpos<0)
		curpos=0;

	if(curpos>m_overallSlideCtrlSize)
		curpos=m_overallSlideCtrlSize;

	SendMessageW(m_SliderSetting,TBM_SETPOS,true, curpos);

}

/**
* FUNCTION getSliderPos
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 9/30/2015 10:09:06 AM
*
*
* @return int 
*/
int AutoCamera::getSliderPos()
{
	return SendMessageW(m_SliderSetting,TBM_GETPOS,0,0);
}


/**
* FUNCTION setVars
*
* @brief Sets the 
*
* @author DAVID.LIBBY
* @date 9/29/2015 10:45:50 AM
*
* @param FPS 
* @param FlashA 
* @param Exposure 
*/
void AutoCamera::setVars(AutoCameraData &FPS,AutoCameraData &FlashA,AutoCameraData & Exposure)
{
	m_CurrentFPS = m_CtxFPS = FPS;
	m_CurrentFlashA = m_CtxFlashA = FlashA;
	m_CurrentExposure = m_CtxExposure = Exposure;
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
INT_PTR AutoCamera::DoModal()
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
BOOL AutoCamera::IsVisible()
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
void AutoCamera::DoModeless(HWND hWndParent)
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
void AutoCamera::SetOnTop(BOOL NewTop)
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
void AutoCamera::Close()
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
LRESULT AutoCamera::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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
LRESULT AutoCamera::OnBnClickedOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);
	return 0;
}

/**
* FUNCTION setSelectionByExposure_us
*
* @brief given a Exposure time in uS return the index in the map.
*
* @version 1.0 
*
* @author David Libby
* @date 9/30/2015 10:09:18 AM
*
* @param findInt 
*/
void AutoCamera::setSelectionByExposure_us(int findInt)
{
	char buffer [33];
	_itoa_s (findInt,buffer,10);

	int loc = SendMessage(m_ComboFPS, CB_FINDSTRING, -1, (LPARAM) CA2W(buffer).m_szBuffer);	 
	if(loc!=0)
	{
		SendMessage(m_ComboFPS, CB_SETCURSEL, (WPARAM) loc, 0);
	}
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
std::string AutoCamera::intToString(int Value)
{
	const int i = 3;
	std::ostringstream s;
	s << Value;
	return s.str();
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
void AutoCamera::SetEditWindowValue(CWindow &CurWindow,int NewValue)
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
* @return int 
*/
int AutoCamera::GetEditWindowValue(CWindow &CurWindow)
{
	std::string RetWindowString=RetriveStdStringFromCWindow(CurWindow);
	if(RetWindowString.empty())
		return 0;
	return (int)atoi(RetWindowString.c_str());
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
std::string AutoCamera::RetriveStdStringFromCWindow(CWindow &TempWindow)
{
	int TestLength = TempWindow.GetWindowTextLengthW()+1;
	wchar_t* buffer = new wchar_t[TestLength+1];

	TempWindow.GetWindowTextW(buffer,TestLength);

	std::string SimpString=CW2A(buffer);

	delete[] buffer;
	return SimpString;
}
/**
* FUNCTION OnHScroll
*
* @brief Windows Handle for scroll
*
* @version 1.0 
*
* @author David Libby
* @date 9/30/2015 10:10:38 AM
*
* @param uMsg 
* @param wParam 
* @param lParam 
* @param bHandled 
*
* @return LRESULT 
*/
LRESULT AutoCamera::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int nScrollCode = (int) LOWORD(wParam); // scroll bar value
	int nPos = (short int) HIWORD(wParam); // scroll box position

	UINT CrtlUsed=0;

	//Determine what control is being changed.
	if(m_SliderSetting.m_hWnd==(HWND) lParam)
	{
		CrtlUsed=IDC_SLIDER_SETTING;
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
				//Get the windows handle.
				CWindow SlideWindowHandle=(HWND) lParam;

				switch (CrtlUsed)
				{
				case IDC_SLIDER_SETTING:
					{
						
						UpdateFlashAndExposure();

						break;
					}
				}					


			}
			break;
		}

	}


	return 0;
}

void AutoCamera::UpdateFlashAndExposure()
{
	int flashPos;
	int exposurePos;

	CalculateExposureAndFlash(exposurePos, flashPos);



	SetEditWindowValue(m_EditExposureUs,m_CameraSpeedMap_uS[exposurePos]);
	SetEditWindowValue(m_EditFlashAmplitude,flashPos);
}

/**
* FUNCTION CalculateExposureAndFlash
*
* @brief Calculates the Exposure and flash based on Slider Position.
*
* @version 1.0 
*
* @author David Libby
* @date 9/29/2015 1:35:50 PM
*
* @param exposurePos 
* @param flashPos 
*/
void AutoCamera::CalculateExposureAndFlash(int &exposurePos, int &flashPos)
{
	int scrollPos=getSliderPos();

	int whole=scrollPos/m_flashASize;
	int remainder=scrollPos%m_flashASize;

	exposurePos= whole + 0;

	//if(remainder>0)
	//	 exposurePos =  remainder + 1;


	if(whole==0)
		exposurePos=0;

	if(exposurePos>(int)m_CameraSpeedMap_uS.size()-1)
		exposurePos=m_CameraSpeedMap_uS.size()-1;

	flashPos= remainder+m_CtxFlashA.min;
}



LRESULT AutoCamera::OnBnClickedButtonSetLevel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int averageBackground = GetEditWindowValue(m_EditAverageBackground);
	
	for (int curpos=0;curpos<=m_overallSlideCtrlSize;curpos++)
	{
		SendMessageW(m_SliderSetting,TBM_SETPOS,true, curpos);
		UpdateFlashAndExposure();
		Sleep(1);
		
		if(averageBackground==curpos)
			break;


	}
	return 0;
}
/**
* FUNCTION findCameraSpeedKey
*
* @brief Finds the map key for a given camera speed. 
*
* @version 1.0 
*
* @author David Libby
* @date 9/29/2015 2:50:50 PM
*
* @param fintValue in microsecond.
*
* @return int 
*/
int AutoCamera::findCameraSpeedKey(int fintValue_uS)
{
	int key=-1;
	std::map<int, int>::iterator it = m_CameraSpeedMap_uS.begin(); // internalMap is std::map
	while(it != m_CameraSpeedMap_uS.end())
	{
		bool found = (it->second == fintValue_uS);
		if(found)
		{
			key=it->first;
			break;
		}
		++it;
	}

	return key;

}

