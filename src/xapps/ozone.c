////////////////////////////////////////////////////////////////////////////////
//
//	OZ Desktop
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "button.h"
#include "label.h"
#include "canvas.h"
#include "menu.h"
#include "list.h"
#include "skin.h"
#include "window.h"
#include "fms2.h"

#include "OZD/desktop.h"

#include <math.h>

l_ulong AppVersion = ULONG_ID(0, 0, 1, 0);
char AppName[] = "OZ Desktop";
l_uid nUID = "app:desktop";
l_uid NeededLibs[] = { "widget", "skin", "button", "label", "menu", "fms2", "window", "grfx", "" };

l_bool AppEventHandler(PWidget o, PEvent Ev);

// Include the desktop files
#include "OZD/theme.c"
#include "OZD/wallppr.c"
#include "OZD/panel.c"

void GenerateSubMenu(PMenuItem o, void* Args)
{
	l_text n; p_bitmap i16; l_ulong fi;

	PMenuItem m = NULL;
	PListItem a, b;
	PList l;

	l = ListDir(Args, 0);
	if (l)
	{
		if (l->Last)
		{
			a = b = l->Last;
			if (a)
				do
				{
					if (FILEINFO(a->Data)->Name[0] != '.')
					{
						if (TextRChr(FILEINFO(a->Data)->Name, '.'))
							n = TextNDup(FILEINFO(a->Data)->Name, TextRChr(FILEINFO(a->Data)->Name, '.') - FILEINFO(a->Data)->Name);
						else
							n = TextDup(FILEINFO(a->Data)->Name);
						//FileGetIcons2(FILEINFO(a->Data),&i16,NULL,NULL,&fi);
						i16 = FileGetIcon2(FILEINFO(a->Data), 16, &fi);
						m = NewMenuItemEx(n, i16, IDM_PROGRAMS, fi ? MIF_FREEICON : 0, NULL, NULL, TextDup(FILEINFO(a->Data)->VPathName), m);
						free(n);
					}

					a = a->Prev;
				} while (a != b);
		}
		FreeList(l);
	}

	l = ListDir(Args, FA_DIREC | FA_LABEL);

	if (l) {
		if (l->Last)
		{
			a = b = l->Last;
			if (a)
				do
				{
					if (FILEINFO(a->Data)->Attributes & FA_DIREC) {
						//FileGetIcons2(FILEINFO(a->Data),&i16,NULL,NULL,&fi);
						i16 = FileGetIcon2(FILEINFO(a->Data), 16, &fi);
						m = NewMenuItemEx(FILEINFO(a->Data)->Name, i16, 0, 0, NULL, &GenerateSubMenu, TextDup(FILEINFO(a->Data)->VPathName), m);
					}
					a = a->Prev;
				} while (a != b);
		}
		FreeList(l);
	}

	o->SubMenu = NewMenu(m);
}

void DesktopWidgetInsert(PWidget o, PWidget i)
{
	_WidgetInsert(o, i);

	if (WidgetIsClassId(i, ULONG_ID('W', 'i', 'n', ' ')))
	{
		//DebugMessage("TaskbarAdd %x",i);
		TaskbarAdd(WINDOW(i));
	}
}

void DesktopWidgetSetFirstEx(PWidget o, PWidget i)
{
	_WidgetSetFirstEx(o, i);

	if (WidgetIsClassId(i, ULONG_ID('W', 'i', 'n', ' ')))
	{
		if (WINDOW(i)->WindowStates & WS_MINIMIZED)
		{
			WINDOW(i)->WindowStates &= ~WS_MINIMIZED;
		}

		TaskbarRedraw(WINDOW(i));
	}
}

void DesktopWidgetRemove(PWidget o, PWidget i)
{
	_WidgetRemove(o, i);

	if (WidgetIsClassId(i, ULONG_ID('W', 'i', 'n', ' ')))
	{
		//DebugMessage("TaskbarRemove %x",i);
		TaskbarRemove(WINDOW(i));
	}
}

/**
 *	Global variables
 */
PMenu Menu = 0;
PTimer ClockTimer = 0;

l_bool AppEventHandler(PWidget o, PEvent Ev)
{
	switch (Ev->Message)
	{
	case BTN_START:
	{
		TPoint p;
		PointAssign(&p, WIDGET(GSStart)->Absolute.a.x - 5, WIDGET(GSStart)->Absolute.b.y + 1);
		PopUpMenu(&Me, p, Menu, WIDGET(GSStart), 0);
		return true;
	}
	break;

	case IDM_QUIT:
	{

		if (MessageBox(&Me, "Codename Phoenix", "Are you sure you want to exit oZone?", MBB_YESNO | MBI_QUESTION) == MSG_YES)
		{
			TEvent Event;
			EventAssign(&Event, EV_MESSAGE, MSG_QUIT, NULL, KState, Mouse->State);
			HandleEvent(&Event);

		}
		return true;
	}
	break;

	/**
	 *	Start menu items
	 */
	case IDM_PROGRAMS:
	{
		FileRun(MENUITEM(Ev->Extra)->Args, NULL);
		return true;
	}
	break;

	case IDM_CP:
	{
		FileRun("xapps/nav.app", "/Control Panel/");
		return true;
	}
	break;

	case IDM_ABOUT:
	{
		FileRun("xapps/about.app", "about");
		return true;
	}
	break;
	}

	return false;
}

l_int Main(int argc, l_text *argv)
{
	/**
	 *	Export symbols
	 */
	APPEXPORT(TraybarAdd);
	APPEXPORT(TraybarRemove);
	APPEXPORT(WallpaperReLoad);

	/**
	 *	Get desktop theme
	 */

	ThemeLoad(KeyGetText("/USER/DESKTOP/ozone", "./DESKTOP/classix.ini"));

	/**
	 *	Initialize panels
	 */
	WallpaperInit();
	GSPanelInit();

	Menu = NewMenu(
		NewMenuItemEx("Games", NULL, 0, 0, NULL, &GenerateSubMenu, "SYSTEM/MENU/games", NULL)
	);
	AddMenuItemEx(Menu, "Media", NULL, 0, 0, NULL, &GenerateSubMenu, "SYSTEM/MENU/media");
	AddMenuItemEx(Menu, "Office", NULL, 0, 0, NULL, &GenerateSubMenu, "SYSTEM/MENU/tools");
	AddMenuItemEx(Menu, "Extra", NULL, 0, 0, NULL, &GenerateSubMenu, "SYSTEM/MENU/extra");
	AddMenuItemEx(Menu, "About", NULL, IDM_PROGRAMS, 0, NULL, NULL, NULL);
	AddMenuItemEx(Menu, "Term", NULL, IDM_CP, 0, NULL, NULL, NULL);

	ClockTimer = NewTimer(&Me, 1000 * 60, &RefreshPanelClock, NULL);

	Desktop->Insert = &DesktopWidgetInsert;
	Desktop->Remove = &DesktopWidgetRemove;
	Desktop->SetFirstEx = &DesktopWidgetSetFirstEx;

	return true;
}

void Close(void)
{
	Desktop->Insert = &_WidgetInsert;
	Desktop->Remove = &_WidgetRemove;
	Desktop->SetFirstEx = &_WidgetSetFirstEx;

	GSPanelDestroy();
	GSWallpaperDestroy();
	KillTimer(ClockTimer);
}
