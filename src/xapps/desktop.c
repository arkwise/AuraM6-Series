/////////////////////////////////////////////////////////////////////////////////
//
//	Desktop
//
//	Aura Milestone 6 (june 2024)
//
//	(c) doscore 2007-2024
//
/////////////////////////////////////////////////////////////////////////////////

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
#include "sound.h"

#include "desktop/desktop.h"

#include <math.h>

l_ulong AppVersion = ULONG_ID(0, 0, 3, 0);
char AppName[] = "Desktop";
l_uid nUID = "app:desktop";
l_uid NeededLibs[] = {"widget", "skin", "button", "label", "menu", "fms2", "window", "grfx", ""};

l_bool AppEventHandler(PWidget o, PEvent Ev);


/*

for the time being i have disbaled translucent docks and windows
0030 desktop taskbar has been fixed. windows can now minimize to the taskbar and reopen. about time!!

*/



// Include the desktop files
#include "desktop/theme.c"
#include "desktop/wallppr.c"
#include "desktop/panel.c"

#define IDM_doslynx 0xFFFF9902

#define IDM_ftp 0xFFFF9903

#define IDM_dillo 0xFFFF9904

TRect r, wr;
PButton b;
PLabel l;
PWindow w = NULL;

void GenerateSubMenu(PMenuItem o, void *Args)
{
        l_text n;
        p_bitmap i16;
        l_ulong fi;

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

        if (l)
        {
                ;
                if (l->Last)
                {
                        a = b = l->Last;
                        if (a)
                                do
                                {
                                        if (FILEINFO(a->Data)->Attributes & FA_DIREC)
                                        {
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
                DebugMessage("TaskbarAdd %x", i);
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
                DebugMessage("TaskbarRemove %x...", i);
                TaskbarRemove(WINDOW(i));
                DebugMessage("...finished");
        }
}
//PTimer  WelcomeTimer      = 0;
//void WelcomeWidget()
//{
//		WelcomeTimer->Enable = 0;
//		MessageBox(&Me, "Welcome", "Welcome To Aura", MBI_HARDWARE);
//}

/**
*       Global variables
*/
PMenu Menu = 0;
PMenu Menu2 = 0;
PMenu Menu3 = 0;
PTimer ClockTimer = 0;

void Livecd(void)
{
        //stream = LoadData2("/system/System/sounds/welcome.mp3",DTYPE_SOUNDSTREAM); doesnt works

        GSPanelInit();

        //PlaySoundStream(stream);

        ClockTimer = NewTimer(&Me, 1000 * 60, &RefreshPanelClock, NULL);

        Menu2 = NewMenu(
            NewMenuItemEx("Games", NULL, 0, 0, NULL, &GenerateSubMenu, "SYSTEM/MENU/games",
                          NewMenuItemEx("Media", NULL, 0, 0, NULL, &GenerateSubMenu, "SYSTEM/MENU/media",
                                        NewMenuItemEx("Office", NULL, 0, 0, NULL, &GenerateSubMenu, "SYSTEM/MENU/tools",
                                                      NewMenuItemEx("Extra", NULL, 0, 0, NULL, &GenerateSubMenu, "SYSTEM/MENU/extra",
                                                                    NewMenuItem("About", NULL, IDM_NAV, 0, NULL,
                                                                     NewMenuItem("Term", NULL, IDM_ftp, 0, NULL,NULL)))))));

        Menu3 = NewMenu(
            NewMenuItemEx("Pictures", NULL, 5, 10, NULL, &GenerateSubMenu, "/system/User/Pix",
                          NewMenuItemEx("Music", NULL, 5, 10, NULL, &GenerateSubMenu, "/system/User/Music",
                                        NewMenuItemEx("Video", NULL, 5, 10, NULL, &GenerateSubMenu, "/User/dox", NULL))));

        Desktop->Insert = &DesktopWidgetInsert;
        Desktop->Remove = &DesktopWidgetRemove;
        Desktop->SetFirstEx = &DesktopWidgetSetFirstEx;
}

l_bool AppEventHandler(PWidget o, PEvent Ev)
{
        switch (Ev->Message)
        {
        case BTN_APP:
        {
                TPoint p;
                PointAssign(&p, WIDGET(GSApp)->Absolute.a.x - 5, WIDGET(GSApp)->Absolute.b.y - 30);
                PopUpMenu(&Me, p, Menu2, WIDGET(GSApp), 1);
                return true;
        }
        break;

        case BTN_navbtn:
        {
                FileRun("xapps/nav.app", "/");
                return true;
        }
        break;

        case BTN_Ok:
        {
                FileRun("xapps/term.app", NULL);
                WidgetDispose(WIDGET(w));
                return true;
        }

        case BTN_Ok2:
        {
                WidgetDispose(WIDGET(w));
                Livecd();
                return true;
        }

        case BTN_USR:
        {
                TPoint p;
                PointAssign(&p, WIDGET(GSUSR)->Absolute.a.x - 5, WIDGET(GSUSR)->Absolute.b.y - 30);
                PopUpMenu(&Me, p, Menu3, WIDGET(GSUSR), 1);
                return true;
        }
        break;

        case BTN_CP:
        {
                FileRun("xapps/nav.app", "/Control Panel/");
                return true;
        }
        break;

        case IDM_QUIT:
        {
                if (MessageBox(&Me, "Quit", "Are you really sure you want to Quit Aura?", MBB_YESNO | MBI_QUESTION) == MSG_YES)
                {
                        TEvent Event;
                        EventAssign(&Event, EV_MESSAGE, MSG_QUIT, NULL, KState, Mouse->State);
                        HandleEvent(&Event);
                }
                return true;
        }
        break;

        /**
                *       Start menu items
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

        case IDM_NAV:
        {
                FileRun("xapps/nav.app", "/");
                return true;
        }
        break;

        case IDM_ABOUT:
        {
                FileRun("xapps/about.app", "about");
                return true;
        }
        break;

        case IDM_doslynx:
        {
                FileRun("xapps/rundos.app", "/v/applications/doslynx/doslynxs.exe");
                return true;
        }
        break;

        case IDM_dillo:
        {
                FileRun("xapps/rundos.app", "/v/applications/dillodos/dillo.bat");
                return true;
        }
        break;

        case IDM_ftp:
        {
                FileRun("xapps/term.app", "Terminal");
                return true;
        }
        break;
        }

        return false;
}

l_int Main(int argc, l_text *argv)
{
        /**
    *       Export symbols
    */
        APPEXPORT(TraybarAdd);
        APPEXPORT(TraybarRemove);
        APPEXPORT(WallpaperReLoad);

        /**
     *      Get desktop theme
     */

        ThemeLoad(KeyGetText("/user/desktop/theme", "/system/theme/aura.ini"));

        /**
     *      Initialize panels
     */
        WallpaperInit();

        // Draw Some Window //
        RectAssign(&r, 0, 0, 255, 90);
        w = CreateWindow(&Me, r, "Welcome", WF_FRAME | WF_CAPTION | WF_MINIMIZE | WF_CENTERED);
        InsertWidget(DeskTop, WIDGET(w));
        WIDGET(w)->AppEvHdl = &AppEventHandler;

        RectAssign(&r, 5, 0, 250, 15);
        l = CreateLabel(&Me, r, "Welcome to Aura M6, ");
        WIDGET(l)->BackgroundColor = COL_3DFACE;
        InsertWidget(WIDGET(w), WIDGET(l));

        RectAssign(&r, 5, 20, 250, 35);
        l = CreateLabel(&Me, r, "Which desktop interface would you like to use?");
        WIDGET(l)->BackgroundColor = COL_3DFACE;
        InsertWidget(WIDGET(w), WIDGET(l));

        RectAssign(&r, 5, 40, 250, 60);
        b = CreateButton(&Me, r, "Aura Desktop", BTN_Ok2);
        InsertWidget(WIDGET(w), WIDGET(b));

        RectAssign(&r, 5, 65, 250, 85);
        b = CreateButton(&Me, r, "Terminal", BTN_Ok);
        InsertWidget(WIDGET(w), WIDGET(b));

        WidgetDrawAll(WIDGET(w));
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
