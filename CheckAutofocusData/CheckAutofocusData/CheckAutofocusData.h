
// CheckAutofocusData.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCheckAutofocusDataApp:
// See CheckAutofocusData.cpp for the implementation of this class
//



class CCheckAutofocusDataApp : public CWinApp
{
public:
	CCheckAutofocusDataApp();

// Overrides
public:
	virtual BOOL InitInstance();

private:
	

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCheckAutofocusDataApp theApp;