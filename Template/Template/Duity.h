#ifndef DUITY_H
#define DUITY_H

#include<TCHAR.H>
#include <Windows.h>
#include "resource.h"

#include <atlbase.h>
#include <atlwin.h>
#include <string>
#include <vector>


class DuityDlg: public CDialogImpl<DuityDlg>
{
public:
	DuityDlg(void);
	~DuityDlg(void);
	
	//After Re basing this you must set IDD = Resource ID of the dialog you want to generate.
	enum { IDD = IDD_DIALOG_DUITY };
	
	//! Is the dialog Visible.
	BOOL IsVisible();
	//! Launches a Modal Version of the Dialog.
	INT_PTR DoModal();
	//! Launches a Mode-less Version of the Dialog.
	void DoModeless(HWND hWndParent = NULL);
	//! Experimental Function for Forcing Dialog always on top.
	void SetOnTop(BOOL NewTop);
	//! Test to see if the user canceled the dialog.
	bool UserCancel();
	//! Close the Dialog.
	void Close();

	//! Track Modal State
	BOOL m_bModal;
	//! Always On Top
	BOOL m_OnTop;
	//! User Cancel variable.
	bool m_UserCancel;

	BEGIN_MSG_MAP(DuityDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		COMMAND_HANDLER(IDC_BUTTON1, BN_CLICKED, OnBnClickedButton1)
		NOTIFY_HANDLER(IDC_SLIDER1, NM_CUSTOMDRAW, OnNMCustomdrawSlider1)
		NOTIFY_HANDLER(IDC_SLIDER1, NM_RELEASEDCAPTURE, OnNMReleasedcaptureSlider1)
	END_MSG_MAP()


private:

	CWindow m_EditBoxExample;


public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
public:
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void DrawDecadeText(HDC &hdc,RECT DrawArea,int size,std::string TextString);
	void DrawVerticalLogScale(HDC &hdc, int yLineStart,int yLineEnd, int xLineStart);
	void DrawSecadeLogScale(HDC &hdc, int yLineStart,int yLineEnd, int xLineStart,int DecadeNumber);

	LRESULT OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNMCustomdrawSlider1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	CWindow m_SlideWindowHandle;
	LRESULT OnNMReleasedcaptureSlider1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
};

#endif // DUITY_H