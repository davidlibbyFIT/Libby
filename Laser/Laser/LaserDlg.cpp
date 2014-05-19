
// LaserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Laser.h"
#include "LaserDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define SHRINK_SIZE 400

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


// CLaserDlg dialog




CLaserDlg::CLaserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLaserDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bIsSmall=false;
	m_IsVisible=false;
}

void CLaserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIBBY, m_Status);
}

BEGIN_MESSAGE_MAP(CLaserDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOVE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CLaserDlg message handlers

BOOL CLaserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	 m_bMouseTracking = FALSE;

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


	
	SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	

	GetWindowRect(&m_LargeRect);
	



	m_SmallRect=m_LargeRect;
	m_SmallRect.right-=SHRINK_SIZE;

	
	
	

	//SetWindowPos(&CWnd::wndTopMost,rect.top,rect.bottom,rect.Width()-400,rect.Height(),SWP_SHOWWINDOW );
	//SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	SetWindowSmall();



	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLaserDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLaserDlg::OnPaint()
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
HCURSOR CLaserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLaserDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (!m_bMouseTracking) {
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = this->m_hWnd;

		if (::_TrackMouseEvent(&tme)) {
			m_bMouseTracking = TRUE;
			if(m_bIsSmall)
			{
				SetWindowSmall(false);
			}

		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CLaserDlg::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default
	 m_bMouseTracking = FALSE;
	 SetWindowSmall(true);
	CDialogEx::OnMouseLeave();
}

void CLaserDlg::SetWindowSmall(bool set/*=true*/)
{
	if(set)
	{
		SetWindowPos(NULL,m_SmallRect.left,m_SmallRect.top,m_SmallRect.Width(),m_SmallRect.Height(),SWP_NOZORDER );
		m_bIsSmall=true;
	}else
	{
		SetWindowPos(NULL,m_LargeRect.left,m_LargeRect.top,m_LargeRect.Width(),m_LargeRect.Height(),SWP_NOZORDER );
		m_bIsSmall=false;
	}
}


void CLaserDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);
	

	if(m_IsVisible)
	{
		int width,height;
		width=m_LargeRect.Width();
		height=m_LargeRect.Height();
		m_LargeRect.top=lpwndpos->y;
		m_LargeRect.left=lpwndpos->x;
		m_LargeRect.bottom=m_LargeRect.top+height;
		m_LargeRect.right=m_LargeRect.left+width;

	}

	if(m_IsVisible)
	{
		int width,height;
		width=m_SmallRect.Width();
		height=m_SmallRect.Height();
		m_SmallRect.top=lpwndpos->y;
		m_SmallRect.left=lpwndpos->x;
		m_SmallRect.bottom=m_SmallRect.top+height;
		m_SmallRect.right=m_SmallRect.left+width;

	}

	int aa=1;
	// TODO: Add your message handler code here
}


void CLaserDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	m_IsVisible=true;
	// TODO: Add your message handler code here
}
