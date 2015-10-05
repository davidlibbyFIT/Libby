#ifndef AUTO_CAMERA_H
#define AUTO_CAMERA_H

#include<TCHAR.H>
#include <Windows.h>
#include "resource.h"

#include <atlbase.h>
#include <atlwin.h>
#include <string>
#include <map>

struct AutoCameraData
{
	int min;
	int max;
	int current;
	AutoCameraData()
	{
		min=-1;
		max=-1;
		current=-1;	
	}
};

class AutoCamera: public CDialogImpl<AutoCamera>
{
public:
	AutoCamera();


	~AutoCamera(void);

	void setVars(AutoCameraData &FPS,AutoCameraData &FlashA,AutoCameraData & Exposure);
	
	//After Re basing this you must set IDD = Resource ID of the dialog you want to generate.
	enum { IDD = IDD_DIALOG_AUTO_CAMERA };
	
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
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)		
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		COMMAND_HANDLER(IDC_BUTTON_SET_LEVEL, BN_CLICKED, OnBnClickedButtonSetLevel)
	END_MSG_MAP()


private:

	CWindow m_ButtonSetLevel;
	CWindow m_ComboFPS;
	CWindow m_EditAverageBackground;
	CWindow m_EditExposureUs;
	CWindow m_EditFlashAmplitude;
	CWindow m_LableExposureUs;
	CWindow m_LableFPS;
	CWindow m_LableFlashAmplitude;
	CWindow m_SliderSetting;




public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void setSelectionByExposure_us(int findInt);

private:
	std::string AutoCamera::intToString(int Value);

	void SetEditWindowValue(CWindow &CurWindow,int NewValue);
	std::string RetriveStdStringFromCWindow(CWindow &TempWindow);
	int GetEditWindowValue(CWindow &CurWindow);
	
	int getSliderPos();
	void setSliderPos();

	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void UpdateFlashAndExposure();

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void CalculateExposureAndFlash(int &exposurePos, int &flashPos);
	int findCameraSpeedKey(int fintValue_uS);


public:

	AutoCameraData m_CurrentFPS;
	AutoCameraData m_CurrentFlashA;
	AutoCameraData m_CurrentExposure;

	AutoCameraData m_CtxFPS;
	AutoCameraData m_CtxFlashA;
	AutoCameraData m_CtxExposure;

	AutoCameraData m_Slider;

	std::map <int,int> m_CameraSpeedMap_uS;

	int m_flashASize;
	int m_exposure_uS_Size;
	int m_overallSlideCtrlSize;

	LRESULT OnBnClickedButtonSetLevel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

#endif // AUTO_CAMERA_H