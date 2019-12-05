#include "pch.h"
#include "framework.h"
#include "ListExSample.h"
#include "ListExSampleDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CListExSampleDlg::CListExSampleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LISTEXSAMPLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CListExSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CListExSampleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

BOOL CListExSampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_myList->CreateDialogCtrl(IDC_LISTEX, this);
	m_myList->SetHeaderHeight(25);

	LISTEXCOLORSTRUCT lcs;
//	lcs.clrHeaderText = RGB(250, 250, 250);
	m_myList->SetColor(lcs);

	m_myList->InsertColumn(0, L"Test column 0", 0, 200);
//	m_myList->SetHeaderColumnColor(0, RGB(50, 50, 50));
	m_myList->InsertItem(0, L"Test item - row:0/column:0");
	m_myList->InsertItem(1, L"Test item - row:1/column:0");
	m_myList->InsertItem(2, L"Test item - row:2/column:0");
	m_myList->InsertItem(3, L"Test item - row:3/column:0");
	m_myList->InsertItem(4, L"Test item - row:4/column:0");
	m_myList->SetCellTooltip(0, 0, L"Tooltip text...", L"Caption of the tooltip:");

	m_myList->InsertColumn(1, L"Test column 1", 0, 200);
	m_myList->SetHeaderColumnColor(1, RGB(125, 125, 125));
	m_myList->SetItemText(0, 1, L"Test item - row:0/column:1");
	m_myList->SetItemText(1, 1, L"Test item - row:1/column:1");
	m_myList->SetItemText(2, 1, L"Test item - row:2/column:1");
	m_myList->SetItemText(3, 1, L"Test item - row:3/column:1");
	m_myList->SetItemText(4, 1, L"Test item - row:4/column:1");

	m_myList->InsertColumn(2, L"Test column 2", 0, 200);
	m_myList->SetHeaderColumnColor(2, RGB(200, 200, 200));
	m_myList->SetItemText(0, 2, L"Test item - row:0/column:2");
	m_myList->SetItemText(1, 2, L"Test item - row:1/column:2");
	m_myList->SetItemText(2, 2, L"Test item - row:2/column:2");
	m_myList->SetItemText(3, 2, L"Test item - row:3/column:2");
	m_myList->SetItemText(4, 2, L"Test item - row:4/column:2");

	m_menuCell.CreatePopupMenu();
	m_menuCell.AppendMenuW(MF_STRING, IDC_LIST_MENU_CELL_FIRST, L"Cell's first menu...");
	m_menuCell.AppendMenuW(MF_STRING, IDC_LIST_MENU_CELL_SECOND, L"Cell's second menu...");

	m_menuList.CreatePopupMenu();
	m_menuList.AppendMenuW(MF_STRING, IDC_LIST_MENU_GLOBAL_FIRST, L"List's first menu...");
	m_menuList.AppendMenuW(MF_STRING, IDC_LIST_MENU_GLOBAL_SECOND, L"List's second menu...");

	m_myList->SetListMenu(&m_menuList);
	m_myList->SetCellMenu(1, 0, &m_menuCell); //Set menu for row:1 column:0.
	m_myList->SetCellColor(1, 0, GetSysColor(COLOR_GRADIENTINACTIVECAPTION));
	m_myList->SetCellColor(1, 1, GetSysColor(COLOR_GRADIENTACTIVECAPTION));
	m_myList->SetCellColor(4, 2, RGB(255, 255, 0));

	return TRUE;
}

void CListExSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

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

BOOL CListExSampleDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT * pResult)
{
	const LPNMITEMACTIVATE pNMI = reinterpret_cast<LPNMITEMACTIVATE>(lParam);

	if (pNMI->hdr.idFrom == IDC_LISTEX)
	{
		if (pNMI->hdr.code == LISTEX_MSG_MENUSELECTED)
		{
			CString ss;
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
	}

	return CDialogEx::OnNotify(wParam, lParam, pResult);
}
