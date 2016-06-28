
// AutoPilotIniDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoPilotIni.h"
#include "AutoPilotIniDlg.h"
#include "afxdialogex.h"
#include "FileInformation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define RINSE_TIMER 1001
#define PURGE_TIMER 1002
#define SAMPLE_TIMER 1003
#define EOD_TIMER 1004
#define COUNT_DOWN_SECONDS 10

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAutoPilotIniDlg dialog



CAutoPilotIniDlg::CAutoPilotIniDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_AUTOPILOTINI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_statusFileName = "C:\\Programdata\\FluidImaging\\VisualSpreadsheet\\Hardware\\Autosampler\\Versa\\status.ini";
	m_pFileInformation.reset(new FileInformation(m_statusFileName.c_str()));
	m_CountDown = COUNT_DOWN_SECONDS;
}
CAutoPilotIniDlg::~CAutoPilotIniDlg()
{
	stopWorkingThread();
}


void CAutoPilotIniDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, EDIT_STATUS, m_EditStatus);
	DDX_Control(pDX, EDIT_SAMPLE_TYPE, m_EditSampleType);
	DDX_Control(pDX, EDIT_PLATE_WELL, m_EditPlateWell);
	DDX_Control(pDX, EDIT_BAR_CODE, m_EditBarCode);
	DDX_Control(pDX, EDIT_DELAY_SECONDS_1, m_EditDelaySeconds1);
	DDX_Control(pDX, EDIT_SAMPLE_ID, m_EditSampleId);
	DDX_Control(pDX, EDIT_SAMPLES, m_EditSample);
	DDX_Control(pDX, EDIT_DELAY_SECONDS_2, m_EditDelaySeconds2);
	DDX_Control(pDX, 1015, m_StatusText);
	DDX_Control(pDX, EDIT_COUNT, m_EditCountDown);
}

BEGIN_MESSAGE_MAP(CAutoPilotIniDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAutoPilotIniDlg message handlers

BOOL CAutoPilotIniDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// TODO: Add extra initialization here
	startWorkingThread();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAutoPilotIniDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAutoPilotIniDlg::OnPaint()
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
HCURSOR CAutoPilotIniDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
bool CAutoPilotIniDlg::stopWorkingThread()
{
	if (m_hGrabThread.joinable()) {
		m_stopGrabThread = true;
		//Turn On Menu Items
		m_hGrabThread.join();
	}
	return true;
}


bool CAutoPilotIniDlg::startWorkingThread()
{
	
	if (m_pFileInformation == NULL)
	{
		
		MessageBox("File Information Missing", "Missing File Info", MB_OK | MB_ICONERROR);
		return false;
	}

	if (m_hGrabThread.joinable())
	{
		MessageBox("Thread Already running.", "Thread error", MB_OK | MB_ICONERROR);
		return false;
	}


	//Force update before we start.
	theFileChanged();

	m_stopGrabThread = false;

	std::thread thread(&CAutoPilotIniDlg::workingThread, this);
	std::swap(thread, m_hGrabThread);
	//ThreadLog(m_hGrabThread);

	return true;
}


void CAutoPilotIniDlg::theFileChanged()
{
	static VersaIniData Current;
	readCurrentStatus(Current);
	if (Current != m_CurrentStatus) 
	{
		m_CurrentStatus = Current;
		UpdateStatus();
	}


}
void CAutoPilotIniDlg::readCurrentStatus(VersaIniData &Current)
{
	char buff[255] = { 0 };

	GetPrivateProfileString("Status", "Status", "5", buff, sizeof(buff), m_statusFileName.c_str());
	Current.status=(VersaStatus)atoi(buff);

	GetPrivateProfileString("Status", "SampleType", "0", buff, sizeof(buff), m_statusFileName.c_str());
	Current.sampleType = (VersaSampleType)atoi(buff);

	GetPrivateProfileString("Status", "PlateWell", "NA", buff, sizeof(buff), m_statusFileName.c_str());
	Current.plateWell = buff;

	GetPrivateProfileString("Status", "Barcode", "NA", buff, sizeof(buff), m_statusFileName.c_str());
	Current.barcode = buff;

	GetPrivateProfileString("Status", "SampleID", "NA", buff, sizeof(buff), m_statusFileName.c_str());
	Current.sampleID = buff;

	GetPrivateProfileString("Status", "DelaySeconds1", "0", buff, sizeof(buff), m_statusFileName.c_str());
	Current.delaySeconds1 = (VersaSampleType)atoi(buff);

	GetPrivateProfileString("Status", "Sample", "0", buff, sizeof(buff), m_statusFileName.c_str());
	int microliters = (VersaSampleType)atoi(buff);
	Current.SampleVolume_ml = (float)microliters / (float)1000.0;//Convert to ml


	GetPrivateProfileString("Status", "DelaySeconds2", "0", buff, sizeof(buff), m_statusFileName.c_str());
	Current.delaySeconds2 = (VersaSampleType)atoi(buff);
}


/**
* FUNCTION workingThread
*
* @brief Thread the watches the ini file.
*
* @version 1.0
*
* @author David Libby
* @date 10/6/2015 4:16:34 PM
*
* @param args
*
* @return unsigned
*/
void CAutoPilotIniDlg::workingThread()
{
	while (!m_stopGrabThread)
	{
		FileInfoWaitResult ret = m_pFileInformation->waitForFileToChange(500);//500 is 1/2 seconds..

		if (ret == FILE_INFO_WAIT_DIFFRENCE)
			theFileChanged();
	}
}
void CAutoPilotIniDlg::writeCurrentStatus()
{
	char Nubuff[255] = { 0 };


	sprintf_s(Nubuff, sizeof(Nubuff), "%d", m_CurrentStatus.status);
	WritePrivateProfileString("Status", "Status", Nubuff, m_statusFileName.c_str());

	sprintf_s(Nubuff, sizeof(Nubuff), "%d", m_CurrentStatus.sampleType);
	WritePrivateProfileString("Status", "SampleType", Nubuff, m_statusFileName.c_str());

	WritePrivateProfileString("Status", "PlateWell", m_CurrentStatus.plateWell.c_str(), m_statusFileName.c_str());
	WritePrivateProfileString("Status", "Barcode", m_CurrentStatus.barcode.c_str(), m_statusFileName.c_str());
	WritePrivateProfileString("Status", "SampleID", m_CurrentStatus.sampleID.c_str(), m_statusFileName.c_str());

	sprintf_s(Nubuff, sizeof(Nubuff), "%d", m_CurrentStatus.delaySeconds1);
	WritePrivateProfileString("Status", "DelaySeconds1", Nubuff, m_statusFileName.c_str());

	//Doc UpdateFix
	// 	int microliters=(int)(m_CurrentStatus.SampleVolume_ml*(float)1000.0);//Convert to microliters.
	// 	sprintf_s(Nubuff, sizeof(Nubuff), "%d", microliters);
	// 	WritePrivateProfileString("Status", "SampleVolume",  Nubuff, m_statusFileName.c_str());

	int microliters = (int)(m_CurrentStatus.SampleVolume_ml*(float)1000.0);//Convert to microliters.
	sprintf_s(Nubuff, sizeof(Nubuff), "%d", microliters);
	WritePrivateProfileString("Status", "Sample", Nubuff, m_statusFileName.c_str());


	sprintf_s(Nubuff, sizeof(Nubuff), "%d", m_CurrentStatus.delaySeconds2);
	WritePrivateProfileString("Status", "DelaySeconds2", Nubuff, m_statusFileName.c_str());
	UpdateStatus();


}
std::string CAutoPilotIniDlg::GetShortStatus()
{

	/*
	Status=0 -- idle status (updated by VERSA Appl. 110 when VERSA 110 instrument is initialized with home toolbar button or when the sequence starts running)
	Status=1 -- acknowledgement by Fluid Imager software (when Fluid Imager reads Status=3, Fluid Imager immediately updates Status=1)
	Status=3 -- VERSA Appl. 110 tells Fluid Imager to start reading a single sample that was just dispensed into injection port (after first delay if available)
	Status=4 -- Fluid Imager tells VERSA Appl. 110 that a single sample was successfully read (VERSA Appl. 110 proceeds to next sample or wash sequence after second delay if available)
	Status=5 -- Fluid Imager returns error or VERSA Appl. 110 returns hardware error (all execution is stopped; the 500 ms timer is not disabled; error	message is displayed by VERSA Appl. 110)
	*/
	std::string ret;
	ret = std::to_string(m_CurrentStatus.status) + " ( ";
	switch (m_CurrentStatus.status)
	{

	case VERSA_STATUS_IDLE:
		ret += "VERSA_STATUS_IDLE";
		break;
	case VERSA_STATUS_UNKNOWEN:
		ret += "VERSA_STATUS_UNKNOWEN";
		break;
	case VERSA_STATUS_SAMPLE_READY:
		ret += "VERSA_STATUS_SAMPLE_READY";
		break;
	case VERSA_STATUS_START_IMAGING:
		ret += "VERSA_STATUS_START_IMAGING";
		break;
	case VERSA_STATUS_FLOWCAM_READY:
		ret += "VERSA_STATUS_FLOWCAM_READY";
		break;
	case VERSA_STATUS_ERROR:
		ret += "VERSA_STATUS_ERROR";
		break;
	case VERSA_ALL_STOP:
		ret += "VERSA_ALL_STOP";
		break;
	case VERSA_VISUAL_CLEANING:
		ret += "VERSA_VISUAL_CLEANING";
		break;
	case VERSA_VISUAL_END_OF_DAY:
		ret += "VERSA_VISUAL_END_OF_DAY";
		break;
	case VERSA_VISUAL_START_OF_DAY:
		ret += "VERSA_VISUAL_START_OF_DAY";
		break;

	}

	ret += " )";
	
	return ret;
}
std::string CAutoPilotIniDlg::GetSampleType()
{

	std::string ret;
	ret = std::to_string(m_CurrentStatus.sampleType) + " ( ";
	switch (m_CurrentStatus.sampleType)
	{

	case VERSA_STATUS_IDLE:
		ret += "VERSA_SAMPLE_STANDARD";
		break;
	case VERSA_STATUS_UNKNOWEN:
		ret += "VERSA_SAMPLE_CLEAN";
		break;
	case VERSA_STATUS_SAMPLE_READY:
		ret += "VERSA_SAMPLE_END_OF_DAY";
		break;

	}

	ret += " )";

	return ret;
}


void CAutoPilotIniDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	
	PURGE_TIMER;

	switch (nIDEvent)
	{
		case PURGE_TIMER: 
		{
			if (m_CountDown <= 0)
			{
				KillTimer(PURGE_TIMER);
				m_CurrentStatus.status = VERSA_STATUS_FLOWCAM_READY;
				writeCurrentStatus();
			}
			UpdateStatus();
			m_CountDown--;
			break;
		}
		case SAMPLE_TIMER:
		{
			if (m_CountDown <= 0)
			{
				KillTimer(SAMPLE_TIMER);
				m_StatusText.SetWindowText("Purging Flowcell");
				SetTimer(PURGE_TIMER, 1000, NULL);
				m_CountDown = COUNT_DOWN_SECONDS;
				m_CurrentStatus.status = VERSA_VISUAL_CLEANING;
				writeCurrentStatus();
			}
			UpdateStatus();
			m_CountDown--;
			break;
		}
		case RINSE_TIMER:
		{
			if (m_CountDown <= 0)
			{
				KillTimer(RINSE_TIMER);
				m_StatusText.SetWindowText("Purging Flowcell");
				SetTimer(PURGE_TIMER, 1000, NULL);
				m_CountDown = COUNT_DOWN_SECONDS;
				m_CurrentStatus.status = VERSA_VISUAL_CLEANING;
				writeCurrentStatus();
			}
			UpdateStatus();
			m_CountDown--;
			break;
		}
		case EOD_TIMER:
		{
			if (m_CountDown <= 0)
			{
				KillTimer(EOD_TIMER);
				m_StatusText.SetWindowText("flooding Flowcell");
				m_CurrentStatus.status = VERSA_STATUS_FLOWCAM_READY;
				writeCurrentStatus();
			}
			UpdateStatus();
			m_CountDown--;
			break;
		}

	}



	CDialogEx::OnTimer(nIDEvent);
}

void CAutoPilotIniDlg::UpdateStatus()
{
	m_EditStatus.SetWindowText(GetShortStatus().c_str());
	m_EditSampleType.SetWindowText(GetSampleType().c_str());
	m_EditPlateWell.SetWindowText(m_CurrentStatus.plateWell.c_str());
	m_EditBarCode.SetWindowText(m_CurrentStatus.barcode.c_str());
	m_EditDelaySeconds1.SetWindowText(std::to_string(m_CurrentStatus.delaySeconds1).c_str());
	m_EditSampleId.SetWindowText(m_CurrentStatus.sampleID.c_str());
	m_EditSample.SetWindowText(std::to_string(m_CurrentStatus.SampleVolume_ml).c_str());
	m_EditDelaySeconds2.SetWindowText(std::to_string(m_CurrentStatus.delaySeconds2).c_str());

	if (m_CurrentStatus.status == VERSA_VISUAL_START_OF_DAY && m_CurrentStatus.sampleType == VERSA_SAMPLE_END_OF_DAY)
	{
		m_StatusText.SetWindowText("Purging Flowcell");
		m_CurrentStatus.status = VERSA_VISUAL_CLEANING;
		writeCurrentStatus();
		SetTimer(PURGE_TIMER, 1000,NULL );
		m_CountDown = COUNT_DOWN_SECONDS;
	}
	else if (m_CurrentStatus.status == VERSA_STATUS_SAMPLE_READY && m_CurrentStatus.sampleType == VERSA_SAMPLE_STANDARD)
	{
		m_StatusText.SetWindowText("Processing Sample");
		SetTimer(SAMPLE_TIMER, 1000, NULL);
		m_CountDown = COUNT_DOWN_SECONDS;
		m_CurrentStatus.status = VERSA_STATUS_START_IMAGING;
		writeCurrentStatus();
	}
	else if (m_CurrentStatus.status == VERSA_STATUS_SAMPLE_READY && m_CurrentStatus.sampleType == VERSA_SAMPLE_CLEAN)
	{
		m_StatusText.SetWindowText("Wash / Rinse Flowcell");
		SetTimer(RINSE_TIMER, 1000, NULL);
		m_CountDown = COUNT_DOWN_SECONDS;
		m_CurrentStatus.status = VERSA_VISUAL_CLEANING;
		writeCurrentStatus();
	}
	else if (m_CurrentStatus.status == VERSA_STATUS_SAMPLE_READY && m_CurrentStatus.sampleType == VERSA_SAMPLE_END_OF_DAY)
	{
		m_StatusText.SetWindowText("Flood Flowcell End Of Day");
		SetTimer(EOD_TIMER, 1000, NULL);
		m_CountDown = COUNT_DOWN_SECONDS;
		m_CurrentStatus.status = VERSA_VISUAL_CLEANING;
		writeCurrentStatus();
	}
	else if (m_CurrentStatus.status == VERSA_VISUAL_END_OF_DAY && m_CurrentStatus.sampleType == VERSA_SAMPLE_END_OF_DAY)
	{
		m_StatusText.SetWindowText("End of run Flooded Flowcell");
		//UpdateStatus();
	}
	else if (m_CurrentStatus.status == VERSA_STATUS_FLOWCAM_READY )
	{
		m_StatusText.SetWindowText("Ready For Next Sample");
		//UpdateStatus();
	}

	m_EditCountDown.SetWindowText(std::to_string(m_CountDown).c_str());
}
