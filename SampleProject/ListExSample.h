#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif

#include "Resource.h"

class CListExSampleApp : public CWinApp
{
public:
	CListExSampleApp() = default;

public:
	BOOL InitInstance()override;
	DECLARE_MESSAGE_MAP()
};