
// AutoFocusAssistantDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoFocusAssistant.h"
#include "AutoFocusAssistantDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CAutoFocusAssistantDlg dialog




CAutoFocusAssistantDlg::CAutoFocusAssistantDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoFocusAssistantDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAutoFocusAssistantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAutoFocusAssistantDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON1, &CAutoFocusAssistantDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAutoFocusAssistantDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CAutoFocusAssistantDlg message handlers

BOOL CAutoFocusAssistantDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAutoFocusAssistantDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAutoFocusAssistantDlg::OnPaint()
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
HCURSOR CAutoFocusAssistantDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAutoFocusAssistantDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	//OnBnClickedButton1();

	// TODO: Add your message handler code here
}


void CAutoFocusAssistantDlg::go(stAFAssistData &stDataStruct)
{
	AFAssistant1 AutoFocusStep1(stDataStruct);
	AFAssistant2 AutoFocusStep2(stDataStruct);
	AFAssistant3 AutoFocusStep3(stDataStruct);
	AFAssistant4 AutoFocusStep4(stDataStruct);
	AFAssistant5 AutoFocusStep5(stDataStruct);
	AFAssistant6 AutoFocusStep6(stDataStruct);
	AFAssistant7 AutoFocusStep7(stDataStruct);

	CPropertySheet sheet(_T("Auto Focus Assistant"));

	sheet.AddPage(&AutoFocusStep1);
	sheet.AddPage(&AutoFocusStep2);
	sheet.AddPage(&AutoFocusStep3);
	sheet.AddPage(&AutoFocusStep4);
	sheet.AddPage(&AutoFocusStep5);
	sheet.AddPage(&AutoFocusStep6);
	sheet.AddPage(&AutoFocusStep7);
	sheet.SetWizardMode();
	

	if(sheet.DoModal()==ID_WIZFINISH)
	{
		int WizOk = 1;
		MessageBox(_T("Wizzard Passed"),_T("Wizzard Passed"));
	}else
	{
		MessageBox(_T("Wizzard Failed"),_T("Wizzard Failed"));
	}



}

void CAutoFocusAssistantDlg::OnBnClickedButton1()
{
	

	stAFAssistData stDataStruct;

	stDataStruct.szFlowCel=_T("FC100");
	stDataStruct.szObjective=_T("10X");
	stDataStruct.SyringeSizeml=12.5;
	stDataStruct.RequiredVolume=5.44;

	stDataStruct.ObjFlowSizeOk=true;
	stDataStruct.ObjFlowVolumeOk=true;
	stDataStruct.ObjEverythingOk=true;

	go(stDataStruct);


//	int cc = myitem.m_btnState;

}




void CAutoFocusAssistantDlg::OnBnClickedButton2()
{

	stAFAssistData stDataStruct;
	stDataStruct.szFlowCel=_T("FC100");
	stDataStruct.szObjective=_T("10X");
	stDataStruct.SyringeSizeml=12.5;
	stDataStruct.RequiredVolume=5.44;

	stDataStruct.ObjFlowSizeOk=false;
	stDataStruct.ObjFlowVolumeOk=false;
	go(stDataStruct);


}
