/////////////////////////////////////////////////////////////////////////////////
//
//	Control Panel (WORK IN PROGRESS)
//
//	Aura Milestone 4 
//
//	(c) Copyright 2013 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "label.h"
#include "button.h"
#include "canvas.h"
#include "tabbook.h"

l_ulong AppVersion	= ULONG_ID(0,0,0,2);
char    AppName[]	= "Control Panel";
l_uid	nUID		= "cp";
l_uid NeededLibs[]	= { "widget", "button","label","window","tabbook","" };

#define FWIDTH  296
#define FHEIGHT 141


PCanvas	c 	= 0;
PLabel l = 0;
PWindow w = 0;


PTabBook bk;
PTab t;

void Draw_home (PWidget o, p_bitmap buffer, PRect w)
{
	rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DFACE);
}


l_bool AppEventHandler ( PWidget o, PEvent Ev )
{
	if ( Ev->Type == EV_MESSAGE )
	{
		if ( Ev->Message == WM_CLOSE )
		{
			CloseApp(&Me);

			return true;
		}

	}

	return false;
}

l_int Main ( int argc, l_text *argv )
{

	TRect r;
	PButton b = NULL;


	RectAssign(&r,0, 0, FWIDTH, FHEIGHT);
	w = CreateWindow( &Me, r, "Control Panel", WF_FRAME|WF_CAPTION|WF_MINIMIZE|WF_CENTERED );
	InsertWidget(DeskTop, WIDGET(w));
	WIDGET(w)->AppEvHdl = &AppEventHandler;


	WidgetSize(&r,0, 0, FWIDTH-1, FHEIGHT);
	c = CreateCanvas(&Me, r);
	WIDGET(c)->Draw = &Draw_home;
	WIDGET(c)->Flags |= WFForceBuffer;
	InsertWidget(WIDGET(w), WIDGET(c));




	WidgetDrawAll(WIDGET(w));


	return true;
}

void Close (void)
{

	WidgetDispose(WIDGET(w));
}
