#include "StdAfx.h"
#include "ProgressDialog.h"


ProgressDialog::ProgressDialog(void):
	m_bModal (FALSE),
	m_LowRange(0),
	m_HiRange(100),
	m_Pos(0),
	m_OnTop(false),
	m_UserCancel(false)
{
}

INT_PTR ProgressDialog::DoModal()
{   
	m_bModal = TRUE;
    const INT_PTR rval = CDialogImpl::DoModal();
    m_bModal = FALSE;
    return rval;
}


ProgressDialog::~ProgressDialog(void)
{
}

void ProgressDialog::SetRange(int Low,int Hi)
{

	//The minimum value in the range can be from 0 to 65,535. Likewise, the maximum value can be from 0 to 65,535.
	m_LowRange=Low;
	m_HiRange=Hi;

	if(this->IsWindow())
		SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_SETRANGE, 0, MAKELPARAM(m_LowRange, m_HiRange));

}
void  ProgressDialog::GetRange(int &Low,int &Hi)
{
	Low = SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_GETRANGE  , true,0); 
	Hi = SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_GETRANGE  , false,0); 
}
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
int ProgressDialog::GetPosition()
{
	return SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_GETPOS , 0, 0); 
};

BOOL ProgressDialog::IsVisible()
{
	BOOL ret =FALSE;
	if(this->IsWindow())
	{
		ret = this->IsWindowVisible();
	}
	return ret;

}

void ProgressDialog::DoModeless(HWND hWndParent)
{
	if(!this->IsWindow())
		this->Create(hWndParent);
	if(!IsVisible())
	{
		this->ShowWindow(SW_SHOWNORMAL);
	}
}

void ProgressDialog::SetOnTop(BOOL NewTop)
{
	m_OnTop=NewTop;
}


bool ProgressDialog::UserCancel()
{
	return m_UserCancel;
}

void ProgressDialog::Close()
{
	if (m_bModal)
		EndDialog(0);
	else
		DestroyWindow();
}


LRESULT ProgressDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	//Show Buisy.. Failing at this now..	
	//LONG_PTR style = ::GetWindowLongPtr(GetDlgItem(IDC_PROGRESS_BAR), GWL_STYLE);
	//::SetWindowLongPtr(GetDlgItem(IDC_PROGRESS_BAR), GWL_STYLE,  PBS_MARQUEE);
	//SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_SETMARQUEE   , TRUE,100); 

	SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_SETRANGE, 0, MAKELPARAM(m_LowRange, m_HiRange));
	SendMessage(GetDlgItem(IDC_PROGRESS_BAR), PBM_SETPOS, m_Pos, 0);	

	m_UserCancel=false;

	if(m_OnTop)
			//::SetForegroundWindow(GetDlgItem(IDD_DIALOG_PROGRESS));
			::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);

	return 1;
};

LRESULT ProgressDialog::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	m_UserCancel=true;

	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		DestroyWindow();


	return 0;
}


LRESULT ProgressDialog::OnNMCustomdrawProgress1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here

	return 0;
}
