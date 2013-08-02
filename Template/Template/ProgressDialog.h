#pragma once

#include<TCHAR.H>
#include <list>
#include <Windows.h>
#include "resource.h"

#include <atlbase.h>
#include <atlwin.h>


class ProgressDialog: public CDialogImpl<ProgressDialog>
{
public:
	ProgressDialog(void);
	~ProgressDialog(void);
	
	//! Sets the Range for the Dialog
	void SetRange(int Low,int Hi);
	//! Gert the current Dialog Range
	void GetRange(int &Low,int &Hi);
	//! Sets the Dialog Position.
	void SetPosition(int pos);
	//! Gets Current Position of Dialog
	int GetPosition();
	//! Is the dialog Visible.
	BOOL IsVisible();
	//! Launches a Modal Version of the Dialog.
	INT_PTR DoModal();
	//! Launches a Modless Version of the Dialog.
	void DoModeless(HWND hWndParent = NULL);
	//! Experimental Function for Forcing Dialog always on top.
	void SetOnTop(BOOL NewTop);
	//! Test to see if the user canceled the dialog.
	bool UserCancel();
	//! Close the Dialog.
	void Close();

	//! Track Modal State
	BOOL m_bModal;
	//! Low Range of Dialog
	int m_LowRange;
	//! Hi Range of Dialog
	int m_HiRange;
	//! Current Position of Dialog
	int m_Pos;
	//! Always On Top
	BOOL m_OnTop;
	//! User Cancel variable.
	bool m_UserCancel;

	enum { IDD = IDD_DIALOG_PROGRESS };

	//! Constructor.
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(CAFAssistant0)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		NOTIFY_HANDLER(IDC_PROGRESS1, NM_CUSTOMDRAW, OnNMCustomdrawProgress1)
	END_MSG_MAP()


private:


public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnNMCustomdrawProgress1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
};

