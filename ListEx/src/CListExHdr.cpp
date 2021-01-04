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

namespace LISTEX::INTERNAL
{
	/********************************************
	* SHDRCOLOR - header column colors.         *
	********************************************/
	struct CListExHdr::SHDRCOLOR
	{
		COLORREF clrBk { };   //Background color.
		COLORREF clrText { }; //Text color.
	};

	/********************************************
	* SHDRICON - header column icons.           *
	********************************************/
	struct CListExHdr::SHDRICON
	{
		int   iIndex { };           //Icon index.
		CRect rc;                   //Icon rect.
		bool  fClickable { false }; //Clickable or not.
		bool  fLMPressed { false }; //Left mouse button pressed atm.
	};
};

BEGIN_MESSAGE_MAP(CListExHdr, CMFCHeaderCtrl)
	ON_MESSAGE(HDM_LAYOUT, &CListExHdr::OnLayout)
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
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

CListExHdr::~CListExHdr() = default;

void CListExHdr::OnDrawItem(CDC * pDC, int iItem, CRect rcOrig, BOOL bIsPressed, BOOL bIsHighlighted)
{
	//Non working area after last column. Or if column resized to zero.
	if (iItem < 0 || rcOrig.IsRectEmpty())
	{
		pDC->FillSolidRect(&rcOrig, m_clrBkNWA);
		return;
	}

	CMemDC memDC(*pDC, rcOrig);
	CDC& rDC = memDC.GetDC();
	COLORREF clrBk, clrText;

	if (m_umapColors.find(iItem) != m_umapColors.end())
		clrText = m_umapColors[iItem].clrText;
	else
		clrText = m_clrText;

	if (bIsHighlighted)
		clrBk = bIsPressed ? m_clrHglActive : m_clrHglInactive;
	else
	{
		if (m_umapColors.find(iItem) != m_umapColors.end())
			clrBk = m_umapColors[iItem].clrBk;
		else
			clrBk = m_clrBk;
	}
	rDC.FillSolidRect(&rcOrig, clrBk);

	rDC.SetTextColor(clrText);
	rDC.SelectObject(m_fontHdr);

	//Set item's text buffer first char to zero, then getting item's text and Draw it.
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

	//Draw icon for column, if any.
	long lIndentLeft { 4 }; //Left text indent.
	if (const auto pData = HasIcon(iItem); pData != nullptr) //If column has icon.
	{
		IMAGEINFO stIMG;
		const auto pImgList = GetImageList(LVSIL_NORMAL);
		pImgList->GetImageInfo(pData->iIndex, &stIMG);
		pData->rc.SetRect(rcOrig.left + lIndentLeft, rcOrig.top, rcOrig.left + (stIMG.rcImage.right - stIMG.rcImage.left),
			rcOrig.top + (stIMG.rcImage.bottom - stIMG.rcImage.top)); //Setting rect for the icon.
		lIndentLeft += pData->rc.Width() + lIndentLeft;
		pImgList->DrawEx(&rDC, pData->iIndex, { pData->rc.left, pData->rc.top }, { }, CLR_NONE, CLR_NONE, ILD_NORMAL);
	}

	constexpr long lIndentRight = 4;
	CRect rcText { rcOrig.left + lIndentLeft, rcOrig.top, rcOrig.right - lIndentRight, rcOrig.bottom };
	if (StrStrW(warrHdrText, L"\n"))
	{	//If it's multiline text, first — calculate rect for the text,
		//with DT_CALCRECT flag (not drawing anything),
		//and then calculate rect for final vertical text alignment.
		CRect rcCalcText;
		rDC.DrawTextW(warrHdrText, &rcCalcText, DT_CENTER | DT_CALCRECT);
		rcText.top = rcText.Height() / 2 - rcCalcText.Height() / 2;
		rDC.DrawTextW(warrHdrText, &rcOrig, uFormat);
	}
	else
		rDC.DrawTextW(warrHdrText, &rcText, uFormat | DT_VCENTER | DT_SINGLELINE);

	//Draw sortable triangle (arrow).
	if (m_fSortable && iItem == m_iSortColumn)
	{
		rDC.SelectObject(m_penLight);
		const auto iOffset = rcOrig.Height() / 4;

		if (m_fSortAscending)
		{
			//Draw the UP arrow.
			rDC.MoveTo(rcOrig.right - 2 * iOffset, iOffset);
			rDC.LineTo(rcOrig.right - iOffset, rcOrig.bottom - iOffset - 1);
			rDC.LineTo(rcOrig.right - 3 * iOffset - 2, rcOrig.bottom - iOffset - 1);
			rDC.SelectObject(m_penShadow);
			rDC.MoveTo(rcOrig.right - 3 * iOffset - 1, rcOrig.bottom - iOffset - 1);
			rDC.LineTo(rcOrig.right - 2 * iOffset, iOffset - 1);
		}
		else
		{
			//Draw the DOWN arrow.
			rDC.MoveTo(rcOrig.right - iOffset - 1, iOffset);
			rDC.LineTo(rcOrig.right - 2 * iOffset - 1, rcOrig.bottom - iOffset);
			rDC.SelectObject(m_penShadow);
			rDC.MoveTo(rcOrig.right - 2 * iOffset - 2, rcOrig.bottom - iOffset);
			rDC.LineTo(rcOrig.right - 3 * iOffset - 1, iOffset);
			rDC.LineTo(rcOrig.right - iOffset - 1, iOffset);
		}
	}

	//rDC.DrawEdge(&rect, EDGE_RAISED, BF_RECT); //3D look edges.
	rDC.SelectObject(m_penGrid);
	rDC.MoveTo(rcOrig.TopLeft());
	rDC.LineTo(rcOrig.left, rcOrig.bottom);
	if (iItem == GetItemCount() - 1) //Last item.
	{
		rDC.MoveTo(rcOrig.right, rcOrig.top);
		rDC.LineTo(rcOrig.BottomRight());
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

void CListExHdr::OnLButtonDown(UINT nFlags, CPoint point)
{
	for (auto& iter : m_umapIcons)
	{
		if (iter.second.fClickable && iter.second.rc.PtInRect(point))
		{
			iter.second.fLMPressed = true;
			return;
		}
	}

	CMFCHeaderCtrl::OnLButtonDown(nFlags, point);
}

void CListExHdr::OnLButtonUp(UINT nFlags, CPoint point)
{
	for (auto& iter : m_umapIcons)
	{
		if (iter.second.fLMPressed && iter.second.rc.PtInRect(point))
		{
			for (auto& iterData : m_umapIcons)
				iterData.second.fLMPressed = false; //Remove fLMPressed flag from all columns.

			if (auto pParent = GetParent(); pParent != nullptr) //List control pointer.
			{
				const auto uCtrlId = static_cast<UINT>(pParent->GetDlgCtrlID());
				NMHEADERW hdr { { pParent->m_hWnd, uCtrlId, LISTEX_MSG_HDRICONCLICK } };
				hdr.iItem = iter.first;
				pParent->GetParent()->SendMessageW(WM_NOTIFY, static_cast<WPARAM>(uCtrlId), reinterpret_cast<LPARAM>(&hdr));
			}

			return;
		}
	}

	CMFCHeaderCtrl::OnLButtonUp(nFlags, point);
}

void CListExHdr::OnDestroy()
{
	CMFCHeaderCtrl::OnDestroy();

	m_umapColors.clear();
	m_umapIcons.clear();
}

auto CListExHdr::HasIcon(int iColumn)->CListExHdr::SHDRICON*
{
	if (GetImageList() == nullptr)
		return nullptr;

	SHDRICON* pRet { };
	if (const auto it = m_umapIcons.find(iColumn); it != m_umapIcons.end())
		pRet = &it->second;

	return pRet;
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

	m_umapColors[iColumn] = SHDRCOLOR { clrBk, clrText };
	RedrawWindow();
}

void CListExHdr::SetColumnIcon(int iColumn, int iIconIndex, bool fClick)
{
	if (iIconIndex == -1) //If column already has icon.
		m_umapIcons.erase(iColumn);
	else
	{
		SHDRICON stIcon;
		stIcon.iIndex = iIconIndex;
		stIcon.fClickable = fClick;
		m_umapIcons[iColumn] = stIcon;
	}
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