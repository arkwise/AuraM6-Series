/////////////////////////////////////////////////////////////////////////////////
//
//	Theme Changer
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
#include "listbox.h"

l_ulong AppVersion = ULONG_ID(0,0,1,0);
char    AppName[] = "Theme Manager";
l_uid	nUID = "cp:settheme";
l_uid NeededLibs[] = { "widget","button","label","listbox","window","" };

p_bitmap BMP_Theme = 0;
PListbox l;

l_bool ReloadSkin (l_text File);

l_bool AppEventHandler ( PWidget o, PEvent Event )
{
char temp[300];
	if ( Event->Type == EV_MESSAGE )
	{
		switch ( Event->Message )
		{
			case WM_CLOSE:
			case MSG_OK:
			{
				WidgetDispose(WIDGET(o));
				CloseApp(&Me);

				return true;
			}
			break;

			case 0xFF00FF10:
				if ( Event->Extra )
   {
					//ReloadSkin (Event->Extra);*/
				KeySetText ("/USER/GUI/SKIN", (l_text)Event->Extra);
   sprintf(temp, "You changed the skin to %s. Please reload oZone!", (l_text)Event->Extra);
   MessageBox(&Me, "Theme", temp, MBB_OK);
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
	//PListbox l = 0;
	TRect r;

	PRegKey a, e;
	PRegKey p = ResolveKey("/USER/GUI/SKINS");


	BMP_Theme = LoadImage("./SYSTEM/ICONS/theme.bmp");

	RectAssign(&r,0, 0, 645, 435);
	w = CreateWindow(&Me, r, "Theme Manager", WF_CAPTION|WF_FRAME|WF_CENTERED|WF_MINIMIZE);
	InsertWidget(WIDGET(DeskTop), WIDGET(w));

	RectAssign(&r,10, 10, 510, 360);
	l = CreateListbox(&Me, r);
//	l->AddItem(l, "No skin (default)", "none.ini", BMP_Theme);
	l->AddItem(l, "Ximian Industrial (default skin)", "ozone.ini", BMP_Theme);
//	l->AddItem(l, "Classix", "Classix.ini", BMP_Theme);

 if ( p )
		if ( p->Last ) {
			a = e = p->Last->Next;
		  do {
     l->AddItem(l, a->Name, (l_text)a->Data, BMP_Theme);
			  a = a->Next;
		  } while ( a != e );
		}

	l->OnSelect = 0xFF00FF10;
	InsertWidget(WIDGET(w), WIDGET(l));

/*
	RectAssign(&r, 515, 10, 635, 40);
	b = CreateButton(&Me, r, "Install Theme...", 0 );
	b->Icon = LoadImage("./SYSTEM/ICONS/add.bmp");
	b->Flags |= BF_ALIGNLEFT;
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r, 515, 45, 635, 75);
	b = CreateButton(&Me, r, "Theme Details...", 0 );
	b->Icon = LoadImage("./SYSTEM/ICONS/tools.bmp");
	b->Flags |= BF_ALIGNLEFT;
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r, 515, 80, 635, 110);
	b = CreateButton(&Me, r, "Save Theme...", 0 );
	b->Icon = LoadImage("./SYSTEM/ICONS/save.bmp");
	b->Flags |= BF_ALIGNLEFT;
	InsertWidget(WIDGET(w), WIDGET(b));
*/

	RectAssign(&r,550, 390, 635, 425);
	b = CreateButton(&Me, r, "Close", MSG_OK );
	b->Icon = LoadImage("./SYSTEM/ICONS/close.bmp");
	InsertWidget(WIDGET(w), WIDGET(b));

	WIDGET(w)->AppEvHdl = &AppEventHandler;

	WidgetDrawAll(WIDGET(w));

	return true;
}

void Close (void)
{
	DestroyImage(BMP_Theme);
}
