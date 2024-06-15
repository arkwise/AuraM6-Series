/////////////////////////////////////////////////////////////////////////////////
//
//	Window Library 
//
//	Aura Milestone 4 
//
//	(c) Copyright 2013 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "button.h"
#include "label.h"
#include "internal.h"

#include "sound.h"


l_ulong	AppVersion = ULONG_ID(5, 0, 0, 0);
l_char	AppName[] = "Window Widget";
l_uid	nUID = "window";
l_uid NeededLibs[] = { "widget", "skin","button","label","progress","mp3","sound","" };

PIcon DefaultIcon = NULL;
PWidgetClass WindowClass;

l_int aw_trans = 0;
l_int pw_trans = 0;


/**
 * Global exported variable defining the
 * maximize area for window
 */
TRect WINDOW_MaximizeArea;
TRect WINDOW_MinimizeArea;


PSStream stream = NULL;

/**
*	Message box icons
*/
_PRIVATE p_bitmap IconWarning		= 0;
_PRIVATE p_bitmap IconQuestion		= 0;
_PRIVATE p_bitmap IconError			= 0;
_PRIVATE p_bitmap IconInformation	= 0;

/**
*	Skin items
*/
PSkin SkinTitleActive	= 0;
PSkin SkinTitlePassive	= 0;
PSkin SkinCloseUp		= 0;
PSkin SkinCloseDown		= 0;
PSkin SkinWindowFace	= 0;
PSkin SkinMaximizeUp	= 0;
PSkin SkinMaximizeDown	= 0;
PSkin SkinMinimizeUp	= 0;
PSkin SkinMinimizeDown	= 0;


_PUBLIC p_bitmap CResTitleActive	= 0;
_PUBLIC p_bitmap CResTitlePassive	= 0;
_PUBLIC p_bitmap CResWindowFace	= 0;

PSkin CSkinTitleActive	= 0;
PSkin CSkinTitlePassive	= 0;
PSkin CSkinWindowFace	= 0;


/**
*	Window actions
*/
#define WDNOTHING		0x00000000
#define WDRESIZING		0x00000001
#define WDMOVING		0x00000002

/**
*	Window button states
*/
#define	WBNOTHING		0xFF000000
#define WBCLOSE			0xFF000001
#define WBMAXIMIZE		0xFF000002
#define WBMINIMIZE		0xFF000003

/**
*	Fixed widget buttons lenghts
*/
#define FIXEDWIDGETSPACE		3
#define FIXEDWIDGETSEPERATOR	5

#define WCLOSE 1
#define WMAXIMIZE 2
#define WMINIMIZE 3
#define WRESIZE 4
#define WICON 5
#define WMOVE 6

void AssignZone ( PRect r, PWidget o, l_ulong id ) {
	if ( !UseSkins )
	switch ( id ) {
		case WCLOSE:
			RectAssign(r,o->Absolute.b.x-16, o->Absolute.a.y+4, o->Absolute.b.x-4, o->Absolute.a.y + 16 );
		break;
		case WMAXIMIZE:
			RectAssign(r,o->Absolute.b.x-33, o->Absolute.a.y+4, o->Absolute.b.x-21, o->Absolute.a.y + 16 );
		break;
		case WMINIMIZE:
			if (WINDOW(o)->WindowFlags & WF_MAXIMIZE)
				RectAssign(r,o->Absolute.b.x-50, o->Absolute.a.y+4, o->Absolute.b.x-38, o->Absolute.a.y + 16 );
			else
				RectAssign(r,o->Absolute.b.x-33, o->Absolute.a.y+4, o->Absolute.b.x-21, o->Absolute.a.y + 16 );
		break;
		case WRESIZE:
			RectAssign(r,o->Absolute.b.x-15, o->Absolute.b.y-15, o->Absolute.b.x, o->Absolute.b.y);
		break;
		case WICON:
			RectAssign(r,o->Absolute.a.x+5, o->Absolute.a.y+2, o->Absolute.a.x + 21, o->Absolute.a.y + 18);
		break;
		case WMOVE:
			RectAssign(r,o->Absolute.a.x+1, o->Absolute.a.y+1, o->Absolute.b.x-1, o->Absolute.a.y + 19);
		break;
	}
	else
	switch ( id ) {
		case WCLOSE:
			RectAssign(r,o->Absolute.b.x - (SkinTitleActive->Right + FIXEDWIDGETSPACE) - SkinCloseUp->Skin->w, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinCloseUp->Skin->h)/2, o->Absolute.b.x - (SkinTitleActive->Right + 3) - SkinCloseUp->Skin->w + SkinCloseUp->Skin->w, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinCloseUp->Skin->h)/2 + SkinCloseUp->Skin->h);
		break;
		case WMAXIMIZE:
			RectAssign(r,o->Absolute.b.x - (SkinTitleActive->Right + FIXEDWIDGETSPACE) - SkinCloseUp->Skin->w - FIXEDWIDGETSEPERATOR - SkinMaximizeUp->Skin->w, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinMaximizeUp->Skin->h)/2, o->Absolute.b.x - (SkinTitleActive->Right + FIXEDWIDGETSPACE) - SkinCloseUp->Skin->w - FIXEDWIDGETSEPERATOR, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinMaximizeUp->Skin->h)/2 + SkinMaximizeUp->Skin->h);
		break;
		case WMINIMIZE:
			if (WINDOW(o)->WindowFlags & WF_MAXIMIZE)
				RectAssign(r,o->Absolute.b.x - (SkinTitleActive->Right + FIXEDWIDGETSPACE) - SkinCloseUp->Skin->w - FIXEDWIDGETSEPERATOR - SkinMaximizeUp->Skin->w - FIXEDWIDGETSEPERATOR - SkinMinimizeUp->Skin->w, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinMinimizeUp->Skin->h)/2, o->Absolute.b.x - (SkinTitleActive->Right + FIXEDWIDGETSPACE) - SkinCloseUp->Skin->w - FIXEDWIDGETSEPERATOR - SkinMaximizeUp->Skin->w - FIXEDWIDGETSEPERATOR, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinMinimizeUp->Skin->h)/2 + SkinMinimizeUp->Skin->h);
			else
				RectAssign(r,o->Absolute.b.x - (SkinTitleActive->Right + FIXEDWIDGETSPACE) - SkinCloseUp->Skin->w - FIXEDWIDGETSEPERATOR - SkinMinimizeUp->Skin->w, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinMinimizeUp->Skin->h)/2, o->Absolute.b.x - (SkinTitleActive->Right + FIXEDWIDGETSPACE) - SkinCloseUp->Skin->w - FIXEDWIDGETSEPERATOR, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinMinimizeUp->Skin->h)/2 + SkinMinimizeUp->Skin->h);
		break;
		case WRESIZE:
			RectAssign(r,o->Absolute.b.x-15, o->Absolute.b.y-15, o->Absolute.b.x, o->Absolute.b.y);
		break;
		case WICON:
			RectAssign(r,o->Absolute.a.x + ((SkinTitleActive->Skin->h - 16)/2), o->Absolute.a.y + ((SkinTitleActive->Skin->h - 16)/2), o->Absolute.a.x + 16, o->Absolute.a.y + 16);
		break;
		case WMOVE:
			RectAssign(r,o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.a.y + SkinTitleActive->Skin->h);
		break;
	}
}




/**
*	Global variables
*/
TPoint p;
l_ulong WDoing = 0; // Ev->Type is WDoing master window
l_ulong WButton = 0;
l_ulong WSelected = 0;

void WindowDraw ( PWidget o, p_bitmap buffer, PRect w )
{

	TRect a,b,c,d,e,f;

	AssignZone(&a,o,WCLOSE);
	if ( WINDOW(o)->WindowFlags & WF_MAXIMIZE ) AssignZone(&b,o,WMAXIMIZE);
	if ( WINDOW(o)->WindowFlags & WF_MINIMIZE ) AssignZone(&c,o,WMINIMIZE);
	AssignZone(&d,o,WRESIZE);
	AssignZone(&e,o,WICON);
	AssignZone(&f,o,WMOVE);

	if ( !UseSkins ) {

		rectfill(buffer,o->Absolute.a.x,f.b.y,o->Absolute.b.x,o->Absolute.b.y,COL_3DFACE);

		rectfill(buffer,f.a.x,f.a.y,f.b.x,f.b.y,(o->Flags & WFSelected)?COL_WTACT:COL_WTPAS);

		textout_centre_ex(buffer, default_font, ((PWindow)o)->Caption, (f.a.x+f.b.x)/2, f.a.y+2, (o->Flags & WFSelected)?COL_WTACTTEXT:COL_WTPASTEXT,-1);

		Rect3D(buffer,o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.b.y,COL_3DLIGHT,COL_3DDARK);

		if (WINDOW(o)->WindowFlags & WF_MINIMIZE) {
			rectfill(buffer,c.a.x+1,c.a.y+1,c.b.x-1,c.b.y-1,COL_3DFACE);
			if (WButton != WBMINIMIZE) {
				Rect3D(buffer,c.a.x,c.a.y,c.b.x,c.b.y,COL_3DLIGHT,COL_3DDARK);
				hline(buffer,c.a.x+2,c.b.y-4,c.b.x-3,COL_3DTEXT);
				hline(buffer,c.a.x+2,c.b.y-3,c.b.x-3,COL_3DTEXT);
			} else {
				Rect3D(buffer,c.a.x,c.a.y,c.b.x,c.b.y,COL_3DDARK,COL_3DLIGHT);
				hline(buffer,c.a.x+3,c.b.y-3,c.b.x-2,COL_3DTEXT);
				hline(buffer,c.a.x+3,c.b.y-2,c.b.x-2,COL_3DTEXT);
			}
		}



		if (WINDOW(o)->WindowFlags & WF_MAXIMIZE) {
			
			rectfill(buffer,b.a.x+1,b.a.y+1,b.b.x-1,b.b.y-1,COL_3DFACE);
			
			if (WButton == WBMAXIMIZE) {
				Rect3D(buffer,b.a.x,b.a.y,b.b.x,b.b.y,COL_3DDARK,COL_3DLIGHT);
				rect(buffer,b.a.x+4,b.a.y+4,b.b.x-2,b.b.y-2,COL_3DTEXT);
				hline(buffer,b.a.x+5,b.a.y+5,b.b.x-3,COL_3DTEXT);
			} else {
				Rect3D(buffer,b.a.x,b.a.y,b.b.x,b.b.y,COL_3DLIGHT,COL_3DDARK);
				rect(buffer,b.a.x+3,b.a.y+3,b.b.x-3,b.b.y-3,COL_3DTEXT);
				hline(buffer,b.a.x+4,b.a.y+4,b.b.x-4,COL_3DTEXT);
			}

		}
		
		rectfill(buffer,a.a.x+1,a.a.y+1,a.b.x-1,a.b.y-1,COL_3DFACE);

		if (WButton == WBCLOSE) {
			Rect3D(buffer,a.a.x,a.a.y,a.b.x,a.b.y,COL_3DDARK,COL_3DLIGHT);
			line(buffer,a.a.x+4,a.a.y+4,a.b.x-2,a.b.y-2,COL_3DTEXT);
			line(buffer,a.b.x-2,a.a.y+4,a.a.x+4,a.b.y-2,COL_3DTEXT);
		} else {
			Rect3D(buffer,a.a.x,a.a.y,a.b.x,a.b.y,COL_3DLIGHT,COL_3DDARK);
			line(buffer,a.a.x+3,a.a.y+3,a.b.x-3,a.b.y-3,COL_3DTEXT);
			line(buffer,a.b.x-3,a.a.y+3,a.a.x+3,a.b.y-3,COL_3DTEXT);
		}

		DrawIcon16(buffer,WINDOW(o)->Icon,e.a.x,e.a.y);

	} else {
	/**
	*	Caption
	*/
	if (((PWindow)o)->WindowFlags & WF_CAPTION)
	{

	aw_trans = KeyGetInt("/USER/DESKTOP/transparency/active_window", 255);
	pw_trans = KeyGetInt("/USER/DESKTOP/transparency/passive_window", 255);

		if ( o->Flags & WFSelected )
		{
				if  (((PWindow)o)->TitleActive)
					DrawTransSkinCustom (buffer, ((PWindow)o)->TitleActive,  o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.a.y+SkinTitleActive->Skin->h, aw_trans, 255);
				else	
					DrawTransSkin(buffer, SkinTitleActive, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.a.y+SkinTitleActive->Skin->h, aw_trans, 255);
		}
		else
				if  (((PWindow)o)->TitlePassive)
					DrawTransSkinCustom (buffer, ((PWindow)o)->TitlePassive,  o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.a.y+SkinTitlePassive->Skin->h, pw_trans, 255);
				else	
					DrawTransSkin(buffer, SkinTitlePassive, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.a.y+SkinTitleActive->Skin->h, pw_trans, 255);
			
		if (WButton != WBCLOSE)
			draw_sprite( buffer,SkinCloseUp->Skin,o->Absolute.b.x - (SkinTitleActive->Right + 3) - SkinCloseUp->Skin->w, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinCloseUp->Skin->h)/2);
		else
			draw_sprite( buffer,SkinCloseDown->Skin, o->Absolute.b.x - (SkinTitleActive->Right + 3) - SkinCloseDown->Skin->w, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinCloseDown->Skin->h)/2);

		if (WINDOW(o)->WindowFlags & WF_MAXIMIZE)
		{
			if (WButton != WBMAXIMIZE)
				draw_sprite(buffer,SkinMaximizeUp->Skin, o->Absolute.b.x - (SkinTitleActive->Right + FIXEDWIDGETSPACE) - SkinCloseUp->Skin->w - FIXEDWIDGETSEPERATOR - SkinMaximizeUp->Skin->w, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinMaximizeUp->Skin->h)/2);
			else
				draw_sprite(buffer,SkinMaximizeDown->Skin, o->Absolute.b.x - (SkinTitleActive->Right + FIXEDWIDGETSPACE) - SkinCloseUp->Skin->w - FIXEDWIDGETSEPERATOR - SkinMaximizeUp->Skin->w, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinMaximizeUp->Skin->h)/2);
		}

		if (WINDOW(o)->WindowFlags & WF_MINIMIZE)
		{
			TPoint r;

			if (WButton != WBMINIMIZE)
			{
				if (WINDOW(o)->WindowFlags & WF_MAXIMIZE)
					PointAssign(&r,o->Absolute.b.x - (SkinTitleActive->Right + FIXEDWIDGETSPACE) - SkinCloseUp->Skin->w - FIXEDWIDGETSEPERATOR - SkinMaximizeUp->Skin->w - FIXEDWIDGETSEPERATOR - SkinMinimizeUp->Skin->w, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinMinimizeUp->Skin->h)/2);
				else
					PointAssign(&r,o->Absolute.b.x - (SkinTitleActive->Right + FIXEDWIDGETSPACE) - SkinCloseUp->Skin->w - FIXEDWIDGETSEPERATOR - SkinMinimizeUp->Skin->w, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinMinimizeUp->Skin->h)/2);

				draw_sprite(buffer, SkinMinimizeUp->Skin, r.x, r.y);
			}
			else
			{
				if (WINDOW(o)->WindowFlags & WF_MAXIMIZE)
					PointAssign(&r,o->Absolute.b.x - (SkinTitleActive->Right + FIXEDWIDGETSPACE) - SkinCloseUp->Skin->w - FIXEDWIDGETSEPERATOR - SkinMaximizeUp->Skin->w - FIXEDWIDGETSEPERATOR - SkinMinimizeDown->Skin->w, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinMinimizeDown->Skin->h)/2);
				else
					PointAssign(&r,o->Absolute.b.x - (SkinTitleActive->Right + FIXEDWIDGETSPACE) - SkinCloseUp->Skin->w - FIXEDWIDGETSEPERATOR - SkinMinimizeDown->Skin->w, o->Absolute.a.y + (SkinTitleActive->Skin->h - SkinMinimizeDown->Skin->h)/2);

				draw_sprite(buffer,SkinMinimizeDown->Skin, r.x, r.y);
			}
		}

		if ( ((PWindow)o)->Caption )
		
		textout_centre_ex(buffer, (o->Flags & WFSelected) ? SkinTitleActive->Font : SkinTitlePassive->Font, ((PWindow)o)->Caption, o->Absolute.a.x+((o->Absolute.b.x-o->Absolute.a.x)/2), o->Absolute.a.y+SkinTitleActive->Skin->h/2-text_height(SkinTitleActive->Font)/2, (o->Flags & WFSelected) ? SkinTitleActive->FontColor : SkinTitlePassive->FontColor, -1);

		/**
		*	Window icon
		*/
		DrawIcon16(buffer,WINDOW(o)->Icon,e.a.x+5, e.a.y );
	}

	/**
	*	Body
	*/
	rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y + SkinTitleActive->Skin->h, o->Absolute.b.x, o->Absolute.b.y ,COL_3DFACE);
	if  (((PWindow)o)->Background)
	DrawSkinCustom (buffer, ((PWindow)o)->Background, o->Absolute.a.x, o->Absolute.a.y + SkinTitleActive->Skin->h, o->Absolute.b.x, o->Absolute.b.y);
		else	
	DrawSkin(buffer, SkinWindowFace, o->Absolute.a.x, o->Absolute.a.y + SkinTitleActive->Skin->h, o->Absolute.b.x, o->Absolute.b.y);

	/**
	*	Resize
	*/
	/*if (((PWindow)o)->WindowFlags & WF_RESIZE )
		draw_sprite(buffer,SkinResize->Skin, o->Absolute.b.x - SkinWindowFace->Right - SkinResize->Skin->w, o->Absolute.b.y - SkinWindowFace->Bottom - SkinResize->Skin->h);*/


	}
}

PApplication App = 0;


void CloseWindowEx(PWidget o)
{
	PWidget t = o->Parent;

	WidgetSendSEvent(o,EV_MESSAGE, WM_CLOSE, NULL);
}

void MaximizeWindow(PWidget o)
{
	TRect tmp;


AssignZone(&tmp,o,WMAXIMIZE);
				
					if (WINDOW(o)->WindowStates & WS_MAXIMIZED)
					{
						WINDOW(o)->WindowStates &=~ WS_MAXIMIZED;
						WButton = WBNOTHING;	// No widget button pressed
						WidgetSetMetrics(o, WINDOW(o)->Bounds.a.x, WINDOW(o)->Bounds.a.y, WINDOW(o)->Bounds.b.x, WINDOW(o)->Bounds.b.y);
					}
					else
					{
						WINDOW(o)->Bounds = o->Absolute;
						WButton = WBNOTHING;	// No widget button pressed
						WidgetSetMetrics(o, WINDOW_MaximizeArea.a.x, WINDOW_MaximizeArea.a.y, WINDOW_MaximizeArea.b.x, WINDOW_MaximizeArea.b.y);
						WINDOW(o)->WindowStates |= WS_MAXIMIZED;
					}                                        
                                
			
}

void MinimizeWindow(PWidget o)
{
	TRect tmp;


                                        if (WINDOW(o)->WindowStates & WS_MINIMIZED)
                                        {
                                                WINDOW(o)->WindowStates &=~ WS_MINIMIZED;
                                                WButton = WBNOTHING;    // No widget button pressed
                                                WidgetSetMetrics(o, WINDOW(o)->Bounds.a.x, WINDOW(o)->Bounds.a.y, WINDOW(o)->Bounds.b.x, WINDOW(o)->Bounds.b.y);
                                        }
                                        else
                                        {
                                                WINDOW(o)->Bounds = o->Absolute;
                                                WButton = WBNOTHING;    // No widget button pressed
                                                WidgetSetMetrics(o, WINDOW(o)->Bounds.a.x+GSScreenWidth, WINDOW(o)->Bounds.a.y+GSScreenHeight, WINDOW(o)->Bounds.b.x+GSScreenWidth, WINDOW(o)->Bounds.b.y+GSScreenHeight);
                                                WINDOW(o)->WindowStates |= WS_MINIMIZED;
                                        }
                                        
                                
			
}

void CloseWindow(PWindow w)
{
	CloseWindowEx(WIDGET(w));
}

l_bool WindowEventHandler ( PWidget o, PEvent Ev )
{
	TRect tmp;

	if ( Ev->Type == EV_MESSAGE )
	{
		if ( Ev->Message == WM_CLOSE )
		{
			WidgetDispose(o);

			return true;
		}

		if ( Ev->Message == WEvNotifySelect )
		{
			WidgetSetFirst(o);

			return true;
		}
		if ( Ev->Message == WEvNotifyUnSelect )
		{
			WidgetDraw(o, NULL);

			return true;
		}
	}

	if (Ev->Type == EV_KEYBOARD)
	{
		if ( Ev->Keyb.Shift & KB_ALT )
		{
			if ( SCANCODE(Ev, KEY_F4) )
			{
				CloseWindowEx(o);
				return true;
			}
		}
	}

	if (Ev->Type == EV_MOUSE)
	{
		TRect MinButtonBounds;

		AssignZone(&MinButtonBounds,o,WMINIMIZE);

		if (Ev->Message == WEvMouseLDown)
		{
			/**
			*	Close button down
			*/
			AssignZone(&tmp,o,WCLOSE);
			if (CURSOR_IN_RECT(tmp))
			{
				WButton = WSelected = WBCLOSE;
				WidgetDraw(o, &tmp);

				return true;
			}

			/**
			*	Maximize
			*/
			AssignZone(&tmp,o,WMAXIMIZE);
			if (WINDOW(o)->WindowFlags & WF_MAXIMIZE && CURSOR_IN_RECT(tmp))
			{
				WButton = WSelected = WBMAXIMIZE;
				WidgetDraw(o, &tmp);
				return true;
			}

			/**
			*	Minimize
			*/

                        AssignZone(&tmp,o,WMINIMIZE);
                        if (WINDOW(o)->WindowFlags & WF_MINIMIZE && CURSOR_IN_RECT(tmp))
                        {
                                WButton = WSelected = WBMINIMIZE;
                                WidgetDraw(o, &tmp);
                                return true;
                        }

			/**
			*	Resize area button down
			*/
			AssignZone(&tmp,o,WRESIZE);
			if (((PWindow)o)->WindowFlags & WF_RESIZE && CURSOR_IN_RECT(tmp) && !(WINDOW(o)->WindowStates & WS_MAXIMIZED))
			{
				MouseSetCursor(CUR_RESIZE);

				MasterWidget = o; // I'm going to get all mouse messages
				p = Mouse->State.p;
				WDoing = WDRESIZING;
				return true;
			}

			/**
			*	Popup menu from icon
			*/
			AssignZone(&tmp,o,WICON);
			if (CURSOR_IN_RECT(tmp))
			{
				TPoint pp;
				PointAssign(&pp,tmp.a.x, tmp.b.y),
				PopUpMenu(&Me, pp, NewMenu(NewMenuItem("Close", NULL, WM_CLOSE, NULL, NULL), NULL), o, 0);

				return true;
			}

			/**
			*	Move area button down
			*/
			AssignZone(&tmp,o,WMOVE);
			if (CURSOR_IN_RECT(tmp) && !(WINDOW(o)->WindowStates & WS_MAXIMIZED) && !(WINDOW(o)->WindowFlags & WF_UNMOVEABLE))
			{
				MasterWidget = o; // I'm going to get all mouse messages
				p = Mouse->State.p;
				WDoing = WDMOVING;

				return true;
			}

		}

		if (Ev->Message == WEvMouseMove)
		{
			/**
			*	The cursor has left any of the window widget buttons
			*/
			if (WButton != WBNOTHING)
			{
				/**
				*	Check if it has left the close button
				*/
				AssignZone(&tmp,o,WCLOSE);
				if (WButton == WBCLOSE && !CURSOR_IN_RECT(tmp))
				{
					WButton = WBNOTHING;
					WidgetDraw(o, &tmp);

					return true;
				}

				/**
				*	Check if it has left the maximize button
				*/
				AssignZone(&tmp,o,WMAXIMIZE);
				if (WButton == WBMAXIMIZE && !CURSOR_IN_RECT(tmp))
				{
					WButton = WBNOTHING;
					WidgetDraw(o, &tmp);

					return true;
				}

				/**
				*	Check if it has left the maximize button
				*/
				if (WButton == WBMINIMIZE && !CURSOR_IN_RECT(MinButtonBounds))
				{
					WButton = WBNOTHING;
					WidgetDraw(o, &MinButtonBounds);

					return true;
				}
			}

			if (Mouse->State.b & BUTTON_LEFT)
			{
				if (MasterWidget == o)
				{
					/**
					*	Move the OSD
					*/
					if ( WDoing == WDMOVING )
					{
						RectAssign(&tmp,o->Absolute.a.x+(Mouse->State.p.x-p.x),o->Absolute.a.y+(Mouse->State.p.y-p.y),o->Absolute.b.x+(Mouse->State.p.x-p.x),o->Absolute.b.y+(Mouse->State.p.y-p.y));
						WidgetSetOSDRect(tmp); // Set/Move OSD rect (xor)
					}
					/**
					*	Resize the OSD
					*/
					else if ( WDoing == WDRESIZING ){
						RectAssign(&tmp,o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.b.y);

						if (WINDOW(o)->MinSize.w <= (o->Absolute.b.x-o->Absolute.a.x-(p.x-Mouse->State.p.x)))
						{
							RectAssign(&tmp,tmp.a.x,tmp.a.y,tmp.b.x-(p.x-Mouse->State.p.x),tmp.b.y);
							//WidgetSetOSDRect(tmp); // Set/Move OSD rect (xor)
						}

						if (WINDOW(o)->MinSize.h <= (o->Absolute.b.y-o->Absolute.a.y-(p.y-Mouse->State.p.y)))
						{
							RectAssign(&tmp,tmp.a.x,tmp.a.y,tmp.b.x,tmp.b.y-(p.y-Mouse->State.p.y));
						}

						if (tmp.b.x == o->Absolute.b.x )
							RectAssign(&tmp,o->Absolute.a.x,o->Absolute.a.y,o->Absolute.a.x+WINDOW(o)->MinSize.w,tmp.b.y);

						if (tmp.b.y == o->Absolute.b.y)
							RectAssign(&tmp,o->Absolute.a.x,o->Absolute.a.y,tmp.b.x,o->Absolute.a.y+WINDOW(o)->MinSize.h);

						WidgetSetOSDRect(tmp); // Set/Move OSD rect (xor)
					}
				}

				/**
				*	Check if the mouse has enetered any of the window
				*		widget buttons
				*/
				if (WButton == WBNOTHING  && WDoing == WDNOTHING)
				{
					AssignZone(&tmp,o,WCLOSE);
					if ( WSelected == WBCLOSE && CURSOR_IN_RECT(tmp))
					{

						WButton = WBCLOSE;
						WidgetDraw(o, &tmp);

						return true;
					}
					AssignZone(&tmp,o,WMAXIMIZE);
					if ( WSelected == WBMAXIMIZE && WINDOW(o)->WindowFlags & WF_MAXIMIZE && CURSOR_IN_RECT(tmp))
					{

						WButton = WBMAXIMIZE;
						WidgetDraw(o, &tmp);

						return true;
					}

					if ( WSelected == WBMINIMIZE && WINDOW(o)->WindowFlags & WF_MINIMIZE && CURSOR_IN_RECT(MinButtonBounds))
					{
						WButton = WBMINIMIZE;
						WidgetDraw(o, &MinButtonBounds);

						return true;
					}

				}
			}
		}

		if (Ev->Message == WEvMouseLUp)
		{
			if (WDoing == WDNOTHING)
			{
				/*
				*	Mouse up on the close button, so close window.
				*/
				AssignZone(&tmp,o,WCLOSE);
				if ( WSelected == WBCLOSE && CURSOR_IN_RECT(tmp))
				{
					WButton = WBNOTHING;
					CloseWindowEx(o);

					return true;
				}

				AssignZone(&tmp,o,WMAXIMIZE);
				if ( WSelected == WBMAXIMIZE && WINDOW(o)->WindowFlags & WF_MAXIMIZE && CURSOR_IN_RECT(tmp))
				{
					if (WINDOW(o)->WindowStates & WS_MAXIMIZED)
					{
						WINDOW(o)->WindowStates &=~ WS_MAXIMIZED;
						WButton = WBNOTHING;	// No widget button pressed
						WidgetSetMetrics(o, WINDOW(o)->Bounds.a.x, WINDOW(o)->Bounds.a.y, WINDOW(o)->Bounds.b.x, WINDOW(o)->Bounds.b.y);
					}
					else
					{
						WINDOW(o)->Bounds = o->Absolute;
						WButton = WBNOTHING;	// No widget button pressed
						WidgetSetMetrics(o, WINDOW_MaximizeArea.a.x, WINDOW_MaximizeArea.a.y, WINDOW_MaximizeArea.b.x, WINDOW_MaximizeArea.b.y);
						WINDOW(o)->WindowStates |= WS_MAXIMIZED;
					}

					return true;
				}

                                AssignZone(&tmp,o,WMINIMIZE);
                                if ( WSelected == WBMINIMIZE && WINDOW(o)->WindowFlags & WF_MINIMIZE && CURSOR_IN_RECT(tmp))
                                {
                                        if (WINDOW(o)->WindowStates & WS_MINIMIZED)
                                        {
                                                WINDOW(o)->WindowStates &=~ WS_MINIMIZED;
                                                WButton = WBNOTHING;    // No widget button pressed
                                                WidgetSetMetrics(o, WINDOW(o)->Bounds.a.x, WINDOW(o)->Bounds.a.y, WINDOW(o)->Bounds.b.x, WINDOW(o)->Bounds.b.y);
                                        }
                                        else
                                        {
                                                WINDOW(o)->Bounds = o->Absolute;
                                                WButton = WBNOTHING;    // No widget button pressed
                                                WidgetSetMetrics(o, WINDOW(o)->Bounds.a.x+GSScreenWidth, WINDOW(o)->Bounds.a.y+GSScreenHeight, WINDOW(o)->Bounds.b.x+GSScreenWidth, WINDOW(o)->Bounds.b.y+GSScreenHeight);
                                                WINDOW(o)->WindowStates |= WS_MINIMIZED;
                                        }

                                        return true;
                                }
			}

			if (MasterWidget == o)
			{
				WidgetUnOSD(); // Unactivate OSD

				/**
				*	If moving, move the window to the new position.
				*/
				if ( WDoing == WDMOVING )
				{

					WidgetMove(o,Mouse->State.p.x-p.x,Mouse->State.p.y-p.y);


				}
				else if (WDoing == WDRESIZING)
				{
					RectAssign(&tmp,o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.b.y);

					if (WINDOW(o)->MinSize.w <= (o->Absolute.b.x-o->Absolute.a.x-(p.x-Mouse->State.p.x)))
					{
						RectAssign(&tmp,tmp.a.x,tmp.a.y,tmp.b.x-(p.x-Mouse->State.p.x),tmp.b.y);
					}

					if (WINDOW(o)->MinSize.h <= (o->Absolute.b.y-o->Absolute.a.y-(p.y-Mouse->State.p.y)))
					{
						RectAssign(&tmp,tmp.a.x,tmp.a.y,tmp.b.x,tmp.b.y-(p.y-Mouse->State.p.y));
					}

					if (tmp.b.x == o->Absolute.b.x )
						RectAssign(&tmp,o->Absolute.a.x,o->Absolute.a.y,tmp.a.x+WINDOW(o)->MinSize.w,tmp.b.y);

					if (tmp.b.y == o->Absolute.b.y)
						RectAssign(&tmp,o->Absolute.a.x,o->Absolute.a.y,tmp.b.x,tmp.a.y+WINDOW(o)->MinSize.h);

					WidgetResize(o, tmp.b.x-o->Absolute.b.x, tmp.b.y-o->Absolute.b.y);

					MouseSetCursor(CUR_POINTER);
				}

				MasterWidget = NULL;	// I will get only my events... Not all mouse ones :}
				WDoing = WDNOTHING;		// Window is idle
				WButton = WBNOTHING;	// No widget button pressed
			}

		}
	}

	return false;
}

void WindowSetup ( PWidget o )
{
	if ( WINDOW(o)->WindowFlags & WF_CENTERED )
	{
		WidgetPreCenter(WIDGET(o));
	}
}

void WindowSetCaptionEx ( PWindow o, l_text Caption )
{
	if ( o->Caption ) free(o->Caption);

	o->Caption = TextDup(Caption);

	WidgetDraw(WIDGET(o), NULL);

}

void WindowSetCaption ( PWindow o, l_text Caption, ... ) {
	l_char Buffer[4096];
	va_list arg;
	va_start(arg, Caption);
	vsprintf(Buffer, Caption, arg);
	WindowSetCaptionEx(o,Buffer);
	va_end(arg);
}

void WindowFreeEx ( PWidget o )
{
	if ( WINDOW(o)->Icon ) 		FreeIcon(WINDOW(o)->Icon);
	if ( WINDOW(o)->Caption ) 	free(WINDOW(o)->Caption);
}

PWindow WindowInit(PWindow w, PApplication App, TRect r, l_text szCaption, l_ulong flags)
{
	TRect a;


	if ( UseSkins )
		RectAssign(&r, r.a.x, r.a.y, r.b.x+SkinWindowFace->Left+SkinWindowFace->Right, r.b.y+SkinTitleActive->Skin->h+SkinWindowFace->Top+SkinWindowFace->Bottom);
	else
		RectAssign(&r, r.a.x, r.a.y, r.b.x+6, r.b.y+22);
		
		
	memset(w, 0, sizeof(TWindow));

	w->Caption	= TextDup(szCaption);
	w->Bounds	= r;
	w->Cursor	= CUR_POINTER;
	
	if ( App->Icon )
		w->Icon		= DuplicateIcon(App->Icon);
	else
		w->Icon		= DuplicateIcon(DefaultIcon);
	
	SizeAssign(&w->MaxSize,65000,65000);
	SizeAssign(&w->MinSize,100,100);

	if (!flags) w->WindowFlags = WF_NORMAL;
	else w->WindowFlags = flags;

	IntialiseWidget(App, WindowClass, WIDGET(w), w->Bounds );
	WIDGET(w)->Flags |= WFSelectable|WFFocusable;
	WIDGET(w)->EventHandler	= &WindowEventHandler;
	WIDGET(w)->Draw = &WindowDraw;
	WIDGET(w)->Setup = &WindowSetup;
	WIDGET(w)->FreeEx = &WindowFreeEx;

	if ( UseSkins )
		RectAssign(&a, SkinWindowFace->Left, SkinTitleActive->Skin->h+SkinWindowFace->Top, r.b.x-r.a.x-SkinWindowFace->Right, r.b.y-r.a.y-SkinWindowFace->Bottom);
	else
		RectAssign(&a,3, 20, r.b.x-r.a.x-3, r.b.y-r.a.y-2);

	WidgetSetChildArea(WIDGET(w), a );

	return w;
}


PWindow CreateWindow(PApplication App, TRect r, l_text szCaption, l_ulong flags)
{
	TRect a;
	PWindow w = malloc(sizeof(TWindow));

	if (!w) return NULL;

	return WindowInit(w,App,r,szCaption,flags);
}

/**
*	MessageBox functions
*/
#define MSGBOX_BTNWIDTH 80
#define MSGBOX_BTNHEIGHT 22
#define MSGBOX_BTNSPACE 6

/**
*	NAME: MessageBox
*	DESCRIPTION: Pops up a small window with some warning
*	RETURN: A message ID type defined in widget.h
*/
_PUBLIC l_ulong  MessageBox ( PApplication App, l_text Title, l_text Message, l_ulong flags )
{
	l_ulong Msg = 0;

	PWindow w		= 0;
	PButton b		= 0;
	PLabel l		= 0;
	PImageLabel i	= 0;
	p_bitmap Icon	= 0;
	l_ulong Flags = flags;
	TRect r;

	l_text t = (char*)strdup(Message),tb;
	l_int Lines = 0;
	l_ulong ww = 240, h = 10;

	if (Flags & MBB_YESYESALLNOCANCEL) ww = 330;

	/**
	*	Calculate number lines of text
	*/
	while (t)
	{
		tb = t;
		t = TextChr(t, '\n');
		if (t) t++;
		Lines++;
		h+=text_height(default_font);
		ww=max(ww,FontTextLengthEx(default_font,tb,t,0));
	}


	/**
	*	Messagebox icon
	*/
	if (Flags & MBI_INFORMATION)
	{
		Icon = IconInformation;
		stream = LoadData2("/system/System/sounds/info.mp3",DTYPE_SOUNDSTREAM);
	}
	else if (Flags & MBI_WARNING)
	{
		Icon = IconWarning;
		stream = LoadData2("/system/System/sounds/error.mp3",DTYPE_SOUNDSTREAM);
	}
	else if (Flags & MBI_QUESTION)
	{
		Icon = IconQuestion;
		stream = LoadData2("/system/System/sounds/info.mp3",DTYPE_SOUNDSTREAM);
	}
	else if (Flags & MBI_ERROR)
	{
		Icon = IconError;
		stream = LoadData2("/system/System/sounds/error.mp3",DTYPE_SOUNDSTREAM);

	}
	else
	{
		Icon = IconInformation;
		stream = LoadData2("/system/System/sounds/info.mp3",DTYPE_SOUNDSTREAM);
	}

	RectAssign(&r,0 , 0, ww + 50, 40+h+5 > 90 ? 40+h : 90);
	w = CreateWindow(App, r, Title, WF_FRAME|WF_CAPTION|WF_CENTERED);
	InsertWidget(DeskTop, WIDGET(w));

	RectAssign(&r,45,5,ww+50,h+5);
	l = CreateLabel(App, r, Message );
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));

	if ( Icon ) {
		RectAssign(&r,5,(WIDGET(w)->ChildArea.b.y-WIDGET(w)->ChildArea.a.y)/2-Icon->h/2-10,40,(WIDGET(w)->ChildArea.b.y-WIDGET(w)->ChildArea.a.y)/2+Icon->h/2-10);
		i = CreateImageLabel(App, r, Icon);
		WIDGET(i)->BackgroundColor = COL_3DFACE;
		InsertWidget(WIDGET(w), WIDGET(i));
	}
	
	if (Flags & MBB_OK || !Flags)
	{
		RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - MSGBOX_BTNWIDTH/2, WIDGET(w)->ChildArea.b.y-WIDGET(w)->ChildArea.a.y-5-MSGBOX_BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + MSGBOX_BTNWIDTH/2, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5);
		b = CreateButton(App, r, "OK", MSG_OK);
		InsertWidget(WIDGET(w), WIDGET(b));
	}
	else if (Flags & MBB_OKCANCEL)
	{
		RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - MSGBOX_BTNSPACE/2 - MSGBOX_BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5 - MSGBOX_BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - MSGBOX_BTNSPACE/2, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
		b = CreateButton(App, r, "OK", MSG_OK );
		InsertWidget(WIDGET(w), WIDGET(b));

		RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + MSGBOX_BTNSPACE/2, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5 - MSGBOX_BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + MSGBOX_BTNSPACE/2 + MSGBOX_BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5);
		b = CreateButton(App, r, "Cancel", MSG_CANCEL );
		InsertWidget(WIDGET(w), WIDGET(b));
	}
	else if (Flags & MBB_YESNO)
	{
		RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - MSGBOX_BTNSPACE/2 - MSGBOX_BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5 - MSGBOX_BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - MSGBOX_BTNSPACE/2, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
		b = CreateButton(App, r, "Yes", MSG_YES );
		InsertWidget(WIDGET(w), WIDGET(b));
		RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + MSGBOX_BTNSPACE/2, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5- MSGBOX_BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + MSGBOX_BTNSPACE/2 + MSGBOX_BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
		b = CreateButton(App, r, "No", MSG_NO );
		InsertWidget(WIDGET(w), WIDGET(b));
	}
	else if (Flags & MBB_YESNOCANCEL)
	{
		RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2)  - MSGBOX_BTNWIDTH/2 - MSGBOX_BTNSPACE - MSGBOX_BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5 - MSGBOX_BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2)  - MSGBOX_BTNWIDTH/2 - MSGBOX_BTNSPACE, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
		b = CreateButton(App, r, "Yes", MSG_YES );
		InsertWidget(WIDGET(w), WIDGET(b));
		RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - MSGBOX_BTNWIDTH/2, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5 - MSGBOX_BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + MSGBOX_BTNWIDTH/2, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
		b = CreateButton(App, r, "No", MSG_NO );
		InsertWidget(WIDGET(w), WIDGET(b));
		RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + MSGBOX_BTNWIDTH/2 + MSGBOX_BTNSPACE, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5 - MSGBOX_BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + MSGBOX_BTNWIDTH/2 + MSGBOX_BTNSPACE + MSGBOX_BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5);
		b = CreateButton(App, r, "Cancel", MSG_CANCEL );
		InsertWidget(WIDGET(w), WIDGET(b));
	}
	else if (Flags & MBB_YESYESALLNOCANCEL)
	{
		RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - (2*MSGBOX_BTNSPACE) - (2*MSGBOX_BTNWIDTH), WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5 - MSGBOX_BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2)  - MSGBOX_BTNWIDTH - (2*MSGBOX_BTNSPACE), WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
		b = CreateButton(App, r, "Yes", MSG_YES );
		InsertWidget(WIDGET(w), WIDGET(b));

		RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - MSGBOX_BTNSPACE - MSGBOX_BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5 - MSGBOX_BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - MSGBOX_BTNSPACE, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
		b = CreateButton(App, r, "Yes all", MSG_YESALL );
		InsertWidget(WIDGET(w), WIDGET(b));


		RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2), WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5 - MSGBOX_BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + MSGBOX_BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
		b = CreateButton(App, r, "No", MSG_NO );
		InsertWidget(WIDGET(w), WIDGET(b));

		RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + MSGBOX_BTNSPACE + MSGBOX_BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5 - MSGBOX_BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + MSGBOX_BTNSPACE + (2*MSGBOX_BTNWIDTH), WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5);
		b = CreateButton(App, r, "Cancel", MSG_CANCEL );
		InsertWidget(WIDGET(w), WIDGET(b));

	}
	PlaySoundStream(stream);
	WidgetDrawAll(WIDGET(w));
	while ( Msg != MSG_CANCEL && Msg != MSG_YES && Msg != MSG_NO && Msg != MSG_OK && Msg != MSG_YESALL ) {
		Msg = WidgetExecute(WIDGET(w));
		if ( Msg == WM_CLOSE ) Msg = MSG_CANCEL;
	}

	PWidget temp = WIDGET(w)->Parent;

	WidgetDispose(WIDGET(w));

	return Msg;
}

void WindowNotifyError ( PApplication App, PErrorHandler h ) {
	// TO DO : better version, support multiple errors.
	
	if ( h ) 
	if ( BLIST(h)->Last ) {
		l_text txt = TextArgs("%s\n\n%s",ERROR(BLIST(h)->Last)->Description,ERROR(BLIST(h)->Last)->Log);
		MessageBox(App,"Error",txt,MBI_ERROR|MBB_OK);
		free(txt);
	}
	
}

PProgressPop ProgressPopUp ( PApplication App, l_ulong Flags, l_text Title, l_text Message, l_ulong Steps, l_text InitInfo ) {
	TRect r;
	l_ulong h = 0;
	PLabel L1; PButton Cancel;
	PProgressPop o = malloc(sizeof(TProgressPop));
	if ( !o ) return NULL;
	memset(o,0,sizeof(TProgressPop));


	if ( Flags & 1 ) h+= 25;
	if ( Flags & 2 ) h+= 20;
	if ( Flags & 4 ) h+= 20;

	RectAssign( &r, 0 , 0, 310, 70+h );

	o->W = CreateWindow(App, r, Title, WF_FRAME|WF_CAPTION|WF_CENTERED);
	InsertWidget(DeskTop, WIDGET(o->W));

	RectAssign( &r, 0, 0, 300, 20);
	L1 = CreateLabel(App, r, Message );
	WIDGET(L1)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(o->W),WIDGET(L1));

	if ( Flags & 1 ) {
		RectAssign( &r, 0, r.b.y+5, 300, r.b.y+25);
		o->L = CreateLabel(App, r, InitInfo );
		WIDGET(o->L)->BackgroundColor = COL_3DFACE;
		WIDGET(o->L)->Flags |= WFForceBuffer;
		InsertWidget(WIDGET(o->W),WIDGET(o->L));
	}

	if ( Flags & 2 ) {
		RectAssign( &r, 0, r.b.y+5, 300, r.b.y+20);
		o->B = CreateProgressBar(App, r, Steps );
		WIDGET(o->B)->Flags |= WFForceBuffer;
		InsertWidget(WIDGET(o->W),WIDGET(o->B));


		if ( Flags & 4 ) {
			RectAssign( &r, 0, r.b.y+5, 300, r.b.y+20);
			o->T = CreateLabel(App, r, "" );
			WIDGET(o->T)->BackgroundColor = COL_3DFACE;
			WIDGET(o->T)->Flags |= WFForceBuffer;
			InsertWidget(WIDGET(o->W),WIDGET(o->T));
			o->Started = time(NULL);
		}
	}

	RectAssign( &r, 200, r.b.y+5, 300, r.b.y+25);
	Cancel = CreateButton(App, r, "Cancel", WM_CLOSE );
	InsertWidget(WIDGET(o->W),WIDGET(Cancel));

	WIDGET(o->W)->Flags |= WFExecuting;

	WidgetDrawAll(WIDGET(o->W));

	return o;
}

void ProgressPopSetInfoEx ( PProgressPop p, l_text Text ) {
	if ( p->L ) {
		LabelSetTextEx(p->L,Text);
	  WidgetDraw(WIDGET(p->L),NULL);
	}
}

void ProgressPopSetInfo ( PProgressPop p, l_text Format, ... ) {
  va_list arg;
  va_start(arg, Format);
  vsprintf(CommonBufferText, Format, arg);
  ProgressPopSetInfoEx(p,CommonBufferText);
  va_end(arg);
}

void ProgressPopStep ( PProgressPop p, l_ulong Steps ) {

	if ( p->B ) {
		p->B->Step(p->B,Steps);
	  WidgetDraw(WIDGET(p->B),NULL);

	  if ( p->T ) {
		 	l_ulong DT = time(NULL)-p->Started;
		 	if ( (DT > 4) && p->B->Promile ) {
 			 	l_ulong DS = p->B->Promile;
			  l_ulong RS = p->B->Steps-p->B->Promile;
			  l_ulong RT = DT*RS/DS;

			  if ( RT <= 30 )
			  	LabelSetText(p->T,"Less than 30 secondes left...");
			  else if ( RT <= 60 )
			  	LabelSetText(p->T,"Less than 1 minute left...");
			  else if ( RT <= 90 )
			  	LabelSetText(p->T,"About 1 minute left...");
				else
			  	LabelSetText(p->T,"About %d minutes left...",(RT/60)+1);
	  		}
	  }

	}

}

void ProgressPopKick ( PProgressPop p ) {
	WidgetDispose(WIDGET(p->W));
  free(p);
}

PWidget  StdCreateWindow ( PApplication App, TRect r )
{
	return WIDGET(CreateWindow( App, r, NULL, 0));
}

// scripting interface

l_bool	WindowSetRProp ( PWidget o, l_text Name, void *Value, l_bool Redraw ) {
	
	if ( !TextCompare(Name,"caption") ) {
		if ( WINDOW(o)->Caption ) free(WINDOW(o)->Caption);
		WINDOW(o)->Caption = TextDup((l_text)Value);
		if ( Redraw ) WidgetDraw(WIDGET(o), NULL);
		return true;
	}
	if ( !TextCompare(Name,"maximize") ) {
		if ( *((l_bool*)Value) ) 
			WINDOW(o)->WindowFlags |= WF_MAXIMIZE;
		else
			WINDOW(o)->WindowFlags &= ~WF_MAXIMIZE;
		if ( Redraw ) WidgetDraw(WIDGET(o), NULL);
		return true;
	}
	if ( !TextCompare(Name,"minimize") ) {
		if ( *((l_bool*)Value) ) 
			WINDOW(o)->WindowFlags |= WF_MINIMIZE;
		else
			WINDOW(o)->WindowFlags &= ~WF_MINIMIZE;
		if ( Redraw ) WidgetDraw(WIDGET(o), NULL);
		return true;
	}
	if ( !TextCompare(Name,"resize") ) {
		if ( *((l_bool*)Value) ) 
			WINDOW(o)->WindowFlags |= WF_RESIZE;
		else
			WINDOW(o)->WindowFlags &= ~WF_RESIZE;
		if ( Redraw ) WidgetDraw(WIDGET(o), NULL);
		return true;
	}
	if ( !TextCompare(Name,"center") ) {
		if ( *((l_bool*)Value) ) 
			WINDOW(o)->WindowFlags |= WF_CENTERED;
		else
			WINDOW(o)->WindowFlags &= ~WF_CENTERED;
		return true;
	}	
		
	return false;
}

void	*WindowGetRProp ( PWidget o, l_text Name ) {
	if ( !TextCompare(Name,"caption") ) return TextDup(WINDOW(o)->Caption);
	
	if ( !TextCompare(Name,"maximize") ) {
		l_bool tmp = WINDOW(o)->WindowFlags & WF_MAXIMIZE;
		return DuplicateBool(&tmp);
	}
	if ( !TextCompare(Name,"minimize") ) {
		l_bool tmp = WINDOW(o)->WindowFlags & WF_MINIMIZE;
		return DuplicateBool(&tmp);
	}
	if ( !TextCompare(Name,"resize") ) {
		l_bool tmp = WINDOW(o)->WindowFlags & WF_RESIZE;
		return DuplicateBool(&tmp);
	}	
	if ( !TextCompare(Name,"center") ) {
		l_bool tmp = WINDOW(o)->WindowFlags & WF_CENTERED;
		return DuplicateBool(&tmp);
	}
	return NULL;
}

l_int LibMain ( int argc, l_text *argv )
{
	PProgressPop pp;

	WindowClass = CreateWidgetClass( ULONG_ID('W','i','n',' '), StdWidgetClass, WCF_CONTAINER );
	WindowClass->StdCreate = &StdCreateWindow;
	WindowClass->SetRProp = &WindowSetRProp;
	WindowClass->GetRProp = &WindowGetRProp;

	WClassRegisterProp(WindowClass,PTYPE_STRING,"caption");
	WClassRegisterProp(WindowClass,PTYPE_BOOL,"maximize");
	WClassRegisterProp(WindowClass,PTYPE_BOOL,"minimize");
	WClassRegisterProp(WindowClass,PTYPE_BOOL,"resize");	
	WClassRegisterProp(WindowClass,PTYPE_BOOL,"center");	
	
	DefaultIcon = NewIcon(LoadImage("SYSTEM/ICONS/app16.bmp"),NULL,NULL);
	
	SkinTitleActive		= GetSkinItem("WindowTitleActive");
	SkinTitlePassive	= GetSkinItem("WindowTitlePassive");
	SkinCloseUp			= GetSkinItem("WindowCloseUp");
	SkinCloseDown		= GetSkinItem("WindowCloseDown");
	SkinWindowFace		= GetSkinItem("WindowFace");
	SkinMaximizeUp		= GetSkinItem("WindowMaximizeUp");
	SkinMaximizeDown	= GetSkinItem("WindowMaximizeDown");
	SkinMinimizeUp		= GetSkinItem("WindowMinimizeUp");
	SkinMinimizeDown	= GetSkinItem("WindowMinimizeDown");

	IconQuestion	= LoadImage("SYSTEM/ICONS/msg01.bmp");
	IconInformation	= LoadImage("SYSTEM/ICONS/msg02.bmp");
	IconWarning		= LoadImage("SYSTEM/ICONS/msg03.bmp");
	IconError		= LoadImage("SYSTEM/ICONS/msg04.bmp");

	/**
	 * Default maximize area
	 */
	RectAssign(&WINDOW_MaximizeArea, 0, 0,GSScreenWidth, GSScreenHeight);

	APPEXPORT(CreateWindow);
	APPEXPORT(CloseWindow);
	APPEXPORT(MinimizeWindow);
	APPEXPORT(MaximizeWindow);
	APPEXPORT(CloseWindowEx);
	APPEXPORT(WindowEventHandler);
	APPEXPORT(MessageBox);
	APPEXPORT(WindowDraw);

	APPEXPORT(WINDOW_MaximizeArea);

	APPEXPORT(WindowSetCaptionEx);
	APPEXPORT(WindowSetCaption);

	APPEXPORT(ProgressPopUp);
	APPEXPORT(ProgressPopSetInfoEx);
	APPEXPORT(ProgressPopSetInfo);
	APPEXPORT(ProgressPopStep);
	APPEXPORT(ProgressPopKick);

	APPEXPORT(WindowInit);
	APPEXPORT(WindowNotifyError);

	return true;
}

void Close (void)
{

	if ( DefaultIcon ) FreeIcon(DefaultIcon);
	
}
