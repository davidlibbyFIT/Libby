
// LaserDlg.h : header file
//

#pragma once
#include "statusbox.h"


// CLaserDlg dialog
class CLaserDlg : public CDialogEx
{
// Construction
public:
	CLaserDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LASER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();

	void SetWindowSmall(bool set=true);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CRect m_LargeRect;
	CRect m_SmallRect;
	BOOL m_bMouseTracking;
	BOOL m_bIsSmall;
	BOOL m_IsVisible;
public:
	CStatusBox m_Status;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
