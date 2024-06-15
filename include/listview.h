////////////////////////////////////////////////////////////////////////////////
//
//	Listview - Header File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LISTVIEW_H_INCLUDED_
#define _LISTVIEW_H_INCLUDED_

/**	
	@addtogroup listview
	@{
*/

/**
	@file
	Listview header file.
*/

#include "widget.h"
#include "sbar.h"
#include "menu.h"

#define LISTVIEW(w) ((PListview)(w))
#define LISTVIEWITEM(w) ((PListviewItem)(w))
#define LISTVIEWCOL(w) ((PListviewCol)(w))

typedef struct TListviewItem *PListviewItem;
typedef struct TListviewItem
{
	char *Caption;

	PList ColumsData;

	PIcon Icon;

	TRect BufAbs;
	TRect BufTop;

	TRect RLabel;
	TRect RIcon;
	TRect RChk;

	l_ulong Flags;

} TListviewItem;

#define LVI_SELECTED 0x01
#define LVI_CHKDISABLE 0x04
#define LVI_CHECKED 0x08

typedef struct TListviewCol
{

	l_text Caption;
	PIcon Icon;
	l_int Width;
	l_ulong Flags;

	l_int (*Comparator)(PListviewItem a, PListviewItem b);

} TListviewCol, *PListviewCol;

typedef struct TListview *PListview;
/**
	Listview structure
*/
typedef struct TListview
{
	struct TScrollWidget o;

	PList Items;

	PList Colums;

	PListviewItem TopItem;
	PListviewItem LPointed;

	l_ulong OnSelMsg;	 ///< If set, message sent when an item is selected. Item is set into @a Extra field of event.
	l_ulong OnValMsg;	 ///< If set, message sent when a double click (or assimilated) event happen on an item. Item is set into @a Extra field of event.
	l_ulong OnRClkMsg; ///< If set, message sent when a right click event happen on an item. Item is set into @a Extra field of event.

	/**
		Listview flags.
		@see listviewflags
	*/
	l_ulong Flags;

	l_uchar Style;

	l_uchar Background;

	l_uchar TransBG;

	l_uchar Noscroll;

	PListviewCol SelColum; // Internal use only

	PListviewItem DragItem;

	void (*ItemStartDrag)(PWidget o);
	l_bool (*ItemDropAccept)(PWidget o, PWidget Caller, PListviewItem Item, l_ulong Type, void *Data, l_ulong SupportedMethods, l_ulong *Method);
	l_bool (*ItemDragBeforeDrop)(PWidget o, PWidget Dest, PListviewItem Item, void *Data, l_ulong Type, l_ulong Method);
	void (*ItemDropData)(PWidget o, PWidget Caller, PListviewItem Item, void *Data, l_ulong Type, l_ulong Method);
	void (*ItemDragAfterDrop)(PWidget o, PWidget Dest, PListviewItem Item, void *Data, l_ulong Type, l_ulong Method);

} TListview;

// Styles
#define LVS_ICONS 0x00
#define LVS_LIST 0x01
#define LVS_MOSC 0x02

/** @defgroup listviewflags Listview flags
	Values that can be set into TListview Flags field.
	Use 'OR' to associate those flags.
	@{
*/

// Flags
#define LVF_MULTISELECT 0x0001 ///< Allow multiple items selection
#define LVF_ALLOWDRAG 0x0040	 ///< Allow items to be dragged throw global system, incompatible with LVF_ORDER

// Flags : LVS_LIST mode
#define LVF_ORDER 0x0002			///< Allow user to change items order by dragging them, incompatible with LVF_ALLOWDRAG
#define LVF_CHECKBOXES 0x0004 ///< Show checkbowes before items caption
#define LVF_DRAWICONS 0x0008	///< Show icons
#define LVF_COLUMS 0x0010			///< Show colums
#define LVF_COLRESIZE 0x0020	///< Allow colums resize

// Flags : Internal use only
#define LVF_ENDODRAG 0x1000
#define LVF_POINTDOWN 0x2000
#define LVF_SIZINGCOL 0x4000
#define LVF_HAVESELECT 0x8000
/**@}*/

PListview CreateListview(PApplication App, TRect r, l_text CaptionColumName, l_uchar Style, l_ulong Flags);
PListview InitListview(PListview p, PWidgetClass Class, PApplication App, TRect r, l_text CaptionColumName, l_uchar Style, l_ulong Flags);

PListviewItem ListviewAddItem(PListview o, char *Caption, PIcon Icon);
PListviewItem ListviewInitItem(PListviewItem p, l_text Caption, PIcon Icon);
void ListviewItemPushCol(PListviewItem p, l_text Text);
void FreeListviewItem(PListviewItem p);

void ListviewRemoveAllItems(PListview o);

l_bool ListviewEventHandler(PWidget o, PEvent Ev);

PListviewItem GetSelectedItem(PListview o);

void ListviewSelectItem(PListview o, PListviewItem p, l_bool Scroll, l_bool Multi);

void ListviewSelectIndex(PListview o, l_ulong Index, l_bool Scroll, l_bool Multi);
void ListviewSelectItemAndTell(PListview o, PListviewItem p, l_bool Scrol, l_bool Multi);

l_ulong ListviewItemIndex(PListview o, PListviewItem a);
PListItem GetSelectedListItem(PListview o);
l_text ListviewRenameItemBox(PListview o, PListviewItem a);
void ListviewItemRemoveItem(PListview o, PListviewItem p);

l_ulong ListviewCountSelectedFiles(PListview o);
PListviewItem ListviewGetItemUnderPoint(PListview o, TPoint p);

l_ulong ListviewEstimateHeight(PList Items);

void ListviewAddColum(PListview p, l_text Caption, PIcon Icon, l_int Width, l_ulong Flags);
void ListviewSetColSize(PListview p, l_int col, l_ulong Width);

void ListviewItemSetColCaption(PListviewItem p, l_int col, l_text Caption);

void ListviewUpdateScrollbars(PListview o);

extern PWidgetClass ListviewClass;

void ListviewFree(PWidget o);
void ListviewUpdateAndReDraw(PListview o);
void ListviewCalculateMax(PWidget o);
l_bool ListviewScrollTo(PListview o, PListviewItem i);

/**	
	@}
*/
#endif /* _LISTVIEW_H_INCLUDED_ */
