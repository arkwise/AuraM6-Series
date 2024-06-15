////////////////////////////////////////////////////////////////////////////////
//
//  Scrollbars - Header File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _SCROLLBAR_H_INCLUDED_
#define _SCROLLBAR_H_INCLUDED_
#include "widget.h"

#define SLLWDG(o) ((PScrollWidget)(o))
#define SLLBAR(o) ((PScrollbar)(o))

typedef struct TScrollWidget *PScrollWidget;
typedef struct TScrollbar    *PScrollbar;

typedef struct TScrollWidget {
	struct	TWidget o;

	void (*Scrolled) ( PScrollWidget o, TPoint Pos );

	TPoint OldPos;
	TPoint Pos;
	TPoint RPos;
	TPoint Max;
	TPoint Delta;

	PScrollbar Vertical;
	PScrollbar Horizontal;

} TScrollWidget;



typedef struct TScrollbar
{
	struct	TWidget o;
	PScrollWidget Scll;
	l_uchar Type;
	TRect  BarCache;
	l_ulong W;
	l_uchar BtnState;

} TScrollbar;

PScrollWidget InitScrollWidget( PScrollWidget o, PWidgetClass Class, PApplication App, TRect r, l_uchar Type );

l_bool ScrollWidgetEventHandler ( PWidget o, PEvent Ev );

void ScrollBarChangeType ( PScrollWidget o, l_uchar Type );
void ScrollBarChangeTypeUninserted ( PScrollWidget o, l_uchar Type );

void ScrollBarRedraw ( PScrollWidget o );


void ScrollBarScrollTo ( PScrollWidget i, TPoint Pos );
void ScrollBarSetPosX ( PScrollWidget i, l_ulong Pos );
void ScrollBarSetPosY ( PScrollWidget i, l_ulong Pos );
void ScrollBarSetMaxY ( PScrollWidget i, l_ulong Max );
void ScrollBarSetMaxY ( PScrollWidget i, l_ulong Max );

extern PWidgetClass ScollWidgetClass;

#endif /* _SCROLLBAR_H_INCLUDED_ */
