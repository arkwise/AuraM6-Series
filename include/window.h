#ifndef _WINDOW_H_INCLUDED_
#define _WINDOW_H_INCLUDED_

#include "widget.h"
#include "label.h"
#include "progress.h"

/**
*	Window structure
*/
typedef struct TWindow *PWindow;
typedef struct TWindow
{
	struct		TWidget o;

	l_text		Caption;
	l_ulong		WindowFlags;
	l_ulong		WindowStates;
	TRect		Bounds;
	p_bitmap Background; ///< Window custom background
	p_bitmap TitleActive; ///< Window custom TitleActive
	p_bitmap TitlePassive; ///< Window custom TitlePassive

	PIcon	Icon;
	PCursor		Cursor;
	TSize		MaxSize;
	TSize		MinSize;

} TWindow;

#define WINDOW(w)	((PWindow)(w))

/**
*	Window flags
*/
#define WF_FRAME        0x00000001
#define WF_CAPTION      0x00000002
#define WF_MAXIMIZE     0x00000004
#define WF_MINIMIZE     0x00000008
#define WF_RESIZE       0x00000010
#define WF_MAINWINDOW   0x00000020
#define WF_UNMOVEABLE   0x00000040
#define WF_UNSELECTABLE 0x00000080
#define WF_DOUBLEBUFFER 0x00000100
#define WF_CENTERED		0x00000200
#define WF_NORMAL 		WF_FRAME|WF_CAPTION|WF_MAXIMIZE|WF_MINIMIZE

/**
*	Window states
*/
#define WS_MAXIMIZED    0x00000001
#define WS_MINIMIZED    0x00000002
#define WS_MODAL        0x00000008


/**
*	Window messages
*/
#define WM_CLOSE		0xFFFF4101
#define WM_ABOUT		0xFFFF4102
#define WM_NEWWINDOW	0xFFFF4104
#define WM_CLOSEWINDOW	0xFFFF4108

void WindowSetCaptionEx ( PWindow o, l_text Caption );
void WindowSetCaption ( PWindow o, l_text Caption, ... );
void CustomWindowDraw ( PApplication App, PWidget o, p_bitmap buffer, PRect w );
PWindow CreateWindow(PApplication App, TRect r, l_text szCaption, l_ulong flags);
l_ulong MessageBox ( PApplication App, l_text Title, l_text Message, l_ulong Flags );

PWindow WindowInit(PWindow w, PApplication App, TRect r, l_text szCaption, l_ulong flags);

l_ulong  Alert ( PApplication App, l_text Message, l_ulong flags );

/**
*	Message box flags
*/
#define MBB_OK          0x000000100
#define MBB_OKCANCEL    0x000000200
#define MBB_YESNO       0x000000400
#define MBB_YESNOCANCEL 0x000000800
#define MBB_YESYESALLNOCANCEL 0x000001000

#define MBI_WARNING		0x000000001
#define MBI_QUESTION	0x000000002
#define MBI_ERROR		0x000000004
#define MBI_INFORMATION	0x000000008

extern TRect WINDOW_MaximizeArea;


typedef struct TProgressPop {

	PWindow 			W;
	PProgressBar 	B;
	PLabel				L;
	PLabel				T;
	l_ulong Started;
	
} TProgressPop, *PProgressPop;

#define PROGRESSPOP(o) ((PProgressPop)(o))

PProgressPop ProgressPopUp ( PApplication App, l_ulong Flags, l_text Title, l_text Message, l_ulong Steps, l_text InitInfo );
void ProgressPopSetInfoEx ( PProgressPop p, l_text Text );
void ProgressPopSetInfo ( PProgressPop p, l_text Format, ... );
void ProgressPopStep ( PProgressPop p, l_ulong Steps );
void ProgressPopKick ( PProgressPop p );
void WindowNotifyError ( PApplication App, PErrorHandler h );
void MinimizeWindow( PWidget o );


#endif /* _WINDOW_H_INCLUDED_ */
