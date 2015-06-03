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

	typedef enum HSFC_ConfigReturn
	{
		HSFC_RET_OK
		, HSFC_RET_CANCEL
		, HSFC_RET_APPLY
		, HSFC_RET_NULL
	};

	HsfcConfig(void/* constructor will need to pass in a pointer to the context file */);
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
	
	HSFC_ConfigReturn m_Ret;
	bool m_IsDirty;

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
		COMMAND_HANDLER(IDC_CHECK_CH1_TRIGG, BN_CLICKED, OnBnClickedCheckCh1Trigg)
		COMMAND_HANDLER(IDC_CHECK_CH2_TRIGG, BN_CLICKED, OnBnClickedCheckCh2Trigg)
		COMMAND_HANDLER(IDC_CHECK_SMALL_PART, BN_CLICKED, OnBnClickedCheckSmallPart)
		COMMAND_HANDLER(ID_APPLY_HSFC, BN_CLICKED, OnBnClickedApply)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		COMMAND_HANDLER(ID_RESTORE, BN_CLICKED, OnBnClickedRestore)
	END_MSG_MAP()


private:

	void DrawVerticalDecadeLogScale(HDC &hdc, int xLineStart, int yLineStart,int yLineEnd,int DecadeCount);
	void DrawVerticalLogScale(HDC &hdc, int yLineStart,int yLineEnd, int xLineStart);
	void DrawHoroztallLogScale(HDC &hdc, int xLineStart, int yLineStart,int xLineEnd);
	void DrawHoroztallDecadeLogScale(HDC &hdc, int xLineStart, int yLineStart,int xLineEnd,int DecadeCount);
	void DrawDecadeText(HDC &hdc,RECT DrawArea,int size,std::string TextString);


	int RetriveIntFromCWindow(CWindow &TempWindow);
	double RetriveDoubleFromCWindow(CWindow &TempWindow);



	CWindow m_EditMinCh1;
	CWindow m_EditMaxCh1;
	CWindow m_EditMinCh2;
	CWindow m_EditMaxCh2;

	CWindow m_SliderMinCh1;
	CWindow m_SliderMaxCh1;
	CWindow m_SliderMinCh2;
	CWindow m_SliderMaxCh2;

	CWindow m_CheckCh1;
	CWindow m_CheckCh2;
	CWindow m_CheckSmallPartBox;
	CWindow m_SliderGain;
	CWindow m_ButtonApply;

	bool m_CheckEnableCh1;
	bool m_CheckEnableCh2;
	bool m_CheckSmallParticle;

	INT m_Gain;



	double m_Min_Ch1;
	double m_Max_Ch1;
	double m_Min_Ch2;
	double m_Max_Ch2;

	int GetSliderPos(CWindow &CurWindow);
	void SetThresholdSliderPos(CWindow &CurWindow,double NewPos);

	std::string DoubleToString(double Value,int decPlaces=2);
	std::string intToString(int Value);

	std::string RetriveStdStringFromCWindow(CWindow &TempWindow);
	void SetEditWindowValue(CWindow &CurWindow,double NewValue);
	void SetEditWindowValue(CWindow &CurWindow,int NewValue);
	double GetEditWindowValue(CWindow &CurWindow);
	int HsfcConfig::getGainSliderPos();
	void HsfcConfig::setGainSliderPos(int newPos);


	void RedrawControls(bool refreshEdit=true);


public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	bool setCh1Min			(double newMin,bool refreshEdit=true);
	bool setCh1Max			(double newMax,bool refreshEdit=true);
	bool setCh2Min			(double newMin,bool refreshEdit=true);
	bool setCh2Max			(double newMax,bool refreshEdit=true);
	void setCh1Enable		(bool newValue);
	void setCh2Enable		(bool newValue);
	void setGain			(int NewGain);
	void setSmallParticle	(bool newValue);

	bool getCh1Enable()		{return m_CheckEnableCh1;};
	bool getCh2Enable()		{return m_CheckEnableCh2;};
	bool getSmallParticle()	{return m_CheckSmallParticle;};
	int getGain()			;

	double getCh1Min()		{return m_Min_Ch1;};
	double getCh1Max()		{return m_Max_Ch1;};
	double getCh2Min()		{return m_Min_Ch2;};
	double getCh2Max()		{return m_Max_Ch2;};
	HSFC_ConfigReturn getExitCode(){return m_Ret;};


	//! Returns the Ret Type.
	HSFC_ConfigReturn getRet(){return m_Ret;};


private:
public:
	LRESULT OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEnKillfocusEditThreshold(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCheckCh1Trigg(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCheckCh2Trigg(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCheckSmallPart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedApply(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void saveToContext();
	void readFromContext();

	LRESULT OnBnClickedRestore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


#endif //HSFC_CONFIG_H