
// EnumNetworkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EnumNetwork.h"
#include "EnumNetworkDlg.h"
#include "afxdialogex.h"
#include <algorithm>
#include <fstream>
#include <iostream>

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


// CEnumNetworkDlg dialog




CEnumNetworkDlg::CEnumNetworkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnumNetworkDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_NetworkThreadHandle=NULL;
	m_DriveSearchThreadHandle=NULL;
	m_UserCancel=false;

}

void CEnumNetworkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_STATIC_CURRENT, m_CurrentText);
	DDX_Control(pDX, IDC_LIST_SERVER_SHARES2, m_ListBoxShareList);
	DDX_Control(pDX, IDC_LIST_SERVER_SHARES_COMPLETE, m_ListBoxComplete);
	DDX_Control(pDX, IDC_BUTTON_LOAD_SHARES, m_ButtonStart);
}

BEGIN_MESSAGE_MAP(CEnumNetworkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_SHARES, &CEnumNetworkDlg::OnBnClickedButtonLoadShares)
	ON_BN_CLICKED(IDC_BUTTON_PROGRESS_CANCEL, &CEnumNetworkDlg::OnBnClickedButtonProgressCancel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CEnumNetworkDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CEnumNetworkDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEnumNetworkDlg message handlers

BOOL CEnumNetworkDlg::OnInitDialog()
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

void CEnumNetworkDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEnumNetworkDlg::OnPaint()
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
HCURSOR CEnumNetworkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




BOOL WINAPI CEnumNetworkDlg::EnumerateFunc(LPNETRESOURCE lpnr)
{
	DWORD dwResult, dwResultEnum;
	HANDLE hEnum;
	DWORD cbBuffer = 16384;     // 16K is a good size
	DWORD cEntries = -1;        // enumerate all possible entries
	LPNETRESOURCE lpnrLocal;    // pointer to enumerated structures
	DWORD i;
	//
	// Call the WNetOpenEnum function to begin the enumeration.
	//
	dwResult = WNetOpenEnum(RESOURCE_GLOBALNET, // all network resources
		RESOURCETYPE_DISK,   // all resources
		0,  // enumerate all resources
		lpnr,       // NULL first time the function is called
		&hEnum);    // handle to the resource

	if (dwResult != NO_ERROR) {
		printf("WnetOpenEnum failed with error %d\n", dwResult);
		return FALSE;
	}
	//
	// Call the GlobalAlloc function to allocate resources.
	//
	lpnrLocal = (LPNETRESOURCE) GlobalAlloc(GPTR, cbBuffer);
	if (lpnrLocal == NULL) {
		printf("WnetOpenEnum failed with error %d\n", dwResult);
		//      NetErrorHandler(hwnd, dwResult, (LPSTR)"WNetOpenEnum");
		return FALSE;
	}

	do {
		m_Progress.StepIt();
		if(DidUserCancel())
		{
			break;
		}



		//
		// Initialize the buffer.
		//
		ZeroMemory(lpnrLocal, cbBuffer);
		//
		// Call the WNetEnumResource function to continue
		//  the enumeration.
		//
		dwResultEnum = WNetEnumResource(hEnum,  // resource handle
			&cEntries,      // defined locally as -1
			lpnrLocal,      // LPNETRESOURCE
			&cbBuffer);     // buffer size
		//
		// If the call succeeds, loop through the structures.
		//
		if (dwResultEnum == NO_ERROR) {
			for (i = 0; i < cEntries; i++) {
				// Call an application-defined function to
				//  display the contents of the NETRESOURCE structures.
				//
				//DisplayStruct(i, &lpnrLocal[i]);

				if(DidUserCancel())
				{
					break;
				}


				
				
				std::string fullpath=CT2A(lpnrLocal[i].lpRemoteName);

				if(lpnrLocal[i].dwDisplayType==RESOURCEDISPLAYTYPE_SHARE)
				{
					
					
					AddShare(fullpath);
				}

				//also check for Hidden Shares.
				FindHiddenSharesOnServer(fullpath);
				

				// If the NETRESOURCE structure represents a container resource, 
				//  call the EnumerateFunc function recursively.

				if (RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage
					& RESOURCEUSAGE_CONTAINER))
					//          if(!EnumerateFunc(hwnd, hdc, &lpnrLocal[i]))
					if (!EnumerateFunc(&lpnrLocal[i]))
						printf("EnumerateFunc returned FALSE\n");
				//            TextOut(hdc, 10, 10, "EnumerateFunc returned FALSE.", 29);
			}
		}
		// Process errors.
		//
		else if (dwResultEnum != ERROR_NO_MORE_ITEMS) {
			printf("WNetEnumResource failed with error %d\n", dwResultEnum);

			//      NetErrorHandler(hwnd, dwResultEnum, (LPSTR)"WNetEnumResource");
			break;
		}
	}
	//
	// End do.
	//
	while (dwResultEnum != ERROR_NO_MORE_ITEMS);
	//
	// Call the GlobalFree function to free the memory.
	//
	GlobalFree((HGLOBAL) lpnrLocal);
	//
	// Call WNetCloseEnum to end the enumeration.
	//
	dwResult = WNetCloseEnum(hEnum);

	if (dwResult != NO_ERROR) {
		//
		// Process errors.
		//
		printf("WNetCloseEnum failed with error %d\n", dwResult);
		//    NetErrorHandler(hwnd, dwResult, (LPSTR)"WNetCloseEnum");
		return FALSE;
	}

	return TRUE;
}

void CEnumNetworkDlg::DisplayStruct(int i, LPNETRESOURCE lpnrLocal)
{
	printf("NETRESOURCE[%d] Scope: ", i);
	switch (lpnrLocal->dwScope) {
	case (RESOURCE_CONNECTED):
		printf("connected\n");
		break;
	case (RESOURCE_GLOBALNET):
		printf("all resources\n");
		break;
	case (RESOURCE_REMEMBERED):
		printf("remembered\n");
		break;
	default:
		printf("unknown scope %d\n", lpnrLocal->dwScope);
		break;
	}

	printf("NETRESOURCE[%d] Type: ", i);
	switch (lpnrLocal->dwType) {
	case (RESOURCETYPE_ANY):
		printf("any\n");
		break;
	case (RESOURCETYPE_DISK):
		printf("disk\n");
		break;
	case (RESOURCETYPE_PRINT):
		printf("print\n");
		break;
	default:
		printf("unknown type %d\n", lpnrLocal->dwType);
		break;
	}

	printf("NETRESOURCE[%d] DisplayType: ", i);
	switch (lpnrLocal->dwDisplayType) {
	case (RESOURCEDISPLAYTYPE_GENERIC):
		printf("generic\n");
		break;
	case (RESOURCEDISPLAYTYPE_DOMAIN):
		printf("domain\n");
		break;
	case (RESOURCEDISPLAYTYPE_SERVER):
		printf("server\n");
		break;
	case (RESOURCEDISPLAYTYPE_SHARE):
		printf("share\n");
		break;
	case (RESOURCEDISPLAYTYPE_FILE):
		printf("file\n");
		break;
	case (RESOURCEDISPLAYTYPE_GROUP):
		printf("group\n");
		break;
	case (RESOURCEDISPLAYTYPE_NETWORK):
		printf("network\n");
		break;
	default:
		printf("unknown display type %d\n", lpnrLocal->dwDisplayType);
		break;
	}

	printf("NETRESOURCE[%d] Usage: 0x%x = ", i, lpnrLocal->dwUsage);
	if (lpnrLocal->dwUsage & RESOURCEUSAGE_CONNECTABLE)
		printf("connectable ");
	if (lpnrLocal->dwUsage & RESOURCEUSAGE_CONTAINER)
		printf("container ");
	printf("\n");

	printf("NETRESOURCE[%d] Localname: %S\n", i, lpnrLocal->lpLocalName);
	printf("NETRESOURCE[%d] Remotename: %S\n", i, lpnrLocal->lpRemoteName);
	printf("NETRESOURCE[%d] Comment: %S\n", i, lpnrLocal->lpComment);
	printf("NETRESOURCE[%d] Provider: %S\n", i, lpnrLocal->lpProvider);
	printf("\n");
}




void CEnumNetworkDlg::FindHiddenSharesOnServer( std::string ServerName)
{
	// TODO: Add your control notification handler code here

	PSHARE_INFO_502 BufPtr,p;

	NET_API_STATUS res;
	DWORD er=0,tr=0,resume=0, i;

	while( ServerName[0] == '\\')
	{	
		ServerName.erase(0,1);
	}
	//
	// Print a report header.
	//
	printf("Share:              Local Path:                   Uses:   Descriptor:\n");
	printf("---------------------------------------------------------------------\n");
	//
	// Call the NetShareEnum function; specify level 502.
	//
	do // begin do
	{
		m_Progress.StepIt();

		if(DidUserCancel())
		{
			break;
		}


		BSTR b = _com_util::ConvertStringToBSTR(ServerName.c_str());
		LPWSTR lp = b;

		res = NetShareEnum (b, 502, (LPBYTE *) &BufPtr, MAX_PREFERRED_LENGTH, &er, &tr, &resume);
		
		SysFreeString(b);

		//
		// If the call succeeds,
		//
		if(res == ERROR_SUCCESS || res == ERROR_MORE_DATA)
		{
			p=BufPtr;
			//
			// Loop through the entries;
			//  print retrieved data.
			//
			for(i=1;i<=er;i++)
			{
				printf("%-20S%-30S%-8u",p->shi502_netname, p->shi502_path, p->shi502_current_uses);
				std::string computer= ServerName;
				std::string share=CT2A(p->shi502_netname);
				computer = "\\\\" + computer + "\\";
				std::string fullpath=computer+share;
				
				AddShare(fullpath);

				//
				// Validate the value of the 
				//  shi502_security_descriptor member.
				//
				if (IsValidSecurityDescriptor(p->shi502_security_descriptor))
					printf("Yes\n");
				else
					printf("No\n");
				p++;
			}
			//
			// Free the allocated buffer.
			//
			NetApiBufferFree(BufPtr);
		}
		else 
			printf("Error: %ld\n",res);
	}
	// Continue to call NetShareEnum while 
	//  there are more entries. 
	// 
	while (res==ERROR_MORE_DATA); // end do
	return;
}


void CEnumNetworkDlg::AddShare( std::string ShareToAdd )
{
	
	if(!IsOkDir(ShareToAdd))
		return;

	if(m_ShareListSet.find(ShareToAdd)==m_ShareListSet.end())
	{
		m_ShareList.push(ShareToAdd);
		m_ShareListSet.insert(ShareToAdd);

		BSTR b = _com_util::ConvertStringToBSTR(ShareToAdd.c_str());
		LPWSTR lp = b;

		m_ListBoxShareList.AddString(lp);

		SysFreeString(b);



		std::ofstream file;
		file.open("DriveShares.txt", std::fstream::out | std::fstream::app);
		if(file.is_open())
		{
			file << ShareToAdd;
			file << "\n";
			file.close();
		}			

	}


}

void CEnumNetworkDlg::OnLbnSelchangeListServerShares()
{
	// TODO: Add your control notification handler code here
}




unsigned _stdcall CEnumNetworkDlg::FindSharesThreadProc(void *data)
{
	unsigned int result = 0;
	//int x =*(int*)data;
	CEnumNetworkDlg *pCEnumNetworkDlg=(CEnumNetworkDlg*)data;

	LPNETRESOURCE lpnr = NULL;
	if (pCEnumNetworkDlg->EnumerateFunc(lpnr) == FALSE) {
		printf("Call to EnumerateFunc failed\n");
	}
	pCEnumNetworkDlg->CloseNetworkThread();
	

	return result ;
}

unsigned _stdcall CEnumNetworkDlg::DriveSearchThreadProc(void *data)
{
	unsigned int result = 0;
	CEnumNetworkDlg *pCEnumNetworkDlg=(CEnumNetworkDlg*)data;

	int count=pCEnumNetworkDlg->ShareListCount();

	while (pCEnumNetworkDlg->IsNetworkThreadAlive() || count>0)
	{
		if(count>0)
		{
			std::string NextDir = pCEnumNetworkDlg->GetNextItem();
			BSTR b = _com_util::ConvertStringToBSTR(NextDir.c_str());
			LPWSTR lp = b;
			pCEnumNetworkDlg->SetCurrentSearchDir(lp);
			pCEnumNetworkDlg->ListDirectoryContents(lp);
			SysFreeString(b);
			pCEnumNetworkDlg->AddCompleteDir(lp);
		}
		count=pCEnumNetworkDlg->ShareListCount();
		if(pCEnumNetworkDlg->DidUserCancel())
		{
			
			pCEnumNetworkDlg->CloseDriveSearchThread();
			return 0;
		}

		

	}
	


	pCEnumNetworkDlg->CloseDriveSearchThread();


	return result ;
}

void CEnumNetworkDlg::SetCurrentSearchDir(LPWSTR dirname)
{
	m_CurrentText.SetWindowTextW(dirname);
}

void CEnumNetworkDlg::AddCompleteDir(LPWSTR dirname)
{
	m_ListBoxComplete.AddString(dirname);
}

std::string CEnumNetworkDlg::GetNextItem()
{
	std::string NextDir = m_ShareList.front();
	m_ShareList.pop();
	return NextDir;
	
}


void CEnumNetworkDlg::OnBnClickedButtonLoadShares()
{

	m_CurrentText.SetWindowTextW(L"N/A");

	ClearShareListQueue();
	m_NetworkThreadHandle =  (HANDLE)_beginthreadex(NULL, 0,CEnumNetworkDlg::FindSharesThreadProc, this, 0, NULL);
	m_DriveSearchThreadHandle =  (HANDLE)_beginthreadex(NULL, 0,CEnumNetworkDlg::DriveSearchThreadProc, this, 0, NULL);
	m_UserCancel=false;
	m_ButtonStart.EnableWindow(FALSE);
	

}

int CEnumNetworkDlg::ShareListCount()
{
	return m_ShareList.size();
}



void CEnumNetworkDlg::UpdateAll()
{
	m_Progress.StepIt();
	Invalidate();
	UpdateWindow();
}

void CEnumNetworkDlg::CloseThread( HANDLE &hThreadToClose )
{
	if(hThreadToClose)
	{
		CloseHandle(hThreadToClose);
		hThreadToClose = NULL;
	}
}

bool CEnumNetworkDlg::IsNetworkThreadAlive()
{
	return (m_NetworkThreadHandle!=NULL);
}


void CEnumNetworkDlg::CloseNetworkThread()
{
	CloseThread(m_NetworkThreadHandle);
	if(m_DriveSearchThreadHandle==NULL)
	{
		m_ButtonStart.EnableWindow(TRUE);
	}
}

void CEnumNetworkDlg::CloseDriveSearchThread()
{
	CloseThread(m_DriveSearchThreadHandle);
	if(m_NetworkThreadHandle==NULL)
	{
		m_ButtonStart.EnableWindow(TRUE);
	}

}

bool CEnumNetworkDlg::ListDirectoryContents(const wchar_t *sDir)
{
	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;
	m_Progress.StepIt();
	wchar_t  sPath[2048];

	//Specify a file mask. *.* = We want everything!
	wsprintf (sPath, L"%s\\*.*", sDir);

	if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
	{
		printf("Path not found: [%s]\n", sDir);
		return false;
	}

	do
	{
		if(DidUserCancel())
		{
			break;
		}
		//Find first file will always return "."
		//    and ".." as the first two directories.
		if(wcscmp(fdFile.cFileName, L".") != 0
			&& wcscmp(fdFile.cFileName, L"..") != 0)
		{
			//Build up our file path using the passed in
			//  [sDir] and the file/foldername we just found:
			wsprintf(sPath, L"%s\\%s", sDir, fdFile.cFileName);

			//Is the entity a File or Folder?
			if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				printf("Directory: %s\n", sPath);
				ListDirectoryContents(sPath); //Recursion, I love it!
			}
			else{
				printf("File: %s\n", sPath);
				
				std::ofstream file;
				file.open("Files.txt", std::fstream::out | std::fstream::app);
				if(file.is_open())
				{

					std::string TempPath ;
					TempPath = CW2A(sPath);
					
										
					file << TempPath;
					file << "\n";
					file.close();
				}			


			}
		}
	}
	while(FindNextFile(hFind, &fdFile)); //Find the next file.

	FindClose(hFind); //Always, Always, clean things up!

	return true;
}


void CEnumNetworkDlg::ClearShareListQueue()
{
	if(!m_ShareList.empty())
	{
		while(!m_ShareList.empty()) m_ShareList.pop();
	}

	if(m_ListBoxShareList.GetCount() >0 )
	{
		m_ListBoxShareList.ResetContent();
	}

	{
		std::ofstream file;
		file.open("Files.txt", std::fstream::out | std::fstream::trunc);
		if(file.is_open())
		{
			file.close();
		}			

	}
	{	
		std::ofstream file;
		file.open("DriveShares.txt", std::fstream::out | std::fstream::trunc);
		if(file.is_open())
		{
			file.close();
		}			

	}
}
void CEnumNetworkDlg::OnBnClickedButtonProgressCancel()
{
	// TODO: Add your control notification handler code here

	CloseNetworkThread();
	CloseDriveSearchThread();
	//ListDirectoryContents(L"\\\\JENKINS\\C$");
	m_UserCancel = true;


}



void CEnumNetworkDlg::OnLbnSelchangeListServerSharesComplete()
{
	// TODO: Add your control notification handler code here
}


void CEnumNetworkDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	m_UserCancel = true;
	OnBnClickedButtonProgressCancel();

	CDialogEx::OnCancel();
}


void CEnumNetworkDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	m_UserCancel = true;
	OnBnClickedButtonProgressCancel();

	CDialogEx::OnOK();
}

bool CEnumNetworkDlg::IsOkDir( std::string &fullpath )
{
	//List of dir's we want to avoid.
	// \\FIT\ADMIN$
	// \\FIT\IPC$
	// \\DC01\NETLOGON
	// \\DC01\SYSVOL


	//Check for Dir's we don't want.
	int stringpos =	fullpath.find("IPC$");
	if(stringpos<0)
		stringpos =	fullpath.find("ADMIN$");
	if(stringpos<0)
		stringpos =	fullpath.find("NETLOGON");
	if(stringpos<0)
		stringpos =	fullpath.find("SYSVOL");

	return (stringpos==-1);
}
