#include "kernel.h"
#include "widget.h"
#include "skin.h"
#include "prevscr.h"

l_ulong AppVersion = ULONG_ID(0,0,1,0);
char    AppName[] = "Screen preview";
l_uid	nUID = "prevsrc";
l_uid NeededLibs[] = { "widget","skin","" };

p_bitmap ScrImg = NULL;

PWidgetClass PrevScrClass;

void  PrevScrDraw (PWidget o, p_bitmap buffer, PRect w) {
	rectfill(buffer, w->a.x, w->a.y, w->b.x, w->b.y, o->BackgroundColor);
	draw_sprite(buffer,ScrImg,o->Absolute.a.x,o->Absolute.a.y);
	if ( PREVSCR(o)->Scr )
		draw_sprite(buffer,PREVSCR(o)->Scr,o->Absolute.a.x+14,o->Absolute.a.y+16);

}

void  PrevScrSetScr ( PPrevScr o, p_bitmap Scr ) {
	o->Scr = Scr;	
	WidgetDraw(WIDGET(o),NULL);
}


PPrevScr  CreatePrevScr ( PApplication App, TRect r, p_bitmap Scr )
{
	PPrevScr o = malloc(sizeof(TPrevScr));
	if (!o) return NULL;
	memset(o, 0, sizeof(TPrevScr));
	o->Scr = Scr;
	IntialiseWidget(App, PrevScrClass, WIDGET(o), r );
	WIDGET(o)->BackgroundColor = COL_3DFACE;
	WIDGET(o)->Flags |= WFForceBuffer;
	WIDGET(o)->Draw = &PrevScrDraw;
	return o;
}

PWidget  StdCreatePrevScr ( PApplication App, TRect r )
{
	return WIDGET(CreatePrevScr(App,r,NULL));
}

l_bool LibMain ( int argc, l_text *argv )
{
	
	PrevScrClass = CreateWidgetClass( ULONG_ID('P','S','c','r'), StdWidgetClass, 0 );
	PrevScrClass->StdCreate = &StdCreatePrevScr;

	
	if ( Me.Ressource ) {
		ScrImg = ListKey(Me.Ressource,"SCREEN");
	}

	APPEXPORT(CreatePrevScr);
	APPEXPORT(PrevScrSetScr);
	
	return true;
}

void Close (void)
{

}

