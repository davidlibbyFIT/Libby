#ifndef VERSA_DLG_H
#define VERSA_DLG_H


#include<TCHAR.H>
#include <Windows.h>
#include "resource.h"

#include <atlbase.h>
#include <atlwin.h>
#include <string>

class VersaDlg: public CDialogImpl<VersaDlg>
{
public:
	VersaDlg(void);
	~VersaDlg(void);
	
	//After Re basing this you must set IDD = Resource ID of the dialog you want to generate.
	enum { IDD = IDD_DIALOG_VERSA };
	
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
		COMMAND_HANDLER(IDC_BUTTON_START, BN_CLICKED, OnBnClickedButtonStart)
		COMMAND_HANDLER(IDC_RADIO_AUTO_IMAGE, BN_CLICKED, OnBnClickedRadioAutoImage)
		COMMAND_HANDLER(IDC_RADIO_TRIGGER, BN_CLICKED, OnBnClickedRadioTrigger)
		COMMAND_HANDLER(IDC_BUTTON_PICK_DIR, BN_CLICKED, OnBnClickedButtonPickDir)
	END_MSG_MAP()


	void setTriggerMode(bool newTrigger){m_hasTriggerMode = newTrigger;};


private:


	CWindow m_Dir;
	CWindow m_RadioAutoImage;
	CWindow m_RadioTrigger;

	bool m_hasTriggerMode;
	


public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:

	static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM lParam, LPARAM lpData);
	std::string BrowseFolder(std::string saved_path);

	std::string RetriveStdStringFromCWindow(CWindow &TempWindow);
	bool dirExists(const char *path);

public:
	LRESULT OnBnClickedButtonStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRadioAutoImage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRadioTrigger(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonPickDir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

#endif // VERSA_DLG_H