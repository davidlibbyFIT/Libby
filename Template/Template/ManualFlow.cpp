#include "StdAfx.h"
#include "ManualFlow.h"

//String Crap
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
//String Crap


/**
* CLASS AFManualFlow
*
* @brief AFManualFlow Back Flush to clean
*
* @version 1.0 
*
* @author David Libby
*
*/
AFManualFlow::AFManualFlow(double CurrentFlowRate):
	m_bModal (false)
	,m_OnTop(false)	
	,m_FlowRate(CurrentFlowRate)
{
}

AFManualFlow::~AFManualFlow(void)
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
INT_PTR AFManualFlow::DoModal()
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
BOOL AFManualFlow::IsVisible()
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
void AFManualFlow::DoModeless(HWND hWndParent)
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
void AFManualFlow::SetOnTop(BOOL NewTop)
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
void AFManualFlow::Close()
{
	if (m_bModal)
		EndDialog(0);
	else
		DestroyWindow();
}
std::string AFManualFlow::DoubleToString(double Value,int decPlaces/*=2*/)
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
LRESULT AFManualFlow::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	//std::string Start_mL="1.0";
	//m_EditBoxExample=GetDlgItem(IDC_EDIT_AMOUNT_ML);
	//m_EditBoxExample.SetWindowTextW(CA2W(Start_mL.c_str()).m_szBuffer);
	m_EditFlowRate = GetDlgItem(IDC_EDIT_FLOW_RATE);

	m_EditFlowRate.SetWindowTextW(CA2W(DoubleToString(m_FlowRate).c_str()).m_szBuffer);

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
LRESULT AFManualFlow::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);

	return 1;
}
std::string AFManualFlow::RetriveStdStringFromCWindow(CWindow &TempWindow)
{
	int TestLength = TempWindow.GetWindowTextLengthW()+1;
	wchar_t* buffer = new wchar_t[TestLength+1];

	TempWindow.GetWindowTextW(buffer,TestLength);
	std::string SimpString=CW2A(buffer);

	delete[] buffer;
	return SimpString;
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
LRESULT AFManualFlow::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	
	std::string RetWindowString=RetriveStdStringFromCWindow(m_EditFlowRate);
	m_FlowRate = (double)atof(RetWindowString.c_str());
	
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);
	return 0;
}






