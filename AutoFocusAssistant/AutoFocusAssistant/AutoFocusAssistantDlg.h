
#include "AFAssistant.h"

// AutoFocusAssistantDlg.h : header file
//

#pragma once


// CAutoFocusAssistantDlg dialog
class CAutoFocusAssistantDlg : public CDialogEx
{
// Construction
public:
	CAutoFocusAssistantDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AUTOFOCUSASSISTANT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void CAutoFocusAssistantDlg::go(stAFAssistData &stDataStruct);
};
