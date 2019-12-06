#pragma once
#include "../ListEx/ListEx.h"

using namespace LISTEX;

class CListExSampleDlg : public CDialogEx
{
public:
	CListExSampleDlg(CWnd* pParent = nullptr);	// standard constructor
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	DECLARE_MESSAGE_MAP()
protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	IListExPtr m_myList { CreateListEx() };
	CMenu m_menuCell;
	CMenu m_menuList;
};

constexpr auto IDC_LIST_MENU_CELL_FIRST = 0x1;
constexpr auto IDC_LIST_MENU_CELL_SECOND = 0x2;
constexpr auto IDC_LIST_MENU_GLOBAL_FIRST = 0x3;
constexpr auto IDC_LIST_MENU_GLOBAL_SECOND = 0x4;