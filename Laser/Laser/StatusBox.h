#pragma once


// CStatusBox

class CStatusBox : public CWnd
{
	DECLARE_DYNAMIC(CStatusBox)

public:
	CStatusBox();
	virtual ~CStatusBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	void DrawHorizontalGraidentRect(CPaintDC &dc, COLORREF Start, COLORREF End,const CRect &rect );
	void DrawVerticalGraidentRect(CPaintDC &dc, COLORREF Start, COLORREF End,const CRect &rect );

};


