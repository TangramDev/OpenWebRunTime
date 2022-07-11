
// MFCApplication3.h : main header file for the MFCApplication3 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMFCApplication3App:
// See MFCApplication3.cpp for the implementation of this class
//

class CMFCApplication3App : public CWebRTApp
{
public:
	CMFCApplication3App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCApplication3App theApp;
