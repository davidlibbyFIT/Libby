#ifndef HSFC_PMT_H
#define HSFC_PMT_H


#include<TCHAR.H>
#include <Windows.h>
#include "resource.h"

#include <atlbase.h>
#include <atlwin.h>
#include <string>

class HsfcPmtDlg: public CDialogImpl<HsfcPmtDlg>
{
public:
	HsfcPmtDlg(void);
	~HsfcPmtDlg(void);
	
	//After Re basing this you must set IDD = Resource ID of the dialog you want to generate.
	enum { IDD = IDD_CTX_HSFC_PMT };
	
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
		COMMAND_HANDLER(BUTTON_APPLY, BN_CLICKED, OnBnClickedApply)
		NOTIFY_HANDLER(IDC_SLIDER_CH1, NM_CUSTOMDRAW, OnNMCustomdrawSlider)
		NOTIFY_HANDLER(IDC_SLIDER_CH2, NM_CUSTOMDRAW, OnNMCustomdrawSlider)
		NOTIFY_HANDLER(IDC_SLIDER_CH1, NM_RELEASEDCAPTURE, OnNMReleasedcaptureSlider)
		NOTIFY_HANDLER(IDC_SLIDER_CH2, NM_RELEASEDCAPTURE, OnNMReleasedcaptureSlider)
		COMMAND_HANDLER(IDC_EDIT_CH1_SENSITIVITY, EN_CHANGE, OnEnChangeEditSensitivity)
		COMMAND_HANDLER(IDC_EDIT_CH2_SENSITIVITY, EN_CHANGE, OnEnChangeEditSensitivity)
		COMMAND_HANDLER(IDC_EDIT_CH1_SENSITIVITY, EN_KILLFOCUS, OnEnKillfocusEditSensitivity)
		COMMAND_HANDLER(IDC_EDIT_CH2_SENSITIVITY, EN_KILLFOCUS, OnEnKillfocusEditSensitivity)
	END_MSG_MAP()


private:

	CWindow m_EditCh1Sensitivity;
	CWindow m_EditCh2Sensitivity;
	CWindow m_SliderCh1Sensitivity;
	CWindow m_SliderCh2Sensitivity;
	

	double m_Ch1Sensitivity;
	double m_Ch2Sensitivity;


public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:

	std::string DoubleToString(double Value,int decPlaces=2);
	std::string IntToString(int Value);
	std::string RetriveStdStringFromCWindow(CWindow &TempWindow);
	void SetEditWindowValue(CWindow &CurWindow,double NewValue);
	double GetEditWindowValue(CWindow &CurWindow);

	double GetSliderPos(CWindow &CurWindow);
	void SetSliderPos(CWindow &CurWindow,int NewPos);
	void RedrawControls();
	LRESULT OnBnClickedApply(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNMCustomdrawSlider(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);

	void DrawDecadeText(HDC &hdc,RECT DrawArea,int size,std::string TextString);
	void DrawVerticalLogScale(HDC &hdc, int yLineStart,int yLineEnd, int xLineStart);
	void DrawSecadeLogScale(HDC &hdc, int yLineStart,int yLineEnd, int xLineStart,int DecadeNumber);

	LRESULT OnNMReleasedcaptureSlider(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);


public:

	void SetCh1Sensitivity(double NewSensitivity){m_Ch1Sensitivity = NewSensitivity;};
	void SetCh2Sensitivity(double NewSensitivity){m_Ch2Sensitivity = NewSensitivity;};
	double GetCh1Sensitivity(){return m_Ch1Sensitivity;};
	double GetCh2Sensitivity(){return m_Ch2Sensitivity;};

	LRESULT OnEnChangeEditSensitivity(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEnKillfocusEditSensitivity(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};

#endif // HSFC_PMT_H