#include "StdAfx.h"
#include "VersaDlg.h"
#include <Shlobj.h>
#include <sys/stat.h> //dirExists
/**
* CLASS VersaDlg
*
* @brief VersaDlg Back Flush to clean
*
* @version 1.0 
*
* @author David Libby
*
*/
VersaDlg::VersaDlg(void):
	m_bModal (false)
	,m_OnTop(false)	
	,m_hasTriggerMode(false)
{
}
VersaDlg::~VersaDlg(void)
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
LRESULT VersaDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_Dir=GetDlgItem(IDC_EDIT_DIR);	
	m_RadioAutoImage=GetDlgItem(IDC_RADIO_AUTO_IMAGE);
	m_RadioTrigger=GetDlgItem(IDC_RADIO_TRIGGER);

	//m_hasTriggerMode=true;
	
	if(!m_hasTriggerMode)
	{
		m_RadioTrigger.EnableWindow(false);
		m_RadioAutoImage.EnableWindow(false);
		SendMessage(m_RadioTrigger, BM_SETCHECK,BST_UNCHECKED, 0);
		SendMessage(m_RadioAutoImage, BM_SETCHECK,BST_CHECKED, 0);
	}
	m_Dir.SetWindowTextW(L"c:\\work2");
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
INT_PTR VersaDlg::DoModal()
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
BOOL VersaDlg::IsVisible()
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
void VersaDlg::DoModeless(HWND hWndParent)
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
void VersaDlg::SetOnTop(BOOL NewTop)
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
void VersaDlg::Close()
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
LRESULT VersaDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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
LRESULT VersaDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);
	return 0;
}

LRESULT VersaDlg::OnBnClickedButtonStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	bool trigger = SendMessage(m_RadioTrigger, BM_GETCHECK, 0, 0)==1?true:false;
	bool autoImage = SendMessage(m_RadioAutoImage, BM_GETCHECK, 0, 0)==1?true:false;
	std::string dirWindowString=RetriveStdStringFromCWindow(m_Dir);

	if(!dirExists(dirWindowString.c_str()))
	{
		MessageBoxW(L"Dir does not exist or you don't have read write permissions.",L"Bad Directory",MB_OK);		
		return 0;
	}
	return 0;
}


LRESULT VersaDlg::OnBnClickedRadioAutoImage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	SendMessage(m_RadioTrigger, BM_SETCHECK,BST_UNCHECKED, 0);


	return 0;
}


LRESULT VersaDlg::OnBnClickedRadioTrigger(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	SendMessage(m_RadioAutoImage, BM_SETCHECK,BST_UNCHECKED, 0);

	return 0;
}
int CALLBACK VersaDlg::BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM lParam, LPARAM lpData)
{

	if(uMsg == BFFM_INITIALIZED)
	{
		std::string tmp = (const char *) lpData;
		//std::cout << "path: " << tmp << std::endl;
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}

std::string VersaDlg::BrowseFolder(std::string saved_path)
{
	TCHAR path[MAX_PATH];

	const char * path_param = saved_path.c_str();

	BROWSEINFO bi = { 0 };
	bi.lpszTitle  = (L"Browse for folder...");
	bi.ulFlags    = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn       = BrowseCallbackProc;
	bi.lParam     = (LPARAM) path_param;

	LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );

	if ( pidl != 0 )
	{
		//get the name of the folder and put it in path
		SHGetPathFromIDList ( pidl, path );

		//free memory used
		IMalloc * imalloc = 0;
		if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
		{
			imalloc->Free ( pidl );
			imalloc->Release ( );
		}

		std::wstring wStr = path;
		std::string str = std::string(wStr.begin(), wStr.end());

		return str;
	}

	return "";
}


LRESULT VersaDlg::OnBnClickedButtonPickDir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	std::string myNewPath=BrowseFolder("c:");

	if(!dirExists(myNewPath.c_str()))
	{
		MessageBoxW(L"Dir does not exist or you don't have read write permissions.",L"Bad Directory",MB_OK);		
		return 0;
	}
	m_Dir.SetWindowTextW(CA2W(myNewPath.c_str()).m_szBuffer);	
	return 0;
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
std::string VersaDlg::RetriveStdStringFromCWindow(CWindow &TempWindow)
{
	int TestLength = TempWindow.GetWindowTextLengthW()+1;
	wchar_t* buffer = new wchar_t[TestLength+1];

	TempWindow.GetWindowTextW(buffer,TestLength);

	std::string SimpString=CW2A(buffer);

	delete[] buffer;
	return SimpString;
}


bool VersaDlg::dirExists(const char *path)
{
	struct stat info;

	if(stat( path, &info ) != 0)
		return false;
	else if(info.st_mode & S_IFDIR && info.st_mode & _S_IREAD && info.st_mode & _S_IWRITE )
		return true;
	else
		return false;
}