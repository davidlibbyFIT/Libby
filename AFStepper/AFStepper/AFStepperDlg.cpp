
// AFStepperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AFStepper.h"
#include "AFStepperDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <time.h>

//#include "ProgressDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_LOCATION 1
#define TIMER_LONG_TEST 2
#define STEP_MIN 0
#define STEP_MAX 1500000
#define VELOCITY_MAX 300000
#define VELOCITY_MIN 100

#define TIME_UPDATE 100

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};



CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAFStepperDlg dialog




CAFStepperDlg::CAFStepperDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAFStepperDlg::IDD, pParent)
	, m_AbsPos(0)
	, m_Abs_Velocity(0)
	, m_JogPos1(0)
	, m_JogPos2(0)
	, m_JogVelocity1(0)
	, m_JogVelocity2(0)
	, m_RepeatCount(0)
	, m_RepetePasses(0)
	, m_bStopTest(false)
	, m_CurrentCount(0)
	, m_Current(0)
	, m_intSwitchState(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAFStepperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_COM_PORT, m_Com_Port);
	DDX_Text(pDX, IDC_EDIT_ABS_POS, m_AbsPos);
	DDV_MinMaxUInt(pDX, m_AbsPos, STEP_MIN, STEP_MAX);
	DDX_Text(pDX, IDC_EDIT_ABS_VELOCITY, m_Abs_Velocity);
	DDV_MinMaxUInt(pDX, m_Abs_Velocity, VELOCITY_MIN, VELOCITY_MAX);
	DDX_Text(pDX, IDC_EDIT_POS_JOG_1, m_JogPos1);
	DDV_MinMaxUInt(pDX, m_JogPos1, STEP_MIN, STEP_MAX);
	DDX_Text(pDX, IDC_EDIT_POS_JOG_2, m_JogPos2);
	DDV_MinMaxUInt(pDX, m_JogPos2, STEP_MIN, STEP_MAX);
	DDX_Text(pDX, IDC_EDIT_VELOCITY_JOG_1, m_JogVelocity1);
	DDV_MinMaxUInt(pDX, m_JogVelocity1, VELOCITY_MIN,VELOCITY_MAX);
	DDX_Text(pDX, IDC_EDIT_VELOCITY_JOG_2, m_JogVelocity2);
	DDV_MinMaxUInt(pDX, m_JogVelocity2, VELOCITY_MIN,VELOCITY_MAX);
	DDX_Text(pDX, IDC_EDIT_REPETE_COUNT, m_RepeatCount);
	DDV_MinMaxInt(pDX, m_RepeatCount, 1, 1000000000);
	DDX_Control(pDX, IDC_STATIC_REPEAT_PASSES, m_StrRepeatPasses);
	DDX_Control(pDX, IDC_STATIC_LOCATION, m_StrLocation);
	DDX_Control(pDX, IDC_STATIC_SWITCH_STATE, m_SwitchState);
	DDX_Text(pDX, IDC_EDIT_REPETE_CURRENT, m_Current);
	DDV_MinMaxInt(pDX, m_Current, 5, 85);
}

BEGIN_MESSAGE_MAP(CAFStepperDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ALL_STOP, &CAFStepperDlg::OnBnClickedButtonAllStop)
	ON_BN_CLICKED(IDC_BUTTON_ABS_GO, &CAFStepperDlg::OnBnClickedButtonAbsGo)
	ON_BN_CLICKED(IDC_BUTTON_REPEAT_GO, &CAFStepperDlg::OnBnClickedButtonRepeatGo)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_HOME, &CAFStepperDlg::OnBnClickedButtonHome)
	ON_STN_CLICKED(IDC_STATIC_LOCATION, &CAFStepperDlg::OnStnClickedStaticLocation)
END_MESSAGE_MAP()


// CAFStepperDlg message handlers
#include <iomanip>      // std::setw

BOOL CAFStepperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	std::ostringstream TempString;
	TempString << "Chan [" << std::setw (10) << (5) <<"]\t";
	std::string bb=TempString.str();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_Font1 = new CFont;
	m_Font1->CreatePointFont(160, "Arial Bold");	
	m_StrLocation.SetFont(m_Font1,1);
	m_Com_Port.SetFont(m_Font1,1);
	m_StrRepeatPasses.SetFont(m_Font1,1);
	m_SwitchState.SetFont(m_Font1,1);

	m_Abs_Velocity = 250000;
	m_AbsPos = STEP_MAX;

	//m_JogPos1=STEP_MAX;
	//m_JogPos2=STEP_MIN;
	//m_JogVelocity1=250000;
	//m_JogVelocity2=250000;

	m_JogPos1=134400;
	m_JogPos2=403200;
	m_JogVelocity1=250000;
	m_JogVelocity2=1250;
	m_Current=35;

	//m_JogVelocity2=50000;

	m_RepeatCount = 1;

	
	UpdateData(false);


	// TODO: Add extra initialization here



	ComPortList CurrentComPorts;
	CreateSystemComPortList(CurrentComPorts);		
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	bool FoundComPort=false;
	for(ComPortList_itr SingleComPort = CurrentComPorts.begin(); SingleComPort != CurrentComPorts.end(); ++SingleComPort)
	{
		if(m_StepControler.CheckComPortForEZStepperAllMotion(*SingleComPort)==EZ_OK)
		{
			FoundComPort=true;
			std::ostringstream LogSting;
			LogSting << "Found Com Port :"; 
			LogSting << m_StepControler.GetComPort();;
			log(LogSting.str());


			break;
		}
	}



	//OnBnClickedButtonHome();
	log("Init Stepper");
	SendCommand("/1m35j256L400K3000f1n2R\r");
	//SendCommand("/1V250000gZ10000000S03G0R\r");
	Sleep(1000);


	SetTimer(TIMER_LOCATION, TIME_UPDATE, NULL);



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAFStepperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAFStepperDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAFStepperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




/**
* FUNCTION CreateSystemComPortList*
* @brief THis will generate a list of com ports available according to the OS.
*
* @version 1.0 
*
* @author David Libby
* @date 12/16/2013 10:50:31 PM
*
* @param CurrentComPorts returns a std::list of all the com ports in the system.
*/
void CAFStepperDlg::CreateSystemComPortList( ComPortList &CurrentComPorts )
{

	char device[20000];

	device[0]='\0';
	DWORD nchar = QueryDosDevice(NULL, device, sizeof(device));

	if (nchar == 0) {
		//sprintf_s(buff, "QueryDosDevice() failure in Focuser::connect(); GetLastError = %d", GetLastError());
		//throw FocusControlError(buff);
		return;
	}
	int i=0;
	while (device[i] != 0) 
	{
		if (strncmp(device + i, "COM", 3) == 0) 
		{
			std::string SinglePort = device + i;
			CurrentComPorts.push_back(SinglePort);
		}
		while (device[i])
			++i;
		++i;
	}			

	device[0]='\0';
}



void CAFStepperDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case TIMER_LOCATION:
		{
			UpdateScreen();
		}	
	default:
		{
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
void CAFStepperDlg::OnBnClickedButtonAllStop()
{
	SendCommand("/1TR\r");
	m_bStopTest=true;
	log("User Pressed All Stop");
}
void CAFStepperDlg::SendCommand(const std::string Command)
{

	EZStepRetType ret = m_StepControler.SendCommand(Command);
	if(ret!=EZ_OK)
	{
		ErrorCondition(ret);
	}
}
void CAFStepperDlg::ErrorCondition(EZStepRetType code)
{
	std::string error;
	switch(code)
	{
	case EZ_BAD_COM_PORT:
		{
			error="Bad Com Port: ";
			break;
		}
	case EZ_BAD_COMMAND:
		{
			error="Bad Command";
			break;
		}
	case EZ_PORT_NOT_OPEN:
		{
			error="Port Not Open";
			break;
		}
	default:
		{
			error="Unknowen Error";
			break;
		}
	}

	log(error);

	MessageBox(error.c_str(),"Error",MB_ICONERROR | MB_OK);
	OnCancel();

}


void CAFStepperDlg::OnBnClickedButtonAbsGo()
{
	m_CurrentCommand.Active=false;
	UpdateData(true);
	if(m_AbsPos>STEP_MAX)
	{
		m_AbsPos=STEP_MAX;
		UpdateData(false);
	}

	if(m_AbsPos<STEP_MIN)
	{
		m_AbsPos=STEP_MIN;
		UpdateData(false);
	}

	if(m_Abs_Velocity>VELOCITY_MAX)
	{
		m_Abs_Velocity=VELOCITY_MAX;
		UpdateData(false);
	}

	if(m_Abs_Velocity<VELOCITY_MIN)
	{
		m_Abs_Velocity=VELOCITY_MIN;
		UpdateData(false);
	}

	std::ostringstream LogSting;
	LogSting <<	"Manual Move to : ";
	LogSting << m_AbsPos;
	LogSting << " At Velocity : ";
	LogSting << m_Abs_Velocity;			
	log(LogSting.str());

	CWaitCursor wait;
	std::ostringstream AbsString;
	UpdateData(true);
	AbsString << "/1V" << m_Abs_Velocity << "A" << m_AbsPos << "R\r";
	SendCommand(AbsString.str());
}


void CAFStepperDlg::OnBnClickedButtonRepeatGo()
{

	m_RepetePasses = 0;
	m_bStopTest=false;
	m_CurrentCount=0;

	std::ostringstream LogSting;
	LogSting << "Starting Run of "; 
	LogSting << m_RepetePasses;
	LogSting <<	" Passes";
	log(LogSting.str());


	UpdateData(true);

	SendCommand("/1m35\r");

	std::ostringstream AbsString;
	AbsString << "/1m" << m_Current << "R\r";
	SendCommand(AbsString.str());


	if(m_JogPos1>STEP_MAX)
	{
		m_JogPos1=STEP_MAX;
		UpdateData(false);
	}

	if(m_JogPos1<STEP_MIN)
	{
		m_JogPos1=STEP_MIN;
		UpdateData(false);
	}

	if(m_JogVelocity1>VELOCITY_MAX)
	{
		m_JogVelocity1=VELOCITY_MAX;
		UpdateData(false);
	}

	if(m_JogVelocity2<VELOCITY_MIN)
	{
		m_JogVelocity2=VELOCITY_MIN;
		UpdateData(false);
	}


	if(m_JogPos2>STEP_MAX)
	{
		m_JogPos2=STEP_MAX;
		UpdateData(false);
	}

	if(m_JogPos2<STEP_MIN)
	{
		m_JogPos2=STEP_MIN;
		UpdateData(false);
	}

	if(m_JogVelocity2>VELOCITY_MAX)
	{
		m_JogVelocity2=VELOCITY_MAX;
		UpdateData(false);
	}

	if(m_JogVelocity1<VELOCITY_MIN)
	{
		m_JogVelocity1=VELOCITY_MIN;
		UpdateData(false);
	}




	SteppCommand Item;

	for(int ct =0;ct<m_RepeatCount;ct++)
	{
		Item.AbsLoc=m_JogPos1;
		Item.Velocity=m_JogVelocity1;
		m_CommandList.push(Item);

		Item.AbsLoc=m_JogPos2;
		Item.Velocity=m_JogVelocity2;
		m_CommandList.push(Item);
	}


}
void CAFStepperDlg::updatetime()
{
	m_CurrentPos = m_StepControler.ReadPosition();
	m_intSwitchState = m_StepControler.ReadSwitchState();
}
void CAFStepperDlg::UpdateScreen()
{
	updatetime();
	std::ostringstream oss;
	oss << m_CurrentPos;
	m_StrLocation.SetWindowText(oss.str().c_str());
	m_Com_Port.SetWindowText(m_StepControler.GetComPort().c_str());

	std::ostringstream CurrentCount;
	CurrentCount << m_CurrentCount;
	m_StrRepeatPasses.SetWindowText(CurrentCount.str().c_str());

	//m_intSwitchState&(1<<4)
	
	if(m_intSwitchState&(1<<2))
		m_SwitchState.SetWindowText("Open");
	else
		m_SwitchState.SetWindowText("Closed");

	if(m_bStopTest)
	{
		m_CurrentCommand.clear();
		while (!m_CommandList.empty())
		{
			m_CommandList.pop();
		}
	}

	//Are we working on a command?
	if(m_CurrentCommand.Active)
	{
		if(m_CurrentPos==m_CurrentCommand.AbsLoc)
		{
			m_CurrentCount+=.5;
			std::ostringstream LogSting;
			LogSting << "Completed Step :"; 
			LogSting << m_CurrentCount;
			LogSting <<	"\t\tMove to : ";
			LogSting << m_CurrentCommand.AbsLoc;
			LogSting << "\t\tAt Velocity : ";
			LogSting << m_CurrentCommand.Velocity;
			LogSting << "\tAt current% : ";
			LogSting << m_Current;			

			log(LogSting.str());
			
			m_CurrentCommand.clear();
			

		}
	}else //We don't have a current command check the stack for one.
	{
		if(!m_CommandList.empty())
		{
			m_CurrentCommand=m_CommandList.front();
			m_CurrentCommand.Active=true;
			m_CommandList.pop();
			std::ostringstream AbsString;
			AbsString << "/1V" << m_CurrentCommand.Velocity << "A" << m_CurrentCommand.AbsLoc << "R\r";
			SendCommand(AbsString.str());
		}else
		{
			m_CurrentCommand.clear();
		}
	}

}


void CAFStepperDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	if(m_Font1)
		delete m_Font1;
	// TODO: Add your message handler code here
}



void CAFStepperDlg::OnBnClickedButtonHome()
{
	OnBnClickedButtonAllStop();
	log("Init Stepper");
	SendCommand("/1m35j256L400K3000f1n2R\r");
	SendCommand("/1V250000gZ10000000S03G0R\r");
	Sleep(1000);
}


void CAFStepperDlg::OnStnClickedStaticLocation()
{
	// TODO: Add your control notification handler code here
}

void CAFStepperDlg::log(const std::string Info)
{
		
		


		std::ofstream file;
		file.open("Runs.txt", std::fstream::out | std::fstream::app);
		if(file.is_open())
		{
			file << currentDateTime();
			file << "\t";
			file << Info;
			file << "\n";
			file.close();
		}			
}
const std::string CAFStepperDlg::currentDateTime()
{
	time_t rawtime;
	time ( &rawtime );
	struct tm timeinfo2;
	localtime_s(&timeinfo2, &rawtime);

	char       buf[80];

	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &timeinfo2);


    return buf;
}
