// StatusBox.cpp : implementation file
//

#include "stdafx.h"
#include "Laser.h"
#include "StatusBox.h"
// CStatusBox
IMPLEMENT_DYNAMIC(CStatusBox, CWnd)
CStatusBox::CStatusBox()
{

}

CStatusBox::~CStatusBox()
{
}

BEGIN_MESSAGE_MAP(CStatusBox, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()
// CStatusBox message handlers

void CStatusBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	CRect rect;
	CBrush brushBlue(RGB(120, 0, 120));
	CBrush* pOldBrush = dc.SelectObject(&brushBlue);

	GetClientRect(&rect);
	dc.Rectangle(0,0,rect.Width()	,rect.Height());
	
	dc.SelectObject(pOldBrush);



	COLORREF StartGreen = RGB(157,227,66);
	COLORREF EndGreen = RGB(103,195,36);

	COLORREF Start = RGB(157,227,66);
	COLORREF End = RGB(103,195,36);

	COLORREF StartRed = RGB(224,49,45);
	COLORREF EndRed = RGB(161,8,10);

	COLORREF StartYellow = RGB(252,227,127);
	COLORREF EndYellow = RGB(253,205,7);

	
	CRect GreenZone(0,60,rect.Width(),rect.Height());
	CRect YellowZone(GreenZone.left,GreenZone.top-30,GreenZone.right,GreenZone.top);
	CRect RedZone(YellowZone.left,YellowZone.top-30,YellowZone.right,YellowZone.top);
	//CRect YellowZone(10,10,20,20);

	//DrawHorizontalGraidentRect(dc,StartGreen, EndGreen, GreenZone );
	//DrawHorizontalGraidentRect(dc,StartYellow, EndYellow, YellowZone );
	//DrawHorizontalGraidentRect(dc,StartRed, EndRed, RedZone );


	DrawVerticalGraidentRect(dc,StartGreen, EndGreen, GreenZone );
	DrawVerticalGraidentRect(dc,StartYellow, EndYellow, YellowZone );
	DrawVerticalGraidentRect(dc,StartRed, EndRed, RedZone );



}

void CStatusBox::DrawHorizontalGraidentRect(CPaintDC &dc, COLORREF Start, COLORREF End,const CRect &rect )
{
	int StartRed=GetRValue(Start),StartGreen=GetGValue(Start),StartBlue=GetBValue(Start);
	int EndRed=GetRValue(End),EndGreen=GetGValue(End),EndBlue=GetBValue(End);


	for(int i=0;i<rect.Width();i++)
	{ 
		int r,g,b;
		r = StartRed + (i * (EndRed-StartRed) / rect.Width());
		g = StartGreen + (i * (EndGreen-StartGreen) / rect.Width());
		b = StartBlue + (i * (EndBlue-StartBlue) / rect.Width());
		dc.FillSolidRect(rect.left+i,rect.top,1,rect.Height(),RGB(r,g,b));
	}
}

void CStatusBox::DrawVerticalGraidentRect(CPaintDC &dc, COLORREF Start, COLORREF End,const CRect &rect )
{
	int StartRed=GetRValue(Start),StartGreen=GetGValue(Start),StartBlue=GetBValue(Start);
	int EndRed=GetRValue(End),EndGreen=GetGValue(End),EndBlue=GetBValue(End);


	for(int i=0;i<rect.Height();i++)
	{ 
		int r,g,b;
		r = StartRed + (i * (EndRed-StartRed) / rect.Height());
		g = StartGreen + (i * (EndGreen-StartGreen) / rect.Height());
		b = StartBlue + (i * (EndBlue-StartBlue) / rect.Height());
		dc.FillSolidRect(rect.left,rect.top+i,rect.Width(),1,RGB(r,g,b));
	}
}
