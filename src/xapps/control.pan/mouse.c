/////////////////////////////////////////////////////////////////////////////////
//
//	Mouse Settings
//
//	Aura Milestone 3 
//
//	(c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "label.h"
#include "button.h"
#include "window.h"
#include "window.h"
#include "label.h"
#include "slider.h"
#include "canvas.h"
#include "button.h"

l_ulong AppVersion = ULONG_ID(0,0,1,0);
char    AppName[] = "Mouse settings";
l_uid	nUID = "cp:setmouse";
l_uid NeededLibs[] = { "widget","button","label","menu","window","canvas","slider","" };

#define BTNWIDTH 80
#define BTNHEIGHT 22
#define BTNSPACE 5

#define MOUSESPEEDDS 10
#define MOUSESPEEDMS 10
l_color CPMouseTestColor = 0;
PSlider CPMouseDS = 0;
PSlider CPMouseMS = 0;

l_bool CPMouseTestEH ( PWidget o, PEvent Ev ){
	if ( Ev->Type == EV_MOUSE ){
		if ( Ev->Message == WEvMouseLDClk ){
			CPMouseTestColor = makecol(rand()%255,rand()%255,rand()%255);
			WidgetDraw(o, NULL);

			return true;
		}
	}

	return false;
}

void CPMouseTestDraw ( PWidget o, p_bitmap buffer, PRect w ){
	rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, CPMouseTestColor);
}

l_bool AppEventHandler ( PWidget o, PEvent Event )
{
	if ( Event->Type == EV_MESSAGE )
	{
		switch ( Event->Message )
		{
			case MSG_CANCEL:
			case WM_CLOSE:
			{
				WidgetDispose(o);
				CloseApp(&Me);
				return true;
			}
			break;

			case MSG_APPLY:
			case MSG_OK:
			{
				KeySetInt("/SYSTEM/MOUSE/speed", MOUSESPEEDMS - CPMouseMS->Value);
				MouseSetCursorSpeed(MOUSESPEEDMS - CPMouseMS->Value);

				KeySetInt("/SYSTEM/MOUSE/dblclk", MOUSESPEEDDS - CPMouseDS->Value);
				MouseSetDblClkSpeed(MOUSESPEEDDS - CPMouseDS->Value);


				if ( Event->Message == MSG_OK ){
					WidgetDispose(o);
					CloseApp(&Me);
				}

				return true;
			}
			break;

		}
	}

	return false;
}

l_int Main ( int argc, l_text *argv )
{
	TRect r;
	PWindow w = 0;
	PButton b = 0;
	PLabel l = 0;
	PCanvas c = 0;

	RectAssign(&r,0, 0, 300, 290);
	w = CreateWindow(&Me, r, "Mouse settings", WF_CAPTION|WF_FRAME|WF_CENTERED|WF_MINIMIZE);
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	InsertWidget(DeskTop, WIDGET(w));

	RectAssign(&r, WIDGET(w)->ChildArea.a.x + BTNSPACE, WIDGET(w)->ChildArea.a.y + BTNSPACE, WIDGET(w)->ChildArea.b.x - BTNSPACE, WIDGET(w)->ChildArea.a.y + BTNSPACE + text_height(default_font));
	l = CreateLabel(&Me, r, "Double click speed:");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));

	RectAssign(&r, WIDGET(w)->ChildArea.a.x + BTNSPACE, r.b.y + BTNSPACE, WIDGET(w)->ChildArea.b.x - BTNSPACE, r.b.y + BTNSPACE + 25);
	CPMouseDS = CreateSlider(&Me, r, MOUSESPEEDDS);
	CPMouseDS->Value = MOUSESPEEDDS - KeyGetInt("/SYSTEM/MOUSE/dblclk",4);
	InsertWidget(WIDGET(w), WIDGET(CPMouseDS));

	RectAssign(&r, WIDGET(w)->ChildArea.a.x + BTNSPACE, r.b.y + BTNSPACE*2, WIDGET(w)->ChildArea.b.x - BTNSPACE, r.b.y + BTNSPACE*2 + text_height(default_font));
	l = CreateLabel(&Me, r, "Test area:");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));

	RectAssign(&r, (WIDGET(w)->Absolute.b.x - WIDGET(w)->Absolute.a.x)/2 - (45/2), r.b.y + BTNSPACE, (WIDGET(w)->Absolute.b.x - WIDGET(w)->Absolute.a.x)/2 + (45/2), r.b.y + BTNSPACE + 45);
	c = CreateCanvas(&Me, r);
	WIDGET(c)->Draw = &CPMouseTestDraw;
	WIDGET(c)->EventHandler = CPMouseTestEH;
	InsertWidget(WIDGET(w), WIDGET(c));

	RectAssign(&r, WIDGET(w)->ChildArea.a.x + BTNSPACE, r.b.y + BTNSPACE + 35, WIDGET(w)->ChildArea.b.x - BTNSPACE, r.b.y + BTNSPACE + 35 + text_height(default_font));
	l = CreateLabel(&Me, r, "Cursor speed:");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));

	RectAssign(&r, WIDGET(w)->ChildArea.a.x + BTNSPACE, r.b.y + BTNSPACE, WIDGET(w)->ChildArea.b.x - BTNSPACE, r.b.y + BTNSPACE + 25);
	CPMouseMS = CreateSlider(&Me, r, MOUSESPEEDMS);
	CPMouseMS->Value = MOUSESPEEDMS - KeyGetInt("/SYSTEM/MOUSE/speed",0);
	InsertWidget(WIDGET(w), WIDGET(CPMouseMS));

	RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - BTNWIDTH/2 - BTNSPACE - BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5 - BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - BTNWIDTH/2 - BTNSPACE, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
	b = CreateButton(&Me, r, "OK", MSG_OK );
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - BTNWIDTH/2, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5 - BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + BTNWIDTH/2, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
	b = CreateButton(&Me, r, "Cancel", MSG_CANCEL );
	b->Icon = LoadImage("./SYSTEM/ICONS/cancel.bmp");
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + BTNWIDTH/2 + BTNSPACE, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5 - BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + BTNSPACE + BTNWIDTH/2 + BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
	b = CreateButton(&Me, r, "Apply", MSG_APPLY );
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetDrawAll(WIDGET(w));

	return true;
}

void Close (void)
{

}
