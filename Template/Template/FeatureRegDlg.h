#ifndef FEATURE_REG_DLG_H
#define FEATURE_REG_DLG_H


#include<TCHAR.H>
#include <Windows.h>
#include "resource.h"

#include <atlbase.h>
#include <atlwin.h>
#include <string>

class FeatureRegDlg: public CDialogImpl<FeatureRegDlg>
{
public:
	FeatureRegDlg(void);
	~FeatureRegDlg(void);
	
	//After Re basing this you must set IDD = Resource ID of the dialog you want to generate.
	enum { IDD = IDD_DIALOG_FEATRUE_REG };
	
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

	BEGIN_MSG_MAP(CAFAssistant0)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
		COMMAND_HANDLER(IDC_BUTTON_COPY_TO_CLIPBOARD, BN_CLICKED, OnBnClickedButtonCopyToClipboard)
		COMMAND_HANDLER(IDC_BUTTON_SEND_EMAIL, BN_CLICKED, OnBnClickedButtonSendEmail)
		COMMAND_HANDLER(IDC_BUTTONADD_FEATURE, BN_CLICKED, OnBnClickedButtonaddFeature)
		MESSAGE_HANDLER(WM_DROPFILES, OnDrop)
	END_MSG_MAP()


private:

	CWindow m_EditBoxUniqueId;
	CWindow m_EditBoxAddFeature;
	CWindow m_ButtonCopyToClipboard;
	CWindow m_ButtonSendMail;
	CWindow m_ButtonAddFeature;


	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnBnClickedButtonCopyToClipboard(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonSendEmail(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonaddFeature(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDrop(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void toClipboard( const std::string &s);
	std::string RetriveStdStringFromCWindow(CWindow &TempWindow);

};

#endif // FEATURE_REG_DLG_H