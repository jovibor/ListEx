/********************************************************************************
* Copyright (C) 2018-2019, Jovibor: https://github.com/jovibor/					*
* Github repository URL: https://github.com/jovibor/ListEx						*
* This software is available under the "MIT License".							*
* This is an extended and featured version of CMFCListCtrl class.				*
* CListEx - list control class with the ability to set tooltips on arbitrary	*
* cells, and also with a lots of other stuff to customize your control in many	*
* different aspects. For more info see official documentation on github.		*
********************************************************************************/
#pragma once
#include <afxwin.h>
#include <unordered_map>
#include "ListExHdr.h"

namespace LISTEX {
	/********************************************************************************************
	* LISTEXCOLORSTRUCT - All ListEx colors.													*
	********************************************************************************************/
	struct LISTEXCOLORSTRUCT
	{
		COLORREF		clrListText { GetSysColor(COLOR_WINDOWTEXT) };				//List text color.
		COLORREF		clrListBkRow1 { GetSysColor(COLOR_WINDOW) };				//List Bk color of the odd rows.
		COLORREF		clrListBkRow2 { GetSysColor(COLOR_WINDOW) };				//List Bk color of the even rows.
		COLORREF		clrListGrid { RGB(220, 220, 220) };							//List grid color.
		COLORREF		clrListTextSelected { GetSysColor(COLOR_HIGHLIGHTTEXT) };	//Selected item text color.
		COLORREF		clrListBkSelected { GetSysColor(COLOR_HIGHLIGHT) };			//Selected item bk color.
		COLORREF		clrTooltipText { GetSysColor(COLOR_INFOTEXT) };				//Tooltip window text color.
		COLORREF		clrTooltipBk { GetSysColor(COLOR_INFOBK) };					//Tooltip window bk color.
		COLORREF		clrListTextCellTt { GetSysColor(COLOR_WINDOWTEXT) };		//Text color of a cell that has tooltip.
		COLORREF		clrListBkCellTt { RGB(170, 170, 230) };						//Bk color of a cell that has tooltip.
		COLORREF		clrHeaderText { GetSysColor(COLOR_WINDOWTEXT) };			//List header text color.
		COLORREF		clrHeaderBk { GetSysColor(COLOR_WINDOW) };					//List header bk color.
		COLORREF		clrBkNWA { GetSysColor(COLOR_WINDOW) };						//Bk of non working area.
	};
	using PLISTEXCOLORSTRUCT = LISTEXCOLORSTRUCT *;

	/********************************************************************************************
	* LISTEXCREATESTRUCT - Main initialization helper struct for CListEx::Create method.		*
	********************************************************************************************/
	struct LISTEXCREATESTRUCT {
		PLISTEXCOLORSTRUCT	pstColor { };					//All control's colors. If nullptr defaults are used.
		DWORD				dwStyle { };					//Control's styles. Zero for default.
		CRect				rect;							//Initial rect.
		CWnd*				pwndParent { };					//Parent window.
		UINT				nID { };						//Control Id.
		const LOGFONT*		pListLogFont { };				//List font.
		const LOGFONT*		pHeaderLogFont { };				//List header font.
		DWORD				dwListGridWidth { 1 };			//Width of the list grid.
		DWORD				dwHeaderHeight { 20 };			//List header height.
		bool				fDialogCtrl { false };			//If it's a list within dialog.
	};

	/********************************************
	* CListEx class definition.					*
	********************************************/
	class CListEx : public CMFCListCtrl
	{
	public:
		DECLARE_DYNAMIC(CListEx)
		CListEx() {}
		virtual ~CListEx() {}
		bool Create(const LISTEXCREATESTRUCT& lcs);
		void CreateDialogCtrl();
		bool IsCreated();
		void SetColor(const LISTEXCOLORSTRUCT& lcs);
		void SetFont(const LOGFONTW* pLogFontNew);
		void SetFontSize(UINT uiSize);
		UINT GetFontSize();
		void SetCellTooltip(int iItem, int iSubitem, const std::wstring& wstrTooltip, const std::wstring& wstrCaption = { });
		void SetCellMenu(int iItem, int iSubitem, CMenu* pMenu);
		void SetListMenu(CMenu* pMenu);
		void SetCellData(int iItem, int iSubitem, DWORD_PTR dwData);
		DWORD_PTR GetCellData(int iItem, int iSubitem);
		void SetHeaderHeight(DWORD dwHeight);
		void SetHeaderFont(const LOGFONT* pLogFontNew);
		void SetHeaderColumnColor(DWORD nColumn, COLORREF clr);
		DECLARE_MESSAGE_MAP()
	protected:
		CListExHdr& GetHeaderCtrl() override { return m_stListHeader; }
		void InitHeader() override;
		bool HasTooltip(int iItem, int iSubitem, std::wstring** ppwstrText = nullptr, std::wstring** ppwstrCaption = nullptr);
		bool HasMenu(int iItem, int iSubitem, CMenu** ppMenu = nullptr);
		void DrawItem(LPDRAWITEMSTRUCT) override;
		afx_msg void OnPaint();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
		afx_msg void OnRButtonDown(UINT nFlags, CPoint pt);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint pt);
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
		afx_msg void OnHdnDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnHdnTrack(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnDestroy();
	private:
		bool m_fCreated { false };
		CListExHdr m_stListHeader;
		LISTEXCOLORSTRUCT m_stColor { };
		CFont m_fontList;
		CPen m_penGrid;
		HWND m_hwndTt { };
		TOOLINFO m_stToolInfo { };
		bool m_fTtShown { false };
		LVHITTESTINFO m_stCurrCell { };
		DWORD m_dwGridWidth { 1 };		//Grid width.
		CMenu* m_pListMenu { };			//List global menu, if set.
		std::unordered_map<int, std::unordered_map<int,
			std::tuple<std::wstring/*tip text*/, std::wstring/*caption text*/>>> m_umapCellTt { }; //Cell's tooltips.
		std::unordered_map<int, std::unordered_map<int, CMenu*>> m_umapCellMenu { };			//Cell's menus.
		std::unordered_map<int, std::unordered_map<int, DWORD_PTR>> m_umapCellData { };			//Cell's custom data.
		NMITEMACTIVATE m_stNMII { };
		const ULONG_PTR ID_TIMER_TOOLTIP { 0x01 };
	};

	/****************************************************************************
	* WM_NOTIFY codes (NMHDR.code values)										*
	****************************************************************************/

	constexpr auto LISTEX_MSG_MENUSELECTED = 0x00001000;

	/*******************Setting a manifest for ComCtl32.dll version 6.***********************/
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
}