#ifndef HSFC_CONFIG_H
#define HSFC_CONFIG_H



#include<TCHAR.H>
#include <Windows.h>
#include "resource.h"

#include <atlbase.h>
#include <atlwin.h>
#include <string>

class HsfcConfig: public CDialogImpl<HsfcConfig>
{
public:
	HsfcConfig(void);
	~HsfcConfig(void);

	//After Re basing this you must set IDD = Resource ID of the dialog you want to generate.
	enum { IDD = IDD_DIALOG_HSFC_FLOUR };

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

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	BEGIN_MSG_MAP(HsfcConfig)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)	
		MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
		COMMAND_HANDLER(IDC_EDIT_MIN_CH1, EN_KILLFOCUS, OnEnKillfocusEditThreshold)
		COMMAND_HANDLER(IDC_EDIT_MAX_CH1, EN_KILLFOCUS, OnEnKillfocusEditThreshold)
		COMMAND_HANDLER(IDC_EDIT_MIN_CH2, EN_KILLFOCUS, OnEnKillfocusEditThreshold)
		COMMAND_HANDLER(IDC_EDIT_MAX_CH2, EN_KILLFOCUS, OnEnKillfocusEditThreshold)
	END_MSG_MAP()


private:

	void DrawVerticalDecadeLogScale(HDC &hdc, int xLineStart, int yLineStart,int yLineEnd,int DecadeCount);
	void DrawVerticalLogScale(HDC &hdc, int yLineStart,int yLineEnd, int xLineStart);
	void DrawHoroztallLogScale(HDC &hdc, int xLineStart, int yLineStart,int xLineEnd);
	void DrawHoroztallDecadeLogScale(HDC &hdc, int xLineStart, int yLineStart,int xLineEnd,int DecadeCount);
	void DrawDecadeText(HDC &hdc,RECT DrawArea,int size,std::string TextString);


	int RetriveIntFromCWindow(CWindow &TempWindow);
	double RetriveDoubleFromCWindow(CWindow &TempWindow);



	CWindow m_EditMinCh1Sensitivity;
	CWindow m_EditMaxCh1Sensitivity;
	CWindow m_EditMinCh2Sensitivity;
	CWindow m_EditMaxCh2Sensitivity;

	CWindow m_SliderMinCh1Sensitivity;
	CWindow m_SliderMaxCh1Sensitivity;
	CWindow m_SliderMinCh2Sensitivity;
	CWindow m_SliderMaxCh2Sensitivity;


	double m_Sens_Min_Ch1;
	double m_Sens_Max_Ch1;
	double m_Sens_Min_Ch2;
	double m_Sens_Max_Ch2;

	int GetSliderPos(CWindow &CurWindow);
	void SetThresholdSliderPos(CWindow &CurWindow,double NewPos);

	std::string DoubleToString(double Value,int decPlaces=2);
	std::string IntToString(int Value);

	std::string RetriveStdStringFromCWindow(CWindow &TempWindow);
	void SetEditWindowValue(CWindow &CurWindow,double NewValue);
	double GetEditWindowValue(CWindow &CurWindow);

	void RedrawControls(bool refreshEdit=true);


public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	bool SetCh1Min(double newMin,bool refreshEdit=true);
	bool SetCh1Max(double newMax,bool refreshEdit=true);
	bool SetCh2Min(double newMin,bool refreshEdit=true);
	bool SetCh2Max(double newMax,bool refreshEdit=true);

private:
public:
	LRESULT OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEnKillfocusEditThreshold(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


#endif //HSFC_CONFIG_H