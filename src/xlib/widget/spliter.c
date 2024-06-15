////////////////////////////////////////////////////////////////////////////////
//
//  Widgets - Core File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "skin.h"

/**	
	@addtogroup widgetsys
	@{
*/
/**		
	@file
	
	@defgroup spliters Auto-resize and spliters
	System to store widgets design in ressources files.
	@{
*/
////////////////////////////////////////////////////////////////////////////////
void RectSelfRealtive ( PRect d, TRect s ) {
	d->a.x = 0;
	d->a.y = 0;
	d->b.x = s.b.x-s.a.x;
	d->b.y = s.b.y-s.a.y;
}
////////////////////////////////////////////////////////////////////////////////
/**
	Calculates zones delimited by spliters in a widget
	Coordinates are relative to child area
	Internal use only
	@param o Widget
	@param p Child area (relative to child area)
*/
PList _CalculatesZones ( PWidget o, TRect p ) {
	PList Zones;

	Zones = NewListFromRect(p);
	
	if ( !Zones ) return NULL;
	
	if ( o->Last ) {
		PWidget a = o->Last;
		PWidget b = a;
		do {
			if ( WidgetIsClassId(a,WCLASS_SPLITER) ) {
				if ( SPLITER(a)->Mode == SP_VERTICAL )
					RemoveZoneInList(Zones,a->Relative);
				else
					RemoveZoneInListV(Zones,a->Relative);
			}
			a = a->Next;
		} while ( a != b );
	}
	return Zones;
}
////////////////////////////////////////////////////////////////////////////////
/**
	Resize a zone in a widget
	First resize decreasing zones, then increasing zones. (to prevents conflicts).
	Internal use only.
	@param o Widget
	@param z Zone (relative to child area)
	@param nz Zone new size (relative to child area)

*/
void _ResizeZone ( PWidget o, TRect z, TRect nz ) {
	if ( o->Last ) {
		PWidget a = o->Last;
		PWidget b = a;
		do {
			if ( _RectOverlay(z,a->Relative) ) {
				
				if ( a->Flags & WF_AUTORESIZE ) {
					TRect n = a->Relative;
					//n.a.x -= z.a.x-nz.a.x;
					//n.a.y -= z.a.y-nz.a.y;
					
					if ( a->Flags & WF_MAGNETRIGHT ) {
						if ( a->Flags & WF_FIXEDWIDTH ) {
							n.a.x -= z.b.x-nz.b.x;
							//n.b.x -= z.a.x-nz.a.x;
						} else
							n.a.x -= z.a.x-nz.a.x;
						n.b.x -= (z.b.x-nz.b.x);
					}
					if ( a->Flags & WF_MAGNETBOTTOM ) {
						if ( a->Flags & WF_FIXEDHEIGHT ) {
							 n.a.y -= z.b.y-nz.b.y;
							 //n.b.y -= z.a.y-nz.a.y;
						} else
							n.a.y -= z.a.y-nz.a.y;
						n.b.y -= (z.b.y-nz.b.y);
					}
					WidgetSetRect(a,n);
					WidgetSendSEvent(a,EV_MESSAGE,MSG_NOTIFY_AUTO_RESIZE,NULL);
				} else
					WidgetRefreshAbsolutePos(a);
			} else
				WidgetRefreshAbsolutePos(a);
			
			a = a->Next;
		} while ( a != b );
	}
}
////////////////////////////////////////////////////////////////////////////////
void _WidgetAutosize ( PWidget o, TRect p, TRect r ) {
	
	PList Zones;
	
	// Transform childs area to be self relative	
	RectSelfRealtive(&p,p);
	RectSelfRealtive(&r,r);
		
	// Calculates Zones
	Zones = _CalculatesZones(o,p);
	
	if ( !Zones ) return;
	
	// Resize Linked zones
	if ( Zones->Last ) {
		PListItem a = Zones->Last;
		PListItem b = a;
		TRect nz;
		do {
			PRect z = ((PRect)a->Data);
			if ( z->b.x == p.b.x && z->b.y == p.b.y ) {
				nz = *z;
				nz.b.x -= p.b.x-r.b.x;
				nz.b.y -= p.b.y-r.b.y;
				_ResizeZone(o,*z,nz);
			} else if ( z->b.x == p.b.x ) {
				nz = *z;
				nz.b.x -= p.b.x-r.b.x;
				_ResizeZone(o,*z,nz);
			} else if ( z->b.y == p.b.y ) {
				nz = *z;
				nz.b.y -= p.b.y-r.b.y;
				_ResizeZone(o,*z,nz);
			}
			a = a->Next;
		} while ( a != b );
	}
	// Magnet Spliters as they are excluded from zones.
	
	if ( o->Last ) {
		PWidget a = o->Last;
		PWidget b = a;
		do {
			if ( WidgetIsClassId(a,WCLASS_SPLITER) ) {
				if ( (SPLITER(a)->Mode == SP_HORIZONTAL) && ( a->Relative.b.x >= p.b.x ) ) {
					a->Relative.b.x -= p.b.x-r.b.x;
				} else if ( (SPLITER(a)->Mode == SP_VERTICAL) && ( a->Relative.b.y >= p.b.y ) ) {
					a->Relative.b.y -= p.b.y-r.b.y;
				}
				WidgetRefreshAbsolutePosEx(a);
			}
			a = a->Next;
		} while ( a != b );
	}
	FreeList(Zones);
}
////////////////////////////////////////////////////////////////////////////////
TPoint MoveStart;
l_long MoveLowLimit;
l_long MoveHightLimit;

void SpliterCalculateMoveLimits ( PSpliter s ) {
	
	PWidget o = WIDGET(s)->Parent;
	
	TRect p, m = WIDGET(s)->Relative;
	PList Zones;
	
	// Transform childs area to be self relative
	RectSelfRealtive(&p,o->ChildArea);
	
	// Calculates Zones
	Zones = _CalculatesZones(o,p);
	
	if ( !Zones ) return;
	
	MoveLowLimit = 0;
	
	if (  s->Mode == SP_HORIZONTAL )
		MoveHightLimit = p.b.y;
	else
		MoveHightLimit = p.b.x;
	
	// Resize Linked zones
	if ( Zones->Last ) {
		PListItem a = Zones->Last;
		PListItem b = a;
		TRect nz;
		do {
			PRect z = ((PRect)a->Data);
			if (  s->Mode == SP_HORIZONTAL ) {
				if ( z->b.y == m.a.y-1 ) {
					MoveLowLimit = max(z->a.y,MoveLowLimit);
				} else if ( z->a.y == m.b.y+1 ) {
					MoveHightLimit = min(z->b.y,MoveHightLimit);
				}
			} else {
				if ( z->b.x == m.a.x-1 ) {
					MoveLowLimit = max(z->a.x,MoveLowLimit);
				} else if ( z->a.x == m.b.x+1 ) {
					MoveHightLimit = min(z->b.x,MoveHightLimit);
				}
			}
			a = a->Next;
		} while ( a != b );
	}	
	
	FreeList(Zones);
	
	if ( s->Mode == SP_HORIZONTAL ) {
		MoveLowLimit 	-= m.a.y;
		MoveHightLimit 	-= m.b.y;
	} else {
		MoveLowLimit 	-= m.a.x;
		MoveHightLimit 	-= m.b.x;
	}
	
}

void SpliterMoveOnLeft ( PWidget o, PSpliter s, PList Zones, int mv ) {
	
	if ( Zones->Last ) {
		PListItem a = Zones->Last;
		PListItem b = a;
		TRect nz;
		do {
			PRect z = ((PRect)a->Data);
			
			if ( z->b.x == WIDGET(s)->Relative.a.x-1 ) {
				nz = *z;
				nz.b.x += mv;
				_ResizeZone(o,*z,nz);
			}
			a = a->Next;
		} while ( a != b );
	}	
}

void SpliterMoveOnRight ( PWidget o, PSpliter s, PList Zones, int mv ) {
	
	if ( Zones->Last ) {
		PListItem a = Zones->Last;
		PListItem b = a;
		TRect nz;
		do {
			PRect z = ((PRect)a->Data);
			
			if ( z->a.x == WIDGET(s)->Relative.b.x+1 ) {
				nz = *z;
				nz.a.x += mv;
				_ResizeZone(o,*z,nz);
			}
			a = a->Next;
		} while ( a != b );
	}	
}

void SpliterMagnetVertical ( PWidget o, PSpliter mys, int mv ) {
	if ( o->Last ) {
		PWidget a = o->Last;
		PWidget b = a;
		do {
			if ( WidgetIsClassId(a,WCLASS_SPLITER) ) {
				if ( SPLITER(a)->Mode == SP_HORIZONTAL ) {
					if ( ( a->Relative.b.x >= WIDGET(mys)->Relative.a.x ) && ( a->Relative.b.x < WIDGET(mys)->Relative.b.x ) ) 
						a->Relative.b.x += mv;
					else if ( ( a->Relative.a.x > WIDGET(mys)->Relative.a.x ) && ( a->Relative.a.x <= WIDGET(mys)->Relative.b.x ) ) 
						a->Relative.a.x += mv;
				}
				WidgetRefreshAbsolutePosEx(a);
			}
			a = a->Next;
		} while ( a != b );
	}
}

void SpliterMoveOnTop ( PWidget o, PSpliter s, PList Zones, int mv ) {
	
	if ( Zones->Last ) {
		PListItem a = Zones->Last;
		PListItem b = a;
		TRect nz;
		do {
			PRect z = ((PRect)a->Data);
			
			if ( z->b.y == WIDGET(s)->Relative.a.y-1 ) {
				nz = *z;
				nz.b.y += mv;
				_ResizeZone(o,*z,nz);
			}
			a = a->Next;
		} while ( a != b );
	}	
}

void SpliterMoveOnBottom ( PWidget o, PSpliter s, PList Zones, int mv ) {
	
	if ( Zones->Last ) {
		PListItem a = Zones->Last;
		PListItem b = a;
		TRect nz;
		do {
			PRect z = ((PRect)a->Data);
			
			if ( z->a.y == WIDGET(s)->Relative.b.y+1 ) {
				nz = *z;
				nz.a.y += mv;
				_ResizeZone(o,*z,nz);
			}
			a = a->Next;
		} while ( a != b );
	}	
}

void SpliterMagnetHorizontal ( PWidget o, PSpliter mys, int mv ) {
	if ( o->Last ) {
		PWidget a = o->Last;
		PWidget b = a;
		do {
			if ( WidgetIsClassId(a,WCLASS_SPLITER) ) {
				if ( SPLITER(a)->Mode == SP_VERTICAL ) {
					if ( ( a->Relative.b.y >= WIDGET(mys)->Relative.a.y ) && ( a->Relative.b.y < WIDGET(mys)->Relative.b.y ) ) 
						a->Relative.b.y += mv;
					else if ( ( a->Relative.a.y > WIDGET(mys)->Relative.a.y ) && ( a->Relative.a.y <= WIDGET(mys)->Relative.b.y ) ) 
						a->Relative.a.y += mv;
				}
				WidgetRefreshAbsolutePosEx(a);
			}
			a = a->Next;
		} while ( a != b );
	}
}

l_bool SpliterEventHandler ( PWidget o, PEvent Ev )
{
	if (Ev->Type == EV_MOUSE)
	{
		if (Ev->Message == WEvMouseLDown)
		{
				MasterWidget = o; 
				MoveStart = Mouse->State.p;
				SPLITER(o)->Moving = true;
				SpliterCalculateMoveLimits(SPLITER(o));
				return true;

		}

		if (Ev->Message == WEvMouseMove)
		{

			if (Mouse->State.b & BUTTON_LEFT)
			{
				if (MasterWidget == o)
				{
					if ( SPLITER(o)->Moving )
					{
						TRect tmp;
						l_long mv; 
						
						if ( SPLITER(o)->Mode == SP_HORIZONTAL )
							mv = (Mouse->State.p.y-MoveStart.y);
						else
							mv = (Mouse->State.p.x-MoveStart.x);
						
						mv = max(MoveLowLimit,mv);
						mv = min(MoveHightLimit,mv);
						
						if ( SPLITER(o)->Mode == SP_HORIZONTAL )
							RectAssign(&tmp,
							o->Absolute.a.x,
							(o->Absolute.a.y+o->Absolute.b.y)/2+mv,
							o->Absolute.b.x,
							(o->Absolute.a.y+o->Absolute.b.y)/2+1+mv);
						else
							RectAssign(&tmp,
							(o->Absolute.a.x+o->Absolute.b.x)/2+mv,
							o->Absolute.a.y,
							(o->Absolute.a.x+o->Absolute.b.x)/2+1+mv,
							o->Absolute.b.y);
						
						WidgetSetOSDRect(tmp);
					}

				}


			}
		}

		if (Ev->Message == WEvMouseLUp)
		{

			if (MasterWidget == o)
			{
				WidgetUnOSD(); // Unactivate OSD

				if ( SPLITER(o)->Moving )
				{
					PWidget op = o->Parent;
					TRect p;
					PList Zones;
					l_int mv;
					TRect r = o->Relative;
						
					RectSelfRealtive(&p,op->ChildArea);
					Zones = _CalculatesZones(op,p);
						
					SPLITER(o)->Moving = false;
					
					if ( SPLITER(o)->Mode == SP_VERTICAL ) {
						
						mv = (Mouse->State.p.x-MoveStart.x);
						mv = max(MoveLowLimit,mv);
						mv = min(MoveHightLimit,mv);						
						
						r.a.x += mv;
						r.b.x += mv;
						
						if ( mv < 0 ) {
							SpliterMoveOnLeft(op,SPLITER(o),Zones,mv);
							SpliterMoveOnRight(op,SPLITER(o),Zones,mv);
						} else {
							SpliterMoveOnRight(op,SPLITER(o),Zones,mv);
							SpliterMoveOnLeft(op,SPLITER(o),Zones,mv);
						}
						SpliterMagnetVertical(op,SPLITER(o),mv);
						
					} else {
						
						mv = (Mouse->State.p.y-MoveStart.y);
						mv = max(MoveLowLimit,mv);
						mv = min(MoveHightLimit,mv);						
						
						r.a.y += mv;
						r.b.y += mv;
						
						if ( mv < 0 ) {
							SpliterMoveOnTop(op,SPLITER(o),Zones,mv);
							SpliterMoveOnBottom(op,SPLITER(o),Zones,mv);
						} else {
							SpliterMoveOnBottom(op,SPLITER(o),Zones,mv);
							SpliterMoveOnTop(op,SPLITER(o),Zones,mv);
						}
						SpliterMagnetHorizontal(op,SPLITER(o),mv);
					}
					o->Relative = r;
					WidgetRefreshAbsolutePosEx(o);
					FreeList(Zones);
					WidgetDrawAll(op);
				}

				
				
				MasterWidget = NULL;	// I will get only my events... Not all mouse ones :}
			}

		}
	}

	return false;
}



////////////////////////////////////////////////////////////////////////////////
void SpliterDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	rectfill(buffer,o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.b.y,COL_3DFACE);
	
	if ( SPLITER(o)->Mode == SP_HORIZONTAL )
		hline(buffer,o->Absolute.a.x,(o->Absolute.a.y+o->Absolute.b.y)/2,o->Absolute.b.x,COL_3DTEXT);
	else
		vline(buffer,(o->Absolute.a.x+o->Absolute.b.x)/2,o->Absolute.a.y,o->Absolute.b.y,COL_3DTEXT);
}

////////////////////////////////////////////////////////////////////////////////
PWidgetClass SpliterClass = NULL;
PWidgetClass VSpliterClass = NULL;
PWidgetClass HSpliterClass = NULL;

////////////////////////////////////////////////////////////////////////////////
PSpliter CreateSpliter  ( PApplication App, TRect r, PWidgetClass Class, l_ulong Mode ) {
	
	
	PSpliter o = malloc(sizeof(TSpliter));
	memset(o,0,sizeof(TSpliter));
	if ( !o ) return NULL;
	IntialiseWidget(App, Class, WIDGET(o), r );
	o->Mode = Mode;
	WIDGET(o)->Draw = &SpliterDraw;
	WIDGET(o)->EventHandler = &SpliterEventHandler;
	return o;
}
////////////////////////////////////////////////////////////////////////////////
PWidget CreateVSpliter  ( PApplication App, TRect r ) {
	return (PWidget)CreateSpliter(App,r,VSpliterClass,SP_VERTICAL);
}
////////////////////////////////////////////////////////////////////////////////
PWidget CreateHSpliter  ( PApplication App, TRect r ) {
	return (PWidget)CreateSpliter(App,r,HSpliterClass,SP_HORIZONTAL);
}
////////////////////////////////////////////////////////////////////////////////
void InitSpliters ( void ) {
	
	SpliterClass = CreateWidgetClass( WCLASS_SPLITER, StdWidgetClass, 0 );
	
	VSpliterClass = CreateWidgetClass( WCLASS_VSPLITER, SpliterClass, 0 );
	VSpliterClass->StdCreate = &CreateVSpliter;
	
	HSpliterClass = CreateWidgetClass( WCLASS_HSPLITER, SpliterClass, 0 );
	HSpliterClass->StdCreate = &CreateHSpliter;
}
////////////////////////////////////////////////////////////////////////////////
/**
@}
@}
*/

