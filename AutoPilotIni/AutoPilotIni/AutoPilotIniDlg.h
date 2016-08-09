
// AutoPilotIniDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <string>
#include <memory>
#include "FileInformation.h"
#include <atomic>
#include <thread>

enum VersaStatus
{
	VERSA_STATUS_IDLE = 0
	, VERSA_STATUS_UNKNOWEN = 1
	, VERSA_STATUS_SAMPLE_READY = 2
	, VERSA_STATUS_START_IMAGING = 3
	, VERSA_STATUS_FLOWCAM_READY = 4
	, VERSA_STATUS_ERROR = 5
	, VERSA_ALL_STOP = 6
	, VERSA_VISUAL_CLEANING = 7
	, VERSA_VISUAL_END_OF_DAY = 8
	, VERSA_VISUAL_START_OF_DAY = 9
	, VERSA_STATUS_NOOP = 999
};

enum RunMode
{
	  AP_RUN_MODE_ALH
	, AP_RUN_MODE_FLOWCAM
	, AP_RUN_MODE_NOOP 
};

enum VersaSampleType
{
	VERSA_SAMPLE_STANDARD = 0
	, VERSA_SAMPLE_CLEAN = 1
	, VERSA_SAMPLE_END_OF_DAY = 2

};

//! Structure for the Data fields of interest in the ini file.
typedef struct VersaIniData
{
	VersaStatus status;
	VersaSampleType sampleType;
	std::string plateWell;
	std::string barcode;
	std::string sampleID;
	int delaySeconds1;
	float SampleVolume_ml;
	int delaySeconds2;


	VersaIniData()
	{
		status = VERSA_STATUS_IDLE;
		sampleType = VERSA_SAMPLE_STANDARD;
		delaySeconds1 = 0; //Not Used
		delaySeconds2 = 0; //Not Used
	}

	VersaIniData& operator =(const VersaIniData& rhs)
	{
		status = rhs.status;
		sampleType = rhs.sampleType;
		plateWell = rhs.plateWell;
		barcode = rhs.barcode;
		sampleID = rhs.sampleID;
		delaySeconds1 = rhs.delaySeconds1;
		SampleVolume_ml = rhs.SampleVolume_ml;
		delaySeconds2 = rhs.delaySeconds2;
		return *this;
	}

	bool operator==(const VersaIniData& rhs)
	{
		bool eq = true;
		if (status != rhs.status)
			eq = false;
		if (eq && sampleType != rhs.sampleType)
			eq = false;
		if (eq && plateWell != rhs.plateWell)
			eq = false;
		if (eq && barcode != rhs.barcode)
			eq = false;
		if (eq && sampleID != rhs.sampleID)
			eq = false;
		// 		if(eq && delaySeconds1 != rhs.delaySeconds1) //Delay Not used at this time
		// 			eq=false;		
		if (eq && SampleVolume_ml != rhs.SampleVolume_ml)
			eq = false;
		// 		if(eq && delaySeconds2 != rhs.delaySeconds2) //Delay Not used at this time
		// 			eq=false;		
		return eq;
	}
	bool operator!=(const VersaIniData& rhs)
	{
		bool eq = false;
		if (status != rhs.status)
			eq = true;
		if (!eq && sampleType != rhs.sampleType)
			eq = true;
		if (eq && plateWell != rhs.plateWell)
			eq = true;
		if (!eq && barcode != rhs.barcode)
			eq = true;
		if (!eq && sampleID != rhs.sampleID)
			eq = true;
		// 		if(!eq && delaySeconds1 != rhs.delaySeconds1) //Delay Not used at this time
		// 			eq=true;		
		if (!eq && SampleVolume_ml != rhs.SampleVolume_ml)
			eq = true;
		// 		if(!eq && delaySeconds2 != rhs.delaySeconds2) //Delay Not used at this time
		// 			eq=true;		
		return eq;
	}

}VersaIniData;




// CAutoPilotIniDlg dialog
class CAutoPilotIniDlg : public CDialogEx
{
// Construction
public:
	CAutoPilotIniDlg(CWnd* pParent = NULL);	// standard constructor
	~CAutoPilotIniDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AUTOPILOTINI_DIALOG };
#endif

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
private:
	CEdit m_EditStatus;
	CEdit m_EditSampleType;
	CEdit m_EditPlateWell;
	CEdit m_EditBarCode;
	CEdit m_EditSampleId;
	CEdit m_EditSample;
	CEdit m_EditDelaySeconds2;
	CEdit m_EditDelaySeconds1;
	
	std::string m_statusFileName;
	std::string m_LocalIni;
	int m_TimeoutAmount;
	int m_AlhCurrentStep;

	VersaIniData m_CurrentStatus;
	int m_CountDown;

	void workingThread();
	bool stopWorkingThread();
	bool startWorkingThread();
	void setCurrentStatus(VersaStatus newstatus, bool forceUpdate = false);


	//! Handel ot the thread
	std::thread m_hGrabThread;
	//! Request to stop thread.
	std::atomic<bool> m_stopGrabThread;
	//! the current ini file metadata ( Last write time etc)
	std::unique_ptr<FileInformation> m_pFileInformation;
	void theFileChanged();
	void readCurrentStatus(VersaIniData &Current);

	RunMode m_logicFlowcam;

	void UpdateStatus();

	void AlhLogic();

	void FlowcamLogic();

	void writeCurrentStatus(bool Update = true);
	std::string GetShortStatus();
	std::string GetSampleType();

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_StatusText;
	CEdit m_EditCountDown;
	CEdit m_Edit_StatusIni;
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonOpen();
	CButton m_ButtonOpen;
	afx_msg void OnBnClickedRadioLogicFlow();
	afx_msg void OnBnClickedRadioLogicAlh();
	afx_msg void OnBnClickedButtonAlhStart();
	CButton m_ButtonALHStart;
	CStatic m_GroupBoxMode;
	afx_msg void OnBnClickedRadioLogicNoop();
};
void StdStringExtractPathFileExt(const std::string &FullString, std::string &Filename, std::string &Path, std::string &Ext);