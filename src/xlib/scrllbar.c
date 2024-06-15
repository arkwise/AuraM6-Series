////////////////////////////////////////////////////////////////////////////////
//
//  Scrollbars - Core File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "scrllbar.h"

l_ulong	AppVersion = ULONG_ID(0,1,0,0);
l_char	AppName[] = "Scrollbar Widget";
l_uid	nUID = "scrllbar";
l_uid NeededLibs[] = { "widget", "skin", "" };

PWidgetClass ScollWidgetClass;
PWidgetClass ScrollbarClass;

PSkin SkinVScrollbarUpUp		= 0;
PSkin SkinVScrollbarUpDown		= 0;
PSkin SkinVScrollbarDownUp		= 0;
PSkin SkinVScrollbarDownDown	= 0;
PSkin SkinVScrollbarBackground	= 0;
PSkin SkinVScrollbarTracker		= 0;

PSkin SkinHScrollbarLeftUp		= 0;
PSkin SkinHScrollbarLeftDown	= 0;
PSkin SkinHScrollbarRightUp		= 0;
PSkin SkinHScrollbarRightDown	= 0;
PSkin SkinHScrollbarBackground	= 0;
PSkin SkinHScrollbarTracker		= 0;

#define MINIMAL_BAR_SIZE 10

#define SIZEWIDTH(o) (o->Relative.b.x-o->Relative.a.x-(SkinHScrollbarLeftUp->Skin->w+SkinHScrollbarRightUp->Skin->w))
#define SIZEHEIGHT(o) (o->Relative.b.y-o->Relative.a.y-(SkinVScrollbarUpUp->Skin->h+SkinVScrollbarDownUp->Skin->h))

#define SIZEWIDTHNOSKIN(o) (o->Relative.b.x-o->Relative.a.x-30)
#define SIZEHEIGHTNOSKIN(o) (o->Relative.b.y-o->Relative.a.y-30)

/*
#define SIZEWIDTH(o) SIZEWIDTH(o)
#define SIZEHEIGHT(o) SIZEHEIGHT(o)
*/
////////////////////////////////////////////////////////////////////////////////
l_ulong GetBarSize ( l_ulong Max, l_ulong Size ) {
	return max(MINIMAL_BAR_SIZE, Size*Size/(Size+1+Max));
}
////////////////////////////////////////////////////////////////////////////////
l_ulong GetBaRPos ( l_ulong Max, l_ulong Size, l_ulong Pos ) {
  return (Size-GetBarSize(Max,Size))*Pos/(1+Max);
}
////////////////////////////////////////////////////////////////////////////////
l_ulong CalcPos ( l_ulong Max, l_ulong Size, l_long PosPx ) {
  return min(Max, (1+Max)*max(0,PosPx)/(Size-GetBarSize(Max,Size)));
}
////////////////////////////////////////////////////////////////////////////////
void NotifyScroll ( PScrollWidget i ) {
	if ( i->Pos.y != i->OldPos.y || i->Pos.x != i->OldPos.x ) {
	  if ( i->Scrolled )
	    i->Scrolled(i,i->Pos);
  }
	i->OldPos = i->Pos;
}
////////////////////////////////////////////////////////////////////////////////
void VerticalScrollbarDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	PScrollWidget i = SLLBAR(o)->Scll;
	TRect r;

	

	if ( i->RPos.y < 0 ) i->RPos.y = 0;

	if ( UseSkins ) {
		l_ulong size = GetBarSize( i->Max.y, o->Absolute.b.y-o->Absolute.a.y-(SkinVScrollbarUpUp->Skin->h+SkinVScrollbarDownUp->Skin->h));
		if ( i->RPos.y > SIZEHEIGHT(o)-size ) i->RPos.y = o->Absolute.b.y-o->Absolute.a.y-size-(SkinVScrollbarUpUp->Skin->h+SkinVScrollbarDownUp->Skin->h);
	  DrawSkin(buffer, SkinVScrollbarBackground, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
	  RectAssign(&r,o->Absolute.a.x,o->Absolute.a.y+SkinVScrollbarUpUp->Skin->h+i->RPos.y,o->Absolute.b.x,o->Absolute.a.y+SkinVScrollbarUpUp->Skin->h+i->RPos.y+size);
		SLLBAR(o)->BarCache=r;
		DrawSkin(buffer, SkinVScrollbarTracker, r.a.x, r.a.y, r.b.x, r.b.y+1);
		if ( SLLBAR(o)->BtnState == 1 )
			DrawSkin(buffer, SkinVScrollbarUpDown, o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.a.y+SkinVScrollbarUpDown->Skin->h-1);
		else
			DrawSkin(buffer, SkinVScrollbarUpUp, o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.a.y+SkinVScrollbarUpUp->Skin->h-1);
		if ( SLLBAR(o)->BtnState == 2 )
			DrawSkin(buffer, SkinVScrollbarDownDown, o->Absolute.a.x,o->Absolute.b.y-SkinVScrollbarDownDown->Skin->h+1,o->Absolute.b.x,o->Absolute.b.y);
		else
			DrawSkin(buffer, SkinVScrollbarDownUp, o->Absolute.a.x,o->Absolute.b.y-SkinVScrollbarDownUp->Skin->h+1,o->Absolute.b.x,o->Absolute.b.y);
	} else {
		l_ulong size = GetBarSize( i->Max.y, o->Absolute.b.y-o->Absolute.a.y-30);
		if ( i->RPos.y > SIZEHEIGHTNOSKIN(o)-size ) i->RPos.y = o->Absolute.b.y-o->Absolute.a.y-size-30;
	  RectAssign(&r,o->Absolute.a.x,o->Absolute.a.y+15+i->RPos.y,o->Absolute.b.x,o->Absolute.a.y+15+i->RPos.y+size);
		SLLBAR(o)->BarCache=r;
		rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,COL_3DFACE);
		Rect3D(buffer,r.a.x,r.a.y,r.b.x,r.b.y+1,COL_3DLIGHT,COL_3DDARK);
		if ( SLLBAR(o)->BtnState == 1 ) {
			Rect3D(buffer,o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.a.y+14,COL_3DDARK,COL_3DLIGHT);
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2+1,o->Absolute.a.y+6,(o->Absolute.a.x+o->Absolute.b.x)/2+1,COL_3DTEXT);
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2,o->Absolute.a.y+7,(o->Absolute.a.x+o->Absolute.b.x)/2+2,COL_3DTEXT);	
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2-1,o->Absolute.a.y+8,(o->Absolute.a.x+o->Absolute.b.x)/2+3,COL_3DTEXT);
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2-2,o->Absolute.a.y+9,(o->Absolute.a.x+o->Absolute.b.x)/2+4,COL_3DTEXT);	
		} else {
			Rect3D(buffer,o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.a.y+14,COL_3DLIGHT,COL_3DDARK);
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2,o->Absolute.a.y+5,(o->Absolute.a.x+o->Absolute.b.x)/2,COL_3DTEXT);
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2-1,o->Absolute.a.y+6,(o->Absolute.a.x+o->Absolute.b.x)/2+1,COL_3DTEXT);	
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2-2,o->Absolute.a.y+7,(o->Absolute.a.x+o->Absolute.b.x)/2+2,COL_3DTEXT);
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2-3,o->Absolute.a.y+8,(o->Absolute.a.x+o->Absolute.b.x)/2+3,COL_3DTEXT);	
		}
		if ( SLLBAR(o)->BtnState == 2 ) {
			Rect3D(buffer,o->Absolute.a.x,o->Absolute.b.y-14,o->Absolute.b.x,o->Absolute.b.y,COL_3DDARK,COL_3DLIGHT);
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2+1,o->Absolute.b.y-4,(o->Absolute.a.x+o->Absolute.b.x)/2+1,COL_3DTEXT);
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2,o->Absolute.b.y-5,(o->Absolute.a.x+o->Absolute.b.x)/2+2,COL_3DTEXT);	
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2-1,o->Absolute.b.y-6,(o->Absolute.a.x+o->Absolute.b.x)/2+3,COL_3DTEXT);
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2-2,o->Absolute.b.y-7,(o->Absolute.a.x+o->Absolute.b.x)/2+4,COL_3DTEXT);	
		} else {
			Rect3D(buffer,o->Absolute.a.x,o->Absolute.b.y-14,o->Absolute.b.x,o->Absolute.b.y,COL_3DLIGHT,COL_3DDARK);
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2,o->Absolute.b.y-5,(o->Absolute.a.x+o->Absolute.b.x)/2,COL_3DTEXT);
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2-1,o->Absolute.b.y-6,(o->Absolute.a.x+o->Absolute.b.x)/2+1,COL_3DTEXT);	
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2-2,o->Absolute.b.y-7,(o->Absolute.a.x+o->Absolute.b.x)/2+2,COL_3DTEXT);
			hline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2-3,o->Absolute.b.y-8,(o->Absolute.a.x+o->Absolute.b.x)/2+3,COL_3DTEXT);			
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
l_bool VerticalScrollbarEventHandler ( PWidget o, PEvent Ev ) {

	if (Ev->Type == EV_MOUSE )
	{
		PScrollWidget i = SLLBAR(o)->Scll;
		l_ulong SizeHeight = SIZEHEIGHTNOSKIN(o);
		l_ulong ButtonHeight = 15;

		if ( UseSkins ) {
			SizeHeight = SIZEHEIGHT(o);
			ButtonHeight = SkinVScrollbarUpUp->Skin->h;
		}


		if ( !i->Max.y ) return true;

		if ( Ev->Message == WEvMouseLDown || Ev->Message == WEvMouseLDClk )
		{
			MasterWidget = o;

			if ( CURSOR_IN_RECT( SLLBAR(o)->BarCache ) )
			{
				SLLBAR(o)->W = Mouse->State.p.y-SLLBAR(o)->BarCache.a.y;
			}
			else if ( Mouse->State.p.y < o->Absolute.a.y+ButtonHeight )
			{
				SLLBAR(o)->BtnState = 1;

				TRect r;
				RectAssign(&r,o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.a.y+ButtonHeight-1);
				WidgetDraw(o, &r);
			}
			else if ( Mouse->State.p.y > o->Absolute.b.y-ButtonHeight )
			{
				SLLBAR(o)->BtnState = 2;

				TRect r;
				RectAssign(&r,o->Absolute.a.x,o->Absolute.b.y-ButtonHeight+1,o->Absolute.b.x,o->Absolute.b.y);
				WidgetDraw(o, &r);
			}
		}
		else if ( Ev->Message == WEvMouseMove && Mouse->State.b & BUTTON_LEFT && !SLLBAR(o)->BtnState )
		{
			i->Pos.y = CalcPos(i->Max.y,SizeHeight,Mouse->State.p.y-SLLBAR(o)->W-o->Absolute.a.y-ButtonHeight);
			i->RPos.y = Mouse->State.p.y-SLLBAR(o)->W-o->Absolute.a.y-ButtonHeight;
			WidgetDraw(o, NULL);
			NotifyScroll(i);


			
		}
		else if ( Ev->Message == WEvMouseLUp )
		{
			MasterWidget = NULL;

			if ( SLLBAR(o)->BtnState == 1 )
			{
			  SLLBAR(o)->BtnState = 0;
			  i->Pos.y = max(0,i->Pos.y-i->Delta.y);
			  i->RPos.y = GetBaRPos( i->Max.y, SizeHeight,i->Pos.y);
			  WidgetDraw(o,NULL);
			  NotifyScroll(i);
			}
			else if ( SLLBAR(o)->BtnState == 2 )
			{
				SLLBAR(o)->BtnState = 0;
				i->Pos.y = min(i->Max.y,i->Pos.y+i->Delta.y);
				i->RPos.y = GetBaRPos( i->Max.y, SizeHeight,i->Pos.y);
				WidgetDraw(o,NULL);
				NotifyScroll(i);
			}
			else if ( !CURSOR_IN_RECT( SLLBAR(o)->BarCache ) )
			{
				i->Pos.y = CalcPos(i->Max.y,SizeHeight,Mouse->State.p.y-o->Absolute.a.y-ButtonHeight);
				i->RPos.y = Mouse->State.p.y-o->Absolute.a.y-ButtonHeight;
				WidgetDraw(o,NULL);
				NotifyScroll(i);
	
			}
		}
		return true;
	}
	if ( Ev->Type == EV_MESSAGE )
		if (  Ev->Message == MSG_NOTIFY_AUTO_RESIZE ) {
			PScrollWidget i = SLLBAR(o)->Scll;
			i->RPos.y = GetBaRPos( i->Max.y, UseSkins?SIZEHEIGHT(o):SIZEHEIGHTNOSKIN(o),i->Pos.y);
			return true;
		}

	return false;
}
////////////////////////////////////////////////////////////////////////////////
void HorizontalScrollbarDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	PScrollWidget i = SLLBAR(o)->Scll;
	TRect r;
	if ( i->RPos.x < 0 ) i->RPos.x = 0;


	if ( UseSkins ) {
		l_ulong size = GetBarSize( i->Max.x, SIZEWIDTH(o));
		if ( i->RPos.x > SIZEWIDTH(o)-size ) i->RPos.x = SIZEWIDTH(o)-size;
		DrawSkin(buffer, SkinHScrollbarBackground, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
		RectAssign(&r,o->Absolute.a.x+SkinHScrollbarLeftUp->Skin->w+i->RPos.x,o->Absolute.a.y,o->Absolute.a.x+SkinHScrollbarLeftUp->Skin->w+i->RPos.x+size,o->Absolute.b.y);
		SLLBAR(o)->BarCache = r;
		DrawSkin(buffer, SkinHScrollbarTracker, r.a.x, r.a.y, r.b.x+1, r.b.y);

		if ( SLLBAR(o)->BtnState == 1 )
			DrawSkin(buffer, SkinHScrollbarLeftDown, o->Absolute.a.x,o->Absolute.a.y,o->Absolute.a.x+SkinHScrollbarLeftDown->Skin->w-1,o->Absolute.b.y);
		else
			DrawSkin(buffer, SkinHScrollbarLeftUp, o->Absolute.a.x,o->Absolute.a.y,o->Absolute.a.x+SkinHScrollbarLeftUp->Skin->w-1,o->Absolute.b.y);

		if ( SLLBAR(o)->BtnState == 2 )
			DrawSkin(buffer, SkinHScrollbarRightDown, o->Absolute.b.x-SkinHScrollbarRightDown->Skin->w+1,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.b.y);
		else
			DrawSkin(buffer, SkinHScrollbarRightUp, o->Absolute.b.x-SkinHScrollbarRightUp->Skin->w+1,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.b.y);
	} else {
		l_ulong size = GetBarSize( i->Max.x, SIZEWIDTHNOSKIN(o));
		if ( i->RPos.x > SIZEWIDTHNOSKIN(o)-size ) i->RPos.x = SIZEWIDTHNOSKIN(o)-size;
		RectAssign(&r,o->Absolute.a.x+15+i->RPos.x,o->Absolute.a.y,o->Absolute.a.x+15+i->RPos.x+size,o->Absolute.b.y);
		SLLBAR(o)->BarCache = r;

		rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,COL_3DFACE);
		Rect3D(buffer,r.a.x,r.a.y,r.b.x+1,r.b.y,COL_3DLIGHT,COL_3DDARK);

		if ( SLLBAR(o)->BtnState == 1 ) {
			Rect3D(buffer,o->Absolute.a.x,o->Absolute.a.y,o->Absolute.a.x+14,o->Absolute.b.y,COL_3DDARK,COL_3DLIGHT);
			vline(buffer,o->Absolute.a.x+6,(o->Absolute.a.y+o->Absolute.b.y)/2+1,(o->Absolute.a.y+o->Absolute.b.y)/2+1,COL_3DTEXT);
			vline(buffer,o->Absolute.a.x+7,(o->Absolute.a.y+o->Absolute.b.y)/2,(o->Absolute.a.y+o->Absolute.b.y)/2+2,COL_3DTEXT);	
			vline(buffer,o->Absolute.a.x+8,(o->Absolute.a.y+o->Absolute.b.y)/2-1,(o->Absolute.a.y+o->Absolute.b.y)/2+3,COL_3DTEXT);
			vline(buffer,o->Absolute.a.x+9,(o->Absolute.a.y+o->Absolute.b.y)/2-2,(o->Absolute.a.y+o->Absolute.b.y)/2+4,COL_3DTEXT);	
		} else {
			Rect3D(buffer,o->Absolute.a.x,o->Absolute.a.y,o->Absolute.a.x+14,o->Absolute.b.y,COL_3DLIGHT,COL_3DDARK);
			vline(buffer,o->Absolute.a.x+5,(o->Absolute.a.y+o->Absolute.b.y)/2,(o->Absolute.a.y+o->Absolute.b.y)/2,COL_3DTEXT);
			vline(buffer,o->Absolute.a.x+6,(o->Absolute.a.y+o->Absolute.b.y)/2-1,(o->Absolute.a.y+o->Absolute.b.y)/2+1,COL_3DTEXT);	
			vline(buffer,o->Absolute.a.x+7,(o->Absolute.a.y+o->Absolute.b.y)/2-2,(o->Absolute.a.y+o->Absolute.b.y)/2+2,COL_3DTEXT);
			vline(buffer,o->Absolute.a.x+8,(o->Absolute.a.y+o->Absolute.b.y)/2-3,(o->Absolute.a.y+o->Absolute.b.y)/2+3,COL_3DTEXT);	
		}
		
		if ( SLLBAR(o)->BtnState == 2 ) {
			Rect3D(buffer,o->Absolute.b.x-14,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.b.y,COL_3DDARK,COL_3DLIGHT);
			vline(buffer,o->Absolute.b.x-4,(o->Absolute.a.y+o->Absolute.b.y)/2+1,(o->Absolute.a.y+o->Absolute.b.y)/2+1,COL_3DTEXT);
			vline(buffer,o->Absolute.b.x-5,(o->Absolute.a.y+o->Absolute.b.y)/2,(o->Absolute.a.y+o->Absolute.b.y)/2+2,COL_3DTEXT);	
			vline(buffer,o->Absolute.b.x-6,(o->Absolute.a.y+o->Absolute.b.y)/2-1,(o->Absolute.a.y+o->Absolute.b.y)/2+3,COL_3DTEXT);
			vline(buffer,o->Absolute.b.x-7,(o->Absolute.a.y+o->Absolute.b.y)/2-2,(o->Absolute.a.y+o->Absolute.b.y)/2+4,COL_3DTEXT);	
		} else {
			Rect3D(buffer,o->Absolute.b.x-14,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.b.y,COL_3DLIGHT,COL_3DDARK);
			vline(buffer,o->Absolute.b.x-5,(o->Absolute.a.y+o->Absolute.b.y)/2,(o->Absolute.a.y+o->Absolute.b.y)/2,COL_3DTEXT);
			vline(buffer,o->Absolute.b.x-6,(o->Absolute.a.y+o->Absolute.b.y)/2-1,(o->Absolute.a.y+o->Absolute.b.y)/2+1,COL_3DTEXT);	
			vline(buffer,o->Absolute.b.x-7,(o->Absolute.a.y+o->Absolute.b.y)/2-2,(o->Absolute.a.y+o->Absolute.b.y)/2+2,COL_3DTEXT);
			vline(buffer,o->Absolute.b.x-8,(o->Absolute.a.y+o->Absolute.b.y)/2-3,(o->Absolute.a.y+o->Absolute.b.y)/2+3,COL_3DTEXT);	

		}
		
			
	}

}
////////////////////////////////////////////////////////////////////////////////
l_bool HorizontalScrollbarEventHandler ( PWidget o, PEvent Ev )
{
	if (Ev->Type == EV_MOUSE )
	{
		PScrollWidget i = SLLBAR(o)->Scll;

		l_ulong SizeWidth = SIZEWIDTHNOSKIN(o);
		l_ulong ButtonWidth = 15;

		if ( UseSkins ) {
			SizeWidth = SIZEWIDTH(o);
			ButtonWidth = SkinVScrollbarUpUp->Skin->h;
		}


		if ( !i->Max.x ) return true;

		if ( Ev->Message == WEvMouseLDown || Ev->Message == WEvMouseLDClk )
		{
			MasterWidget = o;

			if ( CURSOR_IN_RECT( SLLBAR(o)->BarCache ) )
			{
				SLLBAR(o)->W = Mouse->State.p.x-SLLBAR(o)->BarCache.a.x;
			}
			else if ( Mouse->State.p.x < o->Absolute.a.x+ButtonWidth )
			{
				SLLBAR(o)->BtnState = 1;

				TRect r;
				RectAssign(&r,o->Absolute.a.x,o->Absolute.a.y,o->Absolute.a.x+ButtonWidth-1,o->Absolute.b.y);
				WidgetDraw(o,&r);
			}
			else if ( Mouse->State.p.x > o->Absolute.b.x-ButtonWidth )
			{
				SLLBAR(o)->BtnState = 2;

				TRect r;
				RectAssign(&r,o->Absolute.b.x-ButtonWidth+1,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.b.y);
				WidgetDraw(o,&r);
			}
		}
		else if ( Ev->Message == WEvMouseMove && Mouse->State.b & BUTTON_LEFT && !SLLBAR(o)->BtnState )
		{
			i->Pos.x = CalcPos(i->Max.x,SizeWidth,Mouse->State.p.x-SLLBAR(o)->W-o->Absolute.a.x-ButtonWidth);
			i->RPos.x = Mouse->State.p.x-SLLBAR(o)->W-o->Absolute.a.x-ButtonWidth;
			WidgetDraw(o,NULL);
			NotifyScroll(i);
		}
		else if ( Ev->Message == WEvMouseLUp )
		{
			MasterWidget = NULL;

			if ( SLLBAR(o)->BtnState == 1 )
			{
				SLLBAR(o)->BtnState = 0;
				i->Pos.x = max(0,i->Pos.x-i->Delta.x);
				i->RPos.x = GetBaRPos( i->Max.x, SizeWidth,i->Pos.x);
				WidgetDraw(o,NULL);
				NotifyScroll(i);
			}
			else if ( SLLBAR(o)->BtnState == 2 )
			{
				SLLBAR(o)->BtnState = 0;
				i->Pos.x = min(i->Max.x,i->Pos.x+i->Delta.x);
				i->RPos.x = GetBaRPos( i->Max.x, SizeWidth,i->Pos.x);
				WidgetDraw(o,NULL);
				NotifyScroll(i);
			}
			else if ( !CURSOR_IN_RECT( SLLBAR(o)->BarCache ) )
			{
				i->Pos.x = CalcPos(i->Max.x,SizeWidth,Mouse->State.p.x-o->Absolute.a.x-ButtonWidth);
				i->RPos.x = Mouse->State.p.x-o->Absolute.a.x-ButtonWidth;

				WidgetDraw(o,NULL);
				NotifyScroll(i);
			}
		}
		return true;
	}

	if ( Ev->Type == EV_MESSAGE )
		if (  Ev->Message == MSG_NOTIFY_AUTO_RESIZE ) {
			PScrollWidget i = SLLBAR(o)->Scll;
			i->RPos.x = GetBaRPos( i->Max.x, UseSkins?SIZEWIDTH(o):SIZEWIDTHNOSKIN(o),i->Pos.x);
			return true;
		}

	return false;
}
////////////////////////////////////////////////////////////////////////////////
PScrollbar InitScrollbar( PApplication App, TRect r, PScrollWidget Scll, l_uchar Type )
{
	PScrollbar p = malloc(sizeof(TScrollbar));

	if (!p) return NULL;

	memset(p, 0, sizeof(TScrollbar));

	p->Scll	= Scll;
  p->Type = Type;

	IntialiseWidget(App, ScrollbarClass, WIDGET(p), r);

	WIDGET(p)->Flags |= WFForceBuffer; // To prevent blicking as scrollbars are very ofen redrawn

	WIDGET(p)->FocusBack = WIDGET(Scll);


  if ( Type == 1 ) {
	  WIDGET(p)->Draw = &VerticalScrollbarDraw;
	  WIDGET(p)->EventHandler = &VerticalScrollbarEventHandler;

  }	 else if ( Type == 2 ) {

	  WIDGET(p)->Draw = &HorizontalScrollbarDraw;
	  WIDGET(p)->EventHandler = &HorizontalScrollbarEventHandler;

  }

	return p;
}
////////////////////////////////////////////////////////////////////////////////

/*
void TESTDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	l_text lt = TextArgs("%d", SLLWDG(o)->Pos.y);

	rectfill(buffer, w->a.x, w->a.y, w->b.x, w->b.y, o->BackgroundColor);

	textout(buffer, default_font, lt, o->Absolute.a.x, o->Absolute.a.y, makecol(255,255,255));

}

void Scrolled ( PScrollWidget o, TPoint Pos ) {

	WidgetDraw(WIDGET(o), NULL);
}
*/
////////////////////////////////////////////////////////////////////////////////
void ScrollWidgetSmartFlags ( PScrollWidget o ) {
	
	if ( WIDGET(o)->Flags & WF_AUTORESIZE ) {
		
		if ( o->Vertical )
			WIDGET(o->Vertical)->Flags |= (WIDGET(o)->Flags & (WF_FIXEDHEIGHT|WF_MAGNETRIGHT|WF_MAGNETBOTTOM))|WF_FIXEDWIDTH|WF_AUTORESIZE;
		
		if ( o->Horizontal )
			WIDGET(o->Horizontal)->Flags |= (WIDGET(o)->Flags & (WF_FIXEDWIDTH|WF_MAGNETRIGHT|WF_MAGNETBOTTOM))|WF_FIXEDHEIGHT|WF_AUTORESIZE;
	}
	
	
	
}


////////////////////////////////////////////////////////////////////////////////
void ScrollWidgetSetup ( PWidget o ) {
	if ( SLLWDG(o)->Vertical ) {
		InsertWidget(o->Parent, WIDGET(SLLWDG(o)->Vertical));
	}
	if ( SLLWDG(o)->Horizontal ) {
		InsertWidget(o->Parent, WIDGET(SLLWDG(o)->Horizontal));
	}
	ScrollWidgetSmartFlags(SLLWDG(o));
}
/*
void ScrollWidgetFreeEx ( PWidget o ) {
	if ( SLLWDG(o)->Vertical ) WidgetDispose(WIDGET(SLLWDG(o)->Vertical));
}
*/
////////////////////////////////////////////////////////////////////////////////
void ScrollBarScrollTo ( PScrollWidget i, TPoint Pos ) {
	if ( Pos.y != i->Pos.y && i->Vertical ) {
		i->Pos.y = max(0,min(i->Max.y,Pos.y));
		i->RPos.y = GetBaRPos( i->Max.y, UseSkins?SIZEHEIGHT(WIDGET(i->Vertical)):SIZEHEIGHTNOSKIN(WIDGET(i->Vertical)),i->Pos.y);
		WidgetDraw(WIDGET(i->Vertical),NULL);
	}
	if ( Pos.x != i->Pos.x && i->Horizontal ) {
		i->Pos.x = max(0,min(i->Max.x,Pos.x));
		i->RPos.x = GetBaRPos( i->Max.x, UseSkins?SIZEWIDTH(WIDGET(i->Horizontal)):SIZEWIDTHNOSKIN(WIDGET(i->Horizontal)),i->Pos.x);
		WidgetDraw(WIDGET(i->Horizontal),NULL);
	}
	NotifyScroll(i);
}
////////////////////////////////////////////////////////////////////////////////
void ScrollBarSetPosY ( PScrollWidget i, l_ulong Pos ) {
	i->Pos.y = min(i->Max.y,Pos);
	if ( i->Vertical ) i->RPos.y = GetBaRPos( i->Max.y, UseSkins?SIZEHEIGHT(WIDGET(i->Vertical)):SIZEHEIGHTNOSKIN(WIDGET(i->Vertical)),i->Pos.y);
}
////////////////////////////////////////////////////////////////////////////////
void ScrollBarSetPosX ( PScrollWidget i, l_ulong Pos ) {
	i->Pos.x = min(i->Max.x,Pos);
	if ( i->Horizontal ) i->RPos.x = GetBaRPos( i->Max.x, UseSkins?SIZEWIDTH(WIDGET(i->Horizontal)):SIZEWIDTHNOSKIN(WIDGET(i->Horizontal)),i->Pos.x);
}
////////////////////////////////////////////////////////////////////////////////
void ScrollBarSetMaxY ( PScrollWidget i, l_ulong Max ) {
	i->Max.y = Max;
	i->Pos.y = min(i->Max.y,i->Pos.y);
	if ( i->Vertical )
		i->RPos.y = GetBaRPos( i->Max.y, UseSkins?SIZEHEIGHT(WIDGET(i->Vertical)):SIZEHEIGHTNOSKIN(WIDGET(i->Vertical)),i->Pos.y);
}
////////////////////////////////////////////////////////////////////////////////
void ScrollBarSetMaxX ( PScrollWidget i, l_ulong Max ) {
	i->Max.x = Max;
	i->Pos.x = min(i->Max.x,i->Pos.x);
	if ( i->Horizontal )
		i->RPos.x = GetBaRPos( i->Max.x, UseSkins?SIZEWIDTH(WIDGET(i->Horizontal)):SIZEWIDTHNOSKIN(WIDGET(i->Horizontal)),i->Pos.x);
}
////////////////////////////////////////////////////////////////////////////////
void ScrollBarRedraw ( PScrollWidget o ) {
	if ( o->Vertical ) WidgetDraw(WIDGET(o->Vertical),NULL);
	if ( o->Horizontal ) WidgetDraw(WIDGET(o->Horizontal),NULL);
}
////////////////////////////////////////////////////////////////////////////////
void _ScrollBarChangeType ( PScrollWidget o, l_uchar Type ) {

	TRect rb;
	TRect r = WIDGET(o)->Relative;
	l_ulong SWidth = 15;
	l_ulong SHeight = 15;

	if ( UseSkins ) {
		SHeight = SkinHScrollbarBackground->Skin->h;
		SWidth = SkinVScrollbarBackground->Skin->w;
	}

	if ( (Type & 1) && !o->Vertical )   r.b.x -= SWidth+1;
	if ( (Type & 2) && !o->Horizontal ) r.b.y -= SHeight+1;
	
	if ( !(Type & 1) && o->Vertical )   r.b.x += SWidth+1;
	if ( !(Type & 2) && o->Horizontal ) r.b.y += SHeight+1;

	if ( Type & 1 ) {
		RectAssign(&rb,r.b.x+2, r.a.y, r.b.x+SWidth+1, r.b.y);
		if ( !o->Vertical ) {
			o->Vertical = InitScrollbar(WIDGET(o)->AppOwner, rb, o, 1);
			InsertWidget(WIDGET(o)->Parent, WIDGET(o->Vertical));
		} else 
			WidgetSetRect(WIDGET(o->Vertical),rb);
	} else if ( o->Vertical ) {
		WidgetDispose(WIDGET(o->Vertical));
		o->Vertical = NULL;
	}

	if ( Type & 2 ) {
		RectAssign(&rb,r.a.x,r.b.y+2,r.b.x,r.b.y+SHeight+1);
		if ( !o->Horizontal ) {
			o->Horizontal = InitScrollbar(WIDGET(o)->AppOwner,rb,o,2);
			InsertWidget(WIDGET(o)->Parent, WIDGET(o->Horizontal));
		} else 
			WidgetSetRect(WIDGET(o->Horizontal),rb);
	} else if ( o->Horizontal ) {
		WidgetDispose(WIDGET(o->Horizontal));
		o->Horizontal = NULL;
	}

	
	WidgetSetRect(WIDGET(o),r);
	WidgetDrawAll(WIDGET(o));
	ScrollBarRedraw(o);
	
	ScrollWidgetSmartFlags(SLLWDG(o));
}
////////////////////////////////////////////////////////////////////////////////
void ScrollBarChangeTypeUninserted ( PScrollWidget o, l_uchar Type ) {

	TRect rb;
	TRect r = WIDGET(o)->Relative;
	l_ulong SWidth = 15;
	l_ulong SHeight = 15;

	if ( UseSkins ) {
		SHeight = SkinHScrollbarBackground->Skin->h;
		SWidth = SkinVScrollbarBackground->Skin->w;
	}
	
	if ( (Type & 1) && !o->Vertical ) r.b.x -= SWidth+1;
	if ( (Type & 2) && !o->Horizontal ) r.b.y -= SHeight+1;
	
	if ( !(Type & 1) && o->Vertical ) r.b.x += SWidth+1;
	if ( !(Type & 2) && o->Horizontal ) r.b.y += SHeight+1;

	if ( Type & 1 ) {
		RectAssign(&rb,r.b.x+2, r.a.y, r.b.x+SWidth+1, r.b.y);
		if ( !o->Vertical )
			o->Vertical = InitScrollbar(WIDGET(o)->AppOwner, rb, o, 1);
		else
			WidgetSetRect(WIDGET(o->Vertical),rb);
	} else if ( o->Vertical ) {
		free(o->Vertical);
		o->Vertical = NULL;
	}

	if ( Type & 2 ) {
		RectAssign(&rb,r.a.x,r.b.y+2,r.b.x,r.b.y+SHeight+1);
		if ( !o->Horizontal )
			o->Horizontal = InitScrollbar(WIDGET(o)->AppOwner,rb,o,2);
		else
			WidgetSetRect(WIDGET(o->Horizontal),rb);
	} else if ( o->Horizontal ) {
		free(o->Horizontal);
		o->Horizontal = NULL;
	}
	WidgetSetRect(WIDGET(o),r);
	ScrollWidgetSmartFlags(SLLWDG(o));
}
////////////////////////////////////////////////////////////////////////////////
void ScrollBarChangeType ( PScrollWidget o, l_uchar Type ) {
	if ( WIDGET(o)->Setup )
		ScrollBarChangeTypeUninserted(o,Type);
	else
		_ScrollBarChangeType(o,Type);
}
////////////////////////////////////////////////////////////////////////////////
l_bool ScrollWidgetEventHandler ( PWidget o, PEvent Ev ) {

	if ( Ev->Type == EV_MOUSE ) {
		if ( SLLWDG(o)->Max.y )
			if ( Ev->Message == MSG_MOUSE_SCLLUP ) {
				TPoint p = SLLWDG(o)->Pos;
				if ( p.y > 3*SLLWDG(o)->Delta.y )
					p.y -= 3*SLLWDG(o)->Delta.y;
				else
					p.y = 0;
				ScrollBarScrollTo(SLLWDG(o),p);
				return true;
			} else if ( Ev->Message == MSG_MOUSE_SCLLDWN ) {
				TPoint p = SLLWDG(o)->Pos;
				if ( p.y < SLLWDG(o)->Max.y-(3*SLLWDG(o)->Delta.y) )
					p.y += 3*SLLWDG(o)->Delta.y;
				else
					p.y = SLLWDG(o)->Max.y;
				ScrollBarScrollTo(SLLWDG(o),p);
				return true;
			}
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////
PScrollWidget InitScrollWidget( PScrollWidget o, PWidgetClass Class, PApplication App, TRect r, l_uchar Type )
{
	TRect rb;
	l_ulong SWidth = 15;
	l_ulong SHeight = 15;

	if ( UseSkins ) {
		SHeight = SkinHScrollbarBackground->Skin->h;
		SWidth = SkinVScrollbarBackground->Skin->w;
	}


	if ( Type & 1 ) r.b.x -= SWidth+1;
	if ( Type & 2 ) r.b.y -= SHeight+1;

	IntialiseWidget(App, Class?Class:ScollWidgetClass, WIDGET(o), r );

	if ( Type & 1 ) {
		RectAssign(&rb,r.b.x+2, r.a.y, r.b.x+1+SWidth, r.b.y);
		o->Vertical = InitScrollbar(App, rb, o, 1);

	}

	if ( Type & 2 ) {
		RectAssign(&rb,r.a.x,r.b.y+2,r.b.x,r.b.y+1+SHeight);
		o->Horizontal = InitScrollbar(App,rb,o,2);
	}


	WIDGET(o)->Setup = &ScrollWidgetSetup;
	//WIDGET(o)->FreeEx = &ScrollWidgetFreeEx;

	return o;
}
////////////////////////////////////////////////////////////////////////////////
l_bool LibMain ( int argc, l_text *argv )
{
	ScollWidgetClass = CreateWidgetClass( ULONG_ID('S','l','W','g'), StdWidgetClass, 0 );
	
	ScrollbarClass = CreateWidgetClass( ULONG_ID('x','S','c','l'), StdWidgetClass, WCF_SATELLITE );


	SkinVScrollbarUpUp			= GetSkinItem("VScrollbarUpUp");
	SkinVScrollbarUpDown		= GetSkinItem("VScrollbarUpDown");
	SkinVScrollbarDownUp		= GetSkinItem("VScrollbarDownUp");
	SkinVScrollbarDownDown		= GetSkinItem("VScrollbarDownDown");
	SkinVScrollbarBackground	= GetSkinItem("VScrollbarBackground");
	SkinVScrollbarTracker		= GetSkinItem("VScrollbarTracker");

	SkinHScrollbarLeftUp		= GetSkinItem("HScrollbarLeftUp");
	SkinHScrollbarLeftDown		= GetSkinItem("HScrollbarLeftDown");
	SkinHScrollbarRightUp		= GetSkinItem("HScrollbarRightUp");
	SkinHScrollbarRightDown		= GetSkinItem("HScrollbarRightDown");
	SkinHScrollbarBackground	= GetSkinItem("HScrollbarBackground");
	SkinHScrollbarTracker		= GetSkinItem("HScrollbarTracker");

	APPEXPORT(InitScrollWidget);
	APPEXPORT(ScrollBarScrollTo);
	APPEXPORT(ScrollBarSetPosX);
	APPEXPORT(ScrollBarSetPosY);
	APPEXPORT(ScrollBarSetMaxY);
	APPEXPORT(ScrollBarSetMaxX);

	APPEXPORT(ScrollBarChangeType);
	APPEXPORT(ScrollBarRedraw);
	APPEXPORT(ScrollWidgetEventHandler);
	APPEXPORT(ScollWidgetClass);
	APPEXPORT(ScrollBarChangeTypeUninserted);
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{

}
////////////////////////////////////////////////////////////////////////////////
