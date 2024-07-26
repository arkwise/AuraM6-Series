/////////////////////////////////////////////////////////////////////////////////
//
//  Desktop
//
//  Aura Milestone 6 (june 2024)
//
//  (c) doscore 2007-2024
//
//////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "button.h"
#include "label.h"
#include "canvas.h"
#include "tbox.h" // Include tbox.h for textboxes
#include "menu.h"
#include "list.h"
#include "skin.h"
#include "window.h"
#include "fms2.h"
#include "sound.h"
#include "registry.h"

#include "desktop/desktop.h"

#include <math.h>

l_ulong AppVersion = ULONG_ID(0, 0, 3, 0);
char AppName[] = "Desktop";
l_uid nUID = "app:desktop";
l_uid NeededLibs[] = {"widget", "skin", "button", "label", "menu", "fms2", "window", "grfx", ""};

l_bool AppEventHandler(PWidget o, PEvent Ev);
void ShowLoginWindow();
void ShowRegisterWindow();

#define BTN_LOGIN       0x4269
#define BTN_CANCEL      0x4270
#define BTN_REGISTER    0x4271
#define BTN_BACK        0x4272
#define BTN_REGISTER_USER 0x4273

// Include the desktop files
#include "desktop/theme.c"
#include "desktop/wallppr.c"
#include "desktop/panel.c"

#define IDM_doslynx 0xFFFF9902
#define IDM_ftp 0xFFFF9903
#define IDM_dillo 0xFFFF9904
//#define IDM_QUIT 0xFFFF9905//

TRect r, wr;
PButton b;
PLabel l;
PWindow w = NULL;
PTextbox usernameTextbox;
PTextbox passwordTextbox;
PTextbox nameTextbox;
PTextbox emailTextbox;
PTextbox usernameRegisterTextbox;
PTextbox passwordRegisterTextbox;
PTextbox confirmPasswordTextbox;

p_bitmap Bmp1 = NULL;  // Variable to store the resource image

// Volume-related variables and definitions
PWidget VOLEventHandler = 0;
l_int VolumeMute = 0;
PMenu VolTraybarMenu = 0;

l_bool VolumeEventHandler(PWidget o, PEvent Event)
{
    if (Event->Type == EV_MESSAGE)
    {
        switch (Event->Message)
        {
        case IDM_VOL_ADJ:
        {
            FileRun("xapps/setvol.app", NULL);
            return true;
        }
        break;

        case IDM_MUTE:
        {
            l_int VolumeDigi = KeyGetInt("/USER/SOUND/VOLUMEDIGI", 255);
            l_int VolumeMidi = KeyGetInt("/USER/SOUND/VOLUMEMIDI", 255);
            VolumeMute = KeyGetInt("/USER/SOUND/VOLUMEMUTE", 0);
            if (VolumeMute == 0)
            {
                VolumeMute = 1;
                KeySetInt("/USER/SOUND/VOLUMEMUTE", VolumeMute);
                set_volume(0, 0);
            }
            else
            {
                VolumeMute = 0;
                KeySetInt("/USER/SOUND/VOLUMEMUTE", VolumeMute);
                set_volume(VolumeDigi, VolumeMidi);
            }
            return true;
        }
        break;
        }
    }

    return false;
}

void VolumeInit(void)
{
    l_int VolumeDigi = KeyGetInt("/USER/SOUND/VOLUMEDIGI", 255);
    l_int VolumeMidi = KeyGetInt("/USER/SOUND/VOLUMEMIDI", 255);
    VolumeMute = KeyGetInt("/USER/SOUND/VOLUMEMUTE", 0);
    if (VolumeMute == 0)
        set_volume(VolumeDigi, VolumeMidi);
    else
        set_volume(0, 0);
}

void VolumeTrayInit(void)
{
    TRect r;
    RectAssign(&r, 0, 0, 0, 0);
    VOLEventHandler = CreateWidget(&Me, r);
    VOLEventHandler->AppEvHdl = &VolumeEventHandler;

    VolTraybarMenu = NewMenu(
        NewMenuItem("Adjust Volume", NULL, IDM_VOL_ADJ, 0, NULL,
                    NewMenuItem("Mute/UnMute Volume", NULL, IDM_MUTE, 0, NULL, NULL)));

    TraybarAdd(LoadImage("SYSTEM/ICONS/vol.bmp"), "Adjust Volume", VolTraybarMenu, VOLEventHandler);
    WidgetSetTooltipText(WIDGET(GSCP), "Volume");
}

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
        if (l->Last)
        {
            a = b = l->Last;
            if (a)
                do
                {
                    if (FILEINFO(a->Data)->Attributes & FA_DIREC)
                    {
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

PMenu Menu = 0;
PMenu Menu2 = 0;
PMenu Menu3 = 0;
PTimer ClockTimer = 0;

void Livecd(void)
{
    GSPanelInit();
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

    // Initialize Volume Traybar
    VolumeTrayInit();
}

l_bool LoginAppEventHandler(PWidget o, PEvent Ev) {
    switch (Ev->Message) {
        case BTN_LOGIN: {
            // Instead of authentication logic, directly call Livecd()
            Livecd();
            return true;
        }
        case BTN_REGISTER: {
            WidgetDispose(WIDGET(w));
            ShowRegisterWindow();
            return true;
        }
        case BTN_CANCEL: {
            // Trigger shutdown event
            TEvent Event;
            EventAssign(&Event, EV_MESSAGE, IDM_QUIT, NULL, KState, Mouse->State);
            HandleEvent(&Event);
            return true;
        }
        // Handle other events as needed
    }

    return false; // Return false if the event was not handled
}

l_bool RegisterAppEventHandler(PWidget o, PEvent Ev) {
    switch (Ev->Message) {
        case BTN_BACK: {
            WidgetDispose(WIDGET(w));
            ShowLoginWindow();
            return true;
        }
        case BTN_REGISTER_USER: {
            // Handle registration logic here
            return true;
        }
        // Handle other events as needed
    }

    return false; // Return false if the event was not handled
}

void DrawCanvas(PWidget o, p_bitmap buffer, PRect w)
{
    if (Bmp1) {
        draw_sprite(buffer, Bmp1, (o->Absolute.a.x + o->Absolute.b.x - Bmp1->w) / 2, 
                              (o->Absolute.a.y + o->Absolute.b.y - Bmp1->h) / 2);
    }
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

void ShowLoginWindow()
{
    // Draw Login Window
    RectAssign(&r, 0, 0, 400, 280);  // Adjusted window height
    w = CreateWindow(&Me, r, "Login", WF_FRAME | WF_CAPTION | WF_MINIMIZE | WF_CENTERED);
    InsertWidget(DeskTop, WIDGET(w));
    WIDGET(w)->AppEvHdl = &LoginAppEventHandler;

    // Create canvas for image
    RectAssign(&r, 0, 0, 400, 45);
    PCanvas c = CreateCanvas(&Me, r);
    WIDGET(c)->Draw = &DrawCanvas;
    WIDGET(c)->Flags |= WFForceBuffer;
    InsertWidget(WIDGET(w), WIDGET(c));

    // Create labels and textboxes for registration
    RectAssign(&r, 50, 60, 350, 80);
    l = CreateLabel(&Me, r, "Welcome back to Aura. Please enter Username and Password");
    WIDGET(l)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(l));

    // Create labels and textboxes for username and password
    RectAssign(&r, 50, 100, 350, 120);
    l = CreateLabel(&Me, r, "Username:");
    WIDGET(l)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(l));

    RectAssign(&r, 50, 130, 350, 150);
    usernameTextbox = CreateTextbox(&Me, r, TBF_EDITABLE);
    InsertWidget(WIDGET(w), WIDGET(usernameTextbox));

    RectAssign(&r, 50, 160, 350, 180);
    l = CreateLabel(&Me, r, "Password:");
    WIDGET(l)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(l));

    RectAssign(&r, 50, 190, 350, 210);
    passwordTextbox = CreateTextbox(&Me, r, TBF_EDITABLE | TBF_PASSWORD);
    InsertWidget(WIDGET(w), WIDGET(passwordTextbox));

    // Add guest login information label
    RectAssign(&r, 50, 220, 350, 240);
    l = CreateLabel(&Me, r, "Login for guest access: Username = guest, No password");
    WIDGET(l)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(l));

    // Create login, shutdown, and register buttons
    RectAssign(&r, 50, 250, 120, 270);
    b = CreateButton(&Me, r, "Login", BTN_LOGIN);
    InsertWidget(WIDGET(w), WIDGET(b));

    RectAssign(&r, 150, 250, 220, 270);
    b = CreateButton(&Me, r, "Shutdown", BTN_CANCEL);
    InsertWidget(WIDGET(w), WIDGET(b));

    RectAssign(&r, 250, 250, 320, 270);
    b = CreateButton(&Me, r, "Register", BTN_REGISTER);
    InsertWidget(WIDGET(w), WIDGET(b));

    WidgetDrawAll(WIDGET(w));
}

void ShowRegisterWindow()
{
    // Draw Register Window
    RectAssign(&r, 0, 0, 400, 340);  // Adjusted window height
    w = CreateWindow(&Me, r, "Register", WF_FRAME | WF_CAPTION | WF_MINIMIZE | WF_CENTERED);
    InsertWidget(DeskTop, WIDGET(w));
    WIDGET(w)->AppEvHdl = &RegisterAppEventHandler;

    // Create canvas for image
    RectAssign(&r, 0, 0, 400, 45);
    PCanvas c = CreateCanvas(&Me, r);
    WIDGET(c)->Draw = &DrawCanvas;
    WIDGET(c)->Flags |= WFForceBuffer;
    InsertWidget(WIDGET(w), WIDGET(c));

    // Create labels and textboxes for registration
    RectAssign(&r, 50, 60, 350, 80);
    l = CreateLabel(&Me, r, "Welcome to Aura! Let's get you setup...");
    WIDGET(l)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(l));

    RectAssign(&r, 50, 100, 150, 120);
    l = CreateLabel(&Me, r, "First Name:");
    WIDGET(l)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(l));

    RectAssign(&r, 160, 100, 350, 120);
    nameTextbox = CreateTextbox(&Me, r, TBF_EDITABLE);
    InsertWidget(WIDGET(w), WIDGET(nameTextbox));

    RectAssign(&r, 50, 130, 150, 150);
    l = CreateLabel(&Me, r, "Last Name:");
    WIDGET(l)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(l));

    RectAssign(&r, 160, 130, 350, 150);
    nameTextbox = CreateTextbox(&Me, r, TBF_EDITABLE);
    InsertWidget(WIDGET(w), WIDGET(nameTextbox));

    RectAssign(&r, 50, 160, 150, 180);
    l = CreateLabel(&Me, r, "Email:");
    WIDGET(l)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(l));

    RectAssign(&r, 160, 160, 350, 180);
    emailTextbox = CreateTextbox(&Me, r, TBF_EDITABLE);
    InsertWidget(WIDGET(w), WIDGET(emailTextbox));

    RectAssign(&r, 50, 190, 150, 210);
    l = CreateLabel(&Me, r, "Username:");
    WIDGET(l)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(l));

    RectAssign(&r, 160, 190, 350, 210);
    usernameRegisterTextbox = CreateTextbox(&Me, r, TBF_EDITABLE);
    InsertWidget(WIDGET(w), WIDGET(usernameRegisterTextbox));

    RectAssign(&r, 50, 220, 150, 240);
    l = CreateLabel(&Me, r, "Password:");
    WIDGET(l)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(l));

    RectAssign(&r, 160, 220, 350, 240);
    passwordRegisterTextbox = CreateTextbox(&Me, r, TBF_EDITABLE | TBF_PASSWORD);
    InsertWidget(WIDGET(w), WIDGET(passwordRegisterTextbox));

    RectAssign(&r, 50, 250, 150, 270);
    l = CreateLabel(&Me, r, "Confirm Password:");
    WIDGET(l)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(l));

    RectAssign(&r, 160, 250, 350, 270);
    confirmPasswordTextbox = CreateTextbox(&Me, r, TBF_EDITABLE | TBF_PASSWORD);
    InsertWidget(WIDGET(w), WIDGET(confirmPasswordTextbox));

    // Create register and back buttons
    RectAssign(&r, 50, 290, 120, 310);
    b = CreateButton(&Me, r, "Back", BTN_BACK);
    InsertWidget(WIDGET(w), WIDGET(b));

    RectAssign(&r, 250, 290, 320, 310);
    b = CreateButton(&Me, r, "Register", BTN_REGISTER_USER);
    InsertWidget(WIDGET(w), WIDGET(b));

    WidgetDrawAll(WIDGET(w));
}

l_int Main(int argc, l_text *argv)
{
    APPEXPORT(TraybarAdd);
    APPEXPORT(TraybarRemove);
    APPEXPORT(WallpaperReLoad);

    // Load the resource image
    if (ListKey(Me.Ressource, "1")) {
        Bmp1 = ListKey(Me.Ressource, "SETUP");
        MessageBox(&Me, "YAYY", "Verified and initiated", MBB_OK);
    } else {
        Bmp1 = ListKey(Me.Ressource, "SETUP");
        //MessageBox(&Me, "BOO", "This didnt work", MBB_OK);
    }

    ThemeLoad(KeyGetText("/user/desktop/theme", "/desktop/aura.ini"));
    WallpaperInit();

    ShowLoginWindow();
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
