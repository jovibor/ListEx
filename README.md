## **Introduction**
**ListEx** is an advanced owner-drawn **List Control**, an extended version of the [`CMFCListCtrl`](#https://docs.microsoft.com/en-us/cpp/mfc/reference/cmfclistctrl-class?view=vs-2019#sort) class.
![](docs/img/listex_mainwnd.jpg)

## Table of Contents
* [Features](#features)
* [Installation](#installation)
* [Create](#create)
    * [Manually](#manually)
    * [In Dialog](#in-dialog)
* [Tooltips](#tooltips)
* [Sorting](#sorting)
* [Editing Cells](#editing-cells)
* [Data Alignment](#data-alignment)
* [Public Methods](#public-methods)
   * [HideColumn](#hidecolumn)
   * [SetCellColor](#setcellcolor)
   * [SetCellData](#setcelldata)
   * [SetCellTooltip](#setcelltooltip)
   * [SetColumnEditable](setcolumneditable)
   * [SetHdrColumnIcon](#sethdrcolumnicon)
   * [SetHdrHeight](#sethdrheight)
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
   * [LISTEX_MSG_EDITBEGIN](#listex_msg_editbegin)
   * [LISTEX_MSG_DATACHANGED](#listex_msg_datachanged)
* [Example](#example)
* [Appearance](#appearance)

## [](#)Features
* [Editable cells](#editing-cells), not only the first column
* [Hyperlinks](#listex_msg_linkclick) in a cells' text
* [Tooltips](#setcelltooltip) for individual cells
* [Background and text color](#setcellcolor) for individual cells
* Many options to set individual colors for lots of list aspects with the [`LISTEXCOLORSTRUCT`](#listexcolors)
* Set header height and font
* Set header color for individual columns
* Individual text alignment for a header and column itself
* [Header icons](#listex_msg_hdriconclick)
* [Hiding individual columns](#hidecolumn)
* [Additional item data](#setcelldata) for individual cells
* Innate ability to sort list columns with no additional effort
* Dynamically changed list's font size with the **Ctrl+MouseWheel**

## [](#)Installation
The usage of the control is simple:
1. Add *ListEx.h* and *CListEx.cpp* into your project
2. Declare `IListExPtr` variable: `IListExPtr myList { CreateListEx() };`

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
`Create` is the main method to create **ListEx** control, it takes [`LISTEXCREATE`](#listexcreate) structure as an argument.  
Below is a simple example of the **ListEx** creation:
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

### [](#)In Dialog
To create **ListEx** in a Dialog you can manually do it with the [Create](#manually) method.  
But most of the time you prefer to place a standard **List Control** onto a Dialog's template, by dragging it from the **Toolbox** within **Visual studio**.  
To use the latter approach follow these steps:
1. Put a standard **List Control** from the toolbox onto your dialog.
2. Declare `IListExPtr` member variable within your dialog class: `IListExPtr m_myList { CreateListEx() };`
3. In your `OnInitDialog` method call the `m_myList->CreateDialogCtrl(ID_OF_THE_LIST, this);` method.

## [](#)Tooltips
To set a tooltip for a given cell:
```cpp
myList->SetCellTooltip(0, 1, L"Tooltip text", L"Tooltip caption:");
```
This will set a tooltip for a Cell (0, 1) with the text: **_Tooltip text_**, and the caption **_Tooltip caption_**.

## [](#)Sorting
To enable a sorting set the [`LISTEXCREATE::fSortable`](#listexcreate) flag to `true`. In this case, when you click on the header, list will be sorted according to the clicked column. By default **ListEx** performs lexicographical sorting.  
To set your own sorting routine use the [`SetSortable`](#setsortable) method. 

## [](#)Editing Cells
By default **ListEx** works in the read-only mode. To enable cells editing call the [`SetColumnEditable`](#setcolumneditable) method with the column ID which cells you wish to become editable.

## [](#)Data Alignment
Classical List Control allows setting an alignment only for a header and column  simultaneously.  
**ListEx** allows setting alignment separately for the header and the data. The `iDataAlign` argument in the `InsertColumn()` method is responsible exactly for that.

## [](#)Public Methods
`IListEx` class also has a set of additional methods to help customize your control in many different aspects.

### [](#)HideColumn
```cpp
void HideColumn(int iIndex, bool fHide);
```
Hide or show column by `iIndex`.

### [](#)SetCellColor
```cpp
SetCellColor(int iItem, int iSubitem, COLORREF clrBk, COLORREF clrText = -1);
```
Sets background and text color for a given cell.

### [](#)SetCellData
```cpp
void SetCellData(int iItem, int iSubitem, ULONGLONG ullData);
```
Sets arbitrary application-defined data associated with given cell.

### [](#)SetCellTooltip
```cpp
void SetCellTooltip(int iItem, int iSubitem, std::wstring_view wstrTooltip, std::wstring_view wstrCaption = L"");
```
Sets tooltip and caption for a given cell.

### [](#)SetColumnEditable
```cpp
void SetColumnEditable(int iColumn, bool fEditable);
```
Enables or disables edit mode for a given column.

### [](#)SetHdrColumnIcon
```cpp
void SetHdrColumnIcon(int iColumn, int iIconIndex, bool fClick = false);
```
Sets the icon index in the header's image list for a given `iColumn`. To remove icon from column set the `iIconIndex` to `-1`.  
Flag `fClick` means that icon is clickable. See [`LISTEX_MSG_HDRICONCLICK`](#listex_msg_hdriconclick) message for more info.

### [](#)SetHdrHeight
```cpp
void SetHdrHeight(DWORD dwHeight);
```

### [](#)SetSortable
```cpp
void SetSortable(bool fSortable, PFNLVCOMPARE pfnCompare = nullptr, EListExSortMode enSortMode = EListExSortMode::SORT_LEX)
```
**Parameters:**  
`bool fSortable`  
Enables or disables sorting

`PFNLVCOMPARE pfnCompare`  
Callback function pointer with the type `int (CALLBACK *PFNLVCOMPARE)(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)` that is used to set your own comparison function. If it's `nullptr` **ListEx** performs default sorting.  
The comparison function must be either a static member of a class or a stand-alone function that is not a member of any class. For more information see official [MSDN documentation](https://docs.microsoft.com/en-us/cpp/mfc/reference/clistctrl-class?view=vs-2019#remarks-100).

`EListExSortMode enSortMode`  
Default sorting mode for the list.

## [](#)Structures

### [](#)LISTEXCREATE
```cpp
struct LISTEXCREATE {
    LISTEXCOLORS stColor { };             //All control's colors.
    CRect        rect;                    //Initial rect.
    CWnd*        pParent { };             //Parent window.
    LOGFONTW*    pListLogFont { };        //List font.
    LOGFONTW*    pHdrLogFont { };         //Header font.
    UINT         uID { };                 //List control ID.
    DWORD        dwStyle { };             //Control's styles. Zero for default.
    DWORD        dwListGridWidth { 1 };   //Width of the list grid.
    DWORD        dwHdrHeight { };         //Header height.
    bool         fDialogCtrl { false };   //If it's a list within dialog.
    bool         fSortable { false };     //Is list sortable, by clicking on the header column?
    bool         fLinkUnderline { true }; //Links are displayed underlined or not.
    bool         fTooltipBaloon { true }; //Baloon type tooltip for cells.
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
    COLORREF clrListBkOdd { GetSysColor(COLOR_WINDOW) };          //List Bk color of the odd rows.
    COLORREF clrListBkEven { GetSysColor(COLOR_WINDOW) };         //List Bk color of the even rows.
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
These messages are sent to the parent window in form of `WM_NOTIFY` Windows messages.  
The `lParam` will contain a pointer to the `NMHDR` standard Windows struct. `NMHDR::code` can be one of the `LISTEX_MSG_...` messages described below.

### [](#)LISTEX_MSG_GETCOLOR
When in Virtual Mode, sent to the parent window to retrieve cell's color. Expects a pointer to the `LISTEXCOLOR` struct in response, or nothing to use defaults.
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
This message is used in Virtual Mode to obtain an icon index in the List's image-list.

### [](#)LISTEX_MSG_LINKCLICK
List embedded hyperlink has been clicked. `WM_NOTIFY` `lParam` will point to the `NMITEMACTIVATE` struct.  
`NMITEMACTIVATE::lParam` will contain `wchar_t*` pointer to the `link` text of the clicked hyperlink. The `iItem` and `iSubItem` members will contain indexes of the list item/subitem the link was clicked at. 

Hyperlink syntax is: `L"Text with the <link="any_text_here" title="Optional tool-tip text">embedded link</link>"`  
If no optional `title` tag is provided then the link text itself will be used as hyperlink's tool-tip.  
The `link` and the `title`'s text must be quoted `""`.

### [](#)LISTEX_MSG_HDRICONCLICK
Header icon that previously was set by the [`SetHdrColumnIcon`](#sethdrcolumnicon) method has been clicked.  
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
}
```

### [](#)LISTEX_MSG_EDITBEGIN
Sent when the edit box for data editing is about to show up. If you don't want it to show up, you can set `lParam` to `0` in response.
```cpp
BOOL CMyDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    const auto pNMI = reinterpret_cast<LPNMITEMACTIVATE>(lParam);
    pNMI->lParam = 0; //Edit-box won't show up.
    ...
}
```

### [](#)LISTEX_MSG_DATACHANGED
Sent in the Virtual Mode when cell's text has been changed. 
```cpp
BOOL CMyDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    const auto pNMI = reinterpret_cast<LPNMITEMACTIVATE>(lParam);
    const auto iItem = pNMI->iItem;
    const auto iSubItem = pNMI->iSubItem;
    const pwszNewText = reinterpret_cast<LPCWSTR>(pNMI->lParam);
    ...
}
```

## [](#)Example
Let’s imagine that you need a **ListEx** control with the non standard header height, and yellow background color.
Nothing is simpler, see the code below:
```cpp
LISTEXCREATE lcs;
lcs.rect = CRect(0, 0, 500, 300)
lcs.pParent = this;
lcs.dwHdrHeight = 50;
lcs.stColor.clrListBkOdd = RGB(255, 255, 0);
lcs.stColor.clrListBkEven = RGB(255, 255, 0);

IListExPtr myList { CreateListEx() };
myList->Create(lcs);
myList->InsertColumn(...);
myList->InsertItem(...);
```
Here we set both even and odd rows to the same yellow color.

## [](#)Appearance
With the **Ctrl+MouseWheel** you can dynamically change the **ListEx**'s font size.