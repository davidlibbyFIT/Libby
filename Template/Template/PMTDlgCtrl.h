#ifndef PMT_DLG_CTRL_H
#define PMT_DLG_CTRL_H


#include<TCHAR.H>
#include <Windows.h>
#include "resource.h"

#include <atlbase.h>
#include <atlwin.h>
#include <string>

class PmtDlgCtrl: public CDialogImpl<PmtDlgCtrl>
{
public:
	PmtDlgCtrl(void);
	~PmtDlgCtrl(void);

	//After Re basing this you must set IDD = Resource ID of the dialog you want to generate.
	enum { IDD = IDD_DIALOG_PMT_CTRL };

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
		COMMAND_HANDLER(BUTTON_APPILY, BN_CLICKED, OnBnClickedAppily)
		COMMAND_HANDLER(IDC_CHECK_CH1_TRIGGER, BN_CLICKED, OnBnClickedCheckCh1Trigger)
		COMMAND_HANDLER(IDC_CHECK_CH2_TRIGGER, BN_CLICKED, OnBnClickedCheckCh2Trigger)
		COMMAND_HANDLER(IDC_EDIT_CH1_INT_MIN, EN_CHANGE, OnEnChangeEditCh1IntMin)
	END_MSG_MAP()


private:

	CWindow m_EditBoxExample;

	CWindow m_WinEditCh1IntMin;
	CWindow m_WinEditCh1IntMax;
	CWindow m_WinEditCh1TimeMin;
	CWindow m_WinEditCh1TimeMax;
	CWindow m_WinEditCh1CheckTrigger;

	CWindow m_WinEditCh2IntMin;
	CWindow m_WinEditCh2IntMax;
	CWindow m_WinEditCh2TimeMin;
	CWindow m_WinEditCh2TimeMax;
	CWindow m_WinEditCh2CheckTrigger;


	bool m_Dirty;

public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnBnClickedAppily(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	int m_Ch1IntensityMin;
	int m_Ch1IntensityMax;
	double m_Ch1TimeMin;
	double m_Ch1TimeMax;
	
	int m_Ch2IntensityMin;
	int m_Ch2IntensityMax;
	double m_Ch2TimeMin;
	double m_Ch2TimeMax;

	void RedrawValues();

	int getCh1IntensityMin(){return m_Ch1IntensityMin;};
	int getCh1IntensityMax(){return m_Ch1IntensityMax;};
	double getCh1TimeMin(){return m_Ch1TimeMin;};
	double getCh1TimeMax(){return m_Ch1TimeMax;};

	int getCh2IntensityMin(){return m_Ch2IntensityMin;};
	int getCh2IntensityMax(){return m_Ch2IntensityMax;};
	double getCh2TimeMin(){return m_Ch2TimeMin;};
	double getCh2TimeMax(){return m_Ch2TimeMax;};

	void setCh1IntensityMin(int NewValue);
	void setCh1IntensityMax(int NewValue);
	void setCh1TimeMin(double NewValue);
	void setCh1TimeMax(double NewValue);

	void setCh2IntensityMin(int NewValue);
	void setCh2IntensityMax(int NewValue);
	void setCh2TimeMin(double NewValue);
	void setCh2TimeMax(double NewValue);

	bool IsCheckBoxEnabled(HWND hWnd);
	void setCheckBox(HWND hWnd,bool NewValue);
	bool IsChanel1Enabled();
	bool IsChanel2Enabled();
	void setChanel1CheckBox(bool NewValue);
	void setChanel2CheckBox(bool NewValue);




	LRESULT OnBnClickedCheckCh1Trigger(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCheckCh2Trigger(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEditCh1IntMin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

#endif // PMT_DLG_CTRL_H