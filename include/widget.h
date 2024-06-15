////////////////////////////////////////////////////////////////////////////////
//
//  Widgets - Header File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

/**	
	@addtogroup widgetsys
	@{
*/
/**
	@file
	Widget system header file.
	Declarations and structures for widget system.
*/
#ifndef _WIDGET_H_INCLUDED_
#define _WIDGET_H_INCLUDED_

#include "metrics.h"
#include "dynld.h"

/*
*	Alert messages
*/
#define MBI_WELCOME 0xFFFF9551
#define MBI_HARDWARE 0xFFFF9552
#define MBI_SYSERROR 0xFFFF9554

TRect r;

////////////////////////////////////////////////////////////////////////////////
typedef struct TiWidget *PiWidget;
typedef struct TWidget *PWidget;
typedef struct TWidgetClass *PWidgetClass;
////////////////////////////////////////////////////////////////////////////////
/**
	Generic property class structure
*/

typedef struct TPropType
{
	l_ulong Id;

	void *(*Duplicate)(void *Value);
	void (*Free)(void *Value);
	void (*Write)(PFile f, void *Value);
	void *(*Read)(PFile f);

} TPropType, *PPropType;

#define PTYPE(o) ((PPropType)(o))
////////////////////////////////////////////////////////////////////////////////
/**
	Widget property structure
*/

typedef struct TWidgetProp
{
	PPropType Type;

	void *Extra;
	void (*FreeExtra)(void *Extra);

} TWidgetProp, *PWidgetProp;

#define WDGPROP(o) ((PWidgetProp)(o))

typedef struct TWidgetPropFlag
{
	l_ulong Value;
	l_text Name;
	l_text Description;

} TWidgetPropFlag, *PWidgetPropFlag;

#define WDGPFLAG(o) ((PWidgetPropFlag)(o))

////////////////////////////////////////////////////////////////////////////////
/**
	Widget Class structure
*/
typedef struct TWidgetClass
{
	l_ulong Id;
	PWidgetClass Parent;
	l_ulong Flags;

	PList Properties; // Associate Property name to Property type

	PWidget (*StdCreate)(PApplication App, TRect r);

	void *(*GetDefault)(l_text Name);
	l_bool (*SetRProp)(PWidget o, l_text Name, void *Value, l_bool Redraw);
	void *(*GetRProp)(PWidget o, l_text Name);

} TWidgetClass;

#define WCLASS(o) ((PWidgetClass)(o))
////////////////////////////////////////////////////////////////////////////////
/**
	Irreal Widget structure
**/
typedef struct TiWidget
{
	l_text Name;				///< Widget name
	PWidgetClass Class; ///< Widget class
	TRect r;						///< Widget relative coordinates (in child area)
	PList Properties;

	PiWidget Last;
	PiWidget Next;
	PiWidget Prev;
	PiWidget Parent;

} TiWidget;
#define IWIDGET(o) ((PiWidget)(o))
////////////////////////////////////////////////////////////////////////////////
/**
	Widget structure
**/
typedef struct TWidget
{
	PWidgetClass Class; ///< Widget class

	TRect Relative;			///< Coordinates of widget coordinates using ChildArea of parent as reference
	TRect Absolute;			///< Coordinates of widget  using screen as reference
	TRect DrawAbsolute; ///< Coordinates of widget using childarea of parent as reference clipped in ChildDrawAbs of parent
	TRect ChildArea;		///< Coordinates relative to object position of child spacework (equal to Relative by default)
	TRect ChildDrawAbs; ///< Coordinates relative to screen of child spacework (equal to Absolute by default)

	PWidget Prev;
	PWidget Next;
	PWidget Last;
	PWidget Parent;

	l_long BackgroundColor; ///< Widget default background color

	/**
		Widget flags.
		@see widgetflags
	*/
	l_ulong Flags;

	PApplication AppOwner; ///< Owner application

	l_ulong MessageBuffer;

	l_bool (*EventHandler)(PWidget o, PEvent Ev);
	l_bool (*AppEvHdl)(PWidget o, PEvent Ev);
	void (*Refresh)(PWidget o, PRect w, PList List, p_bitmap Out, PPoint delta);
	void (*Draw)(PWidget o, p_bitmap buffer, PRect w);
	void (*FreeEx)(PWidget o);
	void (*Setup)(PWidget o);

	void (*Insert)(PWidget o, PWidget i);
	void (*Remove)(PWidget o, PWidget i);
	void (*SetFirstEx)(PWidget o, PWidget i);

	PWidget FocusBack; ///< If set, when focus will be set on widget, it will focus instead the specified widget

	union
	{
		l_text t;
		l_text (*func)(PWidget o, TPoint p);
	} ToolTip;

	l_bool (*DropAccept)(PWidget o, PWidget Caller, l_ulong Type, void *Data, l_ulong SupportedMethods, l_ulong *Method);

	l_bool (*DragBeforeDrop)(PWidget o, PWidget Dest, void *Data, l_ulong Type, l_ulong Method);
	void (*DropData)(PWidget o, PWidget Caller, void *Data, l_ulong Type, l_ulong Method);
	void (*DragAfterDrop)(PWidget o, PWidget Dest, void *Data, l_ulong Type, l_ulong Method);

	l_text Name;

	l_long Reserved[4];

} TWidget;

#define WIDGET(o) ((PWidget)(o))
////////////////////////////////////////////////////////////////////////////////
/**
	Tool tip widget
*/
typedef struct TToolTip
{

	struct TWidget o;

	l_text Text;

} TToolTip, *PToolTip;

#define TOOLTIP(o) ((PToolTip)(o))
////////////////////////////////////////////////////////////////////////////////

#define MSG_NONE 0x00
#define MSG_MSGCANCEL 0x01

extern p_bitmap wallpaper;
extern PWidget DeskTop;
extern PWidget Desktop;
extern PWidget MasterWidget;
extern PWidget FocusedWidget;

PWidget IntialiseWidget(PApplication App, PWidgetClass Class, PWidget o, TRect r);
PWidget CreateWidget(PApplication App, TRect r);
void WidgetDrawOnRect(PWidget o, TRect w, PWidget Ignore);
void WidgetMove(PWidget o, l_long dX, l_long dY);
void WidgetResize(PWidget o, l_long dW, l_long dH);
void WidgetResizeToSize(PWidget o, l_long SizeX, l_long SizeY);
void WidgetMoveToPoint(PWidget o, l_int x, l_int y);
void WidgetSetMetrics(PWidget o, l_int x, l_int y, l_int w, l_int h);
l_bool WidgetSendEvent(PWidget o, TEvent Ev);
void WidgetInsert(PWidget o, PWidget i);
void WidgetRemove(PWidget o, PWidget i);
void WidgetDrawAll(PWidget o);
void WidgetSetOSDRect(TRect r);
void WidgetUnOSD(void);
void WidgetDispose(PWidget o);
l_ulong WidgetExecute(PWidget o);
void WidgetSetFirst(PWidget o);
void WidgetSetFocusEx(PWidget o);
void WidgetSetFocus(PWidget o);
void WidgetPreCenter(PWidget o); // Use between insertion in parent and 1st draw
void WidgetUnMakeSync(PWidget *o);
void WidgetMakeSync(PWidget *o);
void WidgetSetChildArea(PWidget o, TRect r);
l_bool WidgetSendPEvent(PWidget o, PEvent Ev);
void WidgetSetRect(PWidget o, TRect r);
void WidgetSelect(PWidget o);
void WidgetUnSelectAllSubs(PWidget o);
void WidgetDraw(PWidget o, PRect w);
void WidgetSetFocusExNoNotify(PWidget o);
l_bool WidgetSendSEvent(PWidget o, l_int Type, l_ulong Message, void *Extra);
l_bool WidgetSendEEvent(PWidget o, l_int Type, l_ulong Message, void *Extra, TKeybState Keyb, TMouseState Mouse);
void WidgetSetTooltipText(PWidget o, l_text t);
l_long WidgetGetIndex(PWidget o);
void DragStart(PWidget o, l_ulong DataType, void *Data, l_ulong SupportedMethods);
void WidgetRefreshAbsolutePos(PWidget o); // (not exported)
l_bool WidgetCheckVisible(PWidget o);

PWidget WidgetGetName(PWidget o, l_text Name);

////////////////////////////////////////////////////////////////////////////////
// Compatibility with old software parts
#define InsertWidget(o, i) WidgetInsert(o, i)
#define RemoveWidget(o, i) WidgetRemove(o, i)
#define WidgetSize(r, x, y, w, h) RectAssign(r, x, y, x + w, y + h)

////////////////////////////////////////////////////////////////////////////////
// Drag and drop methods
#define DMETHOD_COPY 0x01
#define DMETHOD_MOVE 0x02

////////////////////////////////////////////////////////////////////////////////
// Default functions for widget structure

void _WidgetInsert(PWidget o, PWidget i);
void _WidgetRemove(PWidget o, PWidget i);
void _WidgetSetFirstEx(PWidget o, PWidget i);

////////////////////////////////////////////////////////////////////////////////
/** @defgroup widgetflags Widget flags
	Values that can be set into TWidget Flags field.
	Use 'OR' to associate those flags.
	@{
*/

#define WFVisible 0x00000001				///< Widget is draw
#define WFForceBuffer 0x00000002		///< System will always draw the widget using the buffer (do not blink, quite slow compared to non buffer for some graphics methods)
#define WFForceNonBuffer 0x00000004 ///< System will always directlt to the screen. (fast for most graphics methods, can blink)
#define WFFocused 0x00000008				///< Widget is focused
#define WFFocusable 0x00000010			///< Widget can be focused
#define WFOnTop 0x00000020
#define WFSelectable 0x00000040 ///< Widget can be selected
#define WFSelected 0x00000080		///< Widget is selected

#define WF_TOOLTIPFUNC 0x00000100 ///< Internal use : Widget is a function to set its tooltip
#define WF_DRAGFOCUS 0x00000200		///< Widget is focused in the current drag'n'drop session, read-only
#define WFExecuting 0x80000000		///< Internal use : Widget is waiting for a message (see WidgetExecute)

#define WF_AUTORESIZE 0x00100000	 ///< System wille resize automaticly widget if its parent is resizer using flolowing flags
#define WF_FIXEDWIDTH 0x00200000	 ///< Widget width is constant
#define WF_FIXEDHEIGHT 0x00400000	 ///< Widget height is constant
#define WF_MAGNETRIGHT 0x00800000	 ///< Widget right segment has a constant distance to the right segment of the widget parent
#define WF_MAGNETBOTTOM 0x01000000 ///< Widget bottom has a constant distance to the bottom of the widget parent

#define WF_SCRIPTFLAGS (WF_AUTORESIZE | WF_FIXEDWIDTH | WF_FIXEDHEIGHT | WF_MAGNETRIGHT | WF_MAGNETBOTTOM)

/**
@}
*/
////////////////////////////////////////////////////////////////////////////////
// Events

// 0xFFFF1XXX reserved for mouse events

#define WEvMouse 0xFFFF1000
#define WEvKeyboard 0xFFFF2000
#define WEvNotify 0xFFFF4000

#define WEvMouseOther 0x100
#define WEvMouseUp 0x200
#define WEvMouseDown 0x400
#define WEvMouseDClk 0x800

#define WEvMouseR 0x01
#define WEvMouseM 0x02
#define WEvMouseL 0x04

#define WEvMouseLeft 0xFFFF1101
#define WEvMouseEnter 0xFFFF1102
#define WEvMouseMove 0xFFFF1103
#define WEvMouseEnterDelay 0xFFFF1104

#define WEvMouseRUp 0xFFFF1201
#define WEvMouseMUp 0xFFFF1202
#define WEvMouseLUp 0xFFFF1204

#define WEvMouseRDown 0xFFFF1401
#define WEvMouseMDown 0xFFFF1402
#define WEvMouseLDown 0xFFFF1404

#define WEvMouseRDClk 0xFFFF1801
#define WEvMouseMDClk 0xFFFF1802
#define WEvMouseLDClk 0xFFFF1804

#define WEvMouseRClick WEvMouseRUp
#define WEvMouseMClick WEvMouseMUp
#define WEvMouseLClick WEvMouseLUp

#define WEvNotifyLostFocus 0xFFFF4001
#define WEvNotifyFocused 0xFFFF4002
#define WEvNotifySameFocus 0xFFFF4003 // A focus change was try, but no change (usefull for some widget like menus)

#define WEvNotifySelect 0xFFFF4004
#define WEvNotifyUnSelect 0xFFFF4005
#define MSG_NOTIFY_AUTO_RESIZE 0xFFFF4006

#define MSG_DRAGCANCEL 0xFFFF4010
////////////////////////////////////////////////////////////////////////////////
/*
*	Default messages (Reserved 0xFFFF8XXX)
*/
#define MSG_OK 0xFFFF8001
#define MSG_CANCEL 0xFFFF8002
#define MSG_YES 0xFFFF8004
#define MSG_NO 0xFFFF8008
#define MSG_APPLY 0xFFFF8016
#define MSG_YESALL 0xFFFF8017

////////////////////////////////////////////////////////////////////////////////
// Modes switching
void SwitchGraphicsMode(void);
void SwitchTextMode(void);

////////////////////////////////////////////////////////////////////////////////
// Widgets class
#define WCF_SATELLITE 0x0001

#define WCF_BOOK 0x0002 // Global book widget composed of page
#define WCF_PAGE 0x0004

#define WCF_CONTAINER 0x0008

extern PWidgetClass StdWidgetClass;
extern PList WidgetClasses;

PWidgetClass CreateWidgetClass(l_ulong Id, PWidgetClass Parent, l_ulong Flags);
l_bool WidgetIsClassId(PWidget o, l_ulong Id);
PWidgetClass GetWidgetClass(l_ulong Id);
void WClassRegisterPropEx(PWidgetClass c, l_ulong tid, l_text name, void *Extra, void (*FreeExtra)(void *Extra));
void WClassRegisterProp(PWidgetClass c, l_ulong tid, l_text name);
PPropType GetPropType(l_ulong Id);
PPropType CreatePropType(l_ulong Id, void *(*Duplicate)(void *), void (*Free)(void *), void (*Write)(PFile, void *), void *(*Read)(PFile));
void WClassResgisterFlag(PWidgetClass c, l_text Property, l_ulong Value, l_text Name, l_text Description);

#define PTYPE_STRING 1
#define PTYPE_INT 2
#define PTYPE_BOOL 3
#define PTYPE_MESSAGE 4
#define PTYPE_FLAGS 5
#define PTYPE_MENU 6
#define PTYPE_MESSAGENAME 7
#define PTYPE_FLAGSNAMELIST 8

////////////////////////////////////////////////////////////////////////////////
// iWidget
PiWidget NewiWidget(PWidgetClass C);
void InsertiWidgetBefore(PiWidget o, PiWidget i, PiWidget b);
PWidget ComplieiWidget(PApplication App, PiWidget iw);
void FreeiWidget(PiWidget o);

////////////////////////////////////////////////////////////////////////////////
// Misc helping functions

// Helper function for multi-windows applications
void DisposeAllAppWidgets(PApplication App);

void *DuplicateBool(void *o);
void *DuplicateInt(void *o);

void iWidgetInstall(void);
void iWidgetUnInstall(void);
void WClassInstall(void);
void WClassUnInstall(void);

PWidgetProp WClassGetProp(PWidgetClass c, l_text name);

////////////////////////////////////////////////////////////////////////////////
// Spliters and auto-resize
typedef struct TSpliter
{

	struct TWidget o;

	l_uchar Mode;

	l_bool Moving;

} TSpliter, *PSpliter;

#define SP_VERTICAL 1
#define SP_HORIZONTAL 2

#define WCLASS_SPLITER ULONG_ID('z', 'S', 'p', 'l')
#define WCLASS_HSPLITER ULONG_ID('H', 'S', 'p', 'l')
#define WCLASS_VSPLITER ULONG_ID('V', 'S', 'p', 'l')

#define SPLITER(o) ((PSpliter)(o))

void _WidgetAutosize(PWidget o, TRect p, TRect r);
void InitSpliters(void);

PWidget CreateVSpliter(PApplication App, TRect r);
PWidget CreateHSpliter(PApplication App, TRect r);

/**
	@}
*/

#endif /* _WIDGET_H_INCLUDED_ */
