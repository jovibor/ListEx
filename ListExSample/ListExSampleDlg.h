#pragma once
#include "../ListEx/ListEx.h"
#include <string>
#include <vector>

using namespace LISTEX;

struct VIRTLISTDATA
{
	std::wstring wstr1;      //Arbitrary data...
	std::wstring wstr2;
	std::wstring wstr3;
	bool fIcon { false };    //Does this row have an icon.
	bool fColor { false };   //Does this row have color.
	bool fToolTip { false }; //Tooltip row.
	LISTEXCOLOR clr { }; //Row color.
};

class CListExSampleDlg : public CDialogEx
{
public:
	CListExSampleDlg(CWnd* pParent = nullptr);
protected:
	void DoDataExchange(CDataExchange* pDX)override;
	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)override;
	BOOL OnInitDialog()override;
	afx_msg void OnPaint();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnListExGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListExGetColor(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListExGetIcon(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListExGetToolTip(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListHdrIconClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListHdrRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	void SortVecData();
	DECLARE_MESSAGE_MAP()
protected:
	HICON m_hIcon;
	IListExPtr m_pList { CreateListEx() };
	CMenu m_menuCell;
	CMenu m_menuList;
	CMenu m_menuHdr;
	std::vector<VIRTLISTDATA> m_vecData { };
	CImageList m_stImgList;
};

constexpr auto IDC_LIST_MENU_CELL_FIRST = 0x1;
constexpr auto IDC_LIST_MENU_CELL_SECOND = 0x2;
constexpr auto IDC_LIST_MENU_GLOBAL_FIRST = 0x3;
constexpr auto IDC_LIST_MENU_GLOBAL_SECOND = 0x4;
constexpr auto IDC_LIST_MENU_HDR_BEGIN = 0x5;