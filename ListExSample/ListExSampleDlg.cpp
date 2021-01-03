#include "stdafx.h"
#include "ListExSample.h"
#include "ListExSampleDlg.h"
#include "afxdialogex.h"
#include "framework.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CListExSampleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_GETDISPINFOW, IDC_LISTEX, &CListExSampleDlg::OnListExGetDispInfo)
	ON_NOTIFY(LISTEX_MSG_GETCOLOR, IDC_LISTEX, &CListExSampleDlg::OnListExGetColor)
	ON_NOTIFY(LISTEX_MSG_GETICON, IDC_LISTEX, &CListExSampleDlg::OnListExGetIcon)
END_MESSAGE_MAP()

CListExSampleDlg::CListExSampleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LISTEXSAMPLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CListExSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CListExSampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	LISTEXCREATESTRUCT lcs;
	lcs.uID = IDC_LISTEX;
	lcs.pParent = this;
	lcs.fDialogCtrl = true;
	lcs.dwHdrHeight = 25;
	lcs.fSortable = true;
	lcs.stColor.clrHdrText = RGB(250, 250, 250);

	m_myList->Create(lcs);
	m_myList->SetExtendedStyle(LVS_EX_HEADERDRAGDROP);

	m_myList->InsertColumn(0, L"Test column 0", 0, 200);
	//First (0 index) column is always left aligned by default.
	//To change its alignment SetColumn() must be called explicitly.
	LVCOLUMNW stCol { };
	stCol.mask = LVCF_FMT;
	stCol.fmt = LVCFMT_CENTER;
	m_myList->SetColumn(0, &stCol);
	m_myList->InsertColumn(1, L"Test column 1", LVCFMT_CENTER, 200);
	m_myList->InsertColumn(2, L"Test column 2", LVCFMT_CENTER, 200);
	m_myList->SetHdrColumnColor(0, RGB(70, 70, 70));
	m_myList->SetHdrColumnColor(1, RGB(125, 125, 125));
	m_myList->SetHdrColumnColor(2, RGB(200, 200, 200));

	//For Virtual list.
	//Sample data for Virtual mode (LVS_OWNERDATA).
	constexpr auto iVirtualDataSize { 11 };
	for (unsigned i = 0; i < iVirtualDataSize; ++i)
	{
		m_vecData.emplace_back(VIRTLISTDATA
			{
				i,
				L"Virtual item "
				L"<link=\"0\" title=\"Custom title\">column:0</link>"
			L"/"
			L"<link=\"" + std::to_wstring(i) + L"\">row:" + std::to_wstring(i) + L"</link>",
			L"Virtual item column:1/row:" + std::to_wstring((i % 2) ? i * i : i),
			L"Virtual item column:2/row:" + std::to_wstring(i),
			i == 2 ? true : false,
			i == 7 ? true : false,
			i == 7 ? LISTEXCELLCOLOR { RGB(0, 220, 0) } : LISTEXCELLCOLOR { }
			});
	}
	m_myList->SetItemCountEx(iVirtualDataSize, LVSICF_NOSCROLL); //Amount of Virtual items.

	//For classical list.
/*	m_myList->InsertItem(0, L"Test item - row:0/column:0");
	m_myList->InsertItem(1, L"Test item - row:1/column:0.");
	m_myList->InsertItem(2, L"Test item - row:2/column:0..");
	m_myList->InsertItem(3, L"Test item - row:3/column:0...");
	m_myList->InsertItem(4, L"Test item - row:4/column:0....");
	m_myList->SetItemText(0, 1, L"Test item - row:0/column:1....");
	m_myList->SetItemText(1, 1, L"Test item - row:1/column:1...");
	m_myList->SetItemText(2, 1, L"Test item - row:2/column:1..");
	m_myList->SetItemText(3, 1, L"Test item - row:3/column:1.");
	m_myList->SetItemText(4, 1, L"Test item - row:4/column:1");
	m_myList->SetItemText(0, 2, L"Test item - row:0/column:2...");
	m_myList->SetItemText(1, 2, L"Test item - row:1/column:2.");
	m_myList->SetItemText(2, 2, L"Test item - row:2/column:2....");
	m_myList->SetItemText(3, 2, L"Test item - row:3/column:2..");
	m_myList->SetItemText(4, 2, L"Test item - row:4/column:2.....");

	m_myList->SetCellColor(2, 0, GetSysColor(COLOR_GRADIENTINACTIVECAPTION));
	m_myList->SetCellColor(3, 1, GetSysColor(COLOR_GRADIENTACTIVECAPTION));
	m_myList->SetCellColor(4, 2, RGB(255, 255, 0));
	m_myList->SetRowColor(7, RGB(0, 220, 0));
	m_myList->SetColumnColor(1, RGB(0, 220, 220));
	*/

	//Set list's cells colors, menu, tool-tips.
	m_menuCell.CreatePopupMenu();
	m_menuCell.AppendMenuW(MF_STRING, IDC_LIST_MENU_CELL_FIRST, L"Cell's first menu");
	m_menuCell.AppendMenuW(MF_STRING, IDC_LIST_MENU_CELL_SECOND, L"Cell's second menu");
	m_menuList.CreatePopupMenu();
	m_menuList.AppendMenuW(MF_STRING, IDC_LIST_MENU_GLOBAL_FIRST, L"List's first menu");
	m_menuList.AppendMenuW(MF_STRING, IDC_LIST_MENU_GLOBAL_SECOND, L"List's second menu");

	m_myList->SetListMenu(&m_menuList);
	m_myList->SetCellTooltip(0, 0, L"Tooltip text...", L"Caption of the tooltip:");
	m_myList->SetCellMenu(1, 0, &m_menuCell); //Set menu for row:1 column:0.

	m_stImgList.Create(16, 16, ILC_COLOR | ILC_MASK, 0, 1);
	m_stImgList.Add(static_cast<HICON>(LoadImageW(AfxGetInstanceHandle(),
		MAKEINTRESOURCEW(IDI_TEST), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR)));
	m_myList->SetImageList(&m_stImgList, LVSIL_NORMAL);

	return TRUE;
}

void CListExSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		const auto cxIcon = GetSystemMetrics(SM_CXICON);
		const auto cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		const auto x = (rect.Width() - cxIcon + 1) / 2;
		const auto y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CListExSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CListExSampleDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	const auto pNMI = reinterpret_cast<LPNMITEMACTIVATE>(lParam);

	if (pNMI->hdr.idFrom == IDC_LISTEX)
	{
		switch (pNMI->hdr.code)
		{
		case LVM_MAPINDEXTOID:
		{
			if (pNMI->iItem < 0 || pNMI->iItem >= static_cast<int>(m_vecData.size()))
				break;
			pNMI->lParam = static_cast<LPARAM>(m_vecData.at(static_cast<size_t>(pNMI->iItem)).ID);
		}
		break;
		case LVN_COLUMNCLICK:
			SortVecData();
			break;
		case LISTEX_MSG_MENUSELECTED:
		{
			CStringW ss;
			switch (pNMI->lParam)
			{
			case IDC_LIST_MENU_CELL_FIRST:
				ss.Format(L"Cell's first menu clicked. Row: %i, Column: %i", pNMI->iItem, pNMI->iSubItem);
				break;
			case IDC_LIST_MENU_CELL_SECOND:
				ss.Format(L"Cell's second menu clicked. Row: %i, Column: %i", pNMI->iItem, pNMI->iSubItem);
				break;
			case IDC_LIST_MENU_GLOBAL_FIRST:
				ss.Format(L"List's first menu clicked. Row: %i, Column: %i", pNMI->iItem, pNMI->iSubItem);
				break;
			case IDC_LIST_MENU_GLOBAL_SECOND:
				ss.Format(L"List's second menu clicked. Row: %i, Column: %i", pNMI->iItem, pNMI->iSubItem);
				break;
			}
			MessageBoxW(ss);
		}
		break;
		case LISTEX_MSG_LINKCLICK:
			MessageBoxW(reinterpret_cast<LPWSTR>(pNMI->lParam));
			break;
		}
	}

	return CDialogEx::OnNotify(wParam, lParam, pResult);
}

void CListExSampleDlg::OnListExGetDispInfo(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	const auto pDispInfo = reinterpret_cast<NMLVDISPINFOW*>(pNMHDR);
	LVITEMW* pItem = &pDispInfo->item;

	if (pItem->mask & LVIF_TEXT)
	{
		switch (pItem->iSubItem)
		{
		case 0:
			pItem->pszText = m_vecData.at(static_cast<size_t>(pItem->iItem)).wstr1.data();
			break;
		case 1:
			pItem->pszText = m_vecData.at(static_cast<size_t>(pItem->iItem)).wstr2.data();
			break;
		case 2:
			pItem->pszText = m_vecData.at(static_cast<size_t>(pItem->iItem)).wstr3.data();
			break;
		}
	}
}

void CListExSampleDlg::OnListExGetColor(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	//Virtual data colors.
	const auto pNMI = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	if (pNMI->iItem < 0 || pNMI->iSubItem < 0)
		return;

	if (pNMI->iSubItem == 1) //Column number 1 colored to RGB(0, 220, 220).
	{
		static LISTEXCELLCOLOR clr { RGB(0, 220, 220), RGB(0, 0, 0) };
		pNMI->lParam = reinterpret_cast<LPARAM>(&clr);
	}

	if (m_vecData.at(static_cast<size_t>(pNMI->iItem)).fColor)
		pNMI->lParam = reinterpret_cast<LPARAM>(&m_vecData.at(static_cast<size_t>(pNMI->iItem)).clr);
}

void CListExSampleDlg::OnListExGetIcon(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	//Virtual data icons.
	const auto pNMI = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);

	if (pNMI->iItem < 0 || pNMI->iSubItem < 0)
		return;

	if (m_vecData.at(static_cast<size_t>(pNMI->iItem)).fIcon && pNMI->iSubItem == 1)
		pNMI->lParam = 0; //Icon index in list's image list.
}

void CListExSampleDlg::SortVecData()
{
	//Sorts the vector of data according to clicked column.
	std::sort(m_vecData.begin(), m_vecData.end(), [&](const VIRTLISTDATA& st1, const VIRTLISTDATA& st2)
		{
			int iCompare { };
			switch (m_myList->GetSortColumn())
			{
			case 0:
				iCompare = st1.wstr1.compare(st2.wstr1);
				break;
			case 1:
				iCompare = st1.wstr2.compare(st2.wstr2);
				break;
			case 2:
				iCompare = st1.wstr3.compare(st2.wstr3);
				break;
			}

			bool result { false };
			if (m_myList->GetSortAscending())
			{
				if (iCompare < 0)
					result = true;
			}
			else
			{
				if (iCompare > 0)
					result = true;
			}

			return result;
		});

	m_myList->RedrawWindow();
}