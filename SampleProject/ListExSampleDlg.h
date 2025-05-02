#pragma once
#include <string>
#include <vector>

import ListEx;
using namespace LISTEX;

struct VIRTLISTDATA {
	std::wstring wstr0;      //Arbitrary data...
	std::wstring wstr1;
	std::wstring wstr2;
	bool fIcon { false };    //Does this row have an icon.
	bool fColor { false };   //Does this row have color.
	bool fToolTip { false }; //Tooltip row.
	LISTEXCOLOR clr { };     //Row color.
};

class CListExSampleDlg : public CDialogEx {
public:
	CListExSampleDlg(CWnd* pParent = nullptr);
protected:
	void DoDataExchange(CDataExchange* pDX)override;
	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)override;
	BOOL OnInitDialog()override;
	afx_msg void OnPaint();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void OnOK();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnListEditBegin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListGetColor(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListGetIcon(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListGetToolTip(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListHdrIconClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListHdrRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListLinkClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListSetData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void SortVecData();
	DECLARE_MESSAGE_MAP();
protected:
	HICON m_hIcon;
	CListEx m_MyList;
	CMenu m_menuHdr;
	std::vector<VIRTLISTDATA> m_vecData { };
	CImageList m_stImgList;
};