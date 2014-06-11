#pragma once

#include<TCHAR.H>
#include <list>
#include <Windows.h>
#include "resource.h"

#include <atlbase.h>
#include <atlwin.h>

class LaserStatusDialog: public CDialogImpl<LaserStatusDialog>
{
public:
	LaserStatusDialog(void);
	~LaserStatusDialog(void);
	
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
	//! Always On Top
	BOOL m_OnTop;
	//! User Cancel variable.
	bool m_UserCancel;

	enum { IDD = IDD_DIALOG_LASER };

	//! Constructor.
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//! Paint
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void DrawTempBackground();

	

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void DrawHorizontalGraidentRect(HDC &hdc, COLORREF Start, COLORREF End,const RECT &rect );
	void DrawVerticalGraidentRect(HDC &hdc, COLORREF Start, COLORREF End,const RECT &rect );
	void DrawTempatureScaleText(HDC &hdc,RECT DrawArea,std::string TempString);

	BEGIN_MSG_MAP(CAFAssistant0)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
	END_MSG_MAP()

	void SetWindowSmall(bool set=true);

private:
	RECT m_LargeRect;
	RECT m_SmallRect;
	BOOL m_bMouseTracking;
	BOOL m_bIsSmall;
	RECT m_StaticTempRectangle;



public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

