#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// CListExSampleApp:
// See ListExSample.cpp for the implementation of this class
//

class CListExSampleApp : public CWinApp
{
public:
	CListExSampleApp() = default;

public:
	BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CListExSampleApp theApp;
