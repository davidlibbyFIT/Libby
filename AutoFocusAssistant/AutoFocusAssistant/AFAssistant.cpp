// AFAssistant1.cpp : implementation file
//

#include "stdafx.h"
#include "AutoFocusAssistant.h"
#include "AFAssistant.h"
#include "afxdialogex.h"



// AFAssistant1 dialog

IMPLEMENT_DYNAMIC(AFAssistant1, CPropertyPage)

	AFAssistant1::AFAssistant1(stAFAssistData &inDataStructure)
	: CPropertyPage(AFAssistant1::IDD1),
	m_pFont (NULL),
	m_stAFDataStructure (inDataStructure) 

{
	m_pFont = new CFont();
	m_pFont->CreatePointFont(165, _T("Arial"));

}

AFAssistant1::~AFAssistant1()
{

	if(m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
}


void AFAssistant1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_AF_1_HEADER_TEXT, m_AF1Header);
}


// AFAssistant1 message handlers
BEGIN_MESSAGE_MAP(AFAssistant1, CPropertyPage)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


void AFAssistant1::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		m_AF1Header.SetFont(m_pFont, TRUE);
	}

}
BOOL AFAssistant1::OnSetActive()
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   
	psheet->SetWizardButtons( PSWIZB_NEXT);	

	CWnd *buttonBack = GetParent()->GetDlgItem(ID_WIZBACK);	
	buttonBack->ShowWindow(SW_HIDE);	


	return CPropertyPage::OnSetActive();
}



// AFAssistant2 dialog
//=============== AFAssistant2 ==============================

IMPLEMENT_DYNAMIC(AFAssistant2, CPropertyPage)

	AFAssistant2::AFAssistant2(stAFAssistData &inDataStructure)
	: CPropertyPage(AFAssistant2::IDD2),
	m_stAFDataStructure (inDataStructure),
	m_FontInit (false)
{



}

AFAssistant2::~AFAssistant2()
{

}


void AFAssistant2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_AF_2_HEADER_TEXT, m_AF2Header);
	DDX_Control(pDX, IDC_STATIC_AF_2_HEADER_SYRINGE, m_AF2HeaderSyringe);
	DDX_Control(pDX, IDC_STATIC_AF_2_HEADER_FLOW, m_AF2HeaderFlow);
	DDX_Control(pDX, IDC_STATIC_AF_HEADER_OBJECTIVE, m_AF2HeaderObjective);
	DDX_Control(pDX, IDC_STATIC_AF_2_BODY_SYRINGE, m_AF2BodySyringe);
	DDX_Control(pDX, IDC_STATIC_AF_2_BODY_FLOW, m_AF2BodyFlow);
	DDX_Control(pDX, IDC_STATIC_AF_BODY_OBJECTIVE, m_AF2BodyObjective);
	DDX_Control(pDX, IDC_RADIO_AF_YES, m_RadioYes);
	DDX_Control(pDX, IDC_RADIO_AF_NO, m_RadioNo);

}


// AFAssistant2 message handlers
BEGIN_MESSAGE_MAP(AFAssistant2, CPropertyPage)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RADIO_AF_YES, &AFAssistant2::OnBnClickedRadioAFYes)
	ON_BN_CLICKED(IDC_RADIO_AF_NO, &AFAssistant2::OnBnClickedRadioAFNo)

END_MESSAGE_MAP()


void AFAssistant2::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	if(bShow && !m_FontInit )
	{
		m_FontInit = true;
		CFont * pfont = m_AF2Header.GetFont();

		LOGFONT lf; 
		pfont->GetLogFont(&lf);

		lf.lfHeight = 24;                      // request a 12-pixel-height font
		_tcsncpy_s(lf.lfFaceName, LF_FACESIZE,  _T("Arial"), 7);                    

		m_Font.CreateFontIndirect(&lf);

		lf.lfWeight = FW_HEAVY;
		lf.lfHeight = 24;                      // request a 12-pixel-height font

		m_BoldFont.CreateFontIndirect(&lf);

		m_AF2Header.SetFont(&m_Font, TRUE);
		m_AF2HeaderSyringe.SetFont(&m_Font, TRUE);
		m_AF2HeaderFlow.SetFont(&m_Font, TRUE);
		m_AF2HeaderObjective.SetFont(&m_Font, TRUE);

		m_RadioYes.SetFont(&m_Font, TRUE);
		m_RadioNo.SetFont(&m_Font, TRUE);

		m_AF2BodySyringe.SetFont(&m_BoldFont, TRUE);
		m_AF2BodyFlow.SetFont(&m_BoldFont, TRUE);
		m_AF2BodyObjective.SetFont(&m_BoldFont, TRUE);


		TCHAR Syringe[20];
		Syringe[0]='\0';


		_stprintf_s(Syringe, _countof(Syringe),_T( "%.1f ml"),m_stAFDataStructure.SyringeSizeml);					

		m_AF2BodyObjective.SetWindowText(m_stAFDataStructure.szObjective);
		m_AF2BodyFlow.SetWindowText(m_stAFDataStructure.szFlowCel);
		m_AF2BodySyringe.SetWindowText(Syringe);

	}




}

BOOL AFAssistant2::OnSetActive()
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);	
	CWnd *buttonBack = GetParent()->GetDlgItem(ID_WIZBACK);	
	buttonBack->ShowWindow(SW_SHOW);	

	CWnd *buttonNext = GetParent()->GetDlgItem(ID_WIZNEXT);	

	if(m_stAFDataStructure.ObjFlowSizeOk)
	{
		m_RadioYes.SetCheck(true);
		m_RadioNo.SetCheck(false);
		psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT );
		GetDlgItem(IDC_STATIC_AF_2_FOOTER_TEXT)->ShowWindow(SW_HIDE);


	}else
	{

		m_RadioYes.SetCheck(false);
		m_RadioNo.SetCheck(true);
		psheet->SetWizardButtons(PSWIZB_BACK );
		GetDlgItem(IDC_STATIC_AF_2_FOOTER_TEXT)->ShowWindow(SW_SHOW);

	}


	return CPropertyPage::OnSetActive();
}
void AFAssistant2::OnBnClickedRadioAFYes()
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT );
	m_stAFDataStructure.ObjFlowSizeOk=true;
	GetDlgItem(IDC_STATIC_AF_2_FOOTER_TEXT)->ShowWindow(SW_HIDE);


}

void AFAssistant2::OnBnClickedRadioAFNo()
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   
	psheet->SetWizardButtons(PSWIZB_BACK );
	m_stAFDataStructure.ObjFlowSizeOk=false;
	GetDlgItem(IDC_STATIC_AF_2_FOOTER_TEXT)->ShowWindow(SW_SHOW);
}
//=============== AFAssistant2 ==============================


//=============== AFAssistant3 ==============================

IMPLEMENT_DYNAMIC(AFAssistant3, CPropertyPage)

	AFAssistant3::AFAssistant3(stAFAssistData &inDataStructure)
	: CPropertyPage(AFAssistant3::IDD3),
	m_stAFDataStructure (inDataStructure),
	m_FontInit (false)
{

	

}

AFAssistant3::~AFAssistant3()
{

}


void AFAssistant3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_AF_3_HEADER_TEXT, m_AF3Header);
	DDX_Control(pDX, IDC_STATIC_AF_3_HEADER_SYRINGE, m_AF3HeaderSyringe);
	DDX_Control(pDX, IDC_STATIC_AF_3_HEADER_VOLUME, m_AF3HeaderVolume);
	DDX_Control(pDX, IDC_STATIC_AF_3_BODY_SYRINGE, m_AF3BodySyringe);
	DDX_Control(pDX, IDC_STATIC_AF_3_BODY_VOLUME, m_AF3BodyVolume);
	DDX_Control(pDX, IDC_RADIO_AF_YES, m_RadioYes);
	DDX_Control(pDX, IDC_RADIO_AF_NO, m_RadioNo);

}


// AFAssistant3 message handlers
BEGIN_MESSAGE_MAP(AFAssistant3, CPropertyPage)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RADIO_AF_YES, &AFAssistant3::OnBnClickedRadioAFYes)
	ON_BN_CLICKED(IDC_RADIO_AF_NO, &AFAssistant3::OnBnClickedRadioAFNo)

END_MESSAGE_MAP()


void AFAssistant3::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);



	if(bShow && !m_FontInit)
	{

		m_FontInit = true;
		CFont * pfont = m_AF3Header.GetFont();

		LOGFONT lf; 
		pfont->GetLogFont(&lf);

		lf.lfHeight = 24;                      // request a 12-pixel-height font
		_tcsncpy_s(lf.lfFaceName, LF_FACESIZE,  _T("Arial"), 7);                    

		m_Font.CreateFontIndirect(&lf);

		lf.lfWeight = FW_HEAVY;
		lf.lfHeight = 24;                      // request a 12-pixel-height font

		m_BoldFont.CreateFontIndirect(&lf);

		m_AF3Header.SetFont(&m_Font, TRUE);
		m_AF3HeaderSyringe.SetFont(&m_Font, TRUE);
		m_AF3HeaderVolume.SetFont(&m_Font, TRUE);

		m_RadioYes.SetFont(&m_Font, TRUE);
		m_RadioNo.SetFont(&m_Font, TRUE);

		m_AF3BodySyringe.SetFont(&m_BoldFont, TRUE);
		m_AF3BodyVolume.SetFont(&m_BoldFont, TRUE);


		TCHAR Syringe[20];
		Syringe[0]='\0';
		_stprintf_s(Syringe, _countof(Syringe),_T( "%.1f ml"),m_stAFDataStructure.SyringeSizeml);					

		TCHAR Volume[20];
		Volume[0]='\0';
		_stprintf_s(Volume, _countof(Syringe),_T( "%.1f ml"),m_stAFDataStructure.RequiredVolume);					


		m_AF3BodyVolume.SetWindowText(Volume);
		m_AF3BodySyringe.SetWindowText(Syringe);
	}



}

BOOL AFAssistant3::OnSetActive()
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);	
	CWnd *buttonBack = GetParent()->GetDlgItem(ID_WIZBACK);	
	buttonBack->ShowWindow(SW_SHOW);	

	CWnd *buttonNext = GetParent()->GetDlgItem(ID_WIZNEXT);	

	if(m_stAFDataStructure.ObjFlowVolumeOk)
	{
		m_RadioYes.SetCheck(true);
		m_RadioNo.SetCheck(false);
		psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT );
		GetDlgItem(IDC_STATIC_AF_3_FOOTER_TEXT)->ShowWindow(SW_HIDE);


	}else
	{
		m_RadioYes.SetCheck(false);
		m_RadioNo.SetCheck(true);
		psheet->SetWizardButtons(PSWIZB_BACK );
		GetDlgItem(IDC_STATIC_AF_3_FOOTER_TEXT)->ShowWindow(SW_SHOW);


	}


	return CPropertyPage::OnSetActive();
}
void AFAssistant3::OnBnClickedRadioAFYes()
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT );
	m_stAFDataStructure.ObjFlowVolumeOk=true;
	GetDlgItem(IDC_STATIC_AF_3_FOOTER_TEXT)->ShowWindow(SW_HIDE);


}

void AFAssistant3::OnBnClickedRadioAFNo()
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   
	psheet->SetWizardButtons(PSWIZB_BACK );
	m_stAFDataStructure.ObjFlowVolumeOk=false;
	GetDlgItem(IDC_STATIC_AF_3_FOOTER_TEXT)->ShowWindow(SW_SHOW);
}


// AFAssistant4 dialog

IMPLEMENT_DYNAMIC(AFAssistant4, CPropertyPage)

	AFAssistant4::AFAssistant4(stAFAssistData &inDataStructure)
	: CPropertyPage(AFAssistant4::IDD4),
	m_pFont (NULL),
	m_stAFDataStructure (inDataStructure) 

{
	m_pFont = new CFont();
	m_pFont->CreatePointFont(165, _T("Arial"));

}

AFAssistant4::~AFAssistant4()
{

	if(m_pFont)
		delete m_pFont;
}


void AFAssistant4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_AF_4_HEADER_TEXT, m_AF4Header);
}


// AFAssistant4 message handlers
BEGIN_MESSAGE_MAP(AFAssistant4, CPropertyPage)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


void AFAssistant4::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		m_AF4Header.SetFont(m_pFont, TRUE);
	}

}
BOOL AFAssistant4::OnSetActive()
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   

	return CPropertyPage::OnSetActive();
}

// AFAssistant5 dialog

IMPLEMENT_DYNAMIC(AFAssistant5, CPropertyPage)

	AFAssistant5::AFAssistant5(stAFAssistData &inDataStructure)
	: CPropertyPage(AFAssistant5::IDD5),
	m_pFont (NULL),
	m_stAFDataStructure (inDataStructure) 

{
	m_pFont = new CFont();
	m_pFont->CreatePointFont(165, _T("Arial"));

}

AFAssistant5::~AFAssistant5()
{

	if(m_pFont)
		delete m_pFont;
}


void AFAssistant5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_AF_5_HEADER_TEXT, m_AF5Header);
}


// AFAssistant5 message handlers
BEGIN_MESSAGE_MAP(AFAssistant5, CPropertyPage)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


void AFAssistant5::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		m_AF5Header.SetFont(m_pFont, TRUE);
	}

}
BOOL AFAssistant5::OnSetActive()
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   

	return CPropertyPage::OnSetActive();
}


// AFAssistant6 dialog

IMPLEMENT_DYNAMIC(AFAssistant6, CPropertyPage)

	AFAssistant6::AFAssistant6(stAFAssistData &inDataStructure)
	: CPropertyPage(AFAssistant6::IDD6),
	m_pFont (NULL),
	m_stAFDataStructure (inDataStructure) 

{
	m_pFont = new CFont();
	m_pFont->CreatePointFont(165, _T("Arial"));

}

AFAssistant6::~AFAssistant6()
{

	if(m_pFont)
		delete m_pFont;
}


void AFAssistant6::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_AF_6_HEADER_TEXT, m_AF6Header);
}


// AFAssistant6 message handlers
BEGIN_MESSAGE_MAP(AFAssistant6, CPropertyPage)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


void AFAssistant6::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		m_AF6Header.SetFont(m_pFont, TRUE);

		//If we are on step 5 then the user has added the breads.
		m_stAFDataStructure.ObjSolutionOk=true;
	}

}
BOOL AFAssistant6::OnSetActive()
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}



// AFAssistant7 message handlers


IMPLEMENT_DYNAMIC(AFAssistant7, CPropertyPage)

	AFAssistant7::AFAssistant7(stAFAssistData &inDataStructure)
	: CPropertyPage(AFAssistant7::IDD7),
	m_stAFDataStructure (inDataStructure),
	m_FontInit (false)
{



}

AFAssistant7::~AFAssistant7()
{

}

void AFAssistant7::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_AF_7_HEADER_TEXT, m_AF7Header);
	DDX_Control(pDX, IDC_STATIC_AF_7_HEADER_SYRINGE, m_AF7HeaderSyringe);
	DDX_Control(pDX, IDC_STATIC_AF_7_HEADER_FLOW, m_AF7HeaderFlow);
	DDX_Control(pDX, IDC_STATIC_AF_7_HEADER_OBJECTIVE, m_AF7HeaderObjective);
	DDX_Control(pDX, IDC_STATIC_AF_7_HEADER_SYRINGE_VOLUME, m_AF7HeaderSyringeVolume);
	DDX_Control(pDX, IDC_STATIC_AF_7_HEADER_SOLUTION_ADDED, m_AF7HeaderSolution);
	DDX_Control(pDX, IDC_STATIC_AF_7_HEADER_COURSE_FOCUS, m_AF7HeaderCourseFocus);

	DDX_Control(pDX, IDC_STATIC_AF_7_BODY_SYRINGE, m_AF7BodySyringe);
	DDX_Control(pDX, IDC_STATIC_AF_7_BODY_FLOW, m_AF7BodyFlow);
	DDX_Control(pDX, IDC_STATIC_AF_7_BODY_OBJECTIVE, m_AF7BodyObjective);
	DDX_Control(pDX, IDC_STATIC_AF_7_BODY_SYRINGE_VOLUME, m_AF7BodySyringeVolume);
	DDX_Control(pDX, IDC_STATIC_AF_7_BODY_SOLUTION_ADDED, m_AF7BodySolution);
	DDX_Control(pDX, IDC_STATIC_AF_7_BODY_COURSE_FOCUS, m_AF7BodyCourseFocus);
	
	DDX_Control(pDX, IDC_RADIO_AF_YES, m_RadioYes);
	DDX_Control(pDX, IDC_RADIO_AF_NO, m_RadioNo);

}


BEGIN_MESSAGE_MAP(AFAssistant7, CPropertyPage)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RADIO_AF_YES, &AFAssistant7::OnBnClickedRadioAFYes)
	ON_BN_CLICKED(IDC_RADIO_AF_NO, &AFAssistant7::OnBnClickedRadioAFNo)

END_MESSAGE_MAP()


void AFAssistant7::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	if(bShow && !m_FontInit )
	{

		//If we are on step 7 then the user has gone they the focus steps.
		m_stAFDataStructure.ObjFocusOk=true;

		m_FontInit = true;
		CFont * pfont = m_AF7Header.GetFont();

		LOGFONT lf; 
		pfont->GetLogFont(&lf);

		lf.lfHeight = 21;                      // request a 12-pixel-height font
		_tcsncpy_s(lf.lfFaceName, LF_FACESIZE,  _T("Arial"), 7);                    

		m_Font.CreateFontIndirect(&lf);

		lf.lfWeight = FW_HEAVY;
		lf.lfHeight = 21;                      // request a 12-pixel-height font

		m_BoldFont.CreateFontIndirect(&lf);

		m_AF7Header.SetFont(&m_Font, TRUE);
		m_AF7HeaderSyringe.SetFont(&m_Font, TRUE);
		m_AF7HeaderFlow.SetFont(&m_Font, TRUE);
		m_AF7HeaderObjective.SetFont(&m_Font, TRUE);
		m_AF7HeaderSyringeVolume.SetFont(&m_Font, TRUE);;
		m_AF7HeaderSolution.SetFont(&m_Font, TRUE);;
		m_AF7HeaderCourseFocus.SetFont(&m_Font, TRUE);;


		m_RadioYes.SetFont(&m_Font, TRUE);
		m_RadioNo.SetFont(&m_Font, TRUE);

		m_AF7BodySyringe.SetFont(&m_BoldFont, TRUE);
		m_AF7BodyFlow.SetFont(&m_BoldFont, TRUE);
		m_AF7BodyObjective.SetFont(&m_BoldFont, TRUE);
		m_AF7BodySyringeVolume.SetFont(&m_BoldFont, TRUE);
		m_AF7BodySolution.SetFont(&m_BoldFont, TRUE);
		m_AF7BodyCourseFocus.SetFont(&m_BoldFont, TRUE);

		TCHAR Syringe[20];
		Syringe[0]='\0';
		_stprintf_s(Syringe, _countof(Syringe),_T( "%.1f ml"),m_stAFDataStructure.SyringeSizeml);					

		TCHAR Volume[20];
		Volume[0]='\0';
		_stprintf_s(Volume, _countof(Syringe),_T( "%.1f ml"),m_stAFDataStructure.RequiredVolume);					


		m_AF7BodyObjective.SetWindowText(m_stAFDataStructure.szObjective);
		m_AF7BodyFlow.SetWindowText(m_stAFDataStructure.szFlowCel);
		m_AF7BodySyringe.SetWindowText(Syringe);
		m_AF7BodySyringeVolume.SetWindowText(Volume);
		m_AF7BodySolution.SetWindowText(m_stAFDataStructure.ObjSolutionOk ? _T("Yes") : _T("No"));
		m_AF7BodyCourseFocus.SetWindowText(m_stAFDataStructure.ObjFocusOk ? _T("Yes") : _T("No"));


	}




}

BOOL AFAssistant7::OnSetActive()
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();  

	psheet->SetFinishText(_T("Start Focus"));



	if(m_stAFDataStructure.ObjEverythingOk)
	{
		m_RadioYes.SetCheck(true);
		m_RadioNo.SetCheck(false);
		GetDlgItem(IDC_STATIC_AF_7_FOOTER_TEXT)->ShowWindow(SW_HIDE);
		psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);//PSWIZB_DISABLEDFINISH  PSWIZB_FINISH


	}else
	{

		m_RadioYes.SetCheck(false);
		m_RadioNo.SetCheck(true);
		GetDlgItem(IDC_STATIC_AF_7_FOOTER_TEXT)->ShowWindow(SW_SHOW);
		psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_DISABLEDFINISH);//PSWIZB_DISABLEDFINISH  PSWIZB_FINISH
	}
	CWnd *buttonBack = GetParent()->GetDlgItem(ID_WIZBACK);	
	buttonBack->ShowWindow(SW_SHOW);	


	return CPropertyPage::OnSetActive();
}
void AFAssistant7::OnBnClickedRadioAFYes()
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);//PSWIZB_DISABLEDFINISH  PSWIZB_FINISH
	m_stAFDataStructure.ObjEverythingOk=true;
	GetDlgItem(IDC_STATIC_AF_7_FOOTER_TEXT)->ShowWindow(SW_HIDE);


}

void AFAssistant7::OnBnClickedRadioAFNo()
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_DISABLEDFINISH);//PSWIZB_DISABLEDFINISH  PSWIZB_FINISH
	m_stAFDataStructure.ObjEverythingOk=false;
	GetDlgItem(IDC_STATIC_AF_7_FOOTER_TEXT)->ShowWindow(SW_SHOW);
}
//=============== AFAssistant7 ==============================
