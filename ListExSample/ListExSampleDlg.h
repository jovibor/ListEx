#pragma once
#include "../ListEx/ListEx.h"
#include <string>
#include <vector>

using namespace LISTEX;

struct VIRTLISTDATA
{
	DWORD ID { };       //Unique ID of the list item. 
	std::wstring wstr1; //Arbitrary data...
	std::wstring wstr2;
	std::wstring wstr3;
};

class CListExSampleDlg : public CDialogEx
{
public:
	CListExSampleDlg(CWnd* pParent = nullptr);
protected:
	void DoDataExchange(CDataExchange* pDX)override;
	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)override;
	afx_msg void OnListExGetDispInfo(NMHDR *pNMHDR, LRESULT *pResult);
	void SortVecData();
	DECLARE_MESSAGE_MAP()
protected:
	HICON m_hIcon;
	BOOL OnInitDialog()override;
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	IListExPtr m_myList { CreateListEx() };
	CMenu m_menuCell;
	CMenu m_menuList;
	std::vector<VIRTLISTDATA> m_vecData { };
};

constexpr auto IDC_LIST_MENU_CELL_FIRST = 0x1;
constexpr auto IDC_LIST_MENU_CELL_SECOND = 0x2;
constexpr auto IDC_LIST_MENU_GLOBAL_FIRST = 0x3;
constexpr auto IDC_LIST_MENU_GLOBAL_SECOND = 0x4;