/**
*       (c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
*       FILE:                   progress.c
*
*       PROJECT:                Phoenix engine - Core
*
*       DESCRIPTION:    Progress bar widget.
*
*       CONTRIBUTORS:
*                                       Lukas Lipka
*
*       TODO:                   @
*
*       BUG:                    @
*
*       MISTAKE:                @
*
*       IDEA:                   @
*/

#include "kernel.h"
#include "widget.h"
#include "progress.h"
#include "string.h"

l_ulong AppVersion      = ULONG_ID(0, 9, 0, 0);
l_char  AppName[]       = "Progressbar Widget";
l_uid   nUID = "progress";
l_uid NeededLibs[] = { "widget", "" };

PSkin SkinBar   = 0;
PSkin SkinBG    = 0;

PWidgetClass ProgressClass;

/**
*       NAME: PBDraw
*       DESCRIPTION: Draws the Progressbar
*/
_PUBLIC void  PBDraw(PWidget o, p_bitmap buffer, PRect w)
{
        if ( UseSkins && SkinBG && SkinBar ) {
                // Background
                DrawSkin(buffer, SkinBG, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);

                // Line indicating percentage
                if (PROGRESSBAR(o)->Promile > 1 && PROGRESSBAR(o)->Steps != 0)
                        DrawSkin(buffer, SkinBar, o->Absolute.a.x + SkinBG->Left, o->Absolute.a.y + SkinBG->Top, (o->Absolute.a.x + SkinBG->Left) + ((PROGRESSBAR(o)->Promile * ((o->Absolute.b.x - SkinBG->Right) - (o->Absolute.a.x + SkinBG->Left))) / PROGRESSBAR(o)->Steps), o->Absolute.b.y  - SkinBG->Bottom);
                /**
                *       Number of percent
                */
                if ( PROGRESSBAR(o)->Steps )
                        textout_centre_ex(buffer, default_font, TextArgs("%i%%", (PROGRESSBAR(o)->Promile * 100)/PROGRESSBAR(o)->Steps, NULL, 10), o->Absolute.a.x+((o->Absolute.b.x-o->Absolute.a.x)/2), o->Absolute.a.y + ((o->Absolute.b.y - o->Absolute.a.y)/6), makecol(255,255,255),-1);

        } else {
                // Background
                rect(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_BOXFACE);
                rectfill(buffer, o->Absolute.a.x+1, o->Absolute.a.y+1, o->Absolute.b.x-1, o->Absolute.b.y-1, COL_BOXDARK);

                // Line indicating percentage
                if (PROGRESSBAR(o)->Promile > 1 && PROGRESSBAR(o)->Steps != 0)
                        rectfill(buffer, o->Absolute.a.x+2, o->Absolute.a.y+2, (o->Absolute.a.x + 2) + ((PROGRESSBAR(o)->Promile * (o->Absolute.b.x - o->Absolute.a.x-4)) / PROGRESSBAR(o)->Steps), o->Absolute.b.y-2, COL_BOXSEL);
                /**
                *       Number of percent
                */
                if ( PROGRESSBAR(o)->Steps )
                        textout_centre_ex(buffer, default_font, TextArgs("%i%%", (PROGRESSBAR(o)->Promile * 100)/PROGRESSBAR(o)->Steps, NULL, 10), o->Absolute.a.x+((o->Absolute.b.x-o->Absolute.a.x)/2), o->Absolute.a.y + ((o->Absolute.b.y - o->Absolute.a.y)/6), COL_BOXTEXT,-1);
        }
}

/**
*       NAME: PEventHandler
*       DESCRIPTION: Event handler for the Progressbar
*/
_PUBLIC l_bool  PEventHandler(PWidget o, PEvent Ev)
{
        if (Ev->Type == EV_MOUSE)
        {

        }

        return false;
}

/**
*       NAME: PBStep
*       DESCRIPTION: Steps @Steps forward
*       EXPORTED: In the TProgressbar structure
*/
_PUBLIC l_int  PBStep(PProgressBar b, l_int Steps)
{
        if ((b->Promile + Steps) < b->Steps || (b->Promile + Steps) == b->Steps)
                b->Promile += Steps;

        WidgetDraw(WIDGET(b), NULL);

        return b->Promile;
}

/**
*       NAME: CreateProgressbar
*       DESCRIPTION: Creates a new Progressbar
*       EXPORTED: Core
*/
_PUBLIC PProgressBar  CreateProgressBar(PApplication App, TRect r, l_uint Steps)
{
        PProgressBar b = malloc(sizeof(TProgressBar));

        if (!b) return NULL;

        memset(b, 0, sizeof(TProgressBar));

        b->Steps = Steps;
        b->Promile = 0;

        b->Step = &PBStep;

        WIDGET(b)->EventHandler = &PEventHandler;
        WIDGET(b)->Draw = &PBDraw;
        IntialiseWidget(App, ProgressClass, WIDGET(b), r);

        return b;
}

////////////////////////////////////////////////////////////////////////////////
PWidget  StdCreateProgress ( PApplication App, TRect r )
{
        return WIDGET(CreateProgressBar( App, r, 100));
}

////////////////////////////////////////////////////////////////////////////////
l_bool  ProgressSetRProp ( PWidget o, l_text Name, void *Value, l_bool Redraw ) {

        if ( !TextCompare(Name,"value") ) {
                PROGRESSBAR(o)->Promile = *((l_int*)Value);
                if ( PROGRESSBAR(o)->Promile > PROGRESSBAR(o)->Steps ) PROGRESSBAR(o)->Promile = PROGRESSBAR(o)->Steps;
                if ( Redraw ) WidgetDraw(WIDGET(o), NULL);
                return true;
        }
        if ( !TextCompare(Name,"max") ) {
                PROGRESSBAR(o)->Steps = *((l_int*)Value);
                if ( PROGRESSBAR(o)->Promile > PROGRESSBAR(o)->Steps ) PROGRESSBAR(o)->Promile = PROGRESSBAR(o)->Steps;
                if ( Redraw ) WidgetDraw(WIDGET(o), NULL);
                return true;
        }

        return false;
}
////////////////////////////////////////////////////////////////////////////////
void    *ProgressGetRProp ( PWidget o, l_text Name ) {
        if ( !TextCompare(Name,"value") ) return DuplicateInt(&(PROGRESSBAR(o)->Promile));
        if ( !TextCompare(Name,"max") ) return DuplicateInt(&(PROGRESSBAR(o)->Steps));
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////


l_bool LibMain (l_text Args)
{
        ProgressClass = CreateWidgetClass( ULONG_ID('P','r','g','s'), StdWidgetClass, 0 );
        ProgressClass->StdCreate = &StdCreateProgress;
        ProgressClass->SetRProp = &ProgressSetRProp;
        ProgressClass->GetRProp = &ProgressGetRProp;

        WClassRegisterProp(ProgressClass,PTYPE_INT,"value");
        WClassRegisterProp(ProgressClass,PTYPE_INT,"max");

        /**
        *       Skin items sizes
        */
        SkinBar         = GetSkinItem("ProgressBar");
        SkinBG          = GetSkinItem("ProgressBackground");

        APPEXPORT(CreateProgressBar);
//        SYSEXPORT(CreateProgressBar);
/*        TRect r;
        RectAssign(&r, 50,50,250,70);
        PProgressBar p = CreateProgressBar(&Me, r, 10);
        p->Promile=0;
        InsertWidget(DeskTop, WIDGET(p));
        WidgetDraw(WIDGET(p), NULL);*/

        return true;
}

void Close (void)
{

}
