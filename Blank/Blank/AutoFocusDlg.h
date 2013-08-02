/*
 * $Id: AutoFocusDlg.h,v 1.0 2013/03/06 14:32:39 Lee Exp $
 *
 * A class to manage the FlowCam auto focus parameters.
 *
 *
 */

#pragma once
#ifndef AUTOFOCUSDLG_H
#define AUTOFOCUSDLG_H

#include "resource.h"

#include <atlbase.h>
#include <atlwin.h>




	class AutoFocusDialog : public CDialogImpl<AutoFocusDialog>
	{
	public:

		enum { IDD = IDD_DIALOG1 };

		//! Constructor.
		AutoFocusDialog ();
		LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnFocusDoneComand(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		//LRESULT OnThreadCaptureDoneMsg(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		//LRESULT OnBnClickedAFStopCapture(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		//LRESULT OnBnClickedAFStartCapture(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

		BEGIN_MSG_MAP(CAutoFocusDialog)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
			COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
		END_MSG_MAP()


	private:

	public:
		LRESULT OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
		LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
		LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	};

#endif // ifndef AUTOFOCUSDLG_H
