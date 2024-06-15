/**
*       (c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
*       FILE:                   mouse.c
*
*       PROJECT:                Phoenix engine - Core
*
*       DESCRIPTION:    Mouse event handler.
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

/** @defgroup mouse Mouse manager
        @{
*/
/**     @file
        Mouse core file.
*/
#include "kernel.h"
#include <string.h>

int Buttons = -1;

PCursor cursor_pointer = 0;
PCursor cursor_resize = 0;
PCursor cursor_clock = 0;
PCursor cursor_pencil = 0;
l_int    old_button = 0;
l_int    hides = 1;
PMouse  Mouse = 0;
TPoint   OldPos = { 0, 0 };
l_int    OldZ=0;


PList Cursors = NULL;


/**
        Changes the mouse speed
        Larger value of Speed reprents slower mouse mouvement
        At startup, value read from registry (/SYSTEM/MOUSE/speed)
        @param Speed Speed, default : 0
*/
void MouseSetCursorSpeed ( l_int Speed ){
        Mouse->Speed = Speed;

        set_mouse_speed(Speed, Speed);
}

/**
        Changes the mouse double click speed
        Set the maximal time between 2 click to be interpreted as a double click (in deciseconds)
        At startup, value read from registry (/SYSTEM/MOUSE/dblclk)
        @param Speed Maximal time between 2 click (in deciseconds), default : 4
*/
void MouseSetDblClkSpeed ( l_int Speed ){
        Mouse->DblClkSpeed = Speed;
}

void MouseDrawCursor ( l_bool Del, l_bool Add ){
        l_int cf = screen->clip;
        l_int cl = screen->cl;
        l_int cr = screen->cr;
        l_int ct = screen->ct;
        l_int cb = screen->cb;

        screen->clip = TRUE;
        screen->cl = screen->ct = 0;
        screen->cr = screen->w;
        screen->cb = screen->h;

        PauseMultitasking();

        if ( Del ){
                blit(Mouse->PrevScr,screen,0,0,OldPos.x-Mouse->Cur->SpotX,OldPos.y-Mouse->Cur->SpotY,Mouse->Cur->Image->w,Mouse->Cur->Image->h);
        }

        if ( Add ){
                blit(screen,Mouse->PrevScr,Mouse->State.p.x-Mouse->Cur->SpotX,Mouse->State.p.y-Mouse->Cur->SpotY,0,0,Mouse->Cur->Image->w,Mouse->Cur->Image->h);
                draw_sprite(screen, Mouse->Cur->Image, Mouse->State.p.x-Mouse->Cur->SpotX,Mouse->State.p.y-Mouse->Cur->SpotY);
        }

        OldPos = Mouse->State.p;

        UnPauseMultitasking();

        screen->clip = cf;
        screen->cl = cl; screen->cr = cr;
        screen->ct = ct; screen->cb = cb;
}

void _MouseRecap ( p_bitmap from, TRect r ){
        if ( Mouse->Visible )
                /*if ( _RectOverlay (Mouse->CursorPosition,r ) )*/ {
                        TRect i;
                        l_int cf = from->clip;
                        l_int cl = from->cl;
                        l_int cr = from->cr;
                        l_int ct = from->ct;
                        l_int cb = from->cb;

                        _RectIntersept(&i,r,Mouse->CursorPosition);

                        from->clip = TRUE;

                        from->cl = max(i.a.x,0);
                        from->ct = max(i.a.y,0);

                        from->cr = min(i.b.x+1,screen->w);
                        from->cb = min(i.b.y+1,screen->h);

                        blit( from, Mouse->PrevScr,i.a.x, i.a.y,i.a.x-Mouse->CursorPosition.a.x,i.a.y-Mouse->CursorPosition.a.y,i.b.x-i.a.x+1,i.b.y-i.a.y+1 );
                        draw_sprite ( from, Mouse->Cur->Image, Mouse->State.p.x-Mouse->Cur->SpotX,Mouse->State.p.y-Mouse->Cur->SpotY);

                        from->clip = cf;
                        from->cl = cl;
                        from->cr = cr;
                        from->ct = ct;
                        from->cb = cb;
                }
}

/**
        Changes the cursor to the specified one.
        @param Cur Pointer to the cursor to use
*/
void MouseSetCursor ( PCursor Cur ){
        if ( Mouse->Visible && Mouse->Cur && Mouse->PrevScr )
                MouseDrawCursor(true,false);

        if ( Mouse->PrevScr ) destroy_bitmap(Mouse->PrevScr);

        Mouse->PrevScr = create_bitmap(Cur->Image->w+2,Cur->Image->h+2);
        Mouse->Cur = Cur;

        if ( Mouse->Visible ) MouseDrawCursor(false,true);
}

/**
        Changes the cursor to the specified one.
        @param Name Name of the pointer to use
*/
void MouseSetCursorNamed ( l_text Name ) {
        PCursor c = ListKey(Cursors,Name);
        if ( c ) MouseSetCursor(c);
}

/**
        Makes the mouse cursor visible
*/
void MouseShow ( void ){
        if ( !Mouse->Visible ){
                Mouse->Visible = true;

                if ( Mouse->Cur ) MouseDrawCursor(false, true);
        }
}

/**
        Hides the mouse cursor
*/
void MouseHide ( void ){
        if ( Mouse->Visible ){
                Mouse->Visible = false;

                if ( Mouse->Cur ) MouseDrawCursor(true,false);
        }
}

/**
        Allocates a new cursor form a bitmap
        @param Img Bitmap of the cursor
        @param SpotX x coordinates in Img of spot point
        @param SpotY y coordinates in Img of spot point
        @see FreeCursor
        @see MouseSetCursor
*/
PCursor NewCursor ( p_bitmap Img, l_int SpotX, l_int SpotY ){
        PCursor c = malloc(sizeof(TCursor));

        if ( !c ) return NULL;

        c->Image = Img;
        c->SpotX = SpotX;
        c->SpotY = SpotY;

        return c;
}

/**
        Free a cursor
        @param c Cursor to free
        @see NewCursor
*/
void FreeCursor ( PCursor c ){
        if ( c->Image ) destroy_bitmap(c->Image);
        free(c);
}

void MouseEventGenerator ( PEvent Event ){
  Event->Type = EV_NOTHING;
  Mouse->Moved = FALSE;

  Mouse->ButtonChanges  = 0;

  Mouse->State.z = mouse_z;

  if ( OldZ != Mouse->State.z ) {

    Event->Type = EV_MOUSE;



          if ( OldZ-Mouse->State.z < 0 )
                        Event->Message = MSG_MOUSE_SCLLUP;
          else
                Event->Message = MSG_MOUSE_SCLLDWN;

          OldZ = Mouse->State.z;
  }

  if ( mouse_x != Mouse->State.p.x || mouse_y != Mouse->State.p.y )
  {

    Event->Type = EV_MOUSE;
    Mouse->Moved = TRUE;

    Mouse->State.p.x = mouse_x;
    Mouse->State.p.y = mouse_y;

    #define CursorSize(r,x,y,w,h) RectAssign(r,x,y,x+w,y+h)

    if ( Mouse->Visible ) {
        CursorSize(&Mouse->CursorPosition,Mouse->State.p.x-Mouse->Cur->SpotX,Mouse->State.p.y-Mouse->Cur->SpotY,Mouse->Cur->Image->w-1,Mouse->Cur->Image->h-1);
        MouseDrawCursor(true,true);
    } else
        RectAssign(&Mouse->CursorPosition,-1,-1,-1,-1);

  }

  Mouse->State.b = mouse_b;

  if ( Mouse->ButtonChanges = Mouse->State.b ^ old_button )
  {

    Event->Type = EV_MOUSE;

    old_button = Mouse->State.b;
  }
}

_PUBLIC void  GSSystemMouseInstall()
{
        Cursors = NewList();

        Buttons = install_mouse();


        if ( Buttons > 0 )
        {
                PRegKey o;

                o =     ResolveKey("/SYSTEM/MOUSE/CURSORS");
                if ( !o ) {
                        DebugMessage("No cursor declared in registry. Mouse can't start");
                        return;
                }

                if ( o->Last ) {
                        PRegKey a = o->Last->Next;
                        PRegKey b = a, i;
                        do {
                                if ( a->Type == RKT_TEXT ) {
                                        PCursor c;
                                        PImage Img;
                                        l_ulong sx = 0, sy = 0;

                                        if ( a->Last ) {
                                                i = a->Last;
                                                do {
                                                        if ( !TextCaseCompare(i->Name,"sy" ) && ( i->Type == RKT_INT ) )
                                                                sy = *((l_int*)i->Data);
                                                        else if ( !TextCaseCompare(i->Name,"sx" ) && ( i->Type == RKT_INT ) )
                                                                sx = *((l_int*)i->Data);
                                                        i = i->Next;
                                                } while ( i != a->Last );
                                        }

                                        Img = LoadImage((l_text)a->Data);

                                        if ( Img ) {

                                                c = NewCursor(Img,sx,sy);

                                                if ( c )
                                                        ListAdd(Cursors,a->Name,c,(void*)&FreeCursor);
                                                else
                                                        destroy_bitmap(Img);
                                        }
//                                        else {
//                                            DebugMessage("No cursor declared in registry. Mouse can't start");
//  good idea??                               return;
//                                        }
                                }
                                a = a->Next;
                        } while ( a != b );
                }

                Mouse = NEW(TMouse);
                CLEAR(Mouse);

                Mouse->State.p.x = mouse_x;
                Mouse->State.p.y = mouse_y;
                Mouse->State.z = mouse_z;
                Mouse->State.b = mouse_b;

                cursor_pointer = ListKey(Cursors,"arrow");
                cursor_resize  = ListKey(Cursors,"resize");

                if ( ! cursor_pointer ) {
                        DebugMessage("Arrow not set, mouse can't start");
                        return;
                }

                MouseSetCursorSpeed(KeyGetInt("/SYSTEM/MOUSE/speed", 0));
                MouseSetDblClkSpeed(KeyGetInt("/SYSTEM/MOUSE/dblclk", 4));
                MouseSetCursor(CUR_POINTER);

                InstallEventGenerator(MouseEventGenerator);
        }
        else
        {
                DebugError("Mouse not found");
        }
}

_PUBLIC void  GSSystemMouseUninstall()
{
        if ( Buttons > 0 )
        {

                MouseHide();
                Mouse->Cur = NULL;
                free(Mouse);
                Mouse = NULL;

                remove_mouse();
        }

        if ( Cursors ) FreeList(Cursors);

}

/**
        Removes temporary mouse support by kernel, and allow extra software to uses it directly
        Never uses it in multitask mode.
        @see __MouseRestart
*/
l_bool  __MouseStop ( void ) {

        if ( Buttons > 0 )
        {
                MouseHide();
                remove_mouse();
        }
        return true;
}

/**
        Restores mouse after a __MouseStop call
        Do not show mouse, use MouseShow(); after it.
        @see MouseShow
        @see __MouseStop
*/
l_bool  __MouseRestart ( void ) {

        Buttons = install_mouse();

        return true;
}

/**
@}
*/
