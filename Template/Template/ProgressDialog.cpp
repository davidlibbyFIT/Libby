#include "StdAfx.h"
#include "ProgressDialog.h"


/**
* CLASS ProgressDialog
*
* @brief ProgressDialog to give update Progress information.
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:39:11 AM
*
*/
ProgressDialog::ProgressDialog(void):
	m_bModal (FALSE),
	m_LowRange(0),
	m_HiRange(100),
	m_Pos(0),
	m_OnTop(false),
	m_UserCancel(false)
{
}

ProgressDialog::~ProgressDialog(void)
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
INT_PTR ProgressDialog::DoModal()
{   
	m_bModal = TRUE;
    const INT_PTR rval = CDialogImpl::DoModal();
    m_bModal = FALSE;
    return rval;
}



/**
* FUNCTION SetRange
*
* @brief Sets the Upper and lower bounds of the Progress bar by default it will be 1 - 100 can be from 0 to 65,535
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:21:19 AM
*
* @param Low Number must be > -1 but not larger than 65,535
* @param Hi Number must be > -1 but not larger than 65,535
*/
void ProgressDialog::SetRange(int Low,int Hi)
{

	//The minimum value in the range can be from 0 to 65,535. Likewise, the maximum value can be from 0 to 65,535.
	m_LowRange=Low;
	m_HiRange=Hi;

	if(this->IsWindow())
		SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_SETRANGE, 0, MAKELPARAM(m_LowRange, m_HiRange));

}
/**
* FUNCTION GetRange
*
* @brief Gets the range of the current Control
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:32:50 AM
*
* @param Low 
* @param Hi 
*/
void  ProgressDialog::GetRange(int &Low,int &Hi)
{
	Low = SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_GETRANGE  , true,0); 
	Hi = SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_GETRANGE  , false,0); 
}

/**
* FUNCTION SetPosition
*
* @brief Sets the Position in the Progress box
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:33:10 AM
*
* @param pos Number indicating current position must be > -1 but not larger than 65,535
*/
void ProgressDialog::SetPosition(int pos)
{
	m_Pos=pos;
	if(this->IsWindow())
	{
		SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_SETPOS, pos, 0);
		if(m_OnTop)
			//::SetForegroundWindow(GetDlgItem(IDD_DIALOG_PROGRESS));
			::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);

	}

}
/**
* FUNCTION GetPosition
*
* @brief Gets the Current Position of the Box.
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:34:12 AM
*
*
* @return int How far along the Progress dlg we are.
*/
int ProgressDialog::GetPosition()
{
	return SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_GETPOS , 0, 0); 
};

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
BOOL ProgressDialog::IsVisible()
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
void ProgressDialog::DoModeless(HWND hWndParent)
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
void ProgressDialog::SetOnTop(BOOL NewTop)
{
	m_OnTop=NewTop;
}


/**
* FUNCTION UserCancel
*
* @brief If set True the user has clicked the Cancel button
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:37:37 AM
*
*
* @return bool if true the user canceled the dialog.
*/
bool ProgressDialog::UserCancel()
{
	return m_UserCancel;
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
void ProgressDialog::Close()
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
LRESULT ProgressDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	//Show Buisy.. Failing at this now..	
	//LONG_PTR style = ::GetWindowLongPtr(GetDlgItem(IDC_PROGRESS_BAR), GWL_STYLE);
	//::SetWindowLongPtr(GetDlgItem(IDC_PROGRESS_BAR), GWL_STYLE,  PBS_MARQUEE);
	SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_SETMARQUEE   , TRUE,100); 

	SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_SETRANGE, 0, MAKELPARAM(m_LowRange, m_HiRange));
	SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_SETPOS, m_Pos, 0);	

	m_UserCancel=false;

	if(m_OnTop)
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
LRESULT ProgressDialog::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	m_UserCancel=true;

	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);


	return 0;
}


/**
* FUNCTION OnNMCustomdrawProgress1
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:38:55 AM
*
* @param  
* @param pNMHDR 
* @param  
*
* @return LRESULT 
*/
LRESULT ProgressDialog::OnNMCustomdrawProgress1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here

	return 0;
}
