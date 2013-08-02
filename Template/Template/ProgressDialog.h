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

	void SetRange(int Low,int Hi);
	void GetRange(int &Low,int &Hi);
	void SetPosition(int pos);
	int GetLowRange();
	int GetHiRange();
	int GetPosition();
	BOOL IsVisible();
	INT_PTR DoModal();
	void DoModeless(HWND hWndParent);
	void SetOnTop(BOOL NewTop);
	bool UserCancel();
	void Close();

	BOOL m_bModal;
	int m_LowRange;
	int m_HiRange;
	int m_Pos;
	BOOL m_OnTop;
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

