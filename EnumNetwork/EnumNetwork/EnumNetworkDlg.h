
// EnumNetworkDlg.h : header file
//

#pragma once

#pragma comment(lib, "mpr.lib")


#include <windows.h>
#include <stdio.h>
#include <winnetwk.h>
#include <lm.h>
#include <string>
#include <queue>
#include <set>
#include "afxwin.h"
#include "afxcmn.h"

typedef std::queue<std::string> string_queue;
typedef std::set<std::string> string_set;

// CEnumNetworkDlg dialog
class CEnumNetworkDlg : public CDialogEx
{
// Construction
public:
	CEnumNetworkDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ENUMNETWORK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:


	void UpdateAll();

	void DisplayStruct(int i, LPNETRESOURCE lpnrLocal);

	HICON m_hIcon;
	string_queue m_ShareList;
	string_set m_ShareListSet;
	HANDLE m_NetworkThreadHandle;
	HANDLE m_DriveSearchThreadHandle;
	bool m_UserCancel;

	static unsigned _stdcall FindSharesThreadProc(void *data);
	static unsigned _stdcall DriveSearchThreadProc(void *data);
	afx_msg void FindHiddenSharesOnServer( std::string ServerName);

	bool IsOkDir( std::string &fullpath );

	bool DidUserCancel(){return m_UserCancel;}

	bool IsNetworkThreadAlive();
	void AddShare( std::string ShareToAdd);
	BOOL WINAPI EnumerateFunc(LPNETRESOURCE lpnr);
	bool ListDirectoryContents(const wchar_t *sDir);
	void CloseNetworkThread();
	void CloseDriveSearchThread();
	int ShareListCount();
	void SetCurrentSearchDir(LPWSTR dirname);
	void AddCompleteDir(LPWSTR dirname);

	std::string GetNextItem();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnBnClickedButtonLoadShares();

	void ClearShareListQueue();


	void CloseThread( HANDLE &hThreadToClose );

	afx_msg void OnLbnSelchangeListServerShares();
	afx_msg void OnBnClickedButtonProgressCancel();
	CProgressCtrl m_Progress;
	CListBox m_SharesSearched;
	afx_msg void OnLbnSelchangeListServerSharesComplete();
	CStatic m_CurrentText;
	CListBox m_ListBoxShareList;
	CListBox m_ListBoxComplete;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CButton m_ButtonStart;
};
