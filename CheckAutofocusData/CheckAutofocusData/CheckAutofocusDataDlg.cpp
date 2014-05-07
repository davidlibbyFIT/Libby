
// CheckAutofocusDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckAutofocusData.h"
#include "CheckAutofocusDataDlg.h"
#include "afxdialogex.h"

#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>    // std::sort



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


// CCheckAutofocusDataDlg dialog




CCheckAutofocusDataDlg::CCheckAutofocusDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckAutofocusDataDlg::IDD, pParent)
	, m_Time(0)
	, m_timeMaxGradient(0)
	, m_positionNear_MaxGrad_BM(0)
	, m_positionCenter_MaxGrad_BM(0)
	, m_positionFar_MaxGrad_BM(0)
	, m_positionNear_WeightedAve_BM(0)
	, m_positionCenter_WeightedAve_BM(0)
	, m_positionFar_WeightedAve_BM(0)
	, m_positionNear_CurveFit_BM(0)
	, m_positionCenter_CurveFit_BM(0)
	, m_positionFar_CurveFit_BM(0)
{
	m_timeMaxGradient = 0.0;
	m_Time_C          = 0.0;

	m_positionNear_MaxGrad_BM = 0.0;
	m_positionCenter_MaxGrad_BM = 0.0;
	m_positionFar_MaxGrad_BM = 0.0;

	m_positionNear_WeightedAve_BM = 0.0;
	m_positionCenter_WeightedAve_BM = 0.0;
	m_positionFar_WeightedAve_BM = 0.0;

	m_positionNear_CurveFit_BM = 0.0;
	m_positionCenter_CurveFit_BM = 0.0;
	m_positionFar_CurveFit_BM = 0.0;


	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCheckAutofocusDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_TIME, m_Time);

	DDX_Text(pDX, IDC_EDIT_MAX_GRADIENT, m_timeMaxGradient);
	DDX_Text(pDX, IDC_EDIT_METHOD_C, m_Time_C);

	DDX_Text(pDX, IDC_EDIT_NEAR_MAX_GRAD_BM, m_positionNear_MaxGrad_BM);
	DDX_Text(pDX, IDC_EDIT_CENTER_MAX_GRAD_BM, m_positionCenter_MaxGrad_BM);
	DDX_Text(pDX, IDC_EDIT_FAR_MAX_GRAD_BM, m_positionFar_MaxGrad_BM);

	DDX_Text(pDX, IDC_EDIT_NEAR_WA_BM, m_positionNear_WeightedAve_BM);
	DDX_Text(pDX, IDC_EDIT_CENTER_WA_BM, m_positionCenter_WeightedAve_BM);
	DDX_Text(pDX, IDC_EDIT_FAR_WA_BM, m_positionFar_WeightedAve_BM);

	DDX_Text(pDX, IDC_EDIT_NEAR_CF_BM, m_positionNear_CurveFit_BM);
	DDX_Text(pDX, IDC_EDIT_CENTER_CF_BM, m_positionCenter_CurveFit_BM);
	DDX_Text(pDX, IDC_EDIT_FAR_WA_CF, m_positionFar_CurveFit_BM);
}

BEGIN_MESSAGE_MAP(CCheckAutofocusDataDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_EN_CHANGE(IDC_EDIT_CENTER_WA_BM, &CCheckAutofocusDataDlg::OnEnChangeEditCenterWaBm)
END_MESSAGE_MAP()


// CCheckAutofocusDataDlg message handlers

bool comp (InterleafData lhs,InterleafData rhs)
{
	return (lhs.TimeStamp<rhs.TimeStamp);
}

BOOL CCheckAutofocusDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DragAcceptFiles(TRUE); // To Accept Dropped file Set this TRUE
	m_PointsVector.clear();

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



	InterleafDataVector idv;
	idv.clear();


	InterleafData test1;
	test1.TimeStamp		=1.9;
	test1.EdgeGradient	=1.8;
	test1.avetime		=1.7;
	test1.cvOverMean	=1.6;
	test1.aveEG			=1.5;
	test1.sdev			=1.4;

	InterleafData test2;
	test2.TimeStamp		=1.8;
	test2.EdgeGradient	=0.8;
	test2.avetime		=0.7;
	test2.cvOverMean	=0.6;
	test2.aveEG			=0.5;
	test2.sdev			=0.4;

	idv.push_back(test1);
	idv.push_back(test2);


	std::sort (idv.begin(),idv.end(),comp);  

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCheckAutofocusDataDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCheckAutofocusDataDlg::OnPaint()
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
HCURSOR CCheckAutofocusDataDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCheckAutofocusDataDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	BeginWaitCursor();


	// Get the number of files dropped 
	CString sFile; 
	DWORD   nBuffer = 0; 

	// Get the number of files dropped 
	int nFilesDropped = DragQueryFile (hDropInfo, 0xFFFFFFFF, NULL, 0); 
	int aa=1;

	for(int i=0; i<nFilesDropped; i++) 
	{ 
		// Get the buffer size of the file. 
		nBuffer = DragQueryFile (hDropInfo, i, NULL, 0); 

		// Get path and name of the file 
		DragQueryFile (hDropInfo, i, sFile.GetBuffer (nBuffer + 1), nBuffer + 1); 
		sFile.ReleaseBuffer (); 

		ExtractData(sFile);

		double focus_time;
		double DeltaTimeSinceFocus;
		OriginalMethod(&focus_time, &DeltaTimeSinceFocus);


		CFocusPosition ft;
		dataPointVector_t points;
		for (int i=0; i<(int)m_PointsVector.size(); i++)
		{
			dataPoint_t pt;
			pt.sharpness = m_PointsVector[i].EdgeGradient;
			pt.position = m_PointsVector[i].TimeStamp;
			points.push_back(pt);
		}
		ft.analyze(points);
		m_timeMaxGradient = ft.getCenterFocusPosition(AF_MAX_SHARPNESS);
		m_Time_C = ft.getCenterFocusPosition(AF_WEIGHTED_AVERAGE);

		m_positionNear_MaxGrad_BM = ft.getNearFocusPosition(AF_BM_MAX_SHARPNESS);
	    m_positionCenter_MaxGrad_BM = ft.getCenterFocusPosition(AF_BM_MAX_SHARPNESS);
	    m_positionFar_MaxGrad_BM = ft.getFarFocusPosition(AF_BM_MAX_SHARPNESS);

	    m_positionNear_WeightedAve_BM = ft.getNearFocusPosition(AF_BM_WEIGHTED_AVERAGE);
	    m_positionCenter_WeightedAve_BM = ft.getCenterFocusPosition(AF_BM_WEIGHTED_AVERAGE);
	    m_positionFar_WeightedAve_BM = ft.getFarFocusPosition(AF_BM_WEIGHTED_AVERAGE);

	    m_positionNear_CurveFit_BM = ft.getNearFocusPosition(AF_BM_CURVE_FIT);
	    m_positionCenter_CurveFit_BM = ft.getCenterFocusPosition(AF_BM_CURVE_FIT);
	    m_positionFar_CurveFit_BM = ft.getFarFocusPosition(AF_BM_CURVE_FIT);

		

		PointStructure FocusPoint;
		FocusPoint = FindFocusPoint(focus_time, FocusPoint);

		m_Time=FocusPoint.TimeStamp;

		UpdateData(false);
	} 

	// Free the memory block containing the dropped-file information 
	DragFinish(hDropInfo); 

	CDialogEx::OnDropFiles(hDropInfo);

    EndWaitCursor();
	SetFocus();
}

void CCheckAutofocusDataDlg::ExtractData( CString sFile )
{
	m_PointsVector.clear();
	BuildVectorData(sFile);
	//list.AddString(PathFindFileName(sFile));
}

void CCheckAutofocusDataDlg::BuildVectorData( CString sFile )
{
	std::ifstream infile(sFile);
	std::string SingleLine;
	bool FirstLine=true;
	int ssId=0;
	while (std::getline(infile, SingleLine))
	{
		if(!FirstLine)//Skip the Header Line.
		{
			std::stringstream ss(SingleLine);
			while( ss.good() )
			{
				PointStructure SinglePoint;
				std::string substr;
				
				getline( ss, substr, '\t' );
				std::istringstream ssTimeStamp(substr);

				getline( ss, substr, '\t' );
				std::istringstream ssEdgeGradient(substr);

				getline( ss, substr, '\t' );
				std::istringstream sscvOverMean(substr);

				getline( ss, substr, '\t' );
				std::istringstream ssaveEG(substr);

				getline( ss, substr, '\t' );
				std::istringstream sssdev(substr);


				ssTimeStamp >> SinglePoint.TimeStamp;
				ssEdgeGradient >> SinglePoint.EdgeGradient;
				
				sscvOverMean >> SinglePoint.cvOverMean;
				ssaveEG >> SinglePoint.aveEG;
				sssdev >> SinglePoint.sdev;


				m_PointsVector.push_back(SinglePoint);
				//result.push_back( substr );
			}
		}
		FirstLine = false;

	}
}

autofocusReturn_t CCheckAutofocusDataDlg::OriginalMethod(double *focus_time, double *DeltaTimeSinceFocus)
{
	int Nparticle = m_PointsVector.size();

	const int m_iSlidingWindowSize = 600;
	const long m_lMinNumberOfParticles = 500;



	if (Nparticle < m_iSlidingWindowSize) 
	{
		return(AF_INSUFFICIENT_POINTS);
	}

	if (Nparticle < m_lMinNumberOfParticles) 
	{
		return(AF_INSUFFICIENT_POINTS);
	}

	double *edge_gradient = NULL;
	edge_gradient = new double[Nparticle];
	double *timestamp = NULL;
	timestamp = new double[Nparticle];

	if (edge_gradient == NULL || timestamp == NULL) 
	{

		return(AF_INTERNAL_ALLOCATION);
	}


	double starttime = 0.0, endtime = 0.0;

	int i = 0;

	for (point_vect_itr itr = m_PointsVector.begin();itr<m_PointsVector.end();itr++  )
	{
		edge_gradient[i] = itr->EdgeGradient;
		timestamp[i] = itr->TimeStamp;

		if (i == 0) {
			starttime = timestamp[0];
		}
		timestamp[i] -= starttime;
		i++;
	}

	endtime = timestamp[Nparticle - 1];
	int Nave = Nparticle - m_iSlidingWindowSize + 1;

	//saveRawData(pList->getFileFactory()->getCurrentDir(), Nparticle, edge_gradient, timestamp);

	double *aveEG = NULL;
	aveEG = new double[Nave];
	double *avetime = NULL;
	avetime = new double[Nave];
	double *sdev = NULL;
	sdev = new double[Nave];
	double *cvOverMean = NULL;
	cvOverMean = new double[Nave];// Standard deviation/mean == cv - this is cv/mean

	if (aveEG == NULL || avetime == NULL || sdev == NULL || cvOverMean == NULL) 
	{
		return(AF_INTERNAL_ALLOCATION);
	}

	for (int j = 0; j < Nave; ++j) 
	{
		double sumEG = 0;
		double sumtime = 0;
		for (int k = 0; k < m_iSlidingWindowSize; ++k) 
		{
			sumEG += edge_gradient[j + k];
			sumtime += timestamp[j + k];
		}

		aveEG[j] = sumEG / m_iSlidingWindowSize;
		avetime[j] = sumtime / m_iSlidingWindowSize;

		double ssqEG = 0;
		for (int k = 0; k < m_iSlidingWindowSize; ++k) 
		{
			ssqEG += (edge_gradient[j + k] - aveEG[j]) * (edge_gradient[j + k] - aveEG[j]);
		}

		sdev[j] = sqrt(ssqEG / (m_iSlidingWindowSize - 1));
		cvOverMean[j] = sdev[j] / ( aveEG[j] * aveEG[j] );
	}


	//saveAveragedData(pList->getFileFactory()->getCurrentDir(), Nave, aveEG, avetime, sdev, cvOverMean);

	delete [] edge_gradient;
	delete[] timestamp;

	// begin to find time of focus point
	//Focuser *pFocus;
	//pFocus = focus::Focuser::getInstance();

	const long THRESHOLDMEANEDGEGRADIENT = 70;

	int FocusMinimumEdgeGradient = THRESHOLDMEANEDGEGRADIENT;// pFocus->GetThresholdMeanEdgeGradient();		// value to be kept in .ini file

	int min_index = 0;
	//20130813 new algorithm find minimum of the stdDev/(mean*mean)
	double min_cvOverMean = cvOverMean[0];
	for (int i =  0; i < Nave; ++i) 
	{
		if (cvOverMean[i] < min_cvOverMean) 
		{
			min_index = i;
			min_cvOverMean = cvOverMean[i];
		}
	}

	if(( 0 == min_index )||( (Nave - 1) == min_index ))//failure criteria never went below
	{
		return(AF_INTERNAL_ERROR);
	}


	*focus_time = avetime[min_index];

	*DeltaTimeSinceFocus = endtime - *focus_time;



	delete [] aveEG;
	delete [] avetime;
	delete [] sdev;
	delete [] cvOverMean;

	return(AF_OK);


}

PointStructure CCheckAutofocusDataDlg::FindFocusPoint( double focus_time, PointStructure FocusPoint )
{
	for (point_vect_itr itr = m_PointsVector.begin();itr<m_PointsVector.end();itr++  )
	{
		itr->EdgeGradient;
		if(focus_time < itr->TimeStamp && itr != m_PointsVector.begin())
		{
			itr--;
			FocusPoint=*itr;
			break;
		}

	}
	return FocusPoint;
}




void CCheckAutofocusDataDlg::OnEnChangeEditCenterWaBm()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
