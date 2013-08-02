#pragma once
#include<TCHAR.H>
#include <list>
#include <Windows.h>
#include "resource.h"

#include <atlbase.h>
#include <atlwin.h>

enum AFWizReturnTypes 
{ 
	AF_WIZ_NEXT, 
	AF_WIZ_BACK, 
	AF_WIZ_FINISH, 
	AF_WIZ_CANCEL
};

struct stAFAssistData
{
	TCHAR *szObjective;
	TCHAR *szFlowCel;
	double SyringeSizeml;
	double RequiredVolume;
	bool ObjFlowSizeOk;
	bool ObjFlowVolumeOk;
	bool ObjSolutionOk;
	bool ObjFocusOk;
	bool ObjEverythingOk;

	stAFAssistData ()
	{
		ObjFlowSizeOk=false;
		ObjFlowVolumeOk=false;
		ObjFocusOk=false;
		ObjSolutionOk=false;
		ObjEverythingOk=false;
	}
};


class AutoFocusAssistant
{
public:
	AutoFocusAssistant(stAFAssistData *pinDataStructure);
	~AutoFocusAssistant(void);
	int DoModal();
	int DisplayDialog(int DialogNumber);
private:
	//! Hold the structure for the Data Passed between the Dialogs.
	stAFAssistData *m_pAssistData;

};

/////////////// 0000000000000000000000
class AFAssistant0 : public CDialogImpl<AFAssistant0>
{
public:

	enum { IDD = IDD_WIZ_AF_0 };

	//! Constructor.
	AFAssistant0 (stAFAssistData *pinDataStructure);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(CAFAssistant0)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDD_BUTTON_FINISH, BN_CLICKED, OnBnClickedButtonFinish)
		COMMAND_HANDLER(IDD_BUTTON_BACK, BN_CLICKED, OnBnClickedButtonBack)
		COMMAND_HANDLER(IDD_BUTTON_NEXT, BN_CLICKED, OnBnClickedButtonNext)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
	END_MSG_MAP()


private:
	stAFAssistData *m_pAFDataStructure;


public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

/////////////// 1
class AFAssistant1 : public CDialogImpl<AFAssistant1>
{
public:

	enum { IDD = IDD_WIZ_AF_1 };

	//! Constructor.
	AFAssistant1 (stAFAssistData *pinDataStructure);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(CAFAssistant1)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDD_BUTTON_FINISH, BN_CLICKED, OnBnClickedButtonFinish)
		COMMAND_HANDLER(IDD_BUTTON_BACK, BN_CLICKED, OnBnClickedButtonBack)
		COMMAND_HANDLER(IDD_BUTTON_NEXT, BN_CLICKED, OnBnClickedButtonNext)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
	END_MSG_MAP()


private:
	stAFAssistData *m_pAFDataStructure;

public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

/////////////// 2
class AFAssistant2 : public CDialogImpl<AFAssistant2>
{
public:

	enum { IDD = IDD_WIZ_AF_2 };

	//! Constructor.
	AFAssistant2 (stAFAssistData *pinDataStructure);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(CAFAssistant2)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDD_BUTTON_FINISH, BN_CLICKED, OnBnClickedButtonFinish)
		COMMAND_HANDLER(IDD_BUTTON_BACK, BN_CLICKED, OnBnClickedButtonBack)
		COMMAND_HANDLER(IDD_BUTTON_NEXT, BN_CLICKED, OnBnClickedButtonNext)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		COMMAND_HANDLER(IDC_RADIO_AF_NO, BN_CLICKED, OnBnClickedRadioAfNo)
		COMMAND_HANDLER(IDC_RADIO_AF_YES, BN_CLICKED, OnBnClickedRadioAfYes)
	END_MSG_MAP()


private:
	stAFAssistData *m_pAFDataStructure;


public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRadioAfNo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRadioAfYes(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

/////////////// 3
class AFAssistant3 : public CDialogImpl<AFAssistant3>
{
public:

	enum { IDD = IDD_WIZ_AF_3 };

	//! Constructor.
	AFAssistant3 (stAFAssistData *pinDataStructure);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(CAFAssistant3)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDD_BUTTON_FINISH, BN_CLICKED, OnBnClickedButtonFinish)
		COMMAND_HANDLER(IDD_BUTTON_BACK, BN_CLICKED, OnBnClickedButtonBack)
		COMMAND_HANDLER(IDD_BUTTON_NEXT, BN_CLICKED, OnBnClickedButtonNext)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		COMMAND_HANDLER(IDC_RADIO_AF_NO, BN_CLICKED, OnBnClickedRadioAfNo)
		COMMAND_HANDLER(IDC_RADIO_AF_YES, BN_CLICKED, OnBnClickedRadioAfYes)
	END_MSG_MAP()


private:
	stAFAssistData *m_pAFDataStructure;


public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRadioAfNo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRadioAfYes(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

/////////////// 4
class AFAssistant4 : public CDialogImpl<AFAssistant4>
{
public:

	enum { IDD = IDD_WIZ_AF_4 };

	//! Constructor.
	AFAssistant4 (stAFAssistData *pinDataStructure);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(CAFAssistant4)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDD_BUTTON_FINISH, BN_CLICKED, OnBnClickedButtonFinish)
		COMMAND_HANDLER(IDD_BUTTON_BACK, BN_CLICKED, OnBnClickedButtonBack)
		COMMAND_HANDLER(IDD_BUTTON_NEXT, BN_CLICKED, OnBnClickedButtonNext)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
	END_MSG_MAP()


private:
	stAFAssistData *m_pAFDataStructure;


public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

/////////////// 5
class AFAssistant5 : public CDialogImpl<AFAssistant5>
{
public:

	enum { IDD = IDD_WIZ_AF_5 };

	//! Constructor.
	AFAssistant5 (stAFAssistData *pinDataStructure);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(CAFAssistant5)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDD_BUTTON_FINISH, BN_CLICKED, OnBnClickedButtonFinish)
		COMMAND_HANDLER(IDD_BUTTON_BACK, BN_CLICKED, OnBnClickedButtonBack)
		COMMAND_HANDLER(IDD_BUTTON_NEXT, BN_CLICKED, OnBnClickedButtonNext)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
	END_MSG_MAP()


private:
	stAFAssistData *m_pAFDataStructure;


public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

/////////////// 6
class AFAssistant6 : public CDialogImpl<AFAssistant6>
{
public:

	enum { IDD = IDD_WIZ_AF_6 };

	//! Constructor.
	AFAssistant6 (stAFAssistData *pinDataStructure);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(CAFAssistant6)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDD_BUTTON_FINISH, BN_CLICKED, OnBnClickedButtonFinish)
		COMMAND_HANDLER(IDD_BUTTON_BACK, BN_CLICKED, OnBnClickedButtonBack)
		COMMAND_HANDLER(IDD_BUTTON_NEXT, BN_CLICKED, OnBnClickedButtonNext)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
	END_MSG_MAP()


private:
	stAFAssistData *m_pAFDataStructure;


public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


/////////////// 7
class AFAssistant7 : public CDialogImpl<AFAssistant7>
{
public:

	enum { IDD = IDD_WIZ_AF_7 };

	//! Constructor.
	AFAssistant7 (stAFAssistData *pinDataStructure);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(CAFAssistant7)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDD_BUTTON_FINISH, BN_CLICKED, OnBnClickedButtonFinish)
		COMMAND_HANDLER(IDD_BUTTON_BACK, BN_CLICKED, OnBnClickedButtonBack)
		COMMAND_HANDLER(IDD_BUTTON_NEXT, BN_CLICKED, OnBnClickedButtonNext)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		COMMAND_HANDLER(IDC_RADIO_AF_NO, BN_CLICKED, OnBnClickedRadioAfNo)
		COMMAND_HANDLER(IDC_RADIO_AF_YES, BN_CLICKED, OnBnClickedRadioAfYes)
	END_MSG_MAP()


private:
	stAFAssistData *m_pAFDataStructure;


public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonFinish(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRadioAfNo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRadioAfYes(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
