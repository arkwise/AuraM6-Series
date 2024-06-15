/////////////////////////////////////////////////////////////////////////////////
//
//	Screen Settings
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
#include "combobox.h"
#include "prevscr.h"

l_ulong AppVersion = ULONG_ID(0,0,1,0);
char    AppName[] = "Screen settings";
l_uid	nUID = "cp:setscr";
l_uid NeededLibs[] = { "widget","button","label","menu","window","combobox","prevscr","" };

#define BTNWIDTH 80
#define BTNHEIGHT 22
#define BTNSPACE 5

p_bitmap PrvScrImg[5]= {0,0,0,0,0};

l_int ScrId = 0;
PCombobox Df;
PCombobox Cl;
PCombobox Rl;
PPrevScr PS;

#define MSG_UPDATESCR 0x00FF0001


l_int TableW[] = { 640, 800, 1024, 1152, 1280 };
l_int TableH[] = { 480, 600, 768,  864,  1024 };
l_int TableD[] = { 8,   15,  16,   24,   32   };
l_int TableR[] = { 60, 70, 72, 75, 80, 85, 100};

l_bool AppEventHandler ( PWidget o, PEvent Event )
{
	if ( Event->Type == EV_MESSAGE )
	{
		switch ( Event->Message )
		{
			case MSG_CANCEL:
			case WM_CLOSE:
			{
				WidgetDispose(WIDGET(o));
				CloseApp(&Me);
				return true;
			}
			break;

			case MSG_UPDATESCR:
			{
				ScrId = ComboboxItemIndex(Df,Df->Selected)-1;

				PrevScrSetScr(PS,PrvScrImg[ScrId]);
			}
			break;

			case MSG_APPLY:
			case MSG_OK:
			{

				l_int a,b,c;
				a = ComboboxItemIndex(Df,Df->Selected)-1;
				b = ComboboxItemIndex(Cl,Cl->Selected)-1;
				c = ComboboxItemIndex(Rl,Rl->Selected)-1;

				KeySetInt("/SYSTEM/SCREEN/WIDTH",TableW[a]);
				KeySetInt("/SYSTEM/SCREEN/HEIGHT",TableH[a]);
				KeySetInt("/SYSTEM/SCREEN/DEPTH",TableD[b]);
				KeySetInt("/SYSTEM/SCREEN/REFRESH",TableR[c]);

				if ( Event->Message == MSG_OK ){
					WidgetDispose(WIDGET(o));
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
	PWindow w	= 0;
	PButton b = 0;
	PLabel l = 0;
	TRect r;

	if ( Me.Ressource )
	{
		PrvScrImg[0] = ListKey(Me.Ressource,"SCR640");
		PrvScrImg[1] = ListKey(Me.Ressource,"SCR800");
		PrvScrImg[2] = ListKey(Me.Ressource,"SCR1024");
	}

	RectAssign(&r,0, 0, 300, 350);

	w = CreateWindow(&Me, r, "Screen settings", WF_CAPTION|WF_FRAME|WF_CENTERED|WF_MINIMIZE);
	InsertWidget(WIDGET(DeskTop), WIDGET(w));

	if ( GSScreenWidth == 640 ) ScrId = 0;
	else if ( GSScreenWidth == 800 ) ScrId = 1;
	else/*if ( GSScreenWidth == 1024 )*/ ScrId = 2;

	WidgetSize(&r,150-(182/2), 5, 182, 163);
	PS = CreatePrevScr(&Me,r,PrvScrImg[ScrId]);
	InsertWidget(WIDGET(w), WIDGET(PS));

	WidgetSize(&r, 25, 180, 250, 20);
	l = CreateLabel(&Me,r,"Screen size:");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));

	WidgetSize(&r, 25, 200, 250, 20);
	Df = NewComboBox(&Me,r,MSG_UPDATESCR);
	ComboboxAddItem(Df,"640x480",NULL);
	ComboboxAddItem(Df,"800x600",NULL);
	ComboboxAddItem(Df,"1024x768",NULL);
	ComboboxAddItem(Df,"1152x864",NULL);
	ComboboxAddItem(Df,"1280x1024",NULL);
	InsertWidget(WIDGET(w), WIDGET(Df));

	if ( GSScreenWidth == 640 ) ComboboxSelectIndex(Df,1);
	else if ( GSScreenWidth == 800 ) ComboboxSelectIndex(Df,2);
	else if ( GSScreenWidth == 1024 ) ComboboxSelectIndex(Df,3);
	else if ( GSScreenWidth == 1152 ) ComboboxSelectIndex(Df,4);
	else if ( GSScreenWidth == 1280 ) ComboboxSelectIndex(Df,5);

	if ( GSScreenWidth == 640 ) ScrId = 0;
	else if ( GSScreenWidth == 800 ) ScrId = 1;
	else/*if ( GSScreenWidth == 1024 )*/ ScrId = 2;


	WidgetSize(&r, 25, 235, 110, 20);
	l = CreateLabel(&Me,r,"Color depth:");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));

	WidgetSize(&r, 25, 255, 110, 20);
	Cl = NewComboBox(&Me,r,0);
	ComboboxAddItem(Cl,"8 Bpp",NULL);
	ComboboxAddItem(Cl,"15 Bpp",NULL);
	ComboboxAddItem(Cl,"16 Bpp",NULL);
	ComboboxAddItem(Cl,"24 Bpp",NULL);
	ComboboxAddItem(Cl,"32 Bpp",NULL);
	InsertWidget(WIDGET(w), WIDGET(Cl));

	if ( GSScreenDepth == 8 ) ComboboxSelectIndex(Cl,1);
	else if ( GSScreenDepth == 15 ) ComboboxSelectIndex(Cl,2);
	else if ( GSScreenDepth == 16 ) ComboboxSelectIndex(Cl,3);
	else if ( GSScreenDepth == 24 ) ComboboxSelectIndex(Cl,4);
	else if ( GSScreenDepth == 32 ) ComboboxSelectIndex(Cl,5);


	WidgetSize(&r, 165, 235, 110, 20);
	l = CreateLabel(&Me,r,"Refresh rate:");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));

	WidgetSize(&r, 165, 255, 110, 20);
	Rl = NewComboBox(&Me,r,0);
	ComboboxAddItem(Rl,"60 Hz",NULL);
	ComboboxAddItem(Rl,"70 Hz",NULL);
	ComboboxAddItem(Rl,"72 Hz",NULL);
	ComboboxAddItem(Rl,"75 Hz",NULL);
	ComboboxAddItem(Rl,"80 Hz",NULL);
	ComboboxAddItem(Rl,"85 Hz",NULL);
	ComboboxAddItem(Rl,"100 Hz",NULL);
	InsertWidget(WIDGET(w), WIDGET(Rl));

	if ( GSScreenRefreshRate == 60 ) ComboboxSelectIndex(Rl,1);
	else if ( GSScreenRefreshRate == 70 ) ComboboxSelectIndex(Rl,2);
	else if ( GSScreenRefreshRate == 72 ) ComboboxSelectIndex(Rl,3);
	else if ( GSScreenRefreshRate == 75 ) ComboboxSelectIndex(Rl,4);
	else if ( GSScreenRefreshRate == 80 ) ComboboxSelectIndex(Rl,5);
	else if ( GSScreenRefreshRate == 100 ) ComboboxSelectIndex(Rl,6);

	RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - BTNWIDTH/2 - BTNSPACE - BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5 - BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - BTNWIDTH/2 - BTNSPACE, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
	b = CreateButton(&Me, r, "OK", MSG_OK );
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - BTNWIDTH/2, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5 - BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + BTNWIDTH/2, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
	b = CreateButton(&Me, r, "Cancel", MSG_CANCEL );
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + BTNWIDTH/2 + BTNSPACE, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5 - BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + BTNSPACE + BTNWIDTH/2 + BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
	b = CreateButton(&Me, r, "Apply", MSG_APPLY );
	InsertWidget(WIDGET(w), WIDGET(b));

	WIDGET(w)->AppEvHdl = &AppEventHandler;

	WidgetDrawAll(WIDGET(w));

	return true;
}

void Close (void)
{

}
