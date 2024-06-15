////////////////////////////////////////////////////////////////////////////////
//
//  Widgets - Core File
//
//      (c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
/** @defgroup widgets Widgets
    System provided widgets.
*/
/** @defgroup widgetsys Widget system
    Manages the widgets displayed onto screen.
    This is the base of all other provided widgets. By exemple a window is a standart widget with special extentions, so can parse it to functions but don't forget to convert the pointer type : use WIDGET(pointer).
    @{
*/
/**     @file
    Widget system core file.
*/

#include "kernel.h"
#include "widget.h"
#include "grfx.h"
#include "stdlib.h"
#include <string.h>
////////////////////////////////////////////////////////////////////////////////
l_uid nUID = "widget";
l_ulong AppVersion = ULONG_ID(0, 2, 0, 0);
char AppName[] = "Widget System";
l_uid NeededLibs[] = {"grfx", "skin", ""};

////////////////////////////////////////////////////////////////////////////////
// Specifics widget management
PWidget DeskTop = NULL;
PWidget TopWidget = NULL;
PWidget MasterWidget = NULL;
PWidget FocusedWidget = NULL;
PWidget *LookFor = NULL;

p_bitmap IconHardware = NULL;
p_bitmap IconWelcome = NULL;
p_bitmap IconSYSError = NULL;
p_bitmap Icon = NULL;
l_ulong Flags = 0;

////////////////////////////////////////////////////////////////////////////////
// Common parameters
p_bitmap buffer = NULL;
l_ulong WorkingFlags = 0;

#define WWF_TOOLTIPUP 0x02
#define WWF_DRAGING 0x04

////////////////////////////////////////////////////////////////////////////////
// Mouse click and delay related
l_ubig LastMouseMove = 0;
l_ubig LastNotified = 0;
l_ubig LastToolTip = 0;
l_ubig LastMouseClickLeft = 0;
l_ubig LastMouseClickRight = 0;
l_ubig LastMouseClickMiddle = 0;
#define DoubleClickSpeed (100 * Mouse->DblClkSpeed)

////////////////////////////////////////////////////////////////////////////////
// Drag & Drop related defintions
l_ulong DragDataType = 0;
PWidget DragCaller;
void *DragData = NULL;
l_ulong DragSupportedMethods = 0;
l_bool DragOldAccept = false;
l_ulong DragOldMethod = 0;
PCursor DragCur = 0, NoDragCur = 0, DragCopyCur = 0;
////////////////////////////////////////////////////////////////////////////////
// "OSD" related defintions
l_bool OSD = 0;
l_bool OSDIsRect = 0;
TRect OSDRect = {0, 0, 0, 0};
l_color OSDColor = 0;

////////////////////////////////////////////////////////////////////////////////
// Drag&Drop related
////////////////////////////////////////////////////////////////////////////////
void DragStop(void)
{
    DebugMessage("Drag n Drop: DragStop");
    if (WorkingFlags & WWF_DRAGING)
    {
        FreeData2(DragDataType, DragData);
        DragCaller = NULL;
        DragData = NULL;
        DragDataType = 0;
        DragSupportedMethods = 0;
        MouseSetCursor(CUR_POINTER);
        WorkingFlags &= ~WWF_DRAGING;
    }
}
////////////////////////////////////////////////////////////////////////////////
void DragStart(PWidget o, l_ulong DataType, void *Data, l_ulong SupportedMethods)
{
    l_ulong Method;
    if (WorkingFlags & WWF_DRAGING)
        DragStop();
    DebugMessage("Drag n Drop: DragStop");

    WorkingFlags |= WWF_DRAGING;
    DragCaller = o;
    DragDataType = DataType;
    DragData = DuplicateData2(DataType, Data);
    DragSupportedMethods = SupportedMethods;
    o->Flags |= WF_DRAGFOCUS;

    if (DragOldAccept = __WidgetDropAccept(TopWidget, &Method))
    {
        if (Method == DMETHOD_COPY)
            MouseSetCursor(DragCopyCur);
        else
            MouseSetCursor(DragCur);
    }
    else
        MouseSetCursor(NoDragCur);

    DragOldMethod = Method;
}
////////////////////////////////////////////////////////////////////////////////
l_bool __WidgetDropAccept(PWidget o, l_ulong *Method)
{
    if (o->DropAccept)
        return o->DropAccept(o, DragCaller, DragDataType, DragData, DragSupportedMethods, Method);
    return false;
}
////////////////////////////////////////////////////////////////////////////////
// Mouse related
////////////////////////////////////////////////////////////////////////////////
PWidget TopWidgetUnderPoint(PWidget o, TPoint p)
{
    if (o->Last)
    {
        PWidget a = o->Last->Next;
        PWidget b = a;
        do
        {
            if (_PointInRect(p, a->Absolute) && (a->Flags & WFVisible))
                return TopWidgetUnderPoint(a, p);
            a = a->Next;
        } while (a != b);
    }
    return o;
}
////////////////////////////////////////////////////////////////////////////////
PWidget GetTopWidgetUnderMouse(void)
{
    return TopWidgetUnderPoint(DeskTop, Mouse->State.p);
}
////////////////////////////////////////////////////////////////////////////////
// OSD related
////////////////////////////////////////////////////////////////////////////////
void DrawOSDOnRect(p_bitmap out, TRect r)
{
    if (OSDIsRect)
    {
        if (_RectOverlay(r, OSDRect))
        {
            TRect Tmp;
            RectIntersept(&Tmp, r, OSDRect);
            xor_mode(TRUE);
            if (Tmp.a.x == OSDRect.a.x)
                vline(out, Tmp.a.x, Tmp.a.y, Tmp.b.y, OSDColor);
            if (Tmp.b.x == OSDRect.b.x)
                vline(out, Tmp.b.x, Tmp.a.y, Tmp.b.y, OSDColor);
            if (Tmp.a.y == OSDRect.a.y)
                hline(out, Tmp.a.x, Tmp.a.y, Tmp.b.x, OSDColor);
            if (Tmp.b.y == OSDRect.b.y)
                hline(out, Tmp.a.x, Tmp.b.y, Tmp.b.x, OSDColor);
            xor_mode(FALSE);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void WidgetSetOSDRect(TRect r)
{
    MouseHide();
    PauseMultitasking();
    xor_mode(TRUE);
    set_clip_rect(screen, 0, 0, screen->w - 1, screen->h - 1);
    if (OSD && OSDIsRect)
    {
        rect(screen, OSDRect.a.x, OSDRect.a.y, OSDRect.b.x, OSDRect.b.y, OSDColor);
        OSDRect = r;
    }
    else
    {
        OSD = 1;
        OSDIsRect = 1;
        OSDRect = r;
    }
    rect(screen, OSDRect.a.x, OSDRect.a.y, OSDRect.b.x, OSDRect.b.y, OSDColor);
    xor_mode(FALSE);
    UnPauseMultitasking();
    MouseShow();
}
////////////////////////////////////////////////////////////////////////////////
void WidgetUnOSD(void)
{
    if (OSD && OSDIsRect)
    {
        MouseHide();
        PauseMultitasking();
        xor_mode(TRUE);
        set_clip_rect(screen, 0, 0, screen->w - 1, screen->h - 1);
        rect(screen, OSDRect.a.x, OSDRect.a.y, OSDRect.b.x, OSDRect.b.y, OSDColor);
        xor_mode(FALSE);
        UnPauseMultitasking();
        MouseShow();
    }
    OSD = 0;
    OSDIsRect = 0;
}
////////////////////////////////////////////////////////////////////////////////
// Draw related
////////////////////////////////////////////////////////////////////////////////
/**
    Check if all widget parent are visible. Using this function you ensure that widget is really visible.
    @param o Widget to check
    @return true if widget and all its parents are visible, else return false
*/
l_bool WidgetCheckVisible(PWidget o)
{
    do
    {
        if (!(o->Flags & WFVisible))
            return false;
    } while (o = o->Parent);
    return true;
}
////////////////////////////////////////////////////////////////////////////////
INLINE void WidgetGenerateCacheExBrothers(PList l, PWidget o, PWidget i, TRect w)
{
    if (o->Parent)
    {
        PWidget a = o->Parent->Last->Next;
        while (a != o)
        {
            if ((a->Flags & WFVisible) && _RectOverlay(w, a->DrawAbsolute))
                RemoveZoneInList(l, a->DrawAbsolute);
            a = a->Next;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
INLINE void WidgetGenerateCacheExSubs(PList l, PWidget o, TRect w)
{
    if (o->Last)
    {
        PWidget a = o->Last->Next;
        PWidget b = o->Last->Next;
        do
        {
            if ((a->Flags & WFVisible) && _RectOverlay(w, a->DrawAbsolute))
                RemoveZoneInList(l, a->DrawAbsolute);
            a = a->Next;
        } while (a != b);
    }
}
////////////////////////////////////////////////////////////////////////////////
PList WidgetGenerateCache(PWidget o, TRect *w, PList Clip)
{
    PWidget a = o;
    PList l = NULL;
    //DebugMessage ("WidgetGenerateCache(%s)",o->Name);

    if (Clip)
    {
        w = &o->DrawAbsolute;
        l = NewListFromRectListIntersept(Clip, *w);
    }
    else if (w)
    { // *w and o->Absolute MUST have a common part
        TRect Tmp;
        RectIntersept(&Tmp, o->DrawAbsolute, *w);
        l = NewListFromRect(Tmp);
    }
    else if (o->DrawAbsolute.a.x - o->DrawAbsolute.b.x + 1 || o->DrawAbsolute.a.y - o->DrawAbsolute.b.y + 1)
    {
        l = NewListFromRect(o->DrawAbsolute);
        w = &o->DrawAbsolute;
    }
    else
        return NULL;

    WidgetGenerateCacheExSubs(l, o, *w);
    do
    {
        WidgetGenerateCacheExBrothers(l, a, o, *w);
    } while (a = a->Parent);

    return l;
}
////////////////////////////////////////////////////////////////////////////////
// Independant function, as it can usefull for some widget
void WidgetDrawFromBuffer(PWidget o, PList l, p_bitmap Out, p_bitmap Buffer, PPoint d, TRect *w)
{
    if (!(o->Flags & WFVisible))
        return;
    if (l)
    {
        PListItem a = l->Last;
        PListItem b = a;
        if (!a)
            return;
        if (d)
        {
            set_clip_rect(Out, max(0, o->DrawAbsolute.a.x + d->x),
                          max(0, o->DrawAbsolute.a.y + d->y),
                          min(Out->w - 1, o->DrawAbsolute.b.x + d->x),
                          min(Out->h - 1, o->DrawAbsolute.b.y + d->y));

            //blit(Buffer,screen,0,screen->h/2,0,0,screen->w, screen->h/2 );
            do
            {
                PRect r = a->Data;
                blit(Buffer, Out, r->a.x, r->a.y,
                     r->a.x + d->x, r->a.y + d->y,
                     r->b.x - r->a.x + 1, r->b.y - r->a.y + 1);
                a = a->Next;
            } while (a != b);
        }
        else
        {
            set_clip_rect(Out, max(0, o->DrawAbsolute.a.x),
                          max(0, o->DrawAbsolute.a.y),
                          min(Out->w - 1, o->DrawAbsolute.b.x),
                          min(Out->h - 1, o->DrawAbsolute.b.y));
            do
            {
                PRect r = a->Data;
                if (OSD)
                    DrawOSDOnRect(Buffer, *r);
                MouseRecap(Buffer, *r);
                blit(Buffer, Out, r->a.x, r->a.y,
                     r->a.x, r->a.y,
                     r->b.x - r->a.x + 1, r->b.y - r->a.y + 1);
                a = a->Next;
            } while (a != b);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void WidgetRefresh(PWidget o, PRect w, PList List, p_bitmap Out, PPoint d)
{
    PList l;

    if (!(o->Flags & WFVisible))
        return;

    if (!(l = WidgetGenerateCache(o, w, List)))
        return;

    if (l->Last)
    { // There is at least something to draw

        PauseMultitasking(); // Disable multitasking to prevent graphics bugs
        if (!o->Draw)
        {
            // No custom draw function //////////////////////////////////////////////////
            // As default is rectfill, no buffer is needed,
            // It's faster than ever !!
            // *BUT* No alpha blending support

            PListItem a = l->Last;
            PListItem b = a;

            acquire_bitmap(Out);

            if (d)
            {
                set_clip_rect(Out, max(0, o->DrawAbsolute.a.x + d->x),
                              max(0, o->DrawAbsolute.a.y + d->y),
                              min(Out->w - 1, o->DrawAbsolute.b.x + d->x),
                              min(Out->h - 1, o->DrawAbsolute.b.y + d->y));
                do
                {
                    PRect r = a->Data;
                    rectfill(Out, r->a.x + d->x, r->a.y + d->y, r->b.x + d->x, r->b.y + d->y, o->BackgroundColor);
                    a = a->Next;
                } while (a != b);
            }
            else
            {
                set_clip_rect(Out, max(0, o->DrawAbsolute.a.x),
                              max(0, o->DrawAbsolute.a.y),
                              min(Out->w - 1, o->DrawAbsolute.b.x),
                              min(Out->h - 1, o->DrawAbsolute.b.y));
                do
                {
                    PRect r = a->Data;
                    rectfill(Out, r->a.x, r->a.y, r->b.x, r->b.y, o->BackgroundColor);
                    if (OSD)
                        DrawOSDOnRect(Out, *r);
                    MouseRecap(Out, *r);
                    a = a->Next;
                } while (a != b);
            }

            release_bitmap(Out);
        }
        else if ((l->Last == l->Last->Next) && !(o->Flags & WFForceBuffer) && !d)
        {
            // Only a rect to draw, so we can draw directly to screen /////////////////
            // Faster
            PRect r = l->Last->Data;

            acquire_bitmap(Out);

            set_clip_rect(Out, max(0, r->a.x),
                          max(0, r->a.y),
                          min(Out->w - 1, r->b.x),
                          min(Out->h - 1, r->b.y));
            o->Draw(o, Out, r);
            if (OSD)
                DrawOSDOnRect(Out, *r);
            MouseRecap(Out, *r);

            release_bitmap(Out);
        }
        else if (o->Flags & WFForceNonBuffer && !(o->Flags & WFForceBuffer) && !d)
        {
            // Widget do not want to use automatic buffer /////////////////////////////
            // Not always faster
            PListItem a = l->Last;
            PListItem b = a;
            acquire_bitmap(Out);

            do
            {
                PRect r = a->Data;
                set_clip_rect(Out, max(0, r->a.x),
                              max(0, r->a.y),
                              min(Out->w - 1, r->b.x),
                              min(Out->h - 1, r->b.y));
                o->Draw(o, Out, r);
                if (OSD)
                    DrawOSDOnRect(Out, *r);
                MouseRecap(Out, *r);
                a = a->Next;
            } while (a != b);

            release_bitmap(Out);
        }
        else if (buffer)
        {
            ///////////////////////////////////////////////////////////////////////////

            TRect Tmp = o->Absolute;
            if (w)
                RectIntersept(&Tmp, o->DrawAbsolute, *w);
            set_clip_rect(buffer, max(0, o->DrawAbsolute.a.x),
                          max(0, o->DrawAbsolute.a.y),
                          min(buffer->w - 1, o->DrawAbsolute.b.x),
                          min(buffer->h - 1, o->DrawAbsolute.b.y));

            o->Draw(o, buffer, &Tmp);
            acquire_bitmap(Out);
            WidgetDrawFromBuffer(o, l, Out, buffer, d, w);
            release_bitmap(Out);
        }
        UnPauseMultitasking();
    }
    FreeList(l);
}
////////////////////////////////////////////////////////////////////////////////
void WidgetDrawOnRectListEx(PWidget o, PList List, PImage Output, PPoint Delta)
{
    if (o->Flags & WFVisible)
    {
        if (RectOverlayList(List, o->DrawAbsolute))
        {
            o->Refresh(o, &o->DrawAbsolute, List, Output, Delta);
            if (o->Last)
            {
                PWidget a = o->Last->Next;
                PWidget b = a;
                do
                {
                    WidgetDrawOnRectListEx(a, List, Output, Delta);
                    a = a->Next;
                } while (a != b);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void WidgetDrawOnRectEx(PWidget o, TRect w, PWidget Ignore)
{
    if (_RectOverlay(o->DrawAbsolute, w) && (o->Flags & WFVisible))
    {
        o->Refresh(o, &w, NULL, screen, NULL);
        if (o->Last)
        {
            PWidget a = o->Last;
            PWidget b = a;
            do
            {
                if (a != Ignore)
                    WidgetDrawOnRect(a, w, Ignore);
                a = a->Next;
            } while (a != b);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void WidgetDrawAllEx(PWidget o)
{
    if (!(o->Flags & WFVisible))
    {
        return;
    }

    o->Refresh(o, NULL, NULL, screen, NULL);
    if (o->Last)
    {
        PWidget a = o->Last;
        PWidget b = a;
        do
        {
            WidgetDrawAll(a);
            a = a->Next;
        } while (a != b);
    }
}
////////////////////////////////////////////////////////////////////////////////
/**
    Draw widget and all its childs clipped by specified Rect.
    @param o Widget to redraw
    @param w Pointer to the clipping rect. Can't be NULL use WidgetDrawAll instead.
    @param Ignore A widget to ignore (won't be redraw). Can be NULL.
    @see WidgetDrawAll
**/
void WidgetDrawOnRect(PWidget o, TRect w, PWidget Ignore)
{
    if (!WidgetCheckVisible(o))
        return;
    WidgetDrawOnRectEx(o, w, Ignore);
}
/**
    Draw widget and all its childs.
    @param o Widget to redraw
*/
////////////////////////////////////////////////////////////////////////////////
void WidgetDrawAll(PWidget o)
{

    if (!WidgetCheckVisible(o))
    {
        return;
    }

    WidgetDrawAllEx(o);
}
////////////////////////////////////////////////////////////////////////////////
/**
    Draw widget clipped by specified Rect (can be NULL).
    Do not redraw its childs.
    @param o Widget to redraw
    @param w Pointer to the clipping rect, if NULL use widget DrawAbsolute
    @see WidgetDrawAll
    @see WidgetDrawOnRect
*/
void WidgetDraw(PWidget o, PRect w)
{
    if (!WidgetCheckVisible(o))
        return;
    o->Refresh(o, w, NULL, screen, NULL);
}
////////////////////////////////////////////////////////////////////////////////
// Widget Tree related
////////////////////////////////////////////////////////////////////////////////
void RemoveWidgetEx(PWidget o, PWidget i)
{
    if (i->Next == i)
    { // Alone ...
        o->Last = NULL;
    }
    else
    {
        i->Next->Prev = i->Prev;
        i->Prev->Next = i->Next;
        if (o->Last == i)
            o->Last = i->Prev;
    }
    i->Parent = NULL;
}
////////////////////////////////////////////////////////////////////////////////
void _WidgetRemove(PWidget o, PWidget i)
{

    RemoveWidgetEx(o, i);
    WidgetDrawOnRect(o, i->DrawAbsolute, NULL);

    if ((i->Flags & WFSelected) && o->Last)
    {
        PWidget a = o->Last->Next;
        PWidget b = a;
        do
        {
            WidgetDrawAll(a);
            if (a->Flags & WFSelectable)
            {
                WidgetSelect(a);
                return;
            }
            a = a->Next;
        } while (a != b);
    }
}
////////////////////////////////////////////////////////////////////////////////
/**
    Remove a widget (@a i) from its parent (@a o)
    @param o Parent widget
    @param i Widget to remove
*/
void WidgetRemove(PWidget o, PWidget i)
{
    o->Remove(o, i);
}
////////////////////////////////////////////////////////////////////////////////
void _WidgetInsertBefore(PWidget o, PWidget i, PWidget b)
{
    PWidget t = b;

    if (!o->Last)
    {
        i->Next = i;
        i->Prev = i;
        o->Last = i;
    }
    else
    {
        if (b)
        {
            i->Prev = b->Prev;
            i->Next = b;
            b->Prev->Next = i;
            b->Prev = i;
        }
        else
        {
            i->Next = o->Last->Next;
            i->Prev = o->Last;
            o->Last->Next->Prev = i;
            o->Last->Next = i;
            o->Last = i;
        }
    }

    i->Parent = o;

    WidgetRefreshAbsolutePos(i);

    /*
    *       Call setup only once, not on z-order cahnge
    */
    if (i->Setup)
    {
        i->Setup(i);
        i->Setup = 0;
    }
}
////////////////////////////////////////////////////////////////////////////////
void _WidgetInsert(PWidget o, PWidget i)
{

    WidgetUnSelectAllSubs(o);
    if (o->Flags & WFSelectable)
        o->Flags |= WFSelected;

    if (o->Last)
        _WidgetInsertBefore(o, i, o->Last->Next);
    else
        _WidgetInsertBefore(o, i, NULL);
}
////////////////////////////////////////////////////////////////////////////////
/**
    Insert a widget (@a i) into an other (@a o)
    @param o Destination widget
    @param i Widget to insert
*/
void WidgetInsert(PWidget o, PWidget i)
{
    o->Insert(o, i);
}
////////////////////////////////////////////////////////////////////////////////
void _WidgetSetFirstEx(PWidget o, PWidget i)
{
    RemoveWidgetEx(o, i);
    _WidgetInsertBefore(o, i, o->Last->Next);
}
////////////////////////////////////////////////////////////////////////////////
void WidgetSetFirst(PWidget o)
{

    if (o->Parent)
        o->Parent->SetFirstEx(o->Parent, o);

    WidgetSelect(o);

    WidgetDraw(o, NULL);
    WidgetDrawAll(o);
}
////////////////////////////////////////////////////////////////////////////////
/**
    Calculate widget index in its parent
    @param o Widget
    @return Widget index or -1 on error (widget not found)
*/
l_long WidgetGetIndex(PWidget o)
{
    PWidget a, b;
    l_long idx = 0;
    if (!o->Parent)
        return -1;
    if (!o->Parent->Last)
        return -1;
    a = b = o->Parent->Last->Next;
    do
    {
        if (a == o)
            return idx;
        idx++;
        a = a->Next;
    } while (a != b);
    return -1;
}
////////////////////////////////////////////////////////////////////////////////
/**
    Get a sub-widget by its name
    @param o Widget to search in
    @param Name Case sensitive name of widget to found
    @return Pointer to widget or NULL if not found.
*/
PWidget WidgetGetName(PWidget o, l_text Name)
{

    if (o->Name)
        if (!TextCompare(o->Name, Name))
            return o;

    if (o->Last)
    {
        PWidget a, b, r;
        a = b = o->Last->Next;
        do
        {
            if (r = WidgetGetName(a, Name))
                return r;
            a = a->Next;
        } while (a != b);
    }

    return NULL;
}
////////////////////////////////////////////////////////////////////////////////
// Widget Positions and related...
////////////////////////////////////////////////////////////////////////////////
void WidgetRefreshAbsolutePos(PWidget o)
{
    TRect Tmp;
    if (o->Parent)
    {
        RectMove(&Tmp, o->Parent->ChildArea, o->Parent->Absolute.a);
        RectMove(&o->Absolute, o->Relative, Tmp.a);
        if (_RectOverlay(o->Absolute, o->Parent->ChildDrawAbs))
        {
            RectIntersept(&o->DrawAbsolute, o->Absolute, o->Parent->ChildDrawAbs); // It can have a part out of it owner !!!
        }
        else
        {
            RectAssign(&o->DrawAbsolute, 0, 0, -1, -1);
        }
    }

    RectMove(&Tmp, o->ChildArea, o->Absolute.a);
    RectIntersept(&o->ChildDrawAbs, Tmp, o->DrawAbsolute); // It can have a part out of it owner !!!

    if (o->Last)
    {
        PWidget a = o->Last;
        PWidget b = a;
        if (a)
            do
            {
                WidgetRefreshAbsolutePos(a);
                a = a->Next;
            } while (a != b);
    }
}
////////////////////////////////////////////////////////////////////////////////
void WidgetRefreshAbsolutePosEx(PWidget o)
{
    TRect Tmp;
    if (o->Parent)
    {
        RectMove(&Tmp, o->Parent->ChildArea, o->Parent->Absolute.a);
        RectMove(&o->Absolute, o->Relative, Tmp.a);
        if (_RectOverlay(o->Absolute, o->Parent->ChildDrawAbs))
        {
            RectIntersept(&o->DrawAbsolute, o->Absolute, o->Parent->ChildDrawAbs); // It can have a part out of it owner !!!
        }
        else
        {
            RectAssign(&o->DrawAbsolute, 0, 0, -1, -1);
        }
    }

    RectMove(&Tmp, o->ChildArea, o->Absolute.a);
    RectIntersept(&o->ChildDrawAbs, Tmp, o->DrawAbsolute); // It can have a part out of it owner !!!
}
////////////////////////////////////////////////////////////////////////////////
void WidgetSetChildArea(PWidget o, TRect r)
{
    o->ChildArea = r;
    WidgetRefreshAbsolutePos(o);
}
////////////////////////////////////////////////////////////////////////////////
void WidgetSetRect(PWidget o, TRect r)
{
    TRect p = o->ChildArea;

    o->ChildArea.b.x = r.b.x - r.a.x - o->Relative.b.x + o->ChildArea.b.x + o->Relative.a.x;
    o->ChildArea.b.y = r.b.y - r.a.y - o->Relative.b.y + o->ChildArea.b.y + o->Relative.a.y;
    o->Relative = r;

    WidgetRefreshAbsolutePosEx(o);
    /*
      if ( o->Last ) {
        PWidget a = o->Last;
        PWidget b = a;
        r = o->ChildArea;
        do {
        if ( a->Flags & WF_AUTORESIZE ) {
            TRect n = a->Relative;
            if ( a->Flags & WF_MAGNETRIGHT ) {
                if ( a->Flags & WF_FIXEDWIDTH ) n.a.x -= p.b.x-o->ChildArea.b.x;
                n.b.x -= p.b.x-o->ChildArea.b.x;
            }
            if ( a->Flags & WF_MAGNETBOTTOM ) {
                if ( a->Flags & WF_FIXEDHEIGHT ) n.a.y -= p.b.y-o->ChildArea.b.y;
                n.b.y -= p.b.y-o->ChildArea.b.y;
            }
            WidgetSetRect(a,n);
            WidgetSendSEvent(a,EV_MESSAGE,MSG_NOTIFY_AUTO_RESIZE,NULL);
        } else
            WidgetRefreshAbsolutePos(a);
          a = a->Next;
        } while ( a != b );
      }*/

    if (o->Last)
        _WidgetAutosize(o, p, o->ChildArea);
}
////////////////////////////////////////////////////////////////////////////////
void WidgetMove(PWidget o, l_long dX, l_long dY)
{
    TRect r = o->Relative;
    TRect t = o->DrawAbsolute;
    PList List;
    r.a.x += dX;
    r.a.y += dY;
    r.b.x += dX;
    r.b.y += dY;
    WidgetSetRect(o, r);
    void vsync();
    WidgetDrawAll(o);
    List = NewListFromRect(t);
    RemoveZoneInList(List, o->DrawAbsolute);
    WidgetDrawOnRectListEx(o->Parent, List, screen, NULL);
    FreeList(List);
}
////////////////////////////////////////////////////////////////////////////////
void WidgetMoveToPoint(PWidget o, l_int x, l_int y)
{
    WidgetMove(o, x - o->Relative.a.x, y - o->Relative.a.y);
}
////////////////////////////////////////////////////////////////////////////////
void WidgetSetMetrics(PWidget o, l_int x, l_int y, l_int w, l_int h)
{
    TRect r = o->Relative;
    TRect t = o->DrawAbsolute;
    PList List;
    r.a.x = x;
    r.a.y = y;
    r.b.x = w;
    r.b.y = h;
    WidgetSetRect(o, r);
    WidgetDrawAll(o);
    List = NewListFromRect(t);
    RemoveZoneInList(List, o->DrawAbsolute);
    WidgetDrawOnRectListEx(o->Parent, List, screen, NULL);
    FreeList(List);
}
////////////////////////////////////////////////////////////////////////////////
void WidgetResize(PWidget o, l_long dW, l_long dH)
{
    TRect r = o->Relative;
    TRect t = o->DrawAbsolute;
    PList List;
    r.b.x += dW;
    r.b.y += dH;
    WidgetSetRect(o, r);
    WidgetDrawAll(o);
    List = NewListFromRect(t);
    RemoveZoneInList(List, o->DrawAbsolute);
    WidgetDrawOnRectListEx(o->Parent, List, screen, NULL);
    FreeList(List);
}
////////////////////////////////////////////////////////////////////////////////
void WidgetResizeToSize(PWidget o, l_long SizeX, l_long SizeY)
{
    TRect r = o->Relative;
    TRect t = o->DrawAbsolute;
    PList List;
    r.b.x = o->Absolute.a.x + SizeX;
    r.b.y = o->Absolute.a.y + SizeY;
    WidgetSetRect(o, r);
    WidgetDrawAll(o);
    List = NewListFromRect(t);
    RemoveZoneInList(List, o->DrawAbsolute);
    WidgetDrawOnRectListEx(o->Parent, List, screen, NULL);
    FreeList(List);
}
////////////////////////////////////////////////////////////////////////////////
void WidgetPreCenter(PWidget o)
{
    TRect r = o->Relative;
    TRect rr = o->Parent->ChildArea;
    l_long w = r.b.x - r.a.x, h = r.b.y - r.a.y;
    l_long ww = rr.b.x - rr.a.x, hh = rr.b.y - rr.a.y;
    o->Relative.a.x = (ww - w) / 2;
    o->Relative.a.y = (hh - h) / 2;
    o->Relative.b.x = o->Relative.a.x + w;
    o->Relative.b.y = o->Relative.a.y + h;
    WidgetRefreshAbsolutePos(o);
}

void WidgetPostCenter(PWidget o)
{
    TRect r = o->Relative;
    TRect rr = o->Parent->ChildArea;
    l_long w = r.b.x - r.a.x, h = r.b.y - r.a.y;
    l_long ww = rr.b.x - rr.a.x, hh = rr.b.y - rr.a.y;
    o->Relative.a.x = (ww - w) / 2;
    o->Relative.a.y = GSScreenHeight - 33;
    o->Relative.b.x = o->Relative.a.x + w;
    o->Relative.b.y = GSScreenHeight;
    WidgetRefreshAbsolutePos(o);
}
////////////////////////////////////////////////////////////////////////////////
// Event related
////////////////////////////////////////////////////////////////////////////////
l_bool WidgetSendEventEx(PWidget o, TEvent Ev)
{

    //DebugMessage("%x :: %d %x",o,Ev.Type,Ev.Message);

    if ((o->Flags & WFExecuting) && Ev.Type == EV_MESSAGE)
    {
        DebugMessage("Catch message( %x, %x )", o, Ev.Message);
        o->MessageBuffer = Ev.Message;
        return true;
    }

    if (o->AppEvHdl)
        if (o->AppEvHdl(o, &Ev))
            return true;

    if (o->EventHandler)
        if (o->EventHandler(o, &Ev))
            return true;

    return false;
}
////////////////////////////////////////////////////////////////////////////////
/* ** Due to unknown threading bugs, this section is unactivated **

    void WidgetRedirectEvent ( void *Dest, PDynLdEvent Ev ) {
  if ( !WidgetSendEventEx(WIDGET(Dest), Ev->Ev ) ) {

      if ( WIDGET(Dest)->Parent && ( Ev->Ev.Type != EV_MOUSE ) )
        WidgetSendEvent( WIDGET(Dest)->Parent, Ev->Ev );
  }
}
*/
////////////////////////////////////////////////////////////////////////////////
l_bool WidgetSendEvent(PWidget o, TEvent Ev)
{
    /* ** Due to unknown threading bugs, this section is unactivated **
  if ( o->AppOwner ) {
        if ( o->AppOwner->Type == DYNLD_TYPEAPP ) { // Only App can handle events, Dl and old style programs can't
        AppSendEventEv( o->AppOwner, Ev, o, &WidgetRedirectEvent );
        return true;
    }
  }*/

    if (WidgetSendEventEx(o, Ev))
        return true;
    if (o->Parent && (Ev.Type != EV_MOUSE) && !((Ev.Type == EV_MESSAGE) && (Ev.Message == MSG_NOTIFY_AUTO_RESIZE)))
        return WidgetSendEvent(o->Parent, Ev);
    else
        return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool WidgetSendPEvent(PWidget o, PEvent Ev)
{
    l_bool r = WidgetSendEvent(o, *Ev);
    free(Ev);
    return r;
}
////////////////////////////////////////////////////////////////////////////////
l_bool WidgetSendEEvent(PWidget o, l_int Type, l_ulong Message, void *Extra, TKeybState Keyb, TMouseState Mouse)
{
    TEvent Ev;
    Ev.Type = Type;
    Ev.Message = Message;
    Ev.Extra = Extra;
    Ev.Keyb = Keyb;
    Ev.Mouse = Mouse;
    return WidgetSendEvent(o, Ev);
}
////////////////////////////////////////////////////////////////////////////////
l_bool WidgetSendSEvent(PWidget o, l_int Type, l_ulong Message, void *Extra)
{
    TEvent Ev;
    Ev.Type = Type;
    Ev.Message = Message;
    Ev.Extra = Extra;
    Ev.Keyb = KState;
    Ev.Mouse = Mouse->State;
    return WidgetSendEvent(o, Ev);
}
////////////////////////////////////////////////////////////////////////////////
void WidgetSetFocusExNoNotify(PWidget o)
{
    if (o->FocusBack)
        o = o->FocusBack;
    if (o->Flags & WFFocusable)
    {
        if (FocusedWidget)
            FocusedWidget->Flags &= ~WFFocused;
        o->Flags |= WFFocused;
        FocusedWidget = o;
    }
}
////////////////////////////////////////////////////////////////////////////////
void WidgetSetFocusEx(PWidget o)
{
    if (o->FocusBack)
        o = o->FocusBack;
    if (o->Flags & WFFocusable)
    {
        PWidget Old = FocusedWidget;
        if (FocusedWidget)
            FocusedWidget->Flags &= ~WFFocused;
        o->Flags |= WFFocused;
        FocusedWidget = o;
        if (Old)
            WidgetSendSEvent(Old, EV_MESSAGE, WEvNotifyLostFocus, NULL);
    }
}
////////////////////////////////////////////////////////////////////////////////
void WidgetSetFocus(PWidget o)
{

    if (o->FocusBack)
        o = o->FocusBack;
    if (FocusedWidget == o)
        return;

    if (o->Flags & WFFocusable)
    {
        WidgetSetFocusEx(o);

        if (FocusedWidget)
            WidgetSendSEvent(FocusedWidget, EV_MESSAGE, WEvNotifyFocused, NULL);
    }
    else
    {
        if (o->Parent)
            WidgetSetFocus(o->Parent);
        else if (FocusedWidget)
            WidgetSendSEvent(FocusedWidget, EV_MESSAGE, WEvNotifySameFocus, NULL);
    }
}
////////////////////////////////////////////////////////////////////////////////
void WidgetSelect(PWidget o)
{

    if (o->Flags & WFSelectable && !(o->Flags & WFSelected))
    {
        o->Flags |= WFSelected;
        WidgetSendSEvent(o, EV_MESSAGE, WEvNotifySelect, NULL);
    }

    if (o->Parent)
    {
        PWidget a = o->Parent->Last;
        PWidget b = a;

        if (a)
            do
            {
                if ((a != o) && (a->Flags & WFSelected) && (a->Flags & WFSelectable))
                {
                    a->Flags &= ~WFSelected;
                    WidgetSendSEvent(a, EV_MESSAGE, WEvNotifyUnSelect, NULL);
                }

                a = a->Next;
            } while (a != b);

        WidgetSelect(o->Parent);
    }
}
////////////////////////////////////////////////////////////////////////////////
void WidgetUnSelectAllSubs(PWidget o)
{
    if (o->Last)
    {
        PWidget a = o->Last;
        PWidget b = a;
        if (a)
            do
            {
                if (a->Flags & WFSelected && a->Flags & WFSelectable)
                {
                    a->Flags &= ~WFSelected;
                    //if (  )
                    WidgetSendSEvent(a, EV_MESSAGE, WEvNotifyUnSelect, NULL);
                }
                a = a->Next;
            } while (a != b);
    }
}
////////////////////////////////////////////////////////////////////////////////
_PRIVATE void WidgetEventHandler(PEvent Event)
{
    /*
    *       Mouse event
    */
    if (Event->Type == EV_MOUSE)
    {
        PWidget OldTopWidget = TopWidget;

        if (MasterWidget)
            TopWidget = MasterWidget;
        else
            TopWidget = GetTopWidgetUnderMouse();

        if (WorkingFlags & WWF_DRAGING)
        {
            l_ulong Method = 0;

            if (OldTopWidget != TopWidget)
            {
                LastMouseMove = ATime;
                if (OldTopWidget)
                {
                    WidgetSendSEvent(OldTopWidget, EV_MOUSE, WEvMouseLeft, NULL);
                    OldTopWidget->Flags &= ~WF_DRAGFOCUS;
                }
                TopWidget->Flags |= WF_DRAGFOCUS;
                if (__WidgetDropAccept(TopWidget, &Method))
                {
                    if (!DragOldAccept || (DragOldMethod != Method))
                    {
                        if (Method == DMETHOD_COPY)
                            MouseSetCursor(DragCopyCur);
                        else
                            MouseSetCursor(DragCur);
                    }
                    DragOldAccept = true;
                    DragOldMethod = Method;
                }
                else
                {
                    if (DragOldAccept)
                        MouseSetCursor(NoDragCur);
                    DragOldAccept = false;
                }
                WidgetSendSEvent(TopWidget, EV_MOUSE, WEvMouseEnter, NULL);
            }
            else if (Mouse->Moved)
            {
                LastMouseClickRight = 0;
                LastMouseClickMiddle = 0;
                LastMouseClickLeft = 0;
                LastMouseMove = ATime;
                if (__WidgetDropAccept(TopWidget, &Method))
                {
                    if (!DragOldAccept || (DragOldMethod != Method))
                    {
                        if (Method == DMETHOD_COPY)
                            MouseSetCursor(DragCopyCur);
                        else
                            MouseSetCursor(DragCur);
                    }
                    DragOldAccept = true;
                    DragOldMethod = Method;
                }
                else
                {
                    if (DragOldAccept)
                        MouseSetCursor(NoDragCur);
                    DragOldAccept = false;
                }
                WidgetSendSEvent(TopWidget, EV_MOUSE, WEvMouseMove, NULL);
            }

            if ((Mouse->ButtonChanges & BUTTON_LEFT) && !(Mouse->State.b & BUTTON_LEFT))
            {
                if (__WidgetDropAccept(TopWidget, &Method))
                {
                    TopWidget->Flags &= ~WF_DRAGFOCUS;
                    if (DragCaller->DragBeforeDrop(DragCaller, TopWidget, DragData, DragDataType, Method))
                    {
                        TopWidget->DropData(TopWidget, DragCaller, DragData, DragDataType, Method);
                        DragCaller->DragAfterDrop(DragCaller, TopWidget, DragData, DragDataType, Method);
                    }
                    else
                        WidgetSendSEvent(DragCaller, EV_MOUSE, MSG_DRAGCANCEL, NULL);
                }
                else
                {
                    TopWidget->Flags &= ~WF_DRAGFOCUS;
                    WidgetSendSEvent(DragCaller, EV_MOUSE, MSG_DRAGCANCEL, NULL);
                }
                DragStop();
            }
        }
        else
        {

            if (OldTopWidget != TopWidget)
            {
                LastMouseMove = ATime;
                if (OldTopWidget)
                    WidgetSendSEvent(OldTopWidget, EV_MOUSE, WEvMouseLeft, NULL);

                WidgetSendSEvent(TopWidget, EV_MOUSE, WEvMouseEnter, NULL);
            }

            if (WidgetSendEventEx) // ?removed because TRUE every time. Florian Xaver?
            {
                if (Event->Message)
                {
                    WidgetSendSEvent(TopWidget, EV_MOUSE, Event->Message, NULL);
                }

                if (Mouse->Moved)
                {
                    LastMouseClickRight = 0;
                    LastMouseClickMiddle = 0;
                    LastMouseClickLeft = 0;
                    LastMouseMove = ATime;
                    WidgetSendSEvent(TopWidget, EV_MOUSE, WEvMouseMove, NULL);
                }

                if (Mouse->ButtonChanges)
                {
                    if (((Mouse->ButtonChanges & BUTTON_LEFT) && (Mouse->State.b & BUTTON_LEFT)) || ((Mouse->ButtonChanges & BUTTON_RIGHT) && (Mouse->State.b & BUTTON_RIGHT)))
                    {
                        WidgetSetFocus(TopWidget);
                        WidgetSelect(TopWidget);
                    }

                    if (!TopWidget)
                        return;

                    // Left mouse down [X| | ]
                    if ((Mouse->ButtonChanges & BUTTON_LEFT) && (Mouse->State.b & BUTTON_LEFT))
                    {
                        if (ATime - LastMouseClickLeft <= DoubleClickSpeed)
                        {
                            if (WidgetSendSEvent(TopWidget, EV_MOUSE, WEvMouseLDClk, NULL))
                                return;
                        }
                        else
                            LastMouseClickLeft = ATime;

                        if (WidgetSendSEvent(TopWidget, EV_MOUSE, WEvMouseLDown, NULL))
                            return;
                    }

                    // Middle mouse down [ |X| ]
                    if ((Mouse->ButtonChanges & BUTTON_MIDDLE) && (Mouse->State.b & BUTTON_MIDDLE))
                    {
                        if (ATime - LastMouseClickMiddle <= DoubleClickSpeed)
                        {
                            if (WidgetSendSEvent(TopWidget, EV_MOUSE, WEvMouseMDClk, NULL))
                                return;
                        }
                        else
                            LastMouseClickMiddle = ATime;

                        if (WidgetSendSEvent(TopWidget, EV_MOUSE, WEvMouseMDown, NULL))
                            return;
                    }

                    // Right mouse down [ | |X]
                    if ((Mouse->ButtonChanges & BUTTON_RIGHT) && (Mouse->State.b & BUTTON_RIGHT))
                    {
                        if (ATime - LastMouseClickRight <= DoubleClickSpeed)
                        {
                            if (WidgetSendSEvent(TopWidget, EV_MOUSE, WEvMouseRDClk, NULL))
                                return;
                        }
                        else
                            LastMouseClickRight = ATime;

                        if (WidgetSendSEvent(TopWidget, EV_MOUSE, WEvMouseRDown, NULL))
                            return;
                    }

                    // Left mouse up/click [X| | ]

                    if ((Mouse->ButtonChanges & BUTTON_LEFT) && !(Mouse->State.b & BUTTON_LEFT))
                        if (WidgetSendSEvent(TopWidget, EV_MOUSE, WEvMouseLUp, NULL))
                            return;

                    // Middle mouse up/click [ |X| ]
                    if ((Mouse->ButtonChanges & BUTTON_RIGHT) && !(Mouse->State.b & BUTTON_RIGHT))
                        if (WidgetSendSEvent(TopWidget, EV_MOUSE, WEvMouseRUp, NULL))
                            return;

                    // Right mouse up/click [ | |X]
                    if ((Mouse->ButtonChanges & BUTTON_MIDDLE) && !(Mouse->State.b & BUTTON_MIDDLE))
                        if (WidgetSendSEvent(TopWidget, EV_MOUSE, WEvMouseMUp, NULL))
                            return;
                }
            }
        }
    }

    /*
    *       Keyboard event
    */
    if (Event->Type == EV_KEYBOARD)
    {
        if (!FocusedWidget)
            return;

        WidgetSendEvent(FocusedWidget, *Event);
    }
}
////////////////////////////////////////////////////////////////////////////////
// Widget Unitilisation
////////////////////////////////////////////////////////////////////////////////
void FreeWidget(PWidget o)
{
    if (o->FreeEx)
        o->FreeEx(o);
    if (!(o->Flags & WF_TOOLTIPFUNC) && (o->ToolTip.t))
        free(o->ToolTip.t);
    if (o->Name)
        free(o->Name);
    free(o);
    o = NULL; // Florian Xaver
}
////////////////////////////////////////////////////////////////////////////////
void WidgetDisposeEx(PWidget o)
{
    if (TopWidget == o)
        TopWidget = NULL;
    if (MasterWidget == o)
        MasterWidget = NULL;
    if (FocusedWidget == o)
        FocusedWidget = NULL;
    if (LookFor)
        if (*LookFor == o)
            *LookFor = NULL;

    if (o->Last)
    {
        PWidget a = o->Last;
        PWidget b = a, n;

        do
        {
            n = a->Next;
            WidgetDisposeEx(a);
            a = n;
        } while (a != b);
    }

    FreeWidget(o);
}
////////////////////////////////////////////////////////////////////////////////
/**     Remove and free a widget
    @param o Widget to dispose
*/
void WidgetDispose(PWidget o)
{
    if (!o)
        return;
    if (o->Parent)
        RemoveWidget(o->Parent, o);
    WidgetDisposeEx(o);
}
////////////////////////////////////////////////////////////////////////////////
void WidgetMakeSync(PWidget *o)
{
    LookFor = o;
}
////////////////////////////////////////////////////////////////////////////////
void WidgetUnMakeSync(PWidget *o)
{
    LookFor = NULL;
}
////////////////////////////////////////////////////////////////////////////////
void _DisposeAllAppWidgets(PWidget o, PApplication App)
{
    if (o->AppOwner == App)
    {
        WidgetDispose(o);
        return;
    }
    if (o->Last)
    {
        PWidget a = o->Last, n;
        do
        {
            n = a->Next;
            _DisposeAllAppWidgets(a, App);
            a = n;
        } while (a != o->Last);
    }
}
////////////////////////////////////////////////////////////////////////////////
void DisposeAllAppWidgets(PApplication App)
{
    if (DeskTop)
        _DisposeAllAppWidgets(DeskTop, App);
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
l_ulong WidgetExecute(PWidget o)
{
    l_char Done = 0;
    l_ulong Msg = 0;
    if (o->Flags & WFExecuting)
        return 0;

    /* ** Due to unknown threading bugs, this section is unactivated **
  if ( o->AppOwner ) {
        if ( o->AppOwner->Type == DYNLD_TYPEAPP ) {
      TDynLdEvent Ev = {0,0,0,0,0};
      if ( o->AppOwner->ThreadId != ThreadId() ) DebugError("PANIC :: WidegtExecute called by non-widget owner");
          while ( AppGetNextEvent(o->AppOwner,&Ev) ) {
              if ( Ev.Ev.Type == EV_MESSAGE && Ev.Dest == o ) {
                return Ev.Ev.Message;
            } else {
        AppRedirectEvent(o->AppOwner,&Ev);
        Yield();
    }
      }
      // Application has recieved exit message, but we deleted it, so we resend it
      AppSendEvent(o->AppOwner,EV_MESSAGE,MSG_KILLAPP,NULL,NULL,NULL);
      return 0;
        }
  }*/

    o->Flags |= WFExecuting;

    DebugMessage("Executing %x", o);
    while (!o->MessageBuffer)
    {
        /*PollEvents();
        Yield();*/
        SysPoll();
    }
    o->Flags &= ~WFExecuting;

    DebugMessage("Return Message %x", o->MessageBuffer);
    Msg = o->MessageBuffer;
    o->MessageBuffer = 0;
    return Msg;
}
////////////////////////////////////////////////////////////////////////////////
// Widget Initialisation
////////////////////////////////////////////////////////////////////////////////
PWidget IntialiseWidget(PApplication App, PWidgetClass Class, PWidget o, TRect r)
{

    if (!o)
        return NULL;

    if (!Class)
        Class = StdWidgetClass;

    o->Class = Class;
    o->Relative = r;
    o->Absolute = r;
    RectAssign(&o->ChildArea, 0, 0, r.b.x - r.a.x, r.b.y - r.a.y);
    o->ChildDrawAbs = r;
    o->DrawAbsolute = r;
    o->Flags = WFVisible;
    o->AppOwner = App;

    o->Refresh = &WidgetRefresh;

    o->Remove = &_WidgetRemove;
    o->Insert = &_WidgetInsert;
    o->SetFirstEx = &_WidgetSetFirstEx;
    return o;
}
////////////////////////////////////////////////////////////////////////////////
/** Create a new standart widget
    @param App Application which want to allocate the widget( use &Me in most cases)
    @param r Relative coordinates of new widget
*/
PWidget CreateWidget(PApplication App, TRect r)
{
    PWidget o = (PWidget)malloc(sizeof(TWidget));
    memset(o, 0, sizeof(TWidget));
    o = IntialiseWidget(App, StdWidgetClass, o, r);
    return o;
}
////////////////////////////////////////////////////////////////////////////////
void SwitchTextMode(void)
{
    if (buffer)
    {
        destroy_bitmap(buffer);
        buffer = NULL;
    }
    __MouseStop();
    __KeyboardStop();
    __ScreenTextMode();
}
////////////////////////////////////////////////////////////////////////////////
void SwitchGraphicsMode(void)
{

    __ScreenRestartGraphics();
    __KeyboardRestart();
    __MouseRestart();
    buffer = create_system_bitmap(GSScreenWidth, GSScreenHeight);
    WidgetDrawAll(DeskTop);
    MouseShow();
}
////////////////////////////////////////////////////////////////////////////////
// Tool tips system
////////////////////////////////////////////////////////////////////////////////
/**
*       Set a widget tooltip text
*       @param o Widget to affect
*       @param t Text to set as tooltip text (text is duplicated, so you can free @a t after)
*
*/
void WidgetSetTooltipText(PWidget o, l_text t)
{
    if (!(o->Flags & WF_TOOLTIPFUNC) && (o->ToolTip.t))
        free(o->ToolTip.t);
    o->Flags &= ~WF_TOOLTIPFUNC;
    o->ToolTip.t = TextDup(t);
}
////////////////////////////////////////////////////////////////////////////////
// Tooltip widget draw fucntion
void ToolTipDraw(PWidget o, p_bitmap buffer, PRect w)
{
    rectfill(buffer, w->a.x, w->a.y, w->b.x, w->b.y, makecol(255, 255, 255));
    rect(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, makecol(0, 0, 0));

    if (TOOLTIP(o)->Text)
    {
        l_text t = TOOLTIP(o)->Text;
        l_text lt = TOOLTIP(o)->Text;
        l_char tmp = 0;
        l_ulong y = o->Absolute.a.y + 2;
        l_ulong h = text_height(default_font) + 1;

        t = TOOLTIP(o)->Text;

        while (t)
        {
            lt = t;

            t = TextChr(t, '\n');

            if (t)
            {
                DrawTextEx(buffer, default_font, o->Absolute.a.x + 2, y, lt, t, makecol(0, 0, 0), false);
                t++;
                y += h;
            }
            else
                textout_ex(buffer, default_font, lt, o->Absolute.a.x + 2, y, makecol(0, 0, 0), -1);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
// General widget timer. Used for
//   - Mouse enter delay event
//   - Tooltips
////////////////////////////////////////////////////////////////////////////////
PTimer WidgetTimerId;
void WidgetTimer(void *Arg)
{
    // Mouse has not moved since 0.2 secconds, so notify widget for mouse enter delay
    if (LastNotified != LastMouseMove && ATime - LastMouseMove >= 200)
    {
        LastNotified = LastMouseMove;
        if (TopWidget)
            WidgetSendSEvent(TopWidget, EV_MOUSE, WEvMouseEnterDelay, NULL);
    }
    // Mouse has not moved since 2 secons, so show widget tooltip (if it exists)
    if (LastToolTip != LastMouseMove && ATime - LastMouseMove >= 2000 && !(WorkingFlags & WWF_TOOLTIPUP))
    {
        LastToolTip = LastMouseMove;

        WorkingFlags |= WWF_TOOLTIPUP;

        if (TopWidget)
            if (TopWidget->ToolTip.t)
            {
                TPoint p = Mouse->State.p;
                TRect r;
                PToolTip o;
                l_ulong w, h;
                l_text txt;

                if (TopWidget->Flags & WF_TOOLTIPFUNC)
                    txt = TopWidget->ToolTip.func(TopWidget, p);
                else
                    txt = TextDup(TopWidget->ToolTip.t);

                TextSize(default_font, txt, &w, &h);

                RectAssign(&r, p.x + Mouse->Cur->Image->w, p.y, p.x + Mouse->Cur->Image->w + w + 4, p.y + h + 4);

                o = malloc(sizeof(TToolTip));
                memset(o, 0, sizeof(TToolTip));
                IntialiseWidget(&Me, NULL, WIDGET(o), r);
                o->Text = txt;
                WIDGET(o)->Draw = &ToolTipDraw;

                WidgetInsert(DeskTop, WIDGET(o));
                WidgetDrawAll(WIDGET(o));
                while (Mouse->State.p.x == p.x && Mouse->State.p.y == p.y && !Mouse->State.b)
                {
                    SysPoll();
                }
                WidgetDispose(WIDGET(o));
                free(txt);
            }

        WorkingFlags &= ~WWF_TOOLTIPUP;
    }
}
////////////////////////////////////////////////////////////////////////////////
// Helper functions to maintain startup logo
////////////////////////////////////////////////////////////////////////////////
PWidget StartMask;
void StartMaskDraw(PWidget o, p_bitmap buffer, PRect w)
{
    blit(StartLogo, buffer, w->a.x - StartRect.a.x, w->a.y - StartRect.a.y, w->a.x, w->a.y, w->b.x - w->a.x + 1, w->b.y - w->a.y + 1);
}
////////////////////////////////////////////////////////////////////////////////
void DisposeStartupLogo(void)
{
    WidgetDispose(StartMask);
    destroy_bitmap(StartLogo);
    StartLogo = NULL;
}
/**

    @}
*/
////////////////////////////////////////////////////////////////////////////////
// Library initialisation function
PAppExtender WidgetExtender = NULL;

l_bool LibMain(int argc, l_text *argv)
{
    TRect r;

    buffer = create_system_bitmap(GSScreenWidth, GSScreenHeight);
    if (!buffer)
    {
        DebugError("Widget :: Could not init double buffer, disabling widgets");
        return false;
    }
    clear_bitmap(buffer);

    WClassInstall();

    iWidgetInstall();

    OSDColor = GetColorFromHex(KeyGetText("/USER/COLOR/OSD", "888888"));
    RectAssign(&r, 0, 0, GSScreenWidth - 1, GSScreenHeight - 1);
    DeskTop = CreateWidget(&Me, r);
    StartMask = CreateWidget(&Me, StartRect);
    StartMask->Draw = &StartMaskDraw;
    InsertWidget(DeskTop, StartMask);
    WidgetDraw(DeskTop, NULL);
    OSD = 0;
    set_mouse_range(DeskTop->Absolute.a.x, DeskTop->Absolute.a.y, DeskTop->Absolute.b.x, DeskTop->Absolute.b.y);
    InstallEventHandler(WidgetEventHandler);

    LastMouseMove = ATime;
    LastNotified = ATime;

    WidgetTimerId = NewTimer(&Me, 50, &WidgetTimer, NULL);
    DragCur = NewCursor(LoadImage("SYSTEM/CURSORS/drag.bmp"), 0, 0);
    NoDragCur = NewCursor(LoadImage("SYSTEM/CURSORS/nodrag.bmp"), 0, 0);
    DragCopyCur = NewCursor(LoadImage("SYSTEM/CURSORS/dragcopy.bmp"), 0, 0);

    // Library exports

    InitSpliters();

    APPEXPORT(_WidgetInsert);
    APPEXPORT(_WidgetRemove);
    APPEXPORT(_WidgetSetFirstEx);

    APPEXPORT(WidgetInsert);
    APPEXPORT(WidgetRemove);
    APPEXPORT(CreatePropType);
    APPEXPORT(DeskTop);
    APPEXPORTAS(DeskTop, "Desktop");
    APPEXPORT(IntialiseWidget);
    APPEXPORT(CreateWidget);
    APPEXPORT(WidgetDrawOnRect);
    APPEXPORT(WidgetMove);
    APPEXPORT(WidgetSetOSDRect);
    APPEXPORT(WidgetUnOSD);
    APPEXPORT(MasterWidget);
    APPEXPORT(WidgetResize);
    APPEXPORT(WidgetResizeToSize);
    APPEXPORT(WidgetMoveToPoint);
    APPEXPORT(WidgetSetMetrics);
    APPEXPORT(WidgetSendEvent);
    APPEXPORT(WidgetDispose);
    APPEXPORT(WidgetDrawAll);
    APPEXPORT(WidgetExecute);
    APPEXPORT(WidgetSetFirst);
    APPEXPORT(WidgetSetFocusEx);
    APPEXPORT(WidgetSetFocus);
    APPEXPORT(FocusedWidget);
    APPEXPORT(WidgetPreCenter);
    APPEXPORT(WidgetPostCenter);
    APPEXPORT(WidgetUnMakeSync);
    APPEXPORT(WidgetMakeSync);
    APPEXPORT(WidgetSetChildArea);
    APPEXPORT(WidgetSendPEvent);
    APPEXPORT(DisposeStartupLogo);
    APPEXPORT(WidgetSetRect);
    APPEXPORT(WidgetSetFocusExNoNotify);
    APPEXPORT(WidgetDraw);
    APPEXPORT(WidgetSendSEvent);
    APPEXPORT(WidgetSendEEvent);
    APPEXPORT(SwitchGraphicsMode);
    APPEXPORT(SwitchTextMode);
    APPEXPORT(WidgetSetTooltipText);
    APPEXPORT(DragStart);
    APPEXPORT(WidgetGetIndex);

    APPEXPORT(StdWidgetClass);
    APPEXPORT(CreateWidgetClass);
    APPEXPORT(WidgetIsClassId);
    APPEXPORT(GetWidgetClass);
    APPEXPORT(WClassRegisterProp);
    APPEXPORT(GetPropType);

    APPEXPORT(NewiWidget);
    APPEXPORT(InsertiWidgetBefore);
    APPEXPORT(ComplieiWidget);
    APPEXPORT(FreeiWidget);

    APPEXPORT(DisposeAllAppWidgets);

    APPEXPORT(DuplicateBool);
    APPEXPORT(DuplicateInt);
    APPEXPORT(WidgetCheckVisible);
    APPEXPORT(WidgetClasses);
    APPEXPORT(WidgetGetName);

    APPEXPORT(CreateVSpliter);
    APPEXPORT(CreateHSpliter);
    WidgetExtender = InstallAppExtender(NULL, &DisposeAllAppWidgets);

    return true;
}
////////////////////////////////////////////////////////////////////////////////
// Library unload function
void Close(void)
{
    RemoveAppExtender(WidgetExtender);

    KillTimer(WidgetTimerId);

    /*
     * Dont handle events when the system is unloaded
     */
    RemoveEventHandler(WidgetEventHandler);

    if (buffer)
        destroy_bitmap(buffer);

    WidgetDisposeEx(DeskTop);
    DeskTop = NULL;

    iWidgetUnInstall();
    WClassUnInstall();
}
////////////////////////////////////////////////////////////////////////////////
