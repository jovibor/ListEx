/****************************************************************************************
* Copyright © 2018-2021 Jovibor https://github.com/jovibor/                             *
* This is very extended and featured version of CMFCListCtrl class.                     *
* Official git repository: https://github.com/jovibor/ListEx/                           *
* This class is available under the "MIT License".                                      *
****************************************************************************************/
#include "stdafx.h"
#include "../ListEx.h"
#include "CListExHdr.h"

using namespace LISTEX;
using namespace LISTEX::INTERNAL;

/****************************************************
* CListExHdr class implementation.					*
****************************************************/
BEGIN_MESSAGE_MAP(CListExHdr, CMFCHeaderCtrl)
	ON_MESSAGE(HDM_LAYOUT, &CListExHdr::OnLayout)
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CListExHdr::CListExHdr()
{
	NONCLIENTMETRICSW ncm { };
	ncm.cbSize = sizeof(NONCLIENTMETRICSW);
	SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
	ncm.lfMessageFont.lfHeight = 16; //For some weird reason above func returns this value as MAX_LONG.

	m_fontHdr.CreateFontIndirectW(&ncm.lfMessageFont);
	m_penGrid.CreatePen(PS_SOLID, 2, RGB(220, 220, 220));
	m_penLight.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
	m_penShadow.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
}

void CListExHdr::OnDrawItem(CDC* pDC, int iItem, CRect rect, BOOL bIsPressed, BOOL bIsHighlighted)
{
	if (iItem < 0) //Non working area, after last column.
	{
		pDC->FillSolidRect(&rect, m_clrBkNWA);
		return;
	}

	CMemDC memDC(*pDC, rect);
	CDC& rDC = memDC.GetDC();
	COLORREF clrBk, clrText;

	if (m_umapClrColumn.find(iItem) != m_umapClrColumn.end())
		clrText = m_umapClrColumn[iItem].clrText;
	else
		clrText = m_clrText;

	if (bIsHighlighted)
		clrBk = bIsPressed ? m_clrHglActive : m_clrHglInactive;
	else
	{
		if (m_umapClrColumn.find(iItem) != m_umapClrColumn.end())
			clrBk = m_umapClrColumn[iItem].clrBk;
		else
			clrBk = m_clrBk;
	}
	rDC.FillSolidRect(&rect, clrBk);

	rDC.SetTextColor(clrText);
	rDC.SelectObject(m_fontHdr);

	//Set item's text buffer first char to zero,
	//then getting item's text and Draw it.
	static WCHAR warrHdrText[MAX_PATH] { };
	warrHdrText[0] = L'\0';
	static HDITEMW hdItem { HDI_FORMAT | HDI_TEXT };
	hdItem.cchTextMax = MAX_PATH;
	hdItem.pszText = warrHdrText;

	GetItem(iItem, &hdItem);
	UINT uFormat { };
	switch (hdItem.fmt)
	{
	case (HDF_STRING | HDF_LEFT):
		uFormat = DT_LEFT;
		break;
	case (HDF_STRING | HDF_CENTER):
		uFormat = DT_CENTER;
		break;
	case (HDF_STRING | HDF_RIGHT):
		uFormat = DT_RIGHT;
		break;
	default:
		break;
	}

	constexpr long lOffset = 4;
	rect.left += lOffset;
	rect.right -= lOffset;
	if (StrStrW(warrHdrText, L"\n"))
	{	//If it's multiline text, first — calculate rect for the text,
		//with DT_CALCRECT flag (not drawing anything),
		//and then calculate rect for final vertical text alignment.
		CRect rcText;
		rDC.DrawTextW(warrHdrText, &rcText, DT_CENTER | DT_CALCRECT);
		rect.top = rect.Height() / 2 - rcText.Height() / 2;
		rDC.DrawTextW(warrHdrText, &rect, uFormat);
	}
	else
		rDC.DrawTextW(warrHdrText, &rect, uFormat | DT_VCENTER | DT_SINGLELINE);
	rect.left -= lOffset;
	rect.right += lOffset;

	//Draw sortable triangle (arrow).
	if (m_fSortable && iItem == m_iSortColumn)
	{
		rDC.SelectObject(m_penLight);
		const auto iOffset = rect.Height() / 4;

		if (m_fSortAscending)
		{
			//Draw the UP arrow.
			rDC.MoveTo(rect.right - 2 * iOffset, iOffset);
			rDC.LineTo(rect.right - iOffset, rect.bottom - iOffset - 1);
			rDC.LineTo(rect.right - 3 * iOffset - 2, rect.bottom - iOffset - 1);
			rDC.SelectObject(m_penShadow);
			rDC.MoveTo(rect.right - 3 * iOffset - 1, rect.bottom - iOffset - 1);
			rDC.LineTo(rect.right - 2 * iOffset, iOffset - 1);
		}
		else
		{
			//Draw the DOWN arrow.
			rDC.MoveTo(rect.right - iOffset - 1, iOffset);
			rDC.LineTo(rect.right - 2 * iOffset - 1, rect.bottom - iOffset);
			rDC.SelectObject(m_penShadow);
			rDC.MoveTo(rect.right - 2 * iOffset - 2, rect.bottom - iOffset);
			rDC.LineTo(rect.right - 3 * iOffset - 1, iOffset);
			rDC.LineTo(rect.right - iOffset - 1, iOffset);
		}
	}

	//rDC.DrawEdge(&rect, EDGE_RAISED, BF_RECT); //3D look edges.
	rDC.SelectObject(m_penGrid);
	rDC.MoveTo(rect.TopLeft());
	rDC.LineTo(rect.left, rect.bottom);
	if (iItem == GetItemCount() - 1) //Last item.
	{
		rDC.MoveTo(rect.right, rect.top);
		rDC.LineTo(rect.BottomRight());
	}
}

LRESULT CListExHdr::OnLayout(WPARAM /*wParam*/, LPARAM lParam)
{
	CMFCHeaderCtrl::DefWindowProcW(HDM_LAYOUT, 0, lParam);

	auto pHDL = reinterpret_cast<LPHDLAYOUT>(lParam);
	pHDL->pwpos->cy = m_dwHeaderHeight;	//New header height.
	pHDL->prc->top = m_dwHeaderHeight;  //Decreasing list's height begining by the new header's height.

	return 0;
}

void CListExHdr::SetHeight(DWORD dwHeight)
{
	m_dwHeaderHeight = dwHeight;
}

void CListExHdr::SetColor(const LISTEXCOLORS& lcs)
{
	m_clrText = lcs.clrHdrText;
	m_clrBk = lcs.clrHdrBk;
	m_clrBkNWA = lcs.clrNWABk;
	m_clrHglInactive = lcs.clrHdrHglInact;
	m_clrHglActive = lcs.clrHdrHglAct;

	RedrawWindow();
}

void CListExHdr::SetColumnColor(int iColumn, COLORREF clrBk, COLORREF clrText)
{
	if (clrText == -1)
		clrText = m_clrText;

	m_umapClrColumn[iColumn] = SHDRCOLOR { clrBk, clrText };
	RedrawWindow();
}

void CListExHdr::SetSortable(bool fSortable)
{
	m_fSortable = fSortable;
	RedrawWindow();
}

void CListExHdr::SetSortArrow(int iColumn, bool fAscending)
{
	m_iSortColumn = iColumn;
	m_fSortAscending = fAscending;
	RedrawWindow();
}

void CListExHdr::SetFont(const LOGFONTW* pLogFontNew)
{
	if (!pLogFontNew)
		return;

	m_fontHdr.DeleteObject();
	m_fontHdr.CreateFontIndirectW(pLogFontNew);

	//If new font's height is higher than current height (m_dwHeaderHeight)
	//we adjust current height as well.
	TEXTMETRICW tm;
	CDC* pDC = GetDC();
	pDC->SelectObject(m_fontHdr);
	pDC->GetTextMetricsW(&tm);
	ReleaseDC(pDC);
	const DWORD dwHeightFont = tm.tmHeight + tm.tmExternalLeading + 1;
	if (dwHeightFont > m_dwHeaderHeight)
		SetHeight(dwHeightFont);
}

void CListExHdr::OnDestroy()
{
	CMFCHeaderCtrl::OnDestroy();

	m_umapClrColumn.clear();
}