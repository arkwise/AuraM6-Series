#include "kernel.h"
#include "canvas.h"
#include "button.h"
#include "window.h"
#include "timer.h"
#include "menu.h"
#include "label.h"

l_uid 	nUID 		= "sandbox";
l_ulong AppVersion	= ULONG_ID(0,0,1,0);
char AppName[]		= "SandBox";
l_uid NeededLibs[]	= { "widget","window","menu","canvas","" };

PWindow w = 0;
PCanvas Surface = 0;

PList lst;
PList arround;

TRect A;
TRect B;

void SurfaceDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	TRect r;
	PListItem a,b;
	rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_BOXFACE);
	
	a = b = lst->Last;
	if ( a ) do {
		r = *((PRect)a->Data);

		/*if ( (A.a.y == r.b.y+1) || (A.b.y == r.a.y-1) ) {
			
			rectfill(buffer, o->Absolute.a.x+r.a.x, o->Absolute.a.y+r.a.y, o->Absolute.a.x+r.b.x, o->Absolute.a.y+r.b.y, makecol(255,255,0));
			rect(buffer, o->Absolute.a.x+r.a.x, o->Absolute.a.y+r.a.y, o->Absolute.a.x+r.b.x, o->Absolute.a.y+r.b.y, makecol(255,255,128));
		
		} else */if ( (B.a.x == r.b.x+1) || (B.b.x == r.a.x-1) ) {
			
			rectfill(buffer, o->Absolute.a.x+r.a.x, o->Absolute.a.y+r.a.y, o->Absolute.a.x+r.b.x, o->Absolute.a.y+r.b.y, makecol(255,0,0));
			rect(buffer, o->Absolute.a.x+r.a.x, o->Absolute.a.y+r.a.y, o->Absolute.a.x+r.b.x, o->Absolute.a.y+r.b.y, makecol(255,128,128));
		
		} else {
			rectfill(buffer, o->Absolute.a.x+r.a.x, o->Absolute.a.y+r.a.y, o->Absolute.a.x+r.b.x, o->Absolute.a.y+r.b.y, makecol(0,0,255));
			rect(buffer, o->Absolute.a.x+r.a.x, o->Absolute.a.y+r.a.y, o->Absolute.a.x+r.b.x, o->Absolute.a.y+r.b.y, makecol(128,128,255));
		}
		
		a = a->Next;
	} while ( a != b );
	
	a = b = arround->Last;
	if ( a ) do {
		r = *((PRect)a->Data);

		rect(buffer, o->Absolute.a.x+r.a.x, o->Absolute.a.y+r.a.y, o->Absolute.a.x+r.b.x, o->Absolute.a.y+r.b.y, makecol(128,128,128));
		a = a->Next;
	} while ( a != b );
}
void OptimiseRectList ( PList l ) {
	TRect r, g;
	PListItem a,c,n,m;
	a = l->Last;
	if ( a ) do {
		c = l->Last;
		do {
			g = *((PRect)c->Data);
			r = *((PRect)a->Data);
			n = c->Next;
			m = NULL;
			if ( c != a ) {
				if ( g.b.x == r.a.x-1 && r.a.y == g.a.y && r.b.y == g.b.y ) {
					((PRect)a->Data)->a.x = g.a.x;
					ListRemoveItem(l,c);
					n = m = l->Last;
				} else if ( r.b.x == g.a.x-1 && r.a.y == g.a.y && r.b.y == g.b.y ) {
					((PRect)a->Data)->b.x = g.b.x;
					ListRemoveItem(l,c);
					n = m = l->Last;
				} else if ( r.b.y == g.a.y-1 && r.a.x == g.a.x && r.b.x == g.b.x ) {
					((PRect)a->Data)->b.y = g.b.y;
					ListRemoveItem(l,c);
					n = m = l->Last;
				} else if ( g.b.y == r.a.y-1 && r.a.x == g.a.x && r.b.x == g.b.x ) {
					((PRect)a->Data)->a.y = g.a.y;
					ListRemoveItem(l,c);
					n = m = l->Last;
				}	
			}
			c = n;
		} while ( c != l->Last || m );
		a = a->Next;
	} while ( a != l->Last );
}

INLINE void _AddRectInList ( PList List, int ax, int ay, int bx, int by )
{
	if ( ax <= bx && ay <= by )
	{
		PRect r = (PRect)malloc(sizeof(TRect));
		r->a.x = ax; r->a.y = ay;
		r->b.x = bx; r->b.y = by;
		ListAdd(List,NULL,r,free);
	}
}

l_int Main ( int argc, l_text *argv )
{
	TRect r;
	
	RectAssign(&r,0, 0, 300, 300);
	w = CreateWindow(&Me, r, "Sand Box : testing app", WF_CAPTION|WF_FRAME|WF_CENTERED|WF_MINIMIZE);
	InsertWidget(WIDGET(DeskTop), WIDGET(w));
	Surface = CreateCanvas(&Me, r);
	WIDGET(Surface)->Draw = &SurfaceDraw;
	WIDGET(Surface)->Flags |= WFForceBuffer;
	InsertWidget(WIDGET(w), WIDGET(Surface));

	lst = NewList();
	arround = NewList();
	RectAssign(&r,0, 0, 300, 300);
	AddZoneInListEx ( lst, r.a.x, r.a.y, r.b.x, r.b.y );
	
	//_AddRectInList ( arround, r.a.x, r.a.y, r.b.x, r.b.y );

	RectAssign(&r, 150, 20, 155, 150);
	RemoveZoneInList(lst,r);
	_AddRectInList ( arround, r.a.x, r.a.y, r.b.x, r.b.y );
	
	B = r;
	
	RectAssign(&r, 20, 150, 280, 155);
	RemoveZoneInListV(lst,r);
	_AddRectInList ( arround, r.a.x, r.a.y, r.b.x, r.b.y );
	
	A = r;
	
	WidgetDrawAll(WIDGET(w));

	
	return true;
}

void Close (void)
{
}
