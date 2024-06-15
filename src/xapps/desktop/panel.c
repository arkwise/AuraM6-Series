/**
*       Project:         Aura OS
*       
*       Codename:   Solar
*
*       Copyright 2008 Chase Finn and Finn Technologies.
*/

/**
*       Global variables
*/

#include <string.h>
#include "treeview.h"
#include "iodlg2.h"
#include "fms2.h"
PMenu MINMenu = 0;
#define MSG_TREEVIEW_ONSEL 0x1256
#define MSG_DBLCLK 0x2266
#define MSG_TB_GOUP 0x4266

#define MSG_VIEWICONS 0x4267
#define MSG_VIEWLIST 0x4268
#define MSG_VIEWMOSC 0x4269
#define MSG_SORTBYNAME 0x426A
#define MSG_SORTBYSIZE 0x426B
#define MSG_SORTBYCTIME 0x426C
#define MSG_SORTBYATIME 0x426D
#define MSG_SORTBYMTIME 0x426E

/**
 * Widgets
 */
PTreeDirview t = 0;
PDirview s = 0;

_PUBLIC PWidget GSPanel = 0;
_PUBLIC PWidget GSSYSMENU = 0;
_PUBLIC PWidget GSTaskbar = 0;
_PUBLIC PWidget GSTraybar = 0;
_PUBLIC PLabel GSClock = 0;

_PUBLIC PList TraybarIconList = 0;
_PUBLIC PList WindowList = 0;
_PUBLIC l_color PanelColor = 0;
_PUBLIC PButton GSApp = 0;
_PUBLIC PButton GSCP = 0;
_PUBLIC PButton GSUSR = 0;
_PUBLIC PButton GSEXIT = 0;
_PUBLIC PButton GSQUIT = 0;
_PUBLIC PButton GSnav = 0;
_PUBLIC l_int GSPanelHeight = 0;

l_int dock = 0;
l_int windowlst = 0;

PWMENTRY TaskbarAdd(PWindow w)
{
        PWMENTRY p = NEW(TWMENTRY);
        CLEAR(p);

        p->Itm.Data = WIDGET(w);
        p->Itm.FreeData = NULL;
        ListAddItem(WindowList, (PListItem)p);

        WidgetDraw(GSTaskbar, NULL);
}

l_bool TaskbarRemove(PWindow w)
{
        PListItem i = ListFoundItem(WindowList, w);
        if (!i)
                return false;
        ListRemoveItem(WindowList, i);

        WidgetDraw(GSTaskbar, NULL);
        return true;
}

l_bool TaskbarRedraw(PWindow w)
{

        WidgetDraw(GSTaskbar, NULL);
        return true;
}

void TaskbarDraw(PWidget o, p_bitmap buffer, PRect w)
{
        l_int x = 5;
        l_int Items = 0;
        l_int ItemWidth = 0;

        windowlst = KeyGetInt("/USER/DESKTOP/transparency/window_list", 255);

        rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DFACE);

        if (THMPanelFace && UseSkins)
                DrawTransTHEMESkin(buffer, THMPanelFace->Skin, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, windowlst, 255);
        else
                Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DLIGHT, COL_3DDARK);

        PListItem a = WindowList->Last, b = a;

        if (!a)
                return;

        do
        {
                Items++;

                a = a->Prev;
        } while (a != WindowList->Last);

        if (!Items)
                return;

        ItemWidth = min((o->Absolute.b.x - o->Absolute.a.x - 10) / Items, 150);
        Items = 0;

        b = a = WindowList->Last->Next;

        do
        {
                TRect r;
                RectAssign(&r, o->Absolute.a.x + 5 + ((ItemWidth + 3) * Items), o->Absolute.a.y + 2, o->Absolute.a.x + ((ItemWidth + 3) * (Items + 1)) - 3, o->Absolute.b.y - 2);

                WMENTRY(a)->BufAbs = r;

                if (Mouse->State.b & BUTTON_LEFT && CURSOR_IN_RECT(r) || Desktop->Last == WIDGET(a->Data)->Prev)
                {
                        Rect3D(buffer, r.a.x, r.a.y, r.b.x, r.b.y, COL_3DDARK, COL_3DLIGHT);
                        //masked_blit(WINDOW(a->Data)->Icon, buffer, 0, 0, r.a.x+6, (r.a.y+r.b.y)/2-8+1, r.a.x+22, (r.a.y+r.b.y)/2+8);
                        //textout_centre(buffer, default_font, WINDOW(a->Data)->Caption, ((r.a.x+r.b.x)/2)+1, ((r.a.y+r.b.y)/2-text_height(default_font)/2)+1, makecol(0,0,0));

                        DrawIcon16(buffer, WINDOW(a->Data)->Icon, r.a.x + 6, (r.a.y + r.b.y) / 2 - 8 + 1);

                        if (WINDOW(a->Data)->Caption)
                                DrawNiceText(buffer, default_font, r.a.x + 6 + 16 + 5, (r.a.y + r.b.y) / 2 - text_height(default_font) / 2, ((r.b.x - 6) - (r.a.x + 6 + 16 + 5)), WINDOW(a->Data)->Caption, COL_3DTEXT);
                }
                else
                {
                        Rect3D(buffer, r.a.x, r.a.y, r.b.x, r.b.y, COL_3DLIGHT, COL_3DDARK);
                        //masked_blit(WINDOW(a->Data)->Icon, buffer, 0, 0, r.a.x+5, (r.a.y+r.b.y)/2-8, r.a.x+21, (r.a.y+r.b.y)/2+8);
                        //textout_centre(buffer, default_font, WINDOW(a->Data)->Caption, (r.a.x+r.b.x)/2, (r.a.y+r.b.y)/2-text_height(default_font)/2, makecol(0,0,0));

                        DrawIcon16(buffer, WINDOW(a->Data)->Icon, r.a.x + 5, (r.a.y + r.b.y) / 2 - 8);

                        if (WINDOW(a->Data)->Caption)
                                DrawNiceText(buffer, default_font, r.a.x + 5 + 16 + 5, (r.a.y + r.b.y) / 2 - text_height(default_font) / 2, ((r.b.x - 5) - (r.a.x + 5 + 16 + 5)), WINDOW(a->Data)->Caption, COL_3DTEXT);
                }

                Items++;

                a = a->Next;
        } while (a != b);
}


l_bool TaskbarEventHandler(PWidget o, PEvent Ev)
{
    if (Ev->Type == EV_MOUSE)
    {
        if (Ev->Message == WEvMouseLDown)
        {
            WidgetDraw(o, NULL);
            return true;
        }

        if (Ev->Message == WEvMouseLUp)
        {
            PListItem a = WindowList->Last;

            if (!a)
                return true;

            do
            {
                if (PointInRect(Mouse->State.p, WMENTRY(a)->BufAbs))
                {
                    PWidget window = WIDGET(a->Data);
                    // If the window is minimized, restore it
                    if (WINDOW(window)->WindowStates & WS_MINIMIZED)
                    {
                        MinimizeWindow(window);
                    }
                    // Bring the window to the front
                    WidgetSetFirst(window);
                }

                a = a->Prev;
            } while (a != WindowList->Last);
            WidgetDraw(o, NULL);
            return true;
        }

        if (Ev->Message == WEvMouseRUp)
        {
            PListItem a = WindowList->Last;

            if (!a)
                return true;

            do
            {
                if (PointInRect(Mouse->State.p, WMENTRY(a)->BufAbs))
                {
                    PWidget window = WIDGET(a->Data);
                    // Minimize the window
                    MinimizeWindow(window);
                }

                a = a->Prev;
            } while (a != WindowList->Last);
            WidgetDraw(o, NULL);
            return true;
        }
    }

    return false;
}


void PanelDraw(PWidget o, p_bitmap buffer, PRect w)
{
        dock = KeyGetInt("/USER/DESKTOP/transparency/dock", 255);
        if (THMDockFace && UseSkins)
                DrawTransTHEMESkin(buffer, THMDockFace->Skin, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, dock, 255);

        else
                Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DLIGHT, COL_3DDARK);
}

void SYSMENUDraw(PWidget o, p_bitmap buffer, PRect w)
{
        dock = KeyGetInt("/USER/DESKTOP/transparency/dock", 255);
        if (THMSYSMENU && UseSkins)
                DrawTransTHEMESkin(buffer, THMSYSMENU->Skin, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, dock, 255);

        else
                Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DLIGHT, COL_3DDARK);
}

l_bool PanelEventHandler(PWidget o, PEvent Ev)
{
        return false;
}

void CPButtonDraw(PWidget o, p_bitmap buffer, PRect w)
{
        if (THMPanelCPDown && THMPanelCPOver && THMPanelCP && UseSkins)
        {

                if (BUTTON(o)->State == BS_OVER)
                        DrawTransTHEMESkin(buffer, THMPanelCPOver->Skin, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, 255, 255);
                else if (BUTTON(o)->State == BS_DOWN || (Mouse->State.b & BUTTON_LEFT && CURSOR_IN_RECT(*w)))
                        masked_blit(THMPanelCPDown->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
                else
                        masked_blit(THMPanelCP->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
                //textout_centre_ex(buffer, default_font, "Settings", (o->Absolute.a.x+o->Absolute.b.x)/2+1, (o->Absolute.a.y+o->Absolute.b.y-text_height(default_font))/2+1, makecol(255, 255, 255),-1);
        }
        else
        {
                if (BUTTON(o)->State == BS_OVER)
                        rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DOVER);

                if (BUTTON(o)->State != BS_DOWN)
                {
                        Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DLIGHT, COL_3DDARK);
                        textout_centre_ex(buffer, default_font, "CP", (o->Absolute.a.x + o->Absolute.b.x) / 2, ((o->Absolute.a.y + o->Absolute.b.y) / 2 - text_height(default_font) / 2), makecol(0, 0, 0), -1);
                }
                else
                {
                        Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DDARK, COL_3DLIGHT);
                        textout_centre_ex(buffer, default_font, "CP", (o->Absolute.a.x + o->Absolute.b.x) / 2 + 1, (o->Absolute.a.y + o->Absolute.b.y - text_height(default_font)) / 2 + 1, makecol(0, 0, 0), -1);
                }
        }
}

void USRButtonDraw(PWidget o, p_bitmap buffer, PRect w)
{
        if (THMPanelUSRDown && THMPanelUSROver && THMPanelUSR && UseSkins)
        {

                if (BUTTON(o)->State == BS_OVER)
                        masked_blit(THMPanelUSROver->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
                else if (BUTTON(o)->State == BS_DOWN || (Mouse->State.b & BUTTON_LEFT && CURSOR_IN_RECT(*w)))
                        masked_blit(THMPanelUSRDown->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
                else
                        masked_blit(THMPanelUSR->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
                //textout_centre_ex(buffer, default_font, "Media", (o->Absolute.a.x+o->Absolute.b.x)/2+1, (o->Absolute.a.y+o->Absolute.b.y-text_height(default_font))/2+1, makecol(255, 255, 255),-1);
        }
        else
        {
                if (BUTTON(o)->State == BS_OVER)
                        rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DOVER);

                if (BUTTON(o)->State != BS_DOWN)
                {
                        Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DLIGHT, COL_3DDARK);
                        textout_centre_ex(buffer, default_font, "USR", (o->Absolute.a.x + o->Absolute.b.x) / 2, ((o->Absolute.a.y + o->Absolute.b.y) / 2 - text_height(default_font) / 2), COL_3DTEXT, -1);
                }
                else
                {
                        Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DDARK, COL_3DLIGHT);
                        textout_centre_ex(buffer, default_font, "USR", (o->Absolute.a.x + o->Absolute.b.x) / 2 + 1, (o->Absolute.a.y + o->Absolute.b.y - text_height(default_font)) / 2 + 1, COL_3DTEXT, -1);
                }
        }
}

void appButtonDraw(PWidget o, p_bitmap buffer, PRect w)
{
        if (THMPanelappDown && THMPanelappOver && THMPanelapp && UseSkins)
        {

                if (BUTTON(o)->State == BS_OVER)
                        masked_blit(THMPanelappOver->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);

                else if (BUTTON(o)->State == BS_DOWN || (Mouse->State.b & BUTTON_LEFT && CURSOR_IN_RECT(*w)))
                        masked_blit(THMPanelappDown->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
                else
                        masked_blit(THMPanelapp->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
                //textout_centre_ex(buffer, default_font, "Applications", (o->Absolute.a.x+o->Absolute.b.x)/2+1, (o->Absolute.a.y+o->Absolute.b.y-text_height(default_font))/2+1, makecol(255, 255, 255),-1);
        }
        else
        {
                if (BUTTON(o)->State == BS_OVER)
                        rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DOVER);

                if (BUTTON(o)->State != BS_DOWN)
                {
                        Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DLIGHT, COL_3DDARK);
                        textout_centre_ex(buffer, default_font, "app", (o->Absolute.a.x + o->Absolute.b.x) / 2, ((o->Absolute.a.y + o->Absolute.b.y) / 2 - text_height(default_font) / 2), COL_3DTEXT, -1);
                }
                else
                {
                        Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DDARK, COL_3DLIGHT);
                        textout_centre_ex(buffer, default_font, "app", (o->Absolute.a.x + o->Absolute.b.x) / 2 + 1, (o->Absolute.a.y + o->Absolute.b.y - text_height(default_font)) / 2 + 1, COL_3DTEXT, -1);
                }
        }
}

void ExitButtonDraw(PWidget o, p_bitmap buffer, PRect w)
{
        if (THMPanelExitDown && THMPanelExitOver && THMPanelExit && UseSkins)
        {

                if (BUTTON(o)->State == BS_OVER)
                        masked_blit(THMPanelExitOver->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);

                else if (BUTTON(o)->State == BS_DOWN || (Mouse->State.b & BUTTON_LEFT && CURSOR_IN_RECT(*w)))
                        masked_blit(THMPanelExitDown->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
                else
                        masked_blit(THMPanelExit->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
                //textout_centre_ex(buffer, default_font, "Exit", (o->Absolute.a.x+o->Absolute.b.x)/2+1, (o->Absolute.a.y+o->Absolute.b.y-text_height(default_font))/2+1, makecol(255, 255, 255),-1);
        }
        else
        {
                if (BUTTON(o)->State == BS_OVER)
                        rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DOVER);

                if (BUTTON(o)->State != BS_DOWN)
                {
                        Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DLIGHT, COL_3DDARK);
                        textout_centre_ex(buffer, default_font, "Exit", (o->Absolute.a.x + o->Absolute.b.x) / 2, ((o->Absolute.a.y + o->Absolute.b.y) / 2 - text_height(default_font) / 2), COL_3DTEXT, -1);
                }
                else
                {
                        Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DDARK, COL_3DLIGHT);
                        textout_centre_ex(buffer, default_font, "Exit", (o->Absolute.a.x + o->Absolute.b.x) / 2 + 1, (o->Absolute.a.y + o->Absolute.b.y - text_height(default_font)) / 2 + 1, COL_3DTEXT, -1);
                }
        }
}

void navButtonDraw(PWidget o, p_bitmap buffer, PRect w)
{
        if (THMPanelnavDown && THMPanelnavOver && THMPanelnav && UseSkins)
        {

                if (BUTTON(o)->State == BS_OVER)
                        masked_blit(THMPanelnavOver->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
                else if (BUTTON(o)->State == BS_DOWN || (Mouse->State.b & BUTTON_LEFT && CURSOR_IN_RECT(*w)))
                        masked_blit(THMPanelnavDown->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
                else
                        masked_blit(THMPanelnav->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
        }
        else
        {
                if (BUTTON(o)->State == BS_OVER)
                        rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DOVER);

                if (BUTTON(o)->State != BS_DOWN)
                {
                        Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DLIGHT, COL_3DDARK);
                        textout_centre_ex(buffer, default_font, "nav", (o->Absolute.a.x + o->Absolute.b.x) / 2, ((o->Absolute.a.y + o->Absolute.b.y) / 2 - text_height(default_font) / 2), makecol(0, 0, 0), -1);
                }
                else
                {
                        Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DDARK, COL_3DLIGHT);
                        textout_centre_ex(buffer, default_font, "nav", (o->Absolute.a.x + o->Absolute.b.x) / 2 + 1, (o->Absolute.a.y + o->Absolute.b.y - text_height(default_font)) / 2 + 1, makecol(0, 0, 0), -1);
                }
        }
}

void RefreshPanelClock(void *A)
{
        //struct time t;
        //ctime(t);

        //LabelSetText(GSClock, "%s", ctime(NULL));//%d:%02d", t.ti_hour, t.ti_min);

        time_t t = time(NULL);
        struct tm *tme;
        tme = localtime(&t);
        char ampm[3] = "AM";

        if (tme->tm_hour > 12)
        {
                tme->tm_hour -= 12;
                strcpy(ampm, "PM");
        }
        if (tme->tm_hour = 12)
        {
                strcpy(ampm, "PM");
        }
        if (tme->tm_hour = 0)
        {
                tme->tm_hour += 12;
        }

        DebugMessage("pretime");

        if (tme)
                LabelSetText(GSClock, "%d:%02d %s", tme->tm_hour, tme->tm_min, ampm);
        //sprintf(GSClock, "%d:%02d %s", tme->tm_hour,tme->tm_min, ampm);

        //VIEW(GSClock)->draw(VIEW(GSClock));

        DebugMessage("posttime");
}

void TraybarDraw(PWidget o, p_bitmap buffer, PRect w)
{
        rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DFACE);
        rect(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DDARK);

        PListItem a = TraybarIconList->Last;

        if (!a)
                return;

#define DEFAULT_ICONW 16
#define DEFAULT_ICONH 16

        l_int x = o->Absolute.a.x + 3;

        do
        {
                RectAssign(&((PTRAYBARICON)(a->Data))->BufAbs, x, o->Absolute.a.y + (o->Absolute.b.y - o->Absolute.a.y) / 2 - (DEFAULT_ICONH) / 2, x + DEFAULT_ICONW, o->Absolute.a.y + (o->Absolute.b.y - o->Absolute.a.y) / 2 + (DEFAULT_ICONH) / 2);

                masked_blit(((PTRAYBARICON)(a->Data))->Icon, buffer, 0, 0, ((PTRAYBARICON)(a->Data))->BufAbs.a.x, ((PTRAYBARICON)(a->Data))->BufAbs.a.y, ((PTRAYBARICON)(a->Data))->BufAbs.b.x, ((PTRAYBARICON)(a->Data))->BufAbs.b.y);

                x += DEFAULT_ICONW + 2;

                a = a->Prev;
        } while (a != TraybarIconList->Last);
}

l_bool TraybarEventHandler(PWidget o, PEvent Ev)
{
        if (Ev->Type == EV_MOUSE)
        {
                if (Ev->Message == WEvMouseLDown || Ev->Message == WEvMouseRDown)
                {
                        PListItem a = TraybarIconList->Last;

                        if (!a)
                                return;

                        do
                        {
                                if (((PTRAYBARICON)(a->Data))->Menu && CURSOR_IN_RECT(((PTRAYBARICON)(a->Data))->BufAbs))
                                {
                                        PopUpMenu(&Me, Mouse->State.p, ((PTRAYBARICON)(a->Data))->Menu, ((PTRAYBARICON)(a->Data))->MsgDest, 0);
                                }

                                a = a->Prev;
                        } while (a != TraybarIconList->Last);

                        return true;
                }
        }

        return false;
}

l_bool TraybarAdd(p_bitmap Icon, l_text Tooltip, PMenu Menu, PWidget MsgDest)
{
        if (!GSTraybar)
        {
                TRect r;
                RectAssign(&r, GSPanel->Absolute.b.x - 7 - text_length(default_font, "12:00") - 10 - 16 - 6 - 5, 1, GSPanel->Absolute.b.x - 7 - text_length(default_font, "12:00") - 10 - 5, GSPanel->Absolute.b.y - 1);
                GSTraybar = CreateWidget(&Me, r);
                GSTraybar->Draw = &TraybarDraw;
                GSTraybar->EventHandler = &TraybarEventHandler;
                InsertWidget(GSPanel, WIDGET(GSTraybar));
        }
        else
                WidgetSetMetrics(GSTraybar, GSTraybar->Absolute.a.x - (DEFAULT_ICONW + 2), GSTraybar->Absolute.a.y, GSTraybar->Absolute.b.x, GSTraybar->Absolute.b.y);

        PTRAYBARICON p = NEW(TTRAYBARICON);

        if (!p || !Icon)
                return 0;

        CLEAR(p);

        p->Icon = Icon;
        p->Tooltip = TextDup(Tooltip);
        p->Menu = Menu;
        p->MsgDest = MsgDest;

        ListAdd(TraybarIconList, Tooltip, p, &free);

        WidgetDraw(GSTraybar, NULL);

        return true;
}

l_bool TraybarRemove(l_text Key)
{
        ListRemoveItem(TraybarIconList, ListKeyItem(TraybarIconList, Key));

        if (!TraybarIconList->Last)
        {
                WidgetDispose(GSTraybar);
                GSTraybar = 0;
        }
        else
        {
                WidgetSetMetrics(GSTraybar, GSTraybar->Absolute.a.x + (DEFAULT_ICONW + 2), GSTraybar->Absolute.a.y, GSTraybar->Absolute.b.x, GSTraybar->Absolute.b.y);
                WidgetDraw(GSTraybar, NULL);
        }
}

_PUBLIC void GSPanelInit()
{
        l_ulong Height = 30;
        l_ulong BHeight = 24;
        l_ulong BWidth = 40;
        TRect r;

        TraybarIconList = NewList();
        WindowList = NewList();

        if (UseSkins && THMPanelFace)
                Height = THMPanelFace->Skin->h;
        if (UseSkins && THMDockFace)
                Height = THMDockFace->Skin->h;
        if (UseSkins && THMPanelCP)
                BHeight = THMPanelCP->Skin->h;
        if (UseSkins && THMPanelCP)
                BWidth = THMPanelCP->Skin->w;

        RectAssign(&r, (GSScreenWidth / 2) - 124, GSScreenHeight - 33, (GSScreenWidth / 2) + 124, GSScreenHeight);
        GSPanel = CreateWidget(&Me, r);
        GSPanel->Draw = &PanelDraw;
        GSPanel->EventHandler = &PanelEventHandler;

        InsertWidget(DeskTop, GSPanel);

        RectAssign(&r, 0, 0, GSScreenWidth, 30);
        GSTaskbar = CreateWidget(&Me, r);
        GSTaskbar->Draw = &TaskbarDraw;
        GSTaskbar->EventHandler = &TaskbarEventHandler;
        GSTaskbar->Flags |= WFOnTop;
        InsertWidget(DeskTop, GSTaskbar);

        RectAssign(&r, 11, 1, 49, 33);
        GSnav = CreateButton(&Me, r, "Navigation", BTN_navbtn);
        WIDGET(GSnav)->Draw = &navButtonDraw;
        WIDGET(GSnav)->AppEvHdl = &AppEventHandler;
        WidgetSetTooltipText(WIDGET(GSnav), "Launch Navigator");
        InsertWidget(GSPanel, WIDGET(GSnav));

        RectAssign(&r, 58, 1, 96, 33);
        GSApp = CreateButton(&Me, r, "Applications", BTN_APP);
        WIDGET(GSApp)->Draw = &appButtonDraw;
        WIDGET(GSApp)->AppEvHdl = &AppEventHandler;
        WidgetSetTooltipText(WIDGET(GSApp), "Launch Applications");
        InsertWidget(GSPanel, WIDGET(GSApp));

        RectAssign(&r, 105, 1, 143, 33);
        GSUSR = CreateButton(&Me, r, "User", BTN_USR);
        WIDGET(GSUSR)->Draw = &USRButtonDraw;
        WIDGET(GSUSR)->AppEvHdl = &AppEventHandler;
        WidgetSetTooltipText(WIDGET(GSUSR), "Media");
        InsertWidget(GSPanel, WIDGET(GSUSR));

        RectAssign(&r, 152, 1, 190, 33);
        GSCP = CreateButton(&Me, r, "Control Panel", BTN_CP);
        WIDGET(GSCP)->Draw = &CPButtonDraw;
        WIDGET(GSCP)->AppEvHdl = &AppEventHandler;
        WIDGET(GSCP)->Flags |= WFOnTop;
        WidgetSetTooltipText(WIDGET(GSCP), "System Settings");
        InsertWidget(GSPanel, WIDGET(GSCP));

        RectAssign(&r, 199, 1, 237, 33);
        GSEXIT = CreateButton(&Me, r, "Exit", IDM_QUIT);
        WIDGET(GSEXIT)->Draw = &ExitButtonDraw;
        WIDGET(GSEXIT)->AppEvHdl = &AppEventHandler;
        WidgetSetTooltipText(WIDGET(GSEXIT), "Exit");
        InsertWidget(GSPanel, WIDGET(GSEXIT));

        RectAssign(&r, GSScreenWidth - 46, (Height - BHeight) / 2 + 7, GSScreenWidth, (Height + BHeight) / 2 - 6);
        //RectAssign(&r,5, (Height-BHeight)/2, 5+BWidth-1, (Height+BHeight)/2-1);
        //RectAssign(&r,GSPanel->Absolute.b.x - 7 - text_length(default_font, "12:00 PM"), (GSPanel->Absolute.b.y - text_height(default_font))/2, GSPanel->Absolute.b.x - 7, (GSPanel->Absolute.b.y - text_height(default_font))/2 + 2 + text_height(default_font) );
        GSClock = CreateLabel(&Me, r, "12:00");
        LABEL(GSClock)->FontColor = COL_3DTEXT;
        InsertWidget(GSTaskbar, WIDGET(GSClock));
        RefreshPanelClock(NULL);

        RectAssign(&WINDOW_MaximizeArea, 0, 31, GSScreenWidth, GSScreenHeight - 33);

        WidgetDrawAll(DeskTop);
}

void GSPanelDestroy()
{
        WidgetDispose(GSPanel);
        WidgetDispose(GSTaskbar);
        FreeList(TraybarIconList);
        FreeList(WindowList);
}
