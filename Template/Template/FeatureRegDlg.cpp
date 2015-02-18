#include "StdAfx.h"
#include "FeatureRegDlg.h"

/**
* CLASS FeatureRegDlg
*
* @brief FeatureRegDlg Back Flush to clean
*
* @version 1.0 
*
* @author David Libby
*
*/
FeatureRegDlg::FeatureRegDlg(void):
	m_bModal (false)
	,m_OnTop(false)	
	,m_EditBoxUniqueId(NULL)
	,m_EditBoxAddFeature(NULL)
	,m_ButtonCopyToClipboard(NULL)
	,m_ButtonSendMail(NULL)
	,m_ButtonAddFeature(NULL)

{
}

FeatureRegDlg::~FeatureRegDlg(void)
{
}

void FeatureRegDlg::toClipboard(const std::string &s)
{
	OpenClipboard();
	EmptyClipboard();	
	HGLOBAL hg=GlobalAlloc(GMEM_MOVEABLE,s.size()+1);
	if (!hg){
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg),s.c_str(),s.size()+1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT,hg);
	CloseClipboard();
	GlobalFree(hg);
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
INT_PTR FeatureRegDlg::DoModal()
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
BOOL FeatureRegDlg::IsVisible()
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
void FeatureRegDlg::DoModeless(HWND hWndParent)
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
void FeatureRegDlg::SetOnTop(BOOL NewTop)
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
void FeatureRegDlg::Close()
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
LRESULT FeatureRegDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	//std::string Start_mL="1.0";
	//m_EditBoxExample=GetDlgItem(IDC_EDIT_AMOUNT_ML);
	//m_EditBoxExample.SetWindowTextW(CA2W(Start_mL.c_str()).m_szBuffer);

	m_EditBoxUniqueId			=GetDlgItem(IDC_EDIT_UNIQUE_ID);
	m_EditBoxAddFeature			=GetDlgItem(IDC_EDIT_NEW_CODE);
	m_ButtonCopyToClipboard		=GetDlgItem(IDC_BUTTON_COPY_TO_CLIPBOARD);
	m_ButtonSendMail			=GetDlgItem(IDC_BUTTON_SEND_EMAIL);
	m_ButtonAddFeature			=GetDlgItem(IDC_BUTTONADD_FEATURE);


	std::string guid="00155DF77106";
	m_EditBoxUniqueId.SetWindowTextW(CA2W(guid.c_str()).m_szBuffer);


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
LRESULT FeatureRegDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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
LRESULT FeatureRegDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);
	return 0;
}








LRESULT FeatureRegDlg::OnBnClickedButtonCopyToClipboard(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	toClipboard(RetriveStdStringFromCWindow(m_EditBoxUniqueId).c_str());

	return 0;
}


LRESULT FeatureRegDlg::OnBnClickedButtonSendEmail(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	
	std::string MailAddress="mailto:CustomerCare@fluidimaging.com?subject=Customer%20Machine%20Id&body=" + RetriveStdStringFromCWindow(m_EditBoxUniqueId);

	ShellExecute(NULL, L"open", CA2W(MailAddress.c_str()).m_szBuffer,NULL, NULL,SW_SHOWNORMAL);
	
	return 0;
}


LRESULT FeatureRegDlg::OnBnClickedButtonaddFeature(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	return 0;
}


LRESULT FeatureRegDlg::OnDrop(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDROP   hDrop;
	hDrop = (HDROP)wParam;
	UINT nCnt  = DragQueryFile(hDrop, (UINT)-1, NULL, 0); 
	for(UINT nIndex = 0; nIndex < nCnt; ++nIndex) {
		UINT nSize;
		if(0 == (nSize = DragQueryFile(hDrop, nIndex, NULL, 0))) 
			continue;

		TCHAR *pszFileName = new TCHAR[++nSize];
		if(DragQueryFile(hDrop, nIndex, pszFileName, nSize)) 
		{
			//next file name is in 'pszFileName', use this
			int aa=1;
		}
		delete [] pszFileName;
	}	

	return 0;
}
std::string FeatureRegDlg::RetriveStdStringFromCWindow(CWindow &TempWindow)
{
	int TestLength = TempWindow.GetWindowTextLengthW()+1;
	wchar_t* buffer = new wchar_t[TestLength+1];

	TempWindow.GetWindowTextW(buffer,TestLength);
	std::string SimpString=CW2A(buffer);

	delete[] buffer;
	return SimpString;
}
