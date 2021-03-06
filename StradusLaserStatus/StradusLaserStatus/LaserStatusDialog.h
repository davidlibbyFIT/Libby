#pragma once

#include<TCHAR.H>
#include <list>
#include <Windows.h>
#include "resource.h"

#include <atlbase.h>
#include <atlwin.h>

//class StradusObserver;

enum LaserDialogLaserStatus
{
	LASER_DIALOG_LASER_ON,
	LASER_DIALOG_LASER_STANDBY,
	LASER_DIALOG_LASER_OFF

};


class LaserStatusDialog: public CDialogImpl<LaserStatusDialog>
{
public:
	LaserStatusDialog();
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


	enum { IDD = IDD_DIALOG_LASER };

	//! Constructor.
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetStartStopStatusText();

	//! Paint
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void DrawTempBackground();

	void SetCurrentTempC(double NewTempature);
	double GetCurrentTempC(){return m_dCurrentTempCelsius;};
	void SetLaserState(LaserDialogLaserStatus newState);

	void DrawTemperaturePointer(POINT &StartLoc, HDC hdc);

	void DrawLaserStatus(HDC hdc, COLORREF LaserColor);

	int CalculateZoneHeightPix(int Percent, int OverallHeight);

	void SetPower(int newPower);
	bool getEmergencyStop(){return m_EmergencyStop;};
	

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void DrawHorizontalGraidentRect(HDC &hdc, COLORREF Start, COLORREF End,const RECT &rect );
	void DrawVerticalGraidentRect(HDC &hdc, COLORREF Start, COLORREF End,const RECT &rect );
	void DrawTempatureScaleText(HDC &hdc,RECT DrawArea,std::string TempString);
	void SetWaveLength(std::string &newWave);
	void SetStatusText(std::string NewStatus);

	BEGIN_MSG_MAP(LaserStatusDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		COMMAND_HANDLER(ID_APPLY, BN_CLICKED, OnBnClickedApply)
		COMMAND_HANDLER(ID_START_STOP, BN_CLICKED, OnBnClickedStartStop)
	END_MSG_MAP()

	void SetWindowSmall(bool set=true);
	
private:

	//! Track Modal State
	BOOL m_bModal;
	//! Always On Top
	BOOL m_OnTop;
	//! User Cancel variable.
	bool m_UserCancel;

	RECT m_LargeRect;
	RECT m_SmallRect;
	BOOL m_bMouseTracking;
	BOOL m_bIsSmall;
	RECT m_StaticTempRectangle;
	RECT m_StaticLaserStatusRectangle;
	double m_dDegreesCPerPix;
	double m_dCurrentTempCelsius;
	LaserDialogLaserStatus m_eLaserState;
	bool m_EmergencyStop;

	int m_Power_mW;
	CWindow m_StaticStringWavelength;
	CWindow m_Static_Text_Power;
	CWindow m_Button_Start_Stop;
	CWindow m_Static_Text_Status;

	//std::shared_ptr <StradusObserver> m_pStratusObserver;



public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedApply(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedStartStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

