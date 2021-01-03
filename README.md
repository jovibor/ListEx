## **List control for MFC applications**
![](docs/img/listex_mainwndtooltip.jpg)

## Table of Contents
* [Introduction](#introduction)
* [Installation](#installation)
* [Create](#create)
    * [Manually](#manually)
    * [In Dialog](#in-dialog)
* [Tooltips](#tooltips)
* [Menu](#menu)
    * [Menu Example](#menu-example)
    * [Handle Menu Clicks](#handle-menu-clicks)
* [Sorting](#sorting)
   * [mode](#virtual-mode)
* [Public Methods](#public-methods)
   * [SetSortable](#setsortable)
* [Structures](#structures)
   * [LISTEXCREATESTRUCT](#listexcreatestruct)
   * [LISTEXCOLORS](#listexcolors)
   * [LISTEXCELLCOLOR](#listexcellcolor)
   * [EListExSortMode](#elistexsortmode)
* [Notification Messages](#notification-messages) <details><summary>_Expand_</summary>
   * [LISTEX_MSG_MENUSELECTED](#listex_msg_menuselected)
   * [LISTEX_MSG_GETCOLOR](#listex_msg_getcolor)
   * [LISTEX_MSG_GETICON](#listex_msg_geticon)
   * [LISTEX_MSG_LINKCLICK](#listex_msg_linkclick)
* [Example](#example)
* [Appearance](#appearance)

## [](#)Introduction
`IListEx` class is basically an extension of the standard **MFC** [`CMFCListCtrl`](#https://docs.microsoft.com/en-us/cpp/mfc/reference/cmfclistctrl-class?view=vs-2019#sort) class with many improvements.

### The main features of the IListEx:
* Set tooltips for individual cells that show up on mouse hover
* Set popup menu for the individual cells as well as for the whole list control
* Set background and text color for individual cells
* Set additional item data for individual cells
* Many options to set individual colors for lots of list aspects with [`LISTEXCOLORSTRUCT`](#listexcolors)
* Set header height and font, as well as color for individual header columns
* Innate ability to sort list columns with no additional efforts
* Dynamically change list font size with <kbd>Ctrl</kbd>+<kbd>MouseWheel</kbd>

## [](#)Installation
The usage of the control is quite simple:
1. Copy *ListEx* folder into your project's folder.
2. Add all files from *ListEx* folder into your project.
3. Add `#include "ListEx/ListEx.h"` where you suppose to use the control.
4. Declare `IListExPtr` variable: `IListExPtr myList { CreateListEx() };`

`IListExPtr` is, in fact, a pointer to the `IListEx` pure abstract base class, wrapped either in `std::unique_ptr` or `std::shared_ptr`. You can choose whatever is best for you by comment/uncomment one of this alliases in `ListEx.h`:
```cpp
	//using IListExPtr = IListExUnPtr;
	using IListExPtr = IListExShPtr;
```
This wrapper is used mainly for convenience, so you don't have to bother about object lifetime, it will be destroyed automatically.
That's why there is a call to the factory function `CreateListEx()`, to properly initialize a pointer.<br>

Control uses its own namespace `LISTEX`. So it's up to you, whether to use namespace prefix before declarations: 
```cpp
LISTEX::
```
or to define namespace in the source file's beginning:
```cpp
using namespace LISTEX;
```

## [](#)Create
### [](#)Manually
`Create` is the main method to create list control. It takes reference to the [`LISTEXCREATESTRUCT`](#listexcreatestruct) structure.

Below is a simple example of the **control**'s creation:
```cpp
IListExPtr myList { CreateListEx() };
.
.
LISTEXCREATESTRUCT lcs;
lcs.pwndParent = this;
lcs.nID = ID_MY_LIST;
lcs.rect = CRect(0, 0, 500, 300);

myList->Create(lcs);
```
With `LISTEXCREATESTRUCT` structure you can adjust a plethora of list’s aspects:

* Color of the list text and bk (background). Bk is set separately for odd and even rows
* Color of the list header
* Height of the list header
* Font of the list header, and font of list itself
* Color of individual header's columns
* Color of tooltip's window text and bk
* Color of the text and bk of a cell that has tooltip
* Color of list grid, and even its width
* Color of row when it's selected
* Make list sortable

### [](#)In Dialog
To create the control in a *Dialog* you can manually do it with the [Create](#manually) method.

But most of the times you prefer to place a standard *List Control* onto the *Dialog*'s template, by dragging it from the **Toolbox** within **Visual studio**.  
To use the latter approach follow these steps:
1. Put standard *List Control* from the toolbox onto your dialog template. Give it appropriate ID (`IDC_LISTEX`) and make it desirable size.
2. Declare `IListExPtr` member varable within your dialog class: `IListExPtr m_myList { CreateListEx() };`
3. In your `OnInitDialog` method call `m_myList->CreateDialogCtrl(IDC_LISTEX, this);` function.

## [](#)Tooltips
To set a tooltip for a given cell, just write:

```cpp
myList->SetCellTooltip(0, 1, L"Tooltip text", L"Tooltip caption:");
```
This will set a tooltip for cell (0, 1) with the text: **_Tooltip text_**, and the caption **_Tooltip caption_**.

## [](#)Menu
`IListEx` class possesses innate ability to set popup menu for individual cells, as well as for the whole list.

![](docs/img/listex_mainwndmenu.jpg)

This is achieved with the help of two public methods:
```cpp
SetListMenu(CMenu* pMenu);
SetCellMenu(int iItem, int iSubitem, CMenu* pMenu);
```
### [](#)Menu Example
With the code below, we are going to set two separate menus:

* For the whole list - `CMenu m_menuList;`<br>
* For the individual cell - `CMenu m_menuCell;`
```cpp
//MyDlg.h
#include "ListEx/ListEx.h"

using namespace LISTEX;

constexpr auto IDC_LIST_MENU_CELL_FIRSTx1;
constexpr auto IDC_LIST_MENU_CELL_SECONDx2;
constexpr auto IDC_LIST_MENU_GLOBAL_FIRSTx3;
constexpr auto IDC_LIST_MENU_GLOBAL_SECONDx4;

class CMyDlg : public CDialogEx
{
private:
  CMenu m_menuCell;
  CMenu m_menuList;
  IListExPtr m_myList { CreateListEx() };
  //...
}

//MyDlg.cpp
CMyDlg::OnInitDialog()
{
  m_myList->CreateDialogCtrl();
  m_myList->InsertColumn(0, L"First column", 0, 100);
  m_myList->InsertColumn(...);
  m_myList->InsertItem(...);. 
  m_myList->SetItemText(...);

  m_menuCell.CreatePopupMenu();
  m_menuCell.AppendMenuW(MF_STRING, IDC_LIST_MENU_CELL_FIRST, L"Cell's first menu...");
  m_menuCell.AppendMenuW(MF_STRING, IDC_LIST_MENU_CELL_SECOND, L"Cell's second menu...");

  m_menuList.CreatePopupMenu();
  m_menuList.AppendMenuW(MF_STRING, IDC_LIST_MENU_GLOBAL_FIRST, L"List's first menu...");
  m_menuList.AppendMenuW(MF_STRING, IDC_LIST_MENU_GLOBAL_SECOND, L"List's second menu...");

  m_myList->SetListMenu(&m_menuList);
  m_myList->SetCellMenu(1, 0, &m_menuCell); //Set menu for row:1 column:0.
}
```
### [](#)Handle Menu Clicks
When user clicks a menu `IListEx` sends `WM_NOTIFY` message, with `NMITEMACTIVATE` struct pointer as `lParam`, to its parent window. So, in order to properly handle clicks you have to process this message in your list's parent window:

```cpp
BOOL CMyDialog::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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
```
`IListEx` fills `NMITEMACTIVATE` struct with `NMITEMACTIVATE::hdr.code` equals `LISTEX_MSG_MENUSELECTED`. And `menuId` is stored as `NMITEMACTIVATE::lParam`.

`NMITEMACTIVATE::iItem` and `NMITEMACTIVATE::iSubItem` both point to a cell the menu was clicked on.

## [](#)Sorting
To enable sorting set the [`LISTEXCREATESTRUCT::fSortable`](#listexcretestruct) flag to true. In this case, when you click on the header, list will be sorted according to the clicked column. By default `IListEx` performs lexicographical sorting.

To set your own sorting routine use [`SetSortable`](#setsortable) method. 

### [](#)mode
In list mode, if created with `LVS_OWNERDATA` style, `IListEx` will notify, in form of `WM_NOTIFY` message, parent window with `NMHDR::code` equal to `LVN_COLUMNCLICK` when user clicked header column.

`LVM_MAPINDEXTOID` message code will be sent to notify parent window that `IListEx::MapIndexToID` method was called internally. Parent window in this case is responsible to provide unique IDs for list items. This is very important for cells individual colors, tool-tips, menu and custom data to work properly in mode.  
Unique ID must be returned in form of `LPNMITEMACTIVATE::lParam`
```cpp
BOOL CMyDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT * pResult)
{
    const LPNMITEMACTIVATE pNMI = reinterpret_cast<LPNMITEMACTIVATE>(lParam);

    if (pNMI->hdr.idFrom == IDC_LISTEX)
    {
    	switch (pNMI->hdr.code)
    	{
    	case LVM_MAPINDEXTOID:
    		pNMI->lParam = //Unique ID of the pNMI->iItem.
    	break;
    	//...
    	}
    }
}
```

## [](#)Public Methods
`IListEx` class also has a set of additional public methods to help customize your control in many different aspects.
```cpp
bool Create(const LISTEXCREATESTRUCT& lcs);
void CreateDialogCtrl(UINT uCtrlID, CWnd* pwndDlg);
BOOL DeleteAllItems();
BOOL DeleteColumn(int nCol);
BOOL DeleteItem(int nItem);
void Destroy();
[[nodiscard]] ULONGLONG GetCellData(int iItem, int iSubitem)const;
[[nodiscard]] LISTEXCOLORS GetColors()const;
[[nodiscard]] EListExSortMode GetColumnSortMode(int iColumn)const;
[[nodiscard]] UINT GetFontSize()const;
[[nodiscard]] int GetSortColumn()const;
[[nodiscard]] bool GetSortAscending()const;
[[nodiscard]] bool IsCreated()const;
void SetCellColor(int iItem, int iSubitem, COLORREF clrBk, COLORREF clrText = -1);
void SetCellData(int iItem, int iSubitem, ULONGLONG ullData);
void SetCellMenu(int iItem, int iSubitem, CMenu* pMenu);
void SetCellTooltip(int iItem, int iSubitem, std::wstring_view wstrTooltip, std::wstring_view wstrCaption = L"");
void SetColors(const LISTEXCOLORS& lcs);
void SetColumnColor(int iColumn, COLORREF clrBk, COLORREF clrText = -1);
void SetColumnSortMode(int iColumn, EListExSortMode enSortMode);
void SetFont(const LOGFONTW* pLogFontNew);
void SetFontSize(UINT uiSize);
void SetHdrHeight(DWORD dwHeight);
void SetHdrFont(const LOGFONTW* pLogFontNew);
void SetHdrColumnColor(int iColumn, COLORREF clrBk, COLORREF clrText = -1);
void SetListMenu(CMenu* pMenu);
void SetRowColor(DWORD dwRow, COLORREF clrBk, COLORREF clrText = -1);
void SetSortable(bool fSortable, PFNLVCOMPARE pfnCompare = nullptr, EListExSortMode enSortMode = EListExSortMode::SORT_LEX);
```

### [](#)SetSortable
```cpp
void SetSortable(bool fSortable, PFNLVCOMPARE pfnCompare = nullptr, EListExSortMode enSortMode = EListExSortMode::SORT_LEX)
```
**Parameters:**  

`bool fSortable`  
Enables or disables sorting

`PFNLVCOMPARE pfnCompare`  
Callback function pointer with type `int (CALLBACK *PFNLVCOMPARE)(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)` that is used to set your own comparison function. If it's `nullptr` `IListEx` performs default sorting.  
The comparison function must be either a static member of a class or a stand-alone function that is not a member of any class. For more information see official [MSDN documentation](https://docs.microsoft.com/en-us/cpp/mfc/reference/clistctrl-class?view=vs-2019#remarks-100).

`EListExSortMode enSortMode`  
Default sorting mode for the list.

## [](#)Structures

### [](#)LISTEXCREATESTRUCT
```cpp
struct LISTEXCREATESTRUCT
{
    LISTEXCOLORS stColor { };             //All control's colors.
    CRect        rect;                    //Initial rect.
    CWnd*        pParent { };             //Parent window.
    LOGFONTW*    pListLogFont { };        //List font.
    LOGFONTW*    pHdrLogFont { };         //Header font.
    UINT         uID { };                 //List control ID.
    DWORD        dwStyle { };             //Control's styles. Zero for default.
    DWORD        dwListGridWidth { 1 };   //Width of the list grid.
    DWORD        dwHdrHeight { 20 };      //Header height.
    bool         fDialogCtrl { false };   //If it's a list within dialog.
    bool         fSortable { false };     //Is list sortable, by clicking on the header column?
    bool         fLinkUnderline { true }; //Links are displayed underlined or not.
    bool         fLinkTooltip { true };   //Show links' toolips.
};
```

### [](#)LISTEXCOLORS
```cpp
struct LISTEXCOLORS
{
    COLORREF clrListText { GetSysColor(COLOR_WINDOWTEXT) };       //List text color.
    COLORREF clrListTextLink { RGB(0, 0, 200) };                  //List hyperlink text color.
    COLORREF clrListTextSel { GetSysColor(COLOR_HIGHLIGHTTEXT) }; //Selected item text color.
    COLORREF clrListTextLinkSel { RGB(250, 250, 250) };           //List hyperlink text color in selected cell.
    COLORREF clrListTextCellTt { GetSysColor(COLOR_WINDOWTEXT) }; //Text color of a cell that has tooltip.
    COLORREF clrListBkRow1 { GetSysColor(COLOR_WINDOW) };         //List Bk color of the odd rows.
    COLORREF clrListBkRow2 { GetSysColor(COLOR_WINDOW) };         //List Bk color of the even rows.
    COLORREF clrListBkSel { GetSysColor(COLOR_HIGHLIGHT) };       //Selected item bk color.
    COLORREF clrListBkCellTt { RGB(170, 170, 230) };              //Bk color of a cell that has tooltip.
    COLORREF clrListGrid { RGB(220, 220, 220) };                  //List grid color.
    COLORREF clrTooltipText { GetSysColor(COLOR_INFOTEXT) };      //Tooltip window text color.
    COLORREF clrTooltipBk { GetSysColor(COLOR_INFOBK) };          //Tooltip window bk color.
    COLORREF clrHdrText { GetSysColor(COLOR_WINDOWTEXT) };        //List header text color.
    COLORREF clrHdrBk { GetSysColor(COLOR_WINDOW) };              //List header bk color.
    COLORREF clrHdrHglInact { GetSysColor(COLOR_GRADIENTINACTIVECAPTION) };//Header highlight inactive.
    COLORREF clrHdrHglAct { GetSysColor(COLOR_GRADIENTACTIVECAPTION) };    //Header highlight active.
    COLORREF clrNWABk { GetSysColor(COLOR_WINDOW) };              //Bk of Non Working Area.
};
```
This struct is also used in `SetColor` method.

### [](#)LISTEXGETCOLOR
```cpp
struct LISTEXCELLCOLOR
{
	COLORREF clrBk { };
	COLORREF clrText { };
};
using PLISTEXCELLCOLOR = LISTEXCELLCOLOR*;
```
Used as a response to `LISTEX_MSG_GETCOLOR` message.

### [](#)LISTEXGETICON
```cpp
void CListDlg::OnListExGetIcon(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
    //Virtual data icons.
    const auto pNMI = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
    ...
	
    pNMI->lParam = SomeIndex; //Icon index in list's image list.
}
```
This message is used in Virtual List mode to obtain icon index in list image list.

### [](#)EListExSortMode
Enum showing sorting type for list columns.
```cpp
enum class EListExSortMode : short
{
    SORT_LEX, SORT_NUMERIC
};
```

## [](#)Notification Messages
These messages are sent to the parent window in form of `WM_NOTIFY` windows message.  
The `lParam` will contain pointer to `NMHDR` standard windows struct. `NMHDR::code` can be one of the `LISTEX_MSG_...` messages described below.

### [](#)LISTEX_MSG_MENUSELECTED
User defined menu selected. See [menu](#menu) section.

### [](#)LISTEX_MSG_CELLCOLOR
Sent to the parent to retrieve current cell color. You can set cells' colors whether handling this message or using [`SetCellColor`](#setcolor) member function.  
This message has higher priority over `SetCellColor` method.

### [](#)LISTEX_MSG_LINKCLICK
List embedded hyperlink has been clicked. `WM_NOTIFY` `lParam` will point to the `NMITEMACTIVATE` struct.  
`NMITEMACTIVATE::lParam` will contain `wchar_t*` pointer to the `link` text of the clicked hyperlink. The `iItem` and `iSubItem` members will contain indexes of the list item/subitem the link was clicked at. 

Hyperlink syntax is: `L"Text with the <link="any_text_here" title="Optional tool-tip text">embedded link</link>"`  
If no optional `title` tag is provided then the `link` text itself will be used as hyperlink's tool-tip.  
Link and title text must be quoted `""`.

## [](#)Example
Let’s imagine that you need a list control with a non standard header height, and yellow background color.
Nothing is simpler, see code below:
```cpp
LISTEXCREATESTRUCT lcs;
lcs.rect = CRect(0, 0, 500, 300)
lcs.pwndParent = this;
lcs.dwHdrHeight = 50;
lcs.stColor.clrListBkRow1 = RGB(255, 255, 0);
lcs.stColor.clrListBkRow2 = RGB(255, 255, 0);

myList->Create(lcs);

myList->InsertColumn(...);
myList->InsertItem(...);
```
Here, we set both - even and odd rows (`clrListBkRow1` and `clrListBkRow2`) to the same yellow color.

## [](#)Appearance
With the **Ctrl+MouseWheel** combination you can dynamically change list's font size.