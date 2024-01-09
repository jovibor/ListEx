#include "stdafx.h"
#include "ListExSample.h"
#include "ListExSampleDlg.h"
#include "afxdialogex.h"
#include "framework.h"
#include <algorithm>
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

constexpr auto g_iColumns { 3 };
constexpr auto g_iRows { 10 };
constexpr auto g_iDataSize { 10 };
constexpr auto IDC_LIST_MENU_HDR_BEGIN = 0x5;

BEGIN_MESSAGE_MAP(CListExSampleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_GETDISPINFOW, IDC_LISTEX, &CListExSampleDlg::OnListGetDispInfo)
	ON_NOTIFY(LISTEX_MSG_GETCOLOR, IDC_LISTEX, &CListExSampleDlg::OnListGetColor)
	ON_NOTIFY(LISTEX_MSG_GETICON, IDC_LISTEX, &CListExSampleDlg::OnListGetIcon)
	ON_NOTIFY(LISTEX_MSG_GETTOOLTIP, IDC_LISTEX, &CListExSampleDlg::OnListGetToolTip)
	ON_NOTIFY(LISTEX_MSG_HDRICONCLICK, IDC_LISTEX, &CListExSampleDlg::OnListHdrIconClick)
	ON_NOTIFY(LISTEX_MSG_HDRRBTNUP, IDC_LISTEX, &CListExSampleDlg::OnListHdrRClick)
	ON_NOTIFY(LISTEX_MSG_LINKCLICK, IDC_LISTEX, &CListExSampleDlg::OnListLinkClick)
	ON_NOTIFY(LISTEX_MSG_SETDATA, IDC_LISTEX, &CListExSampleDlg::OnListSetData)
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

	LISTEXCREATE lcs;
	lcs.uID = IDC_LISTEX;
	lcs.pParent = this;
	lcs.fDialogCtrl = true;
	lcs.dwHdrHeight = 30;
	lcs.fSortable = true;

	LISTEXCOLORS stColor;
	stColor.clrHdrText = RGB(250, 250, 250);
	lcs.pColors = &stColor;

	m_pList->Create(lcs);
	m_pList->SetExtendedStyle(LVS_EX_HEADERDRAGDROP);
	m_pList->InsertColumn(0, L"Test column 0\n Multiline", LVCFMT_CENTER, 200, -1, LVCFMT_CENTER);

	//Header menu.
	m_menuHdr.CreatePopupMenu();
	m_menuHdr.AppendMenuW(MF_STRING, IDC_LIST_MENU_HDR_BEGIN, L"Test column 0");
	m_menuHdr.CheckMenuItem(IDC_LIST_MENU_HDR_BEGIN, MF_CHECKED | MF_BYCOMMAND);

	for (int i = 1; i < g_iColumns; ++i) {
		const auto wstrName = std::wstring(L"Test column ") + std::to_wstring(i);
		m_pList->InsertColumn(i, wstrName.data(), LVCFMT_CENTER, 200, -1, LVCFMT_CENTER, true);
		m_pList->SetHdrColumnColor(i, RGB(200, 200, 200));
		m_menuHdr.AppendMenuW(MF_STRING, IDC_LIST_MENU_HDR_BEGIN + i, wstrName.data());
		m_menuHdr.CheckMenuItem(IDC_LIST_MENU_HDR_BEGIN + i, MF_CHECKED | MF_BYCOMMAND);
	}

	m_pList->SetHdrColumnColor(0, RGB(70, 70, 70));
	//	m_pList->SetHdrColumnColor(1, RGB(125, 125, 125));
	//	m_pList->SetHdrColumnColor(2, RGB(200, 200, 200));
	//	m_myList->SetColumnSortMode(0, false);

	//For Virtual list.
	//Sample data for Virtual mode (LVS_OWNERDATA).
	std::srand(static_cast<unsigned>(std::time(0)));
	for (unsigned i = 0; i < g_iDataSize; ++i) {
		m_vecData.emplace_back(VIRTLISTDATA {
			L"<link=\"0\" title=\"Custom title\">Column:0</link>/"
			L"<link=\"" + std::to_wstring(i) + L"\">row:" + std::to_wstring(i) + L"</link>",
			//Some random numbers at the beginning, for checking the sorting.
			L"[" + std::to_wstring(std::rand()) + L"] " + L"Column:1/row:" + std::to_wstring(i),
			L"Column:2/row:" + std::to_wstring(i),
			i == 2, i == 7, i == 1,
			i == 7 ? LISTEXCOLOR { RGB(0, 220, 0) } : LISTEXCOLOR { } //Row number 7 (for all columns) colored to RGB(0, 220, 0).
			});
	}
	m_pList->SetItemCountEx(g_iRows, LVSICF_NOSCROLL); //Amount of Virtual items.

	//For classical list.
/*	m_pList->InsertItem(0, L"Test item - row:0/column:0");
	m_pList->InsertItem(1, L"Test item - row:1/column:0.");
	m_pList->InsertItem(2, L"Test item - row:2/column:0..");
	m_pList->InsertItem(3, L"Test item - row:3/column:0...");
	m_pList->InsertItem(4, L"Test item - row:4/column:0....");
	m_pList->SetItemText(0, 1, L"Test item - row:0/column:1....");
	m_pList->SetItemText(1, 1, L"Test item - row:1/column:1...");
	m_pList->SetItemText(2, 1, L"Test item - row:2/column:1..");
	m_pList->SetItemText(3, 1, L"Test item - row:3/column:1.");
	m_pList->SetItemText(4, 1, L"Test item - row:4/column:1");
	m_pList->SetItemText(0, 2, L"Test item - row:0/column:2...");
	m_pList->SetItemText(1, 2, L"Test item - row:1/column:2.");
	m_pList->SetItemText(2, 2, L"Test item - row:2/column:2....");
	m_pList->SetItemText(3, 2, L"Test item - row:3/column:2..");
	m_pList->SetItemText(4, 2, L"Test item - row:4/column:2.....");

	m_pList->SetCellColor(2, 0, GetSysColor(COLOR_GRADIENTINACTIVECAPTION));
	m_pList->SetCellColor(3, 1, GetSysColor(COLOR_GRADIENTACTIVECAPTION));
	m_pList->SetCellColor(4, 2, RGB(255, 255, 0));
	m_pList->SetRowColor(7, RGB(0, 220, 0));
	m_pList->SetColumnColor(1, RGB(0, 220, 220));
	m_pList->SetCellTooltip(0, 0, L"Tooltip text...", L"Caption of the tooltip:");
	*/

	m_stImgList.Create(16, 16, ILC_COLOR | ILC_MASK, 0, 1);
	m_stImgList.Add(static_cast<HICON>(LoadImageW(AfxGetInstanceHandle(),
		MAKEINTRESOURCEW(IDI_TEST), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR)));
	m_pList->SetImageList(&m_stImgList, LVSIL_NORMAL);
	m_pList->SetHdrImageList(&m_stImgList);

	LISTEXHDRICON stHdrIcon;
	stHdrIcon.iIndex = 0;
	stHdrIcon.pt.x = 4;
	stHdrIcon.pt.y = 4;
	m_pList->SetHdrColumnIcon(0, stHdrIcon);

	return TRUE;
}

void CListExSampleDlg::OnPaint()
{
	if (IsIconic()) {
		CPaintDC dc(this); // device context for painting

		SendMessageW(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
	else {
		CDialogEx::OnPaint();
	}
}

HCURSOR CListExSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CListExSampleDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	const auto wMenuID = LOWORD(wParam);
	if (wMenuID < IDC_LIST_MENU_HDR_BEGIN || wMenuID > IDC_LIST_MENU_HDR_BEGIN + g_iColumns)
		return CDialogEx::OnCommand(wParam, lParam);

	const auto uState = m_menuHdr.GetMenuState(wMenuID, MF_BYCOMMAND);

	if (uState & MF_CHECKED) {
		m_menuHdr.CheckMenuItem(wMenuID, MF_UNCHECKED | MF_BYCOMMAND);
		m_pList->HideColumn(wMenuID - IDC_LIST_MENU_HDR_BEGIN, true);
	}
	else {
		m_menuHdr.CheckMenuItem(wMenuID, MF_CHECKED | MF_BYCOMMAND);
		m_pList->HideColumn(wMenuID - IDC_LIST_MENU_HDR_BEGIN, false);
	}

	return CDialogEx::OnCommand(wParam, lParam);
}

BOOL CListExSampleDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	const auto pNMI = reinterpret_cast<LPNMITEMACTIVATE>(lParam);

	if (pNMI->hdr.idFrom == IDC_LISTEX) {
		switch (pNMI->hdr.code) {
		case LVN_COLUMNCLICK:
			SortVecData();
			return TRUE; //Disable further message processing.
		}
	}

	return CDialogEx::OnNotify(wParam, lParam, pResult);
}

void CListExSampleDlg::OnOK()
{
}

void CListExSampleDlg::OnListGetDispInfo(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	const auto pDispInfo = reinterpret_cast<NMLVDISPINFOW*>(pNMHDR);
	const auto pItem = &pDispInfo->item;
	const auto iItem = pItem->iItem;
	if (iItem >= g_iDataSize)
		return;

	if ((pItem->mask & LVIF_TEXT) == 0)
		return;

	switch (pItem->iSubItem) {
	case 0:
		pItem->pszText = m_vecData[static_cast<size_t>(iItem)].wstr0.data();
		break;
	case 1:
		pItem->pszText = m_vecData[static_cast<size_t>(iItem)].wstr1.data();
		break;
	case 2:
		pItem->pszText = m_vecData[static_cast<size_t>(iItem)].wstr2.data();
		break;
	}
}

void CListExSampleDlg::OnListGetColor(NMHDR* pNMHDR, LRESULT* pResult)
{
	//Virtual data colors.
	const auto pLCI = reinterpret_cast<PLISTEXCOLORINFO>(pNMHDR);
	if (pLCI->iItem < 0 || pLCI->iSubItem < 0)
		return;

	if (pLCI->iItem >= g_iDataSize)
		return;

	if (pLCI->iSubItem == 1) { //Column number 1 (for all rows) colored to RGB(0, 220, 220).
		pLCI->stClr = { RGB(0, 220, 220), RGB(0, 0, 0) };
		*pResult = TRUE;
		return;
	}

	if (m_vecData.at(static_cast<size_t>(pLCI->iItem)).fColor) {
		pLCI->stClr = m_vecData[static_cast<size_t>(pLCI->iItem)].clr;
		*pResult = TRUE;
		return;
	}
}

void CListExSampleDlg::OnListGetIcon(NMHDR* pNMHDR, LRESULT* pResult)
{
	//Virtual data icons.
	const auto pLII = reinterpret_cast<PLISTEXICONINFO>(pNMHDR);
	if (pLII->iItem < 0 || pLII->iSubItem < 0)
		return;

	const auto index = pLII->iItem < g_iDataSize ? pLII->iItem : 1;
	if (m_vecData.at(static_cast<size_t>(index)).fIcon && pLII->iSubItem == 1) {
		pLII->iIconIndex = 0; //Icon index in the list's image list.
		*pResult = TRUE;
		return;
	}
}

void CListExSampleDlg::OnListGetToolTip(NMHDR* pNMHDR, LRESULT* pResult)
{
	//Virtual data tooltips.
	const auto pTTI = reinterpret_cast<PLISTEXTTINFO>(pNMHDR);
	const auto iItem = pTTI->iItem;
	if (iItem < 0 || pTTI->iSubItem < 0 || iItem >= g_iDataSize)
		return;

	if (m_vecData.at(static_cast<size_t>(iItem)).fToolTip && pTTI->iSubItem == 0) {
		static constexpr const wchar_t* ttData[] { L"Tooltip text...", L"Caption of the tooltip:" };
		pTTI->stData.pwszText = ttData[0];
		pTTI->stData.pwszCaption = ttData[1];
		*pResult = TRUE; //TRUE means that we set a tooltip for the given cell.
		return;
	}
}

void CListExSampleDlg::OnListHdrIconClick(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	const auto pNMI = reinterpret_cast<NMHEADERW*>(pNMHDR);
	const auto wstr = L"Header icon clicked at column: " + std::to_wstring(pNMI->iItem);
	MessageBoxW(wstr.data());
}

void CListExSampleDlg::OnListHdrRClick(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	CPoint pt;
	GetCursorPos(&pt);
	m_menuHdr.TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
}

void CListExSampleDlg::OnListLinkClick(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	const auto pLLI = reinterpret_cast<PLISTEXLINKINFO>(pNMHDR);
	MessageBoxW(pLLI->pwszText);
}

void CListExSampleDlg::OnListSetData(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	//Changing virtual data in internal m_vecData.
	const auto pLDI = reinterpret_cast<PLISTEXDATAINFO>(pNMHDR);
	auto& ref = m_vecData[pLDI->iItem];

	switch (pLDI->iSubItem) {
	case 0:
		ref.wstr0 = pLDI->pwszData;
		break;
	case 1:
		ref.wstr1 = pLDI->pwszData;
		break;
	case 2:
		ref.wstr2 = pLDI->pwszData;
		break;
	default:
		break;
	}
}

void CListExSampleDlg::SortVecData()
{
	const auto iColumnIndex = m_pList->GetSortColumn();
	if (iColumnIndex < 0)
		return;

	//Sorts the vector of data according to clicked column.
	std::sort(m_vecData.begin(), m_vecData.end(), [&](const VIRTLISTDATA& st1, const VIRTLISTDATA& st2) {
		int iCompare { };
		switch (iColumnIndex) {
		case 0:
			iCompare = st1.wstr0.compare(st2.wstr0);
			break;
		case 1:
			iCompare = st1.wstr1.compare(st2.wstr1);
			break;
		case 2:
			iCompare = st1.wstr2.compare(st2.wstr2);
			break;
		}

		bool result { false };
		if (m_pList->GetSortAscending()) {
			if (iCompare < 0)
				result = true;
		}
		else {
			if (iCompare > 0)
				result = true;
		}

		return result;
		});

	m_pList->RedrawWindow();
}