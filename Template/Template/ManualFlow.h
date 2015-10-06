#ifndef MANUAL_FLOW_H
#define MANUAL_FLOW_H


#include<TCHAR.H>
#include <Windows.h>
#include "resource.h"

#include <atlbase.h>
#include <atlwin.h>
#include <string>

class AFManualFlow: public CDialogImpl<AFManualFlow>
{
public:
	AFManualFlow(double CurrentFlowRate);
	~AFManualFlow(void);
	
	//After Re basing this you must set IDD = Resource ID of the dialog you want to generate.
	enum { IDD = IDD_DIALOG_PUMP_FLOW };
	
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
	std::string DoubleToString(double Value,int decPlaces=2);
	std::string RetriveStdStringFromCWindow(CWindow &TempWindow);
	//! Track Modal State
	BOOL m_bModal;
	//! Always On Top
	BOOL m_OnTop;
	//! User Cancel variable.
	bool m_UserCancel;


	double m_FlowRate;
	
	double getFlowRate(){return m_FlowRate;};
	void   setFlowRate(double NewRate){m_FlowRate = NewRate;};

	BEGIN_MSG_MAP(CAFAssistant0)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
	END_MSG_MAP()


private:

	CWindow m_EditFlowRate;


public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
public:
};

#endif // MANUAL_FLOW_H