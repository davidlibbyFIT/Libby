#include "StdAfx.h"
#include "PmtDlgCtrl.h"

//String Crap
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
//String Crap


const std::string PMT_NOT_USED_STRING = "None";



//Add to Windows Utils FIle

/**
* FUNCTION AddComboItem
*
* @brief Add new String to Combo Box
*
* @author DAVID.LIBBY
* @date 2/9/2015 2:31:21 PM
*
* @param ComboWindow 
* @param NewItem 
*
* @return int 
*/
int PmtDlgCtrl::AddComboItem(CWindow &ComboWindow,const std::string &NewItem)
{
	return SendMessage(ComboWindow,CB_ADDSTRING,0,(LPARAM) CA2W(NewItem.c_str()).m_szBuffer);
}
int PmtDlgCtrl::DeleteComboItem(CWindow &ComboWindow,int IndexItem)
{
	if(GetComboCount(ComboWindow)<=IndexItem)
		return CB_ERR;

	return SendMessage(ComboWindow,CB_DELETESTRING,IndexItem,0);
}

/**
* FUNCTION GetComboCount
*
* @brief Get Zero Based COunt of Items in ComboBox
*
* @author DAVID.LIBBY
* @date 2/9/2015 2:31:38 PM
*
* @param ComboWindow 
*
* @return int 
*/
int PmtDlgCtrl::GetComboCount(CWindow &ComboWindow)
{
	return (int)SendMessage(ComboWindow,	 CB_GETCOUNT,	 0,	 0);
}
/**
* FUNCTION SetComboSelection
*
* @brief Set Selected Item from 0 based index
*
* @author DAVID.LIBBY
* @date 2/9/2015 2:32:02 PM
*
* @param ComboWindow 
* @param IndexItem 
*
* @return int 
*/
int PmtDlgCtrl::SetComboSelection(CWindow &ComboWindow,int IndexItem)
{
	if(GetComboCount(ComboWindow)<=IndexItem)
		return CB_ERR;
	return SendMessage(ComboWindow,	 CB_SETCURSEL,	 (WPARAM) IndexItem,	 0);
}
/**
* FUNCTION GetComboSelection
*
* @brief Get the Zero Based Indexed Item of the currently Selected Object.
*
* @author DAVID.LIBBY
* @date 2/9/2015 2:32:49 PM
*
* @param ComboWindow 
*
* @return int 
*/
int PmtDlgCtrl::GetComboSelection(CWindow &ComboWindow)
{
	return SendMessage(ComboWindow,	 CB_GETCURSEL,	 0,	 0);
}

/**
* FUNCTION GetComboString
*
* @brief Get the String from an Index Item.
*
* @author DAVID.LIBBY
* @date 2/9/2015 2:33:20 PM
*
* @param ComboWindow 
* @param IndexItem 
*
* @return std::string 
*/
std::string PmtDlgCtrl::GetComboString(CWindow &ComboWindow,int IndexItem)
{
	if(GetComboCount(ComboWindow)<=IndexItem)
		return "";

	TCHAR charString[255];
	SendMessage(ComboWindow,	 CB_GETLBTEXT,	 IndexItem,	 (LPARAM) charString);
	std::wstring arr_w( charString );
	std::string arr_s( arr_w.begin(), arr_w.end() );

	return arr_s;
}

/**
* FUNCTION FindComboString
*
* @brief Find Index Id for a Given string in a combo Box.
*
* @author DAVID.LIBBY
* @date 2/9/2015 2:33:42 PM
*
* @param ComboWindow 
* @param FindString 
*
* @return int 
*/
int PmtDlgCtrl::FindComboString(CWindow &ComboWindow,const std::string &FindString)
{

	if(FindString.empty())
		return CB_ERR;
	return (int) SendMessage(ComboWindow, CB_FINDSTRING, -1,(LPARAM)  CA2W(FindString.c_str()).m_szBuffer);	
}

/**
* FUNCTION SetComboItemData
*
* @brief Sets the Combox Data Item for a given Index Object.
*
* @author DAVID.LIBBY
* @date 2/9/2015 2:34:07 PM
*
* @param ComboWindow 
* @param IndexItem 
* @param DataItem 
*
* @return int 
*/
int PmtDlgCtrl::SetComboItemData(CWindow &ComboWindow,int IndexItem,int DataItem)
{
	if(GetComboCount(ComboWindow)<=IndexItem)
		return CB_ERR;
	return SendMessage(ComboWindow,	 CB_SETITEMDATA,	 IndexItem,	 DataItem);
}
/**
* FUNCTION GetComboItemData
*
* @brief Get THe Combo Data Item For a given Index Item.
*
* @author DAVID.LIBBY
* @date 2/9/2015 2:34:35 PM
*
* @param ComboWindow 
* @param IndexItem 
*
* @return int 
*/
int PmtDlgCtrl::GetComboItemData(CWindow &ComboWindow,int IndexItem)
{
	if(GetComboCount(ComboWindow)<=IndexItem)
		return CB_ERR;
	return SendMessage(ComboWindow,	 CB_GETITEMDATA,	 IndexItem,0);
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

int PmtDlgCtrl::RetriveIntFromCWindow(CWindow &TempWindow)
{
	std::string WindosString=RetriveStdStringFromCWindow(TempWindow);
	if(WindosString.empty()||WindosString==PMT_NOT_USED_STRING)
		return -1;
	return atoi( WindosString.c_str() );
}
double PmtDlgCtrl::RetriveDoubleFromCWindow(CWindow &TempWindow)
{
	std::string WindosString=RetriveStdStringFromCWindow(TempWindow);
	if(WindosString.empty())
		return -1.0;
	return atof(WindosString.c_str());
}

/**
* FUNCTION RetriveStdStringFromCWindow
*
* @brief Retrives the Test form a Given window.
*
* @author DAVID.LIBBY
* @date 2/12/2015 10:20:31 AM
*
* @param TempWindow 
*
* @return std::string 
*/
std::string PmtDlgCtrl::RetriveStdStringFromCWindow(CWindow &TempWindow)
{
	std::string SimpString="";
	int TestLength = TempWindow.GetWindowTextLengthW()+1;
	wchar_t* buffer = new wchar_t[TestLength+1];
	if(TempWindow.GetWindowTextW(buffer,TestLength)==0)
		return "";	
	SimpString=CW2A(buffer);
	delete[] buffer;
	return SimpString;
}



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
	,m_Ch1IntensityMax(-1)
	,m_Ch1TimeMin(0.0)
	,m_Ch1TimeMax(-1.0)
	,m_Ch2IntensityMin(0)
	,m_Ch2IntensityMax(-1)
	,m_Ch2TimeMin(0.0)
	,m_Ch2TimeMax(-1.0)
	,m_Ch1LaserWavelength("")
	,m_Ch2LaserWavelength("")
{
	m_Ch1ComboItems.clear();
	m_Ch2ComboItems.clear();
	m_LaserComboItems.clear();
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
	m_WinGroupCh1=GetDlgItem(IDC_STATIC_CH1_GROUP);
	m_WinComboGainCh1=GetDlgItem(IDC_COMBO_CH1_GAIN);

	m_WinEditCh2IntMin=GetDlgItem(IDC_EDIT_CH2_INT_MIN);	
	m_WinEditCh2IntMax=GetDlgItem(IDC_EDIT_CH2_INT_MAX);	
	m_WinEditCh2TimeMin=GetDlgItem(IDC_EDIT_CH2_TIME_MIN);	
	m_WinEditCh2TimeMax=GetDlgItem(IDC_EDIT_CH2_TIME_MAX);	
	m_WinEditCh2CheckTrigger=GetDlgItem(IDC_CHECK_CH2_TRIGGER);	
	m_WinGroupCh2=GetDlgItem(IDC_STATIC_CH2_GROUP);	
	m_WinComboGainCh2=GetDlgItem(IDC_COMBO_CH2_GAIN);

	m_WinComboLaserPower=GetDlgItem(IDC_COMBO_LASER_POWER);

	/*
	int aa= AddComboItem(m_WinComboGainCh1,"2irst Item Ch1");
	AddComboItem(m_WinComboGainCh1,"Second Item Ch1");
	int bb= AddComboItem(m_WinComboGainCh2,"First Item Ch2");
	int cc= AddComboItem(m_WinComboLaserPower,"First Laser");

	SetComboSelection(m_WinComboGainCh1,1);
	SetComboSelection(m_WinComboGainCh2,0);
	SetComboSelection(m_WinComboLaserPower,0);

	int aadd=GetComboSelection(m_WinComboGainCh1);
	std::string my=GetComboString(m_WinComboGainCh1,0);

	int ghgh=FindComboString(m_WinComboGainCh1,"Second Item Ch1");

	int rrt=SetComboItemData(m_WinComboGainCh1,0,555);
	int rrtyy=GetComboItemData(m_WinComboGainCh1,0);
	*/

	CWindow m_WinComboLaserPower;
	CWindow m_WinCheckLaserEnable;



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



void PmtDlgCtrl::RedrawValues()
{
	m_WinEditCh1IntMin.SetWindowTextW(CA2W(IntToString(m_Ch1IntensityMin).c_str()).m_szBuffer);
	if(m_Ch1IntensityMax<0)
		m_WinEditCh1IntMax.SetWindowTextW(CA2W(PMT_NOT_USED_STRING.c_str()).m_szBuffer);	
	else
		m_WinEditCh1IntMax.SetWindowTextW(CA2W(IntToString(m_Ch1IntensityMax).c_str()).m_szBuffer);

	m_WinEditCh1TimeMin.SetWindowTextW(CA2W(DoubleToString(m_Ch1TimeMin).c_str()).m_szBuffer);
	if(m_Ch1TimeMax<0)
		m_WinEditCh1TimeMax.SetWindowTextW(CA2W(PMT_NOT_USED_STRING.c_str()).m_szBuffer);
	else
		m_WinEditCh1TimeMax.SetWindowTextW(CA2W(DoubleToString(m_Ch1TimeMax).c_str()).m_szBuffer);


	
	std::string Ch1WaveString="Ch1";
	if(!m_Ch1LaserWavelength.empty())
		Ch1WaveString+=" ( " + m_Ch1LaserWavelength + " )";
	m_WinGroupCh1.SetWindowTextW(CA2W(Ch1WaveString.c_str()).m_szBuffer);

	std::string Ch2WaveString="Ch2";
	if(!m_Ch2LaserWavelength.empty())
		Ch2WaveString+=" ( " + m_Ch2LaserWavelength + " )";
	m_WinGroupCh2.SetWindowTextW(CA2W(Ch2WaveString.c_str()).m_szBuffer);
	

	if(IsChanel1Enabled())
	{
		m_WinEditCh1IntMin.EnableWindow(true);
		m_WinEditCh1IntMax.EnableWindow(true);
		m_WinEditCh1TimeMin.EnableWindow(true);
		m_WinEditCh1TimeMax.EnableWindow(true);
		m_WinComboGainCh1.EnableWindow(true);
	
	}else
	{
		m_WinEditCh1IntMin.EnableWindow(false);
		m_WinEditCh1IntMax.EnableWindow(false);
		m_WinEditCh1TimeMin.EnableWindow(false);
		m_WinEditCh1TimeMax.EnableWindow(false);
		m_WinComboGainCh1.EnableWindow(true);
	}



	m_WinEditCh2IntMin.SetWindowTextW(CA2W(IntToString(m_Ch2IntensityMin).c_str()).m_szBuffer);
	if(m_Ch2IntensityMax<0)
		m_WinEditCh2IntMax.SetWindowTextW(CA2W(PMT_NOT_USED_STRING.c_str()).m_szBuffer);
	else
		m_WinEditCh2IntMax.SetWindowTextW(CA2W(IntToString(m_Ch2IntensityMax).c_str()).m_szBuffer);



	m_WinEditCh2TimeMin.SetWindowTextW(CA2W(DoubleToString(m_Ch2TimeMin).c_str()).m_szBuffer);
	if(m_Ch2TimeMax<0)
		m_WinEditCh2TimeMax.SetWindowTextW(CA2W(PMT_NOT_USED_STRING.c_str()).m_szBuffer);
	else
		m_WinEditCh2TimeMax.SetWindowTextW(CA2W(DoubleToString(m_Ch2TimeMax).c_str()).m_szBuffer);
	

	if(IsChanel2Enabled())
	{
		m_WinEditCh2IntMin.EnableWindow(true);
		m_WinEditCh2IntMax.EnableWindow(true);
		m_WinEditCh2TimeMin.EnableWindow(true);
		m_WinEditCh2TimeMax.EnableWindow(true);
		m_WinComboGainCh2.EnableWindow(true);

	}else
	{
		m_WinEditCh2IntMin.EnableWindow(false);
		m_WinEditCh2IntMax.EnableWindow(false);
		m_WinEditCh2TimeMin.EnableWindow(false);
		m_WinEditCh2TimeMax.EnableWindow(false);
		m_WinComboGainCh2.EnableWindow(true);
	}

	m_WinComboLaserPower.EnableWindow(true);

	if(m_Ch1ComboItems.size()!=GetComboCount(m_WinComboGainCh1))
	{
		AddComboList(m_WinComboGainCh1,m_Ch1ComboItems);
	}
	if(m_Ch2ComboItems.size()!=GetComboCount(m_WinComboGainCh2))
	{
		AddComboList(m_WinComboGainCh2,m_Ch2ComboItems);
	}

	if(m_LaserComboItems.size()!=GetComboCount(m_WinComboLaserPower))
	{
		AddComboList(m_WinComboLaserPower,m_LaserComboItems);
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
	m_Dirty=true;
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




void PmtDlgCtrl::SetCh1WaveLengthString(const std::string &WaveLength)
{
	m_Ch1LaserWavelength=WaveLength;

}
void PmtDlgCtrl::SetCh2WaveLengthString(const std::string &WaveLength)
{
	m_Ch2LaserWavelength=WaveLength;
}


void PmtDlgCtrl::AddComboListCh1(PmtDlgComboVector &ListVector)
{
	m_Ch1ComboItems=ListVector;

}
void PmtDlgCtrl::AddComboListCh2(PmtDlgComboVector &ListVector)
{
	m_Ch2ComboItems=ListVector;
}

void PmtDlgCtrl::AddComboListLaser(PmtDlgComboVector &ListVector)
{
	m_LaserComboItems=ListVector;
}

int PmtDlgCtrl::AddComboList(CWindow &ComboWindow,PmtDlgComboVector &ListVector)
{
	if(ListVector.size()<1)
		return CB_ERR;

	int SelectedItem=-1;

	for(iPmtDlgComboVector ittr=ListVector.begin();ittr!=ListVector.end();ittr++)
	{
		//Combo Add failed
		int currentIndex=AddComboItem(ComboWindow,ittr->UserVisibleName);
		if(currentIndex<0)
			continue;
		//If the data failed then delete the record.
		if(SetComboItemData(ComboWindow,currentIndex,ittr->Data)<0)
		{
			DeleteComboItem(ComboWindow,currentIndex);
			continue;
		}
		if(ittr->SelectedItem)
		{
			SetComboSelection(ComboWindow,currentIndex);
			SelectedItem=currentIndex;
		}
	}

	return GetComboCount(ComboWindow);
}

//int PmtDlgCtrl::RetriveIntFromCWindow(CWindow &TempWindow)
//double PmtDlgCtrl::RetriveDoubleFromCWindow(CWindow &TempWindow)



LRESULT PmtDlgCtrl::OnEnChangeEditCh1IntMin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int CurInt=RetriveIntFromCWindow(m_WinEditCh1IntMin);
	if( m_Ch1IntensityMin!=CurInt)
	{
		m_Ch1IntensityMin=CurInt;
		m_Dirty=true;
	}
	return 0;
}
LRESULT PmtDlgCtrl::OnEnChangeEditCh1IntMax(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int CurInt=RetriveIntFromCWindow(m_WinEditCh1IntMax);
	if( m_Ch1IntensityMax!=CurInt)
	{
		m_Ch1IntensityMax=CurInt;
		m_Dirty=true;
	}
	return 0;
}
LRESULT PmtDlgCtrl::OnEnChangeEditCh2IntMin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int CurInt=RetriveIntFromCWindow(m_WinEditCh2IntMin);
	if( m_Ch2IntensityMin!=CurInt)
	{
		m_Ch2IntensityMin=CurInt;
		m_Dirty=true;
	}

	return 0;
}
LRESULT PmtDlgCtrl::OnEnChangeEditCh2IntMax(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int CurInt=RetriveIntFromCWindow(m_WinEditCh2IntMax);
	if( m_Ch2IntensityMax!=CurInt)
	{
		m_Ch2IntensityMax=CurInt;
		m_Dirty=true;
	}

	return 0;
}

LRESULT PmtDlgCtrl::OnEnChangeEditCh1TimeMin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	double CurDbl=RetriveDoubleFromCWindow(m_WinEditCh1TimeMin);
	if( m_Ch1TimeMin!=CurDbl)
	{
		m_Ch1TimeMin=CurDbl;
		m_Dirty=true;
	}
	return 0;

}

LRESULT PmtDlgCtrl::OnEnChangeEditCh1TimeMax(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	double CurDbl=RetriveDoubleFromCWindow(m_WinEditCh1TimeMax);
	if( m_Ch1TimeMax!=CurDbl)
	{
		m_Ch1TimeMax=CurDbl;
		m_Dirty=true;
	}

	return 0;
}


LRESULT PmtDlgCtrl::OnEnChangeEditCh2TimeMin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	double CurDbl=RetriveDoubleFromCWindow(m_WinEditCh2TimeMin);
	if( m_Ch2TimeMin!=CurDbl)
	{
		m_Ch2TimeMin=CurDbl;
		m_Dirty=true;
	}
	return 0;
}

LRESULT PmtDlgCtrl::OnEnChangeEditCh2TimeMax(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	double CurDbl=RetriveDoubleFromCWindow(m_WinEditCh2TimeMax);
	if( m_Ch2TimeMax!=CurDbl)
	{
		m_Ch2TimeMax=CurDbl;
		m_Dirty=true;
	}

	return 0;
}


LRESULT PmtDlgCtrl::OnEnKillfocusEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	RedrawValues();
	return 0;
}







