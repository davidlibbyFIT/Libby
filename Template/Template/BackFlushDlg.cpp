#include "StdAfx.h"
#include "BackFlushDlg.h"




/**
* CLASS BackFlushDlg
*
* @brief BackFlushDlg Back Flush to clean
*
* @version 1.0 
*
* @author David Libby
*
*/
BackFlushDlg::BackFlushDlg(void):
	m_bModal (false)
	,m_OnTop(false)
	,m_Amount_mL(1.0)
	,m_RepCount(1)
	,m_DoRinse(true)
{
}

BackFlushDlg::~BackFlushDlg(void)
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
INT_PTR BackFlushDlg::DoModal()
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
BOOL BackFlushDlg::IsVisible()
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
void BackFlushDlg::DoModeless(HWND hWndParent)
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
void BackFlushDlg::SetOnTop(BOOL NewTop)
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
void BackFlushDlg::Close()
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
LRESULT BackFlushDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{


	//CWindow m_EditBoxAmountMl;
	//CWindow m_EditBoxCount;
	//CWindow m_CheckBoxRinse;
	//CWindow m_ButtonStart;
	//CWindow m_ButtonStop;

	//m_Amount_mL;
	//m_RepCount;
	//m_DoRinse



	std::string Start_mL="1.0";
	m_EditBoxAmountMl=GetDlgItem(IDC_EDIT_AMOUNT_ML);
	m_EditBoxAmountMl.SetWindowTextW(CA2W(Start_mL.c_str()).m_szBuffer);

	std::string Start_Count="1";
	m_EditBoxCount=GetDlgItem(IDC_EDIT_COUNT);
	m_EditBoxCount.SetWindowTextW(CA2W(Start_Count.c_str()).m_szBuffer);

	m_CheckBoxRinse=GetDlgItem(IDC_CHECK_DO_RINSE);	
	SendMessage(m_CheckBoxRinse, BM_SETCHECK,m_DoRinse,0);

	m_ButtonStart=GetDlgItem(IDC_BUTTON_START);	
	m_ButtonStart.EnableWindow(true);

	m_ButtonStop=GetDlgItem(IDC_BUTTON_STOP);	
	m_ButtonStop.EnableWindow(false);
	

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
LRESULT BackFlushDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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
LRESULT BackFlushDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);


	return 0;
}





LRESULT BackFlushDlg::OnBnClickedButtonStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	std::string Amount_mL = RetriveStdStringFromCWindow(m_EditBoxAmountMl);
	std::string Count =RetriveStdStringFromCWindow(m_EditBoxCount);
	m_DoRinse = SendMessage(m_CheckBoxRinse,BM_GETCHECK,0,0);	

	m_ButtonStart.EnableWindow(false);
	m_ButtonStop.EnableWindow(true);


	return 0;
}


LRESULT BackFlushDlg::OnBnClickedButtonStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	m_ButtonStart.EnableWindow(true);
	m_ButtonStop.EnableWindow(false);


	return 0;
}


LRESULT BackFlushDlg::OnBnClickedCheckDoRinse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	return 0;
}

std::string BackFlushDlg::RetriveStdStringFromCWindow(CWindow &TempWindow)
{
	int TestLength = TempWindow.GetWindowTextLengthW()+1;
	wchar_t* buffer = new wchar_t[TestLength+1];

	TempWindow.GetWindowTextW(buffer,TestLength);
	std::string SimpString=CW2A(buffer);

	delete[] buffer;
	return SimpString;
}

