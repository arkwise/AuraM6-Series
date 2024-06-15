/**
*	oZone example
*		(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
* Flamme algorithm by Kostas Michalopoulos aka Bad Sector
* 	Copyright (C) 2001,2002,2003 Kostas Michalopoulos
*
* Licenses
*   Algorithm : GNU General Public License (TimerPoll,NormalFlame,ColdFlame,GreenFlame)
*   Software  : Public domain
*
**/
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "label.h"
#include "button.h"
#include "canvas.h"
////////////////////////////////////////////////////////////////////////////////
l_ulong AppVersion = ULONG_ID(0,1,0,0);
char    AppName[] = "Sample Application";
l_uid	nUID = "app:sample";
l_uid NeededLibs[] = { "widget", "button","label","window","" };
////////////////////////////////////////////////////////////////////////////////
#define FWIDTH  320
#define FHEIGHT 220
////////////////////////////////////////////////////////////////////////////////
l_color	Col[256]; // Color palette
l_uchar	Scr[FWIDTH][FHEIGHT+2]; // was 160,100
l_int	cnt = 0;
PCanvas	c 	= 0;
PLabel l = 0;
PTimer t = 0;
l_ulong nb = 0;
l_ubig ltime = 0;
PWindow w = 0;
////////////////////////////////////////////////////////////////////////////////
#define MSG_COLRED		0x20101
#define MSG_COLBLUE		0x20102
#define MSG_COLGREEN	0x20103
#define MSG_REDRAW		0x20001
////////////////////////////////////////////////////////////////////////////////
void TimerPoll( void *A ) {
	l_int  i, j;
	for (i=1;i<FWIDTH;i++) Scr[i][FHEIGHT] = rand() % 255;
	for (j=1;j<FHEIGHT;j++)
		for (i=1;i<FWIDTH-2;i++) {
			int c = (Scr[i-1][j-1]+Scr[i+1][j-1]+Scr[i-1][j+1]+Scr[i+1][j+1])/4;
			if (c < 0) c = 0;
			if (cnt == 0) Scr[i-1][j-1] = c;
			if (cnt == 1) Scr[i][j-1] = c; else
			if (cnt == 2) Scr[i+1][j-1] = c; else
			if (cnt == 3) Scr[i][j-1] = c;
			Scr[i][j] = c;
		}
	cnt++;
	if (cnt == 4) cnt = 0;
	nb++;
	if ( ATime-ltime >= 1000 ) {
		ltime = ATime;
		LabelSetText(l,"%d FPS",nb);
		nb = 0;
	}
	WidgetDraw(WIDGET(c), NULL);
}
////////////////////////////////////////////////////////////////////////////////
void NormalFlame(void) {
	l_int  i, r;
	for (i=0;i<64;i++)
		Col[i] = RGB(i*3, 0, 0);
	r = 192;
	for (i=64;i<128;i++) {
		Col[i] = RGB(r, (i-64)*4, 0);
		if (r < 255) r++;
	}
	for (i=128;i<255;i++)
		Col[i] = RGB(255, 255, i-128);
}
////////////////////////////////////////////////////////////////////////////////
void ColdFlame(void) {
	l_int  i, r;
	for (i=0;i<64;i++)
		Col[i] = RGB(0, 0, i*3);
	r = 192;
	for (i=64;i<128;i++) {
		Col[i] = RGB(0, (i-64)*4, r);
		if (r < 255) r++;
	}
	for (i=128;i<255;i++)
		Col[i] = RGB(i-128, 255, 255);
}
////////////////////////////////////////////////////////////////////////////////
void GreenFlame(void) {
	l_int  i, r;
	for (i=0;i<64;i++)
		Col[i] = RGB(0, i*3, 0);
	r = 192;
	for (i=64;i<128;i++) {
		Col[i] = RGB((i-64)*4, r, 0);
		if (r < 255) r++;
	}
	for (i=128;i<255;i++)
		Col[i] = RGB(255, 255, i-128);
}
////////////////////////////////////////////////////////////////////////////////
void FlameDraw (PWidget o, p_bitmap buffer, PRect w) {
	TPoint p;
	l_int x, y;
	for (y=w->a.y-o->Absolute.a.y;y<w->b.y-o->Absolute.a.y+1;y++) 
		for (x=w->a.x-o->Absolute.a.x;x<w->b.x-o->Absolute.a.x+1;x++)
		{
			l_int c = Scr[x+1][y+1];
			p.x = o->Absolute.a.x+x;
			p.y = o->Absolute.a.y+y;
			if ( _PointInRect( p,Mouse->CursorPosition) ) {
				if ( getpixel(Mouse->Cur->Image,p.x-Mouse->CursorPosition.a.x,p.y-Mouse->CursorPosition.a.y) == Mouse->Cur->Image->vtable->mask_color )
					putpixel(buffer, p.x, p.y, Col[c]);
			} else
				putpixel(buffer, p.x, p.y, Col[c]);
		}
}
////////////////////////////////////////////////////////////////////////////////
l_bool AppEventHandler ( PWidget o, PEvent Ev ) {
	if ( Ev->Type == EV_MESSAGE ) {
		if ( Ev->Message == WM_CLOSE ) {
			CloseApp(&Me);
			return true;
		}
		if ( Ev->Message == MSG_COLRED ) NormalFlame();
		if ( Ev->Message == MSG_COLBLUE ) ColdFlame();
		if ( Ev->Message == MSG_COLGREEN ) GreenFlame();
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_int Main( int argc, l_text *argv ) {
	TRect r;
	PButton b = NULL;
	
	RectAssign(&r,0, 0, FWIDTH+110, FHEIGHT);
	w = CreateWindow( &Me, r, "oZone example : Fire Effect", WF_FRAME|WF_CAPTION|WF_MINIMIZE|WF_CENTERED );
	InsertWidget(DeskTop, WIDGET(w));
	WIDGET(w)->AppEvHdl = &AppEventHandler;

	WidgetSize(&r,FWIDTH+5, 0, 100, 15);
	l = CreateLabel(&Me, r,"Color");
	InsertWidget(WIDGET(w), WIDGET(l));
	
	WidgetSize(&r,FWIDTH+15, r.b.y+2, 90, 20);
	b = CreateButton(&Me, r,"Normal",MSG_COLRED);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,FWIDTH+15, r.b.y+2, 90, 20);
	b = CreateButton(&Me, r,"Blue",MSG_COLBLUE);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,FWIDTH+15, r.b.y+2, 90, 20);
	b = CreateButton(&Me, r,"Green",MSG_COLGREEN);
	InsertWidget(WIDGET(w), WIDGET(b));
	
	WidgetSize(&r,FWIDTH+5, r.b.y+12, 100, 15);
	l = CreateLabel(&Me, r,"Statistics");
	InsertWidget(WIDGET(w), WIDGET(l));

	WidgetSize(&r,FWIDTH+15, r.b.y+2, 90, 15);
	l = CreateLabel(&Me, r,"");
	InsertWidget(WIDGET(w), WIDGET(l));

	NormalFlame();

	WidgetSize(&r,2, 1, FWIDTH-3, FHEIGHT-4);
	c = CreateCanvas(&Me, r);
	WIDGET(c)->Draw = &FlameDraw;
	WIDGET(c)->Flags |= WFForceNonBuffer;
	InsertWidget(WIDGET(w), WIDGET(c));
	
	WidgetDrawAll(WIDGET(w));

	t = NewTimer(&Me,0, &TimerPoll,NULL);
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close( void ) {
	WidgetDispose(WIDGET(w));
	KillTimer(t);
}
////////////////////////////////////////////////////////////////////////////////
