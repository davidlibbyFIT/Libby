
// CheckAutofocusDataDlg.h : header file
//

#pragma once

#include <vector>
#include "afxwin.h"
#include "FocusPosition.h"



typedef struct InterleafData
{
	double TimeStamp;
	double EdgeGradient;
	double avetime;
	double cvOverMean;
	double aveEG;
	double sdev;
	InterleafData()
	{
		TimeStamp		= 0.0;			
		EdgeGradient	= 0.0;
		avetime			= 0.0;
		cvOverMean		= 0.0;
		aveEG			= 0.0;
		sdev			= 0.0;
	}

}InterleafData;


typedef std::vector <InterleafData> InterleafDataVector;
typedef InterleafDataVector::iterator Interleaf_Data_itr;




struct PointStructure 
{
	float TimeStamp;
	float EdgeGradient;
	float cvOverMean;
	float aveEG;
	float sdev;



	PointStructure()
	{
		TimeStamp = 0.0;
		EdgeGradient=0.0;
		cvOverMean=0.0;
		aveEG=0.0;
		sdev=0.0;
	}

	//
	// This function is used by std::sort to sort by edge gradient in descending order 
	//
	static bool comp_gradient_decending(const PointStructure &lhs, const PointStructure &rhs)
	{
		return(lhs.EdgeGradient > rhs.EdgeGradient);
	}

};

typedef std::vector <PointStructure> PointStructureVector;
typedef PointStructureVector::iterator point_vect_itr;


autofocusReturn_t method_A(const PointStructureVector &dataPoints, double *focus_time, double *DeltaTimeSinceFocus);
autofocusReturn_t method_B(const PointStructureVector &dataPoints, double *focus_time, double *DeltaTimeSinceFocus);



// CCheckAutofocusDataDlg dialog
class CCheckAutofocusDataDlg : public CDialogEx
{
// Construction
public:
	CCheckAutofocusDataDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CHECKAUTOFOCUSDATA_DIALOG };

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
	afx_msg void OnDropFiles(HDROP hDropInfo);

	PointStructure FindFocusPoint( double focus_time, PointStructure FocusPoint );

	autofocusReturn_t OriginalMethod(double *focus_time, double *DeltaTimeSinceFocus);

	void ExtractData( CString sFile );

	void BuildVectorData( CString sFile );

private:
	PointStructureVector m_PointsVector;
public:
	
	float m_Time;

public:
	double m_Time_C;
	double m_timeMaxGradient;

	double m_positionNear_MaxGrad_BM;
	double m_positionCenter_MaxGrad_BM;
	double m_positionFar_MaxGrad_BM;
	double m_positionNear_WeightedAve_BM;
	double m_positionCenter_WeightedAve_BM;
	double m_positionFar_WeightedAve_BM;
	double m_positionNear_CurveFit_BM;
	double m_positionCenter_CurveFit_BM;
	double m_positionFar_CurveFit_BM;
	afx_msg void OnEnChangeEditCenterWaBm();
};
