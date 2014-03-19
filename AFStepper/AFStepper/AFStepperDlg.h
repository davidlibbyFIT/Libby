
// AFStepperDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "EzStep.h"
#include <list>
#include <queue>



typedef std::list <std::string> ComPortList;
typedef ComPortList::iterator ComPortList_itr;

typedef struct SteppCommand
{
	unsigned int AbsLoc;
	unsigned int Velocity;
	bool Active;
	SteppCommand()
	{
		clear();
	}
	void clear()
	{
		AbsLoc		= 0;
		Velocity	= 0;
		Active		= false;
	}
}SteppCommand;

typedef std::queue <SteppCommand> CommandListQueue;


// CAFStepperDlg dialog
class CAFStepperDlg : public CDialogEx
{
// Construction
public:
	CAFStepperDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AFSTEPPER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
private:
	void CAFStepperDlg::CreateSystemComPortList( ComPortList &CurrentComPorts );

	EzStep m_StepControler;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonAllStop();
	void SendCommand(const std::string Command);
	void ErrorCondition(EZStepRetType code);
	void updatetime();
	void UpdateScreen();

	CStatic m_Com_Port;
	// Enter a Step Position
	UINT m_AbsPos;
	// Velocity to travel in steps per second Max 300000
	UINT m_Abs_Velocity;
	afx_msg void OnBnClickedButtonAbsGo();
	UINT m_JogPos1;
	UINT m_JogPos2;
	UINT m_JogVelocity1;
	UINT m_JogVelocity2;
	int m_RepetePasses;
	int m_RepeatCount;
	unsigned int m_CurrentPos;
	int m_intSwitchState;
	
	CommandListQueue m_CommandList;
	SteppCommand m_CurrentCommand;

	CStatic m_StrRepeatPasses;
	CStatic m_StrLocation;
	bool m_bStopTest;
	float m_CurrentCount;
	CFont *m_Font1;
	afx_msg void OnBnClickedButtonRepeatGo();
	afx_msg void OnDestroy();
	
	CStatic m_SwitchState;
	int m_Current;
	afx_msg void OnBnClickedButtonHome();
	afx_msg void OnStnClickedStaticLocation();

	void log(const std::string Info);
	const std::string currentDateTime();

	afx_msg void OnCbnSelchangeComboPreset();
	CComboBox m_ComboPreset;
};

