#pragma once
#include <afxwin.h>

class CListExSampleApp : public CWinApp {
public:
	CListExSampleApp() = default;
public:
	BOOL InitInstance()override;
	DECLARE_MESSAGE_MAP()
};