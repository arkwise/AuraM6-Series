//
//
//   this is the xtheme application as of 22/08/2022
//
//
//

// started again on 8th jan 2023, trying to add functionality to the program. badsaddam 0.3v
// fixed theme install function but requires reboot to take effect.
//


// 0.4 the next step is to make tabs for the two different desktop themes and window skins.
// 0.5 add resource images to demo the scene.
// 

#include "kernel.h"
#include "widget.h"
#include "label.h"
#include "button.h"
#include "window.h"
#include "combobox.h"
#include "prevscr.h"
#include "listbox.h"

// added tcp includes

/* #include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <io.h>
#include <tcp.h>
*/

l_ulong AppVersion = ULONG_ID(0,0,3,0);
char    AppName[] = "xTheme";
l_uid	nUID = "cp:appstore";
l_uid NeededLibs[] = { "widget","button","label","listbox","window","" };

// define actions here

// this will save everything on close
#define MSG_SAVEREG   	0x00010002

// you can click on it to save without closing
#define MSG_SAVEVALUE   0x00010007
#define MSG_RELOADREG 	0x00010001
#define MSG_SETKEY      0xFF00FF10
#define MBB_OK          0x000000100
//#define MSG_SETGUI      0xFF00FF11

// theme info test

#define WM_THEME        0xFFFF5555

p_bitmap BMP_Theme = 0;
PListbox l;

l_bool ReloadSkin (l_text File);

// set registry to selected skin

l_bool AppEventHandler ( PWidget o, PEvent Event )
{
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

			case MSG_SETKEY:
				/*if ( Event->Extra )
					ReloadSkin (Event->Extra);*/
				KeySetText ("/USER/GUI/SKIN", (l_text)Event->Extra);
				// MessageBox ( &Me, "Theme name","Theme version 0.3\n\nAura M5\n\n(c) Theme creator \n\n this feature is coming soon", MBB_OK );

			break;

			case MSG_APPLY:
				/*if ( Event->Extra )
					ReloadSkin (Event->Extra);*/
				KeySetText ("/USER/DESKTOP/theme", (l_text)Event->Extra);
			   // MessageBox ( &Me, "Theme name","Theme version 0.3\n\nAura M5\n\n(c) Theme creator \n\n this feature is coming soon", MBB_OK );

			break;

// this part will actually update and save it to the registry

			case MSG_SAVEREG:
		{
			RegistrySave();
			return true;
		}

			case WM_ABOUT:
		{

			MessageBox ( &Me, "xTheme Manager","Theme manager 0.3\n\nAura M5\n\n(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved. \n\n Doscore 2007-2022", MBB_OK );


			return true;
		}
		break;
		case WM_THEME:
		{

			MessageBox ( &Me, "Theme name","Theme version 0.3\n\nAura M5\n\n(c) Theme creator \n\n this feature is coming soon", MBB_OK );


			return true;
		}
		break;
			case MSG_RELOADREG:
		{
			RegistryReLoad();
			//EmptySubTreeItems(t->Items);
			//AddTreeItem(t->Items,"Registry","/",NULL,&GenerateSubs);

			//WidgetDrawAll(WIDGET(w));

			return true;
		}
		break;


		}
	}

	return false;
}


// add these to the registry to allow for translucent windows. it will slow the system down however.
// taken from window.c
// 	aw_trans = KeyGetInt("/USER/DESKTOP/transparency/active_window", 255);
//	pw_trans = KeyGetInt("/USER/DESKTOP/transparency/passive_window", 255);


// the main function of the app is here?


l_int Main ( int argc, l_text *argv )
{
	PWindow w	= 0;
	PButton b = 0;
	//PListbox l = 0;
	TRect r;
	// added for text 23
	//PLabel p = 0;
	PLabel j;
	PLabel j1;
	PLabel j2;
	PLabel j3;
	PLabel j4;




	BMP_Theme = LoadImage("./SYSTEM/ICONS/theme.bmp");

	// draw the window here

	RectAssign(&r,0, 0, 645, 435);
	w = CreateWindow(&Me, r, "xTheme Manager 0.4v 2023", WF_CAPTION|WF_FRAME|WF_CENTERED|WF_MINIMIZE);
	InsertWidget(WIDGET(DeskTop), WIDGET(w));

	// draw dropdown menu

// #1leftboarder  #2 top boarder #3 width #4 bottom of element from the top

	RectAssign(&r,155, 10, 500, 60);
	j = CreateLabel(&Me, r, "Welcome to the new Aura M5 Theme and skin manager,");
	WIDGET(j)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(j));

	RectAssign(&r,155, 25, 500, 60);
	j = CreateLabel(&Me, r, "Window Skins can be changed here.");
	WIDGET(j)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(j));

	RectAssign(&r,155, 160, 500, 225);
	j = CreateLabel(&Me, r, "Desktop Themes can be changed here.");
	WIDGET(j)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(j));

	RectAssign(&r,155, 180, 500, 225);
	j = CreateLabel(&Me, r, "Eventually there will be downloads and images as an example");
	WIDGET(j)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(j));


/*
	// draw gui skins
   WidgetSize(&r, 150, 20, 135, 20);
	p = CreateLabel(&Me,r,"Wallpaper");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l)); 

	*/

	RectAssign(&r,10, 10, 150, 150);
	l = CreateListbox(&Me, r);
	l->AddItem(l, "Ximian Industrial", "ozone.ini", BMP_Theme);
	l->AddItem(l, "Classix", "Classix.ini", BMP_Theme);
	l->AddItem(l, "Ozone", "ozone.ini", BMP_Theme);
	l->AddItem(l, "Carbon", "carbon.ini", BMP_Theme);
	l->AddItem(l, "Black", "black.ini", BMP_Theme);
	l->AddItem(l, "Aura", "aura.ini", BMP_Theme);
	l->OnSelect = 0xFF00FF10;
	InsertWidget(WIDGET(w), WIDGET(l));

    // draw desktop theme
//x,y,z is width,a

	RectAssign(&r,10, 160, 150, 300);
	l = CreateListbox(&Me, r);
	l->AddItem(l, "Silver", "system/theme/silver.ini", BMP_Theme);
	l->AddItem(l, "Netdock", "system/theme/netdock.ini", BMP_Theme);
	l->AddItem(l, "Classix", "system/theme/Classix.ini", BMP_Theme);
	l->AddItem(l, "Aura m4", "system/theme/carbon.ini", BMP_Theme);
	l->AddItem(l, "Aura m5", "system/theme/black.ini", BMP_Theme);
	l->AddItem(l, "Chase's theme", "system/theme/aura.ini", BMP_Theme);
	l->OnSelect = MSG_APPLY;
	InsertWidget(WIDGET(w), WIDGET(l));

	// draw gui theme


	// assign button to install theme

	RectAssign(&r, 515, 10, 635, 40);
	b = CreateButton(&Me, r, "Apply", MSG_SAVEREG );
	b->Icon = LoadImage("./SYSTEM/ICONS/add.bmp");
	b->Flags |= BF_ALIGNLEFT;
	//l->OnSelect = MSG_SETKEY;
	InsertWidget(WIDGET(w), WIDGET(b));

	// theme details can be loaded like the about MB fucntion but we need to have this information read from the theme ini


	RectAssign(&r, 515, 45, 635, 75);
	b = CreateButton(&Me, r, "Theme Details...", WM_THEME );
	b->Icon = LoadImage("./SYSTEM/ICONS/tools.bmp");
	b->Flags |= BF_ALIGNLEFT;
	InsertWidget(WIDGET(w), WIDGET(b));

	// 

	RectAssign(&r, 515, 80, 635, 110);
	b = CreateButton(&Me, r, "Download", MSG_SETKEY );
	b->Icon = LoadImage("./SYSTEM/ICONS/save.bmp");
	b->Flags |= BF_ALIGNLEFT;
    // l->OnSelect = MSG_SETKEY;
	InsertWidget(WIDGET(w), WIDGET(b));

	// clos application with custom image

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
