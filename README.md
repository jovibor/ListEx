## **List control for MFC applications**
![](docs/img/listex_mainwndtooltip.jpg)

## Table of Contents
* [Introduction](#introduction)
* [Installation](#installation)
* [Create](#create)
    * [Manually](#manually)
    * [In Dialog](#in-dialog)
* [Tooltips](#tooltips)
* [Sorting](#sorting)
* [Public Methods](#public-methods)
   * [SetHdrColumnIcon](#sethdrcolumnicon)
   * [SetSortable](#setsortable)
* [Structures](#structures)
   * [LISTEXCREATE](#listexcreate)
   * [LISTEXCOLORS](#listexcolors)
   * [LISTEXCOLOR](#listexcolor)
   * [LISTEXHDRICON](#listexhdricon)
   * [EListExSortMode](#elistexsortmode)
* [Notification Messages](#notification-messages) <details><summary>_Expand_</summary>
   * [LISTEX_MSG_GETCOLOR](#listex_msg_getcolor)
   * [LISTEX_MSG_GETICON](#listex_msg_geticon)
   * [LISTEX_MSG_GETTOOLTIP](#)
   * [LISTEX_MSG_LINKCLICK](#listex_msg_linkclick)
   * [LISTEX_MSG_HDRICONCLICK](#listex_msg_hdriconclick)
   * [LISTEX_MSG_HDRRBTNDOWN](#)
   * [LISTEX_MSG_HDRRBTNUP](#)
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

`IListExPtr` is a pointer to the `IListEx` class wrapped in `std::unique_ptr`.
This wrapper is used mainly for convenience, so you don't have to bother about object lifetime, it will be destroyed automatically.
That's why there is a call to the factory function `CreateListEx()`, to properly initialize a pointer.

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
`Create` is the main method to create list control. It takes reference to the [`LISTEXCREATE`](#listexcreate) structure.

Below is a simple example of the **control**'s creation:
```cpp
IListExPtr myList { CreateListEx() };
.
.
LISTEXCREATE lcs;
lcs.pParent = this;
lcs.uID = ID_MY_LIST;
lcs.rect = CRect(0, 0, 500, 300);

myList->Create(lcs);
```
With `LISTEXCREATE` structure you can adjust a plethora of list’s aspects:

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

## [](#)Sorting
To enable sorting set the [`LISTEXCREATE::fSortable`](#listexcreate) flag to true. In this case, when you click on the header, list will be sorted according to the clicked column. By default `IListEx` performs lexicographical sorting.

To set your own sorting routine use [`SetSortable`](#setsortable) method. 

## [](#)Public Methods
`IListEx` class also has a set of additional public methods to help customize your control in many different aspects.
```cpp
bool Create(const LISTEXCREATE& lcs);
void CreateDialogCtrl(UINT uCtrlID, CWnd* pwndDlg);
BOOL DeleteAllItems();
BOOL DeleteColumn(int nCol);
BOOL DeleteItem(int nItem);
void Destroy();
[[nodiscard]] ULONGLONG GetCellData(int iItem, int iSubitem)const;
[[nodiscard]] LISTEXCOLORS GetColors()const;
[[nodiscard]] EListExSortMode GetColumnSortMode(int iColumn)const;
[[nodiscard]] int GetSortColumn()const;
[[nodiscard]] bool GetSortAscending()const;
void HideColumn(int iIndex, bool fHide);
[[nodiscard]] bool IsCreated()const;
void ResetSort();
void SetCellColor(int iItem, int iSubitem, COLORREF clrBk, COLORREF clrText = -1);
void SetCellData(int iItem, int iSubitem, ULONGLONG ullData);
void SetCellTooltip(int iItem, int iSubitem, std::wstring_view wstrTooltip, std::wstring_view wstrCaption = L"");
void SetColors(const LISTEXCOLORS& lcs);
void SetColumnColor(int iColumn, COLORREF clrBk, COLORREF clrText = -1);
void SetColumnSortMode(int iColumn, bool fSortable, EListExSortMode enSortMode = { });
void SetFont(const LOGFONTW* pLogFontNew);
void SetHdrHeight(DWORD dwHeight);
void SetHdrFont(const LOGFONTW* pLogFontNew);
void SetHdrColumnColor(int iColumn, COLORREF clrBk, COLORREF clrText = -1);
void SetHdrColumnIcon(int iColumn, const LISTEXHDRICON& stIcon);
void SetHdrImageList(CImageList* pList);
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

### [](#)SetHdrColumnIcon
```cpp
void SetHdrColumnIcon(int iColumn, int iIconIndex, bool fClick = false);
```
Sets the icon index in the header's image list for a given `iColumn`. To remove icon from column set the `iIconIndex` to `-1`.  
Flag `fClick` means that icon is clickable. See [`LISTEX_MSG_HDRICONCLICK`](#listex_msg_hdriconclick) message for more info.

## [](#)Structures

### [](#)LISTEXCREATE
```cpp
struct LISTEXCREATE
{
    LISTEXCOLORS stColor { };             //All control's colors.
    CRect        rect;                    //Initial rect.
    CWnd*        pParent { };             //Parent window.
    LOGFONTW*    pListLogFont { };        //List font.
    LOGFONTW*    pHdrLogFont { };         //Header font.
    UINT         uID { };                 //List control ID.
    DWORD        dwStyle { };             //Control's styles. Zero for default.
    DWORD        dwListGridWidth { 1 };   //Width of the list grid.
    DWORD        dwHdrHeight { };      //Header height.
    bool         fDialogCtrl { false };   //If it's a list within dialog.
    bool         fSortable { false };     //Is list sortable, by clicking on the header column?
    bool         fLinkUnderline { true }; //Links are displayed underlined or not.
    bool         fLinkTooltip { true };   //Show links' toolips.
    bool         fHighLatency { false };  //Do not redraw window until scrolling completes.
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

### [](#)LISTEXCOLOR
```cpp
struct LISTEXCOLOR
{
    COLORREF clrBk { };
    COLORREF clrText { };
};
using PLISTEXCOLOR = LISTEXCOLOR*;
```

### [](#)LISTEXHDRICON
```cpp
struct LISTEXHDRICON
{
    POINT pt { };              //Point of the top-left corner.
    int   iIconIndex { };      //Icon index in the header's image list.
    bool  fClickable { true }; //Is icon sending LISTEX_MSG_HDRICONCLICK message when clicked.
};
```

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

### [](#)LISTEX_MSG_GETCOLOR
When in virtual mode, sent to the parent window to retrieve cell's color. Expects pointer to the `LISTEXCOLOR` struct in response, or nothing to use defaults.
```cpp
void CListDlg::OnListExGetColor(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
    const auto pNMI = reintepret_cast<NMITEMACTIVATE*>(pNMHDR);

    //For column number 1 (all rows) set color to RGB(0, 220, 220).
    if (pNMI->iSubItem == 1)
    {
        static LISTEXCOLOR clr { RGB(0, 220, 220), RGB(0, 0, 0) };
        pNMI->lParam = reinterpret_cast<LPARAM>(&clr);
    }
}
```

### [](#)LISTEX_MSG_GETICON
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

### [](#)LISTEX_MSG_LINKCLICK
List embedded hyperlink has been clicked. `WM_NOTIFY` `lParam` will point to the `NMITEMACTIVATE` struct.  
`NMITEMACTIVATE::lParam` will contain `wchar_t*` pointer to the `link` text of the clicked hyperlink. The `iItem` and `iSubItem` members will contain indexes of the list item/subitem the link was clicked at. 

Hyperlink syntax is: `L"Text with the <link="any_text_here" title="Optional tool-tip text">embedded link</link>"`  
If no optional `title` tag is provided then the `link` text itself will be used as hyperlink's tool-tip.  
Link and title text must be quoted `""`.

### [](#)LISTEX_MSG_HDRICONCLICK
Header icon that previously was set by [`SetHdrColumnIcon`](#sethdrcolumnicon) call has been clicked.  
Example code for handling this message:
```cpp
BOOL CMyDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    const auto pNMI = reinterpret_cast<LPNMITEMACTIVATE>(lParam);

    if (pNMI->hdr.code == LISTEX_MSG_HDRICONCLICK && pNMI->hdr.idFrom == IDC_MYLIST)
    {
    	const auto pNMI = reinterpret_cast<NMHEADERW*>(lParam);
    	//pNMI->iItem holds clicked column index.
    }
    ...
```

## [](#)Example
Let’s imagine that you need a list control with a non standard header height, and yellow background color.
Nothing is simpler, see code below:
```cpp
LISTEXCREATE lcs;
lcs.rect = CRect(0, 0, 500, 300)
lcs.pParent = this;
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