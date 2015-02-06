#include "StdAfx.h"
#include "PmtDlgCtrl.h"

//String Crap
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
//String Crap



/**
* CLASS PmtDlgCtrl
*
* @brief PmtDlgCtrl Back Flush to clean
*
* @version 1.0 
*
* @author David Libby
*
*/
PmtDlgCtrl::PmtDlgCtrl(void):
m_bModal (false)
	,m_OnTop(false)	
	,m_Dirty(false)
	,m_Ch1IntensityMin(0)
	,m_Ch1IntensityMax(0)
	,m_Ch1TimeMin(0.0)
	,m_Ch1TimeMax(0.0)
	,m_Ch2IntensityMin(0)
	,m_Ch2IntensityMax(0)
	,m_Ch2TimeMin(0.0)
	,m_Ch2TimeMax(0.0)
{
}

PmtDlgCtrl::~PmtDlgCtrl(void)
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
INT_PTR PmtDlgCtrl::DoModal()
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
BOOL PmtDlgCtrl::IsVisible()
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
void PmtDlgCtrl::DoModeless(HWND hWndParent)
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
void PmtDlgCtrl::SetOnTop(BOOL NewTop)
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
void PmtDlgCtrl::Close()
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
LRESULT PmtDlgCtrl::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	m_WinEditCh1IntMin=GetDlgItem(IDC_EDIT_CH1_INT_MIN);	
	m_WinEditCh1IntMax=GetDlgItem(IDC_EDIT_CH1_INT_MAX);
	m_WinEditCh1TimeMin=GetDlgItem(IDC_EDIT_CH1_TIME_MIN);	
	m_WinEditCh1TimeMax=GetDlgItem(IDC_EDIT_CH1_TIME_MAX);	
	m_WinEditCh1CheckTrigger=GetDlgItem(IDC_CHECK_CH1_TRIGGER);	

	m_WinEditCh2IntMin=GetDlgItem(IDC_EDIT_CH2_INT_MIN);	
	m_WinEditCh2IntMax=GetDlgItem(IDC_EDIT_CH2_INT_MAX);	
	m_WinEditCh2TimeMin=GetDlgItem(IDC_EDIT_CH2_TIME_MIN);	
	m_WinEditCh2TimeMax=GetDlgItem(IDC_EDIT_CH2_TIME_MAX);	
	m_WinEditCh2CheckTrigger=GetDlgItem(IDC_CHECK_CH2_TRIGGER);	

	RedrawValues();

	if(m_OnTop)
	{
		SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}else
	{		
		SetWindowPos(HWND_BOTTOM,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

	//We need to reset the dirty flag after the dialog becomes visible..
	m_Dirty=false;

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
LRESULT PmtDlgCtrl::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

	if(m_Dirty)
	{
		int DisplatSaveWarning=1;
	}

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
LRESULT PmtDlgCtrl::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);
	return 0;
}







LRESULT PmtDlgCtrl::OnBnClickedAppily(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	m_Dirty=false;

	return 0;
}


std::string IntToString(int Value)
{
	const int i = 3;
	std::ostringstream s;
	s << Value;
	return s.str();
}


std::string DoubleToString(double Value,int decPlaces=2)
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

void PmtDlgCtrl::RedrawValues()
{
	m_WinEditCh1IntMin.SetWindowTextW(CA2W(IntToString(m_Ch1IntensityMin).c_str()).m_szBuffer);
	m_WinEditCh1IntMax.SetWindowTextW(CA2W(IntToString(m_Ch1IntensityMax).c_str()).m_szBuffer);

	m_WinEditCh1TimeMin.SetWindowTextW(CA2W(DoubleToString(m_Ch1TimeMin).c_str()).m_szBuffer);
	m_WinEditCh1TimeMax.SetWindowTextW(CA2W(DoubleToString(m_Ch1TimeMax).c_str()).m_szBuffer);

	if(IsChanel1Enabled())
	{
		m_WinEditCh1IntMin.EnableWindow(true);
		m_WinEditCh1IntMax.EnableWindow(true);
		m_WinEditCh1TimeMin.EnableWindow(true);
		m_WinEditCh1TimeMax.EnableWindow(true);
	
	}else
	{
		m_WinEditCh1IntMin.EnableWindow(false);
		m_WinEditCh1IntMax.EnableWindow(false);
		m_WinEditCh1TimeMin.EnableWindow(false);
		m_WinEditCh1TimeMax.EnableWindow(false);
	}



	m_WinEditCh2IntMin.SetWindowTextW(CA2W(IntToString(m_Ch2IntensityMin).c_str()).m_szBuffer);
	m_WinEditCh2IntMax.SetWindowTextW(CA2W(IntToString(m_Ch2IntensityMax).c_str()).m_szBuffer);

	m_WinEditCh2TimeMin.SetWindowTextW(CA2W(DoubleToString(m_Ch2TimeMin).c_str()).m_szBuffer);
	m_WinEditCh2TimeMax.SetWindowTextW(CA2W(DoubleToString(m_Ch2TimeMax).c_str()).m_szBuffer);

	if(IsChanel2Enabled())
	{
		m_WinEditCh2IntMin.EnableWindow(true);
		m_WinEditCh2IntMax.EnableWindow(true);
		m_WinEditCh2TimeMin.EnableWindow(true);
		m_WinEditCh2TimeMax.EnableWindow(true);

	}else
	{
		m_WinEditCh2IntMin.EnableWindow(false);
		m_WinEditCh2IntMax.EnableWindow(false);
		m_WinEditCh2TimeMin.EnableWindow(false);
		m_WinEditCh2TimeMax.EnableWindow(false);
	}


}


void PmtDlgCtrl::setCh1IntensityMin(int NewValue)
{

	m_Ch1IntensityMin = NewValue;
	RedrawValues();

}
void PmtDlgCtrl::setCh1IntensityMax(int NewValue)
{
	int m_Ch1IntensityMax = NewValue;
	RedrawValues();

}
void PmtDlgCtrl::setCh1TimeMin(double NewValue)
{
	double m_Ch1TimeMin = NewValue;
	RedrawValues();

}
void PmtDlgCtrl::setCh1TimeMax(double NewValue)
{
	double m_Ch1TimeMax = NewValue;
	RedrawValues();

}

void PmtDlgCtrl::setCh2IntensityMin(int NewValue)
{
	int m_Ch2IntensityMin = NewValue;
	RedrawValues();


}
void PmtDlgCtrl::setCh2IntensityMax(int NewValue)
{
	int m_Ch2IntensityMax = NewValue;
	RedrawValues();

}
void PmtDlgCtrl::setCh2TimeMin(double NewValue)
{
	double m_Ch2TimeMin = NewValue;
	RedrawValues();

}
void PmtDlgCtrl::setCh2TimeMax(double NewValue)
{
	double m_Ch2TimeMax = NewValue;
	RedrawValues();
}


LRESULT PmtDlgCtrl::OnBnClickedCheckCh1Trigger(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{	
	RedrawValues();
	m_Dirty=true;
	return 0;
}


LRESULT PmtDlgCtrl::OnBnClickedCheckCh2Trigger(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{	
	RedrawValues();
	m_Dirty=true;
	return 0;
}

bool PmtDlgCtrl::IsCheckBoxEnabled(HWND hWnd)
{
	if(SendMessage(hWnd,BM_GETCHECK,0,0)==1)
	{
		return true;	
	}
	return false;
}

void PmtDlgCtrl::setCheckBox(HWND hWnd,bool NewValue)
{
	SendMessage(hWnd, BM_SETCHECK,NewValue,0);
	RedrawValues();
}

bool PmtDlgCtrl::IsChanel1Enabled()
{
	return IsCheckBoxEnabled(m_WinEditCh1CheckTrigger);
}

bool PmtDlgCtrl::IsChanel2Enabled()
{
	return IsCheckBoxEnabled(m_WinEditCh2CheckTrigger);
}

void PmtDlgCtrl::setChanel1CheckBox(bool NewValue)
{
	setCheckBox(m_WinEditCh1CheckTrigger,NewValue);
}
void PmtDlgCtrl::setChanel2CheckBox(bool NewValue)
{
	setCheckBox(m_WinEditCh2CheckTrigger,NewValue);
}




LRESULT PmtDlgCtrl::OnEnChangeEditCh1IntMin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_Dirty=true;
	return 0;
}
