#include "StdAfx.h"
#include "Duity.h"

int yLineStart=10;
int xLineStart=100;
static int yLineEnd=200;


/**


* CLASS DuityDlg
*
* @brief DuityDlg Back Flush to clean
*
* @version 1.0 
*
* @author David Libby
*
*/
DuityDlg::DuityDlg(void):
	m_bModal (false)
	,m_OnTop(false)	
{
}



DuityDlg::~DuityDlg(void)
{
}


/**
* FUNCTION DoModal
*
* @brief Launches the Model version of the Dialog.
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:39:53 AM
*
*
* @return INT_PTR 
*/
INT_PTR DuityDlg::DoModal()
{   
	m_bModal = TRUE;
    const INT_PTR rval = CDialogImpl::DoModal();
    m_bModal = FALSE;
    return rval;
}



/**
* FUNCTION IsVisible
*
* @brief Is the Dialog Currently Visible?
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:34:55 AM
*
*
* @return BOOL True if Visible.
*/
BOOL DuityDlg::IsVisible()
{
	BOOL ret =FALSE;
	if(this->IsWindow())
	{
		ret = this->IsWindowVisible();
	}
	return ret;

}

/**
* FUNCTION DoModeless
*
* @brief Launches Modeless version of Dialog
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:35:21 AM
*
* @param hWndParent By Default is NULL but can contain a parent Window handle if Parent provided it will stay above this window.
*/
void DuityDlg::DoModeless(HWND hWndParent)
{
	if(!this->IsWindow())
		this->Create(hWndParent);
	if(!IsVisible())
	{
		this->ShowWindow(SW_SHOWNORMAL);
	}
}

/**
* FUNCTION SetOnTop
*
* @brief Experimental It will force this window to the top.
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:36:43 AM
*
* @param NewTop 
*/
void DuityDlg::SetOnTop(BOOL NewTop)
{
	m_OnTop=NewTop;
}



/**
* FUNCTION Close
*
* @brief Closes the current Dialog.
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:38:19 AM
*
*/
void DuityDlg::Close()
{
	if (m_bModal)
		EndDialog(0);
	else
		DestroyWindow();
}


/**
* FUNCTION OnInitDialog
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:38:34 AM
*
* @param uMsg 
* @param wParam 
* @param lParam 
* @param bHandled 
*
* @return LRESULT 
*/
LRESULT DuityDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	//std::string Start_mL="1.0";
	//m_EditBoxExample=GetDlgItem(IDC_EDIT_AMOUNT_ML);
	//m_EditBoxExample.SetWindowTextW(CA2W(Start_mL.c_str()).m_szBuffer);

	if(m_OnTop)
	{
		SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}else
	{		
		SetWindowPos(HWND_BOTTOM,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

	return 1;
};

/**
* FUNCTION OnBnClickedCancel
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:38:41 AM
*
* @param  
* @param  
* @param  
* @param  
*
* @return LRESULT 
*/
LRESULT DuityDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);

	return 1;
}

/**
* FUNCTION OnBnClickedCancel
*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 8/2/2013 11:38:41 AM
*
* @param  
* @param  
* @param  
* @param  
*
* @return LRESULT 
*/
LRESULT DuityDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_bModal)
		EndDialog(IDCANCEL);
	else
		::DestroyWindow(this->m_hWnd);
	return 0;
}

LRESULT DuityDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PAINTSTRUCT ps; 
	HDC hdc; 
	hdc = BeginPaint( &ps); 	

	int LIneLength=yLineEnd-yLineStart;
	//DrawVerticalLogScale(hdc, yLineStart,yLineEnd,  xLineStart);
	//DrawVerticalLogScale(hdc, yLineStart+LIneLength+2,yLineEnd+LIneLength*2,  xLineStart);
	DrawVerticalLogScale(hdc,10,110,   100);
	DrawVerticalLogScale(hdc, 10,210,  200);
	DrawVerticalLogScale(hdc, 10,50 , 300);

	EndPaint( &ps);



	return 0;
}

void DuityDlg::DrawVerticalLogScale(HDC &hdc, int yLineStart,int yLineEnd, int xLineStart)
{
	LPPOINT lpPoint(0);

	int divcount=5;
	
	int DashWidth=10;
	int LineLength=yLineEnd-yLineStart;
	MoveToEx (hdc,xLineStart,10,lpPoint);
	LineTo(hdc,xLineStart,yLineEnd);
	double y=1.1;
	//while(y >= yLineStart)
	int a=0;
	//double ff=log(y)*(double)LineLength;
	//double DistMult=pow(double(1.0/5.0),10);
	double Mult=pow((double)LineLength,1/(double)divcount);
	int PreviousPix=yLineStart;

	MoveToEx (hdc,xLineStart-DashWidth,yLineStart,lpPoint);
	LineTo (hdc,xLineStart+DashWidth,yLineStart);


	while(a<divcount)
	//while(y >= yLineStart)
	{
		
		a++;		
		if(yLineEnd==y)
			DashWidth=20;
		else
			DashWidth=10;
		if(yLineStart+(int)y-PreviousPix>1)
		{
			MoveToEx (hdc,xLineStart-DashWidth,yLineStart+(int)y,lpPoint);
			LineTo (hdc,xLineStart+DashWidth,yLineStart+(int)y);
			PreviousPix=yLineStart+(int)y;
		}
		
		y=((double)y)*Mult;	

	}
	MoveToEx (hdc,xLineStart-DashWidth*2,yLineEnd,lpPoint);
	LineTo (hdc,xLineStart+DashWidth*2,yLineEnd);

}


LRESULT DuityDlg::OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	while(1)
	{
		yLineEnd-=5;

		if(yLineEnd<20)
			yLineEnd=250;

		//.this->InvalidateRect();
		::InvalidateRect(this->m_hWnd, 0, true);
		::UpdateWindow (this->m_hWnd);
		Sleep(250);

	}

	return 0;
}

