#pragma once
#include "afxwin.h"


// AFAssistant1 dialog

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


class AFAssistant1 : public CPropertyPage
{
	DECLARE_DYNAMIC(AFAssistant1)

public:
	AFAssistant1(stAFAssistData &inDataStructure);
	virtual ~AFAssistant1();

// Dialog Data
	enum { IDD1 = IDD_PROPPAGE_WIZ_AF_1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnSetActive();


	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CFont *m_pFont;
public:
	CStatic m_AF1Header;
	stAFAssistData m_stAFDataStructure;
};


//=============== AFAssistant2 ==============================

class AFAssistant2 : public CPropertyPage
{
	DECLARE_DYNAMIC(AFAssistant2)

public:
	AFAssistant2(stAFAssistData &inDataStructure);
	virtual ~AFAssistant2();

// Dialog Data
	enum { IDD2 = IDD_PROPPAGE_WIZ_AF_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL OnSetActive();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CFont m_Font;
	CFont m_BoldFont;
	bool m_FontInit;

	afx_msg void OnBnClickedRadioAFNo();
	afx_msg void OnBnClickedRadioAFYes();
public:
	CStatic m_AF2Header;
	CStatic m_AF2HeaderSyringe;
	CStatic m_AF2HeaderFlow;
	CStatic m_AF2HeaderObjective;
	CStatic m_AF2BodySyringe;
	CStatic m_AF2BodyFlow;
	CStatic m_AF2BodyObjective;
	stAFAssistData m_stAFDataStructure;
	CButton m_RadioYes;
	CButton m_RadioNo;
};

//=============== AFAssistant2 ==============================

//=============== AFAssistant3 ==============================

class AFAssistant3 : public CPropertyPage
{
	DECLARE_DYNAMIC(AFAssistant3)

public:
	AFAssistant3(stAFAssistData &inDataStructure);
	virtual ~AFAssistant3();

// Dialog Data
	enum { IDD3 = IDD_PROPPAGE_WIZ_AF_3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL OnSetActive();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CFont m_Font;
	CFont m_BoldFont;
	bool m_FontInit;

	afx_msg void OnBnClickedRadioAFNo();
	afx_msg void OnBnClickedRadioAFYes();
public:
	CStatic m_AF3Header;
	CStatic m_AF3HeaderSyringe;
	CStatic m_AF3HeaderVolume;
	CStatic m_AF3BodySyringe;
	CStatic m_AF3BodyVolume;
	stAFAssistData m_stAFDataStructure;
	CButton m_RadioYes;
	CButton m_RadioNo;
};

//=============== AFAssistant4 ==============================

class AFAssistant4 : public CPropertyPage
{
	DECLARE_DYNAMIC(AFAssistant4)

public:
	AFAssistant4(stAFAssistData &inDataStructure);
	virtual ~AFAssistant4();

// Dialog Data
	enum { IDD4 = IDD_PROPPAGE_WIZ_AF_4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnSetActive();


	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CFont *m_pFont;
public:
	CStatic m_AF4Header;
	stAFAssistData m_stAFDataStructure;
};

//=============== AFAssistant5 ==============================

class AFAssistant5 : public CPropertyPage
{
	DECLARE_DYNAMIC(AFAssistant5)

public:
	AFAssistant5(stAFAssistData &inDataStructure);
	virtual ~AFAssistant5();

// Dialog Data
	enum { IDD5 = IDD_PROPPAGE_WIZ_AF_5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnSetActive();


	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CFont *m_pFont;
public:
	CStatic m_AF5Header;
	stAFAssistData m_stAFDataStructure;
};


//=============== AFAssistant6 ==============================

class AFAssistant6 : public CPropertyPage
{
	DECLARE_DYNAMIC(AFAssistant6)

public:
	AFAssistant6(stAFAssistData &inDataStructure);
	virtual ~AFAssistant6();

// Dialog Data
	enum { IDD6 = IDD_PROPPAGE_WIZ_AF_6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnSetActive();


	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CFont *m_pFont;
public:
	CStatic m_AF6Header;
	stAFAssistData m_stAFDataStructure;
};

//=============== AFAssistant7 ==============================

class AFAssistant7 : public CPropertyPage
{
	DECLARE_DYNAMIC(AFAssistant7)

public:
	AFAssistant7(stAFAssistData &inDataStructure);
	virtual ~AFAssistant7();

// Dialog Data
	enum { IDD7 = IDD_PROPPAGE_WIZ_AF_7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL OnSetActive();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CFont m_Font;
	CFont m_BoldFont;
	bool m_FontInit;

	afx_msg void OnBnClickedRadioAFNo();
	afx_msg void OnBnClickedRadioAFYes();
public:
	CStatic m_AF7Header;
	CStatic m_AF7HeaderSyringe;
	CStatic m_AF7HeaderFlow;
	CStatic m_AF7HeaderObjective;
	CStatic m_AF7HeaderSyringeVolume;
	CStatic m_AF7HeaderSolution;
	CStatic m_AF7HeaderCourseFocus;

	CStatic m_AF7BodySyringe;
	CStatic m_AF7BodyFlow;
	CStatic m_AF7BodyObjective;
	CStatic m_AF7BodySyringeVolume;
	CStatic m_AF7BodySolution;
	CStatic m_AF7BodyCourseFocus;

	stAFAssistData m_stAFDataStructure;
	CButton m_RadioYes;
	CButton m_RadioNo;
};

//=============== AFAssistant7 ==============================

