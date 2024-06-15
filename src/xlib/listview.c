////////////////////////////////////////////////////////////////////////////////
//
//	Listview - Core file
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "listview.h"
#include "tbox.h"

/** @defgroup listview Listview widget
	Display icons lists in differents styles.
	@ingroup widgets
	@{
*/
/**	@file
	Listvisw core file.
*/

////////////////////////////////////////////////////////////////////////////////
l_ulong	AppVersion = ULONG_ID(0,0,0,1);
l_char	AppName[] = "Listview Widget";
l_uid	nUID = "listview";
l_uid NeededLibs[] = { "widget", "skin","textbox","" };

PWidgetClass ListviewClass;


////////////////////////////////////////////////////////////////////////////////
#define ITEMWIDTH 65
#define ITEMHEIGHT 65
#define BORDEROFFSET 2
#define BORDERTOPOFFSET 1
////////////////////////////////////////////////////////////////////////////////
PSkin SkinListview = 0;
FONT *ListviewFont;
l_int ListviewFontColor;

l_uchar TransBG = 0;

////////////////////////////////////////////////////////////////////////////////
l_bool ListviewDropAccept ( PWidget o, PWidget Caller, l_ulong Type, void *Data, l_ulong SupportedMethods, l_ulong *Method ) {
	if ( LISTVIEW(o)->Flags & LVF_ALLOWDRAG ) {

		PListviewItem old;
		PListviewItem p = ListviewGetItemUnderPoint(LISTVIEW(o), Mouse->State.p);					
		l_ulong LFlags = LISTVIEW(o)->Flags;
		
		LISTVIEW(o)->TopItem = p;
		if ( p != LISTVIEW(o)->DragItem ) {
			PListviewItem old = LISTVIEW(o)->DragItem;
			LISTVIEW(o)->DragItem = p;
			if ( old ) WidgetDraw(WIDGET(o), &(old->BufTop));
			if ( LISTVIEW(o)->DragItem ) WidgetDraw(WIDGET(o), &(LISTVIEW(o)->DragItem->BufTop));
		}
		
		if (o == Caller) {
			if ( !p ) return false;
			if ( p->Flags & LVI_SELECTED ) return false;
		}
		
		if ( LISTVIEW(o)->ItemDropAccept ) 
			return LISTVIEW(o)->ItemDropAccept(o,Caller,LISTVIEW(o)->DragItem,Type,Data,SupportedMethods,Method);
	
	}
		
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool ListviewDragBeforeDrop    ( PWidget o, PWidget Dest, void *Data, l_ulong Type, l_ulong Method ) {
	if ( o == Dest )
		if ( LISTVIEW(o)->DragItem )
			if ( LISTVIEW(o)->DragItem->Flags & LVI_SELECTED )
				return false;
	if ( LISTVIEW(o)->ItemDragBeforeDrop ) 
		return LISTVIEW(o)->ItemDragBeforeDrop(o,Dest,(o==Dest)?LISTVIEW(o)->DragItem:NULL,Data,Type,Method);
	
	return false;
}
////////////////////////////////////////////////////////////////////////////////
void ListviewDropData    ( PWidget o, PWidget Caller, void *Data, l_ulong Type, l_ulong Method ) {
	PListviewItem old = LISTVIEW(o)->DragItem;
	
	LISTVIEW(o)->DragItem = NULL;
	LISTVIEW(o)->TopItem = NULL;	
	
	ListviewSelectItem(LISTVIEW(o),old,false,false);
	
	if ( LISTVIEW(o)->ItemDropData ) 
		LISTVIEW(o)->ItemDropData(o,Caller,old,Data,Type,Method);
	
}
////////////////////////////////////////////////////////////////////////////////
void ListviewDragAfterDrop ( PWidget o, PWidget Dest, void*Data, l_ulong Type, l_ulong Method ) {

	if ( LISTVIEW(o)->ItemDragAfterDrop ) 
		LISTVIEW(o)->ItemDragAfterDrop(o,Dest,(o==Dest)?LISTVIEW(o)->DragItem:NULL,Data,Type,Method);
}
////////////////////////////////////////////////////////////////////////////////
void ListviewDrawItems (PWidget o, p_bitmap buffer, TRect w)
{
	TRect OldClip,Clip;
	
	PListItem a = LISTVIEW(o)->Items->Last, b;
	PListviewItem i = 0;
	l_int SelColor = ( o->Flags & WFFocused ) ? COL_BOXSEL : makecol(215,215,215);
	l_int TextColor;
	
	l_int x = o->Absolute.a.x+BORDEROFFSET;
	l_int y = o->Absolute.a.y+BORDERTOPOFFSET-SLLWDG(o)->Pos.y;
	l_int yy= o->Absolute.a.y+2;
  l_int l  = 0;

  	if ( a ) {
		a = a->Next;
		b = a;
	}

	GetClip(buffer,&OldClip);
	
	if ( LISTVIEW(o)->Style == LVS_ICONS ) {
		
		if ( !a ) return;
		
		RectAssign(&Clip,o->Absolute.a.x+1,o->Absolute.a.y+1,o->Absolute.b.x-1,o->Absolute.b.y-1);
		RectIntersept(&Clip,Clip,w);
		set_clip_rect(buffer,o->Absolute.a.x+1,o->Absolute.a.y+1,o->Absolute.b.x-1,o->Absolute.b.y-1);
		
		yy += ITEMHEIGHT;

		do
		{


			i = (PListviewItem)a->Data;

			l = text_length(ListviewFont, i->Caption)+4;

			RectAssign ( &i->BufAbs, x, y, x + ITEMWIDTH,	y + ITEMHEIGHT);
			
			RectAssign ( &i->RIcon, i->BufAbs.a.x+(ITEMWIDTH/2)-16,i->BufAbs.a.y+(ITEMHEIGHT/2)-16, i->BufAbs.a.x+(ITEMWIDTH/2)+16,i->BufAbs.a.y+(ITEMHEIGHT/2)+16);
			
			if ( l+4 > ITEMWIDTH ) {
				if ( x+(ITEMWIDTH/2)-(l/2)-2 < o->Absolute.a.x+BORDEROFFSET )
					RectAssign ( &i->BufTop,o->Absolute.a.x+BORDEROFFSET,y,o->Absolute.a.x+BORDEROFFSET+l+4,y+ITEMHEIGHT);
				else if ( x+(ITEMWIDTH/2)+(l/2)+2 > o->Absolute.b.x-BORDEROFFSET  )
					RectAssign ( &i->BufTop,o->Absolute.b.x-BORDEROFFSET-l-4,y,o->Absolute.b.x-BORDEROFFSET,y+ITEMHEIGHT);
				else
					RectAssign ( &i->BufTop, x+(ITEMWIDTH/2)-(l/2)-3, y, x+(ITEMWIDTH/2)+(l/2)+3 ,	y + ITEMHEIGHT);
			} else
				RectAssign ( &i->BufTop,x,y,x+ITEMWIDTH,y+ITEMHEIGHT);
			
			
			l = min(l,ITEMWIDTH);	
			
			RectAssign(&i->RLabel,	i->BufAbs.a.x+(ITEMWIDTH/2)-(l/2)-2,
									i->BufAbs.a.y+(ITEMHEIGHT/2)+(16)+2,
 									i->BufAbs.a.x+(ITEMWIDTH/2)+(l/2)+2,
									i->BufAbs.a.y+(ITEMHEIGHT/2)+(16)+text_height(ListviewFont)+2);
													 
			if (_RectOverlay(i->BufAbs,w) && i != LISTVIEW(o)->TopItem)
			{
				if ((i->Flags & LVI_SELECTED) || (i == LISTVIEW(o)->DragItem))
				{
					rectfill(buffer,i->RLabel.a.x,i->RLabel.a.y,i->RLabel.b.x,i->RLabel.b.y,SelColor);
					TextColor = makecol(255,255,255);
				} else
					TextColor = ListviewFontColor;
				
				DrawIcon32(buffer,i->Icon,i->RIcon.a.x,i->RIcon.a.y);
					
				DrawNiceTextCenter(buffer, ListviewFont,
					x+(ITEMWIDTH/2), y+(ITEMHEIGHT/2+16)+2,
					ITEMWIDTH, i->Caption, TextColor);
			}

			x += ITEMWIDTH;
			if ( x > o->Absolute.b.x-BORDEROFFSET-ITEMWIDTH ) {
				x  = o->Absolute.a.x+BORDEROFFSET;
				y += ITEMHEIGHT;
				yy += ITEMHEIGHT;
			}

			a = a->Next;
		}
		while ( a != b );

		i = LISTVIEW(o)->TopItem;
		
		if ( i ) 
		if (_RectOverlay(i->BufAbs,w) )
		{
			l = text_length(ListviewFont, i->Caption);
			
			if ( i->BufAbs.a.x != i->BufTop.a.x || i->BufAbs.b.x != i->BufTop.b.x ) {
				
				if ((i->Flags & LVI_SELECTED) || (i == LISTVIEW(o)->DragItem))
				{
					rectfill(buffer,
					i->BufTop.a.x-2,
					i->BufTop.a.y+(ITEMHEIGHT/2)+(16)+2,
	 				i->BufTop.b.x+2,
					i->BufTop.a.y+(ITEMHEIGHT/2)+(16)+text_height(ListviewFont)+2,
					SelColor);
					TextColor = makecol(255,255,255);
				} else
					TextColor = ListviewFontColor;
				
				DrawIcon32(buffer,i->Icon,i->RIcon.a.x,i->RIcon.a.y);
				
				textout_centre_ex(buffer, ListviewFont, i->Caption,
				(i->BufTop.a.x+i->BufTop.b.x)/2,i->BufAbs.a.y+(ITEMHEIGHT/2+16)+2, TextColor, -1);

				
			} else {
				if ((i->Flags & LVI_SELECTED) || (i == LISTVIEW(o)->DragItem))
				{
					rectfill(buffer,i->RLabel.a.x,i->RLabel.a.y,i->RLabel.b.x,i->RLabel.b.y,SelColor);
					TextColor = makecol(255,255,255);
				} else
					TextColor = ListviewFontColor;
				
				DrawIcon32(buffer,i->Icon,i->RIcon.a.x,i->RIcon.a.y);
				
				textout_centre_ex(buffer, ListviewFont, i->Caption,
				i->BufAbs.a.x+(ITEMWIDTH/2),i->BufAbs.a.y+(ITEMHEIGHT/2+16)+2, TextColor, -1);
			}
			
		}


	} else if ( LISTVIEW(o)->Style == LVS_LIST ) {
		
		
		
		PListviewItem point = NULL;
		PListItem ca = NULL,cb = NULL,cia = NULL,cib = NULL;
		
		if ( LISTVIEW(o)->Flags & LVF_ENDODRAG )
			point = ListviewGetItemUnderPoint(LISTVIEW(o), Mouse->State.p);					

		if ( (LISTVIEW(o)->Flags & LVF_COLUMS) && (LISTVIEW(o)->Colums->Last) ) {
			
			l_int cx = o->Absolute.a.x+1-SLLWDG(o)->Pos.x, first = 1;
			
			ca = cb = LISTVIEW(o)->Colums->Last->Next;
			
			if ( LISTVIEW(o)->Flags & LVF_DRAWICONS ) {
				cx += 18;
				first = 0;
			}
			
			rectfill(buffer,o->Absolute.a.x+1,o->Absolute.a.y+1,o->Absolute.b.x-1,o->Absolute.a.y+18,COL_BOXLIGHT);
			
			do {
				if ( first )
					first = 0;
				else {				
					vline(buffer,cx,max(w.a.y,o->Absolute.a.y+1),min(w.b.y,o->Absolute.b.y-1),COL_BOXLIGHT);
					vline(buffer,cx,o->Absolute.a.y+1,o->Absolute.a.y+18,COL_BOXFACE);
				}
				DrawNiceText ( buffer, ListviewFont, cx+2, o->Absolute.a.y+1, LISTVIEWCOL(ca->Data)->Width-3, LISTVIEWCOL(ca->Data)->Caption, COL_BOXTEXT );
				cx += LISTVIEWCOL(ca->Data)->Width+1;
				vline(buffer,cx,o->Absolute.a.y+1,o->Absolute.a.y+18,COL_BOXFACE);
				ca = ca->Next;
			} while ( ca != cb );
						
			y += 18;
			RectAssign(&Clip,o->Absolute.a.x+1,o->Absolute.a.y+19,o->Absolute.b.x-1,o->Absolute.b.y-1);
			RectIntersept(&Clip,Clip,w);
			set_clip_rect(buffer,o->Absolute.a.x+1,o->Absolute.a.y+19,o->Absolute.b.x-1,o->Absolute.b.y-1);
		} else {
			RectAssign(&Clip,o->Absolute.a.x+1,o->Absolute.a.y+1,o->Absolute.b.x-1,o->Absolute.b.y-1);
			RectIntersept(&Clip,Clip,w);
			set_clip_rect(buffer,o->Absolute.a.x+1,o->Absolute.a.y+1,o->Absolute.b.x-1,o->Absolute.b.y-1);
		}
		
		if ( a ) do
		{
			l_int cx;
			i = (PListviewItem)a->Data;
			
			RectAssign ( &i->BufAbs, o->Absolute.a.x+1, y, max(o->Absolute.b.x-1,o->Absolute.a.x+22),	y + 17);
			
			i->BufTop = i->BufAbs;
			
			
			cx = o->Absolute.a.x+1-SLLWDG(o)->Pos.x;
			
			if ( LISTVIEW(o)->Flags & LVF_DRAWICONS ) {
				RectAssign ( &i->RIcon,cx+1,y+1,cx+19,y+19);
				cx += 18;
			}	
			if ( LISTVIEW(o)->Flags & LVF_CHECKBOXES ) {
				RectAssign ( &i->RChk,cx+2,y+4,cx+12,y+14);
				cx += 14;
			}
			
			if ( LISTVIEW(o)->Flags & LVF_COLUMS ) {
				RectAssign ( &i->RLabel, cx+3, y+((16-text_height(ListviewFont))/2)+1,cx+3+min(text_length(ListviewFont,i->Caption),LISTVIEWCOL(ca->Data)->Width-3), y+((16+text_height(ListviewFont))/2)+1);
			} else {
				RectAssign ( &i->RLabel, cx+1, y+((16-text_height(ListviewFont))/2)+1,cx+1+text_length(ListviewFont,i->Caption), y+((16+text_height(ListviewFont))/2)+1);
			}
			
			if (_RectOverlay(i->BufAbs,w))
			{
				l_int cx, first = 1;
				if ((i->Flags & LVI_SELECTED) || (i == LISTVIEW(o)->DragItem))
				{
					rectfill(buffer,
					i->RLabel.a.x,
					i->RLabel.a.y,
					i->RLabel.b.x,
					i->RLabel.b.y,
					SelColor);
					TextColor = ListviewFontColor;
				} else
					TextColor = ListviewFontColor;

				if ( i == point ) {
					if ( LISTVIEW(o)->Flags & LVF_POINTDOWN )
						rectfill(buffer,	i->BufAbs.a.x,
						i->BufAbs.a.y,
						i->BufAbs.b.x,
						i->BufAbs.a.y+1,makecol(128,128,255));
					else
						rectfill(buffer,	i->BufAbs.a.x,
						i->BufAbs.b.y-1,
						i->BufAbs.b.x,
						i->BufAbs.b.y,makecol(128,128,255));
				}

				cx = o->Absolute.a.x+1-SLLWDG(o)->Pos.x;
				
				if ( LISTVIEW(o)->Flags & LVF_DRAWICONS ) {
					DrawIcon16(buffer,i->Icon,cx+1,y+1);
					cx += 18;
					first = 0;	
				}
				
				if ( (LISTVIEW(o)->Flags & LVF_COLUMS) && (i->ColumsData->Last) && (ca)) {
					cia = cib = i->ColumsData->Last->Next;
					
					do {
						if ( first )
							first = 0;
						else
							vline(buffer,cx,i->BufAbs.a.y,i->BufAbs.b.y,COL_BOXLIGHT);
						if ( cia == cib )
							if ( LISTVIEW(o)->Flags & LVF_CHECKBOXES ) {
								rect( buffer,cx+2,y+4,cx+12,y+14,makecol(0,0,0));
								rectfill( buffer,cx+3,y+5,cx+11,y+13,makecol(255,255,255));
								if ( i->Flags & LVI_CHECKED ) rectfill( buffer,cx+4,y+6,cx+10,y+12,makecol(0,0,0));
								cx += 14;
							}
						if ( cia->Data ) DrawNiceText ( buffer, ListviewFont, cx+3, y+((16-text_height(ListviewFont))/2)+1, LISTVIEWCOL(ca->Data)->Width-3, (l_text)cia->Data, TextColor );
						cx += LISTVIEWCOL(ca->Data)->Width+1;
						ca = ca->Next;

					} while ( (cia != cib) && (ca != cb) );
					
				} else {
					if ( LISTVIEW(o)->Flags & LVF_CHECKBOXES ) {
						rect( buffer,cx+2,y+4,cx+12,y+14,makecol(0,0,0));
						rectfill( buffer,cx+3,y+5,cx+11,y+13,makecol(255,255,255));
						if ( i->Flags & LVI_CHECKED ) rectfill( buffer,cx+4,y+6,cx+10,y+12,makecol(0,0,0));
						cx += 14;
					}
					textout_ex(buffer, ListviewFont, i->Caption,cx+1,y+((16-text_height(ListviewFont))/2)+1, TextColor, -1);
				}
			}
			y += 18;
			yy += 18;
			a = a->Next;
		}
		while ( a != b );
		






	} else if ( LISTVIEW(o)->Style == LVS_MOSC ) {

if ( a ) do
		{
			i = (PListviewItem)a->Data;
			
			RectAssign ( &i->BufAbs, x, y, x+200,	 y+50);
			
			RectAssign ( &i->RIcon, i->BufAbs.a.x+1, i->BufAbs.a.y+1, i->BufAbs.a.x+49, i->BufAbs.a.y+49);
			RectAssign ( &i->RLabel, i->BufAbs.a.x+52, (i->BufAbs.a.y+i->BufAbs.b.y-text_height(ListviewFont))/2, i->BufAbs.a.x+52+text_length(ListviewFont,i->Caption), (i->BufAbs.a.y+i->BufAbs.b.y+text_height(ListviewFont))/2);
			
			i->BufTop = i->BufAbs;
			
			if (_RectOverlay(i->BufAbs,w))
			{
				if ((i->Flags & LVI_SELECTED) || (i == LISTVIEW(o)->DragItem))
				{
					rectfill(buffer,i->RLabel.a.x,i->RLabel.a.y,i->RLabel.b.x,i->RLabel.b.y,SelColor);
					rectfill(buffer,i->RIcon.a.x,i->RIcon.a.y,i->RIcon.b.x,i->RIcon.b.y,SelColor);
					TextColor = makecol(255,255,255);
				} else
					TextColor = ListviewFontColor;
				
				DrawIcon48(buffer,i->Icon,i->RIcon.a.x,i->RIcon.a.y);
				
				textout_ex(buffer, ListviewFont, i->Caption,i->RLabel.a.x,i->RLabel.a.y,TextColor, -1);
				
			}
			
			x += 202;
			if ( x+200 > o->Absolute.b.x ) {
				x  = o->Absolute.a.x+BORDEROFFSET;
				y += 52;
				yy += 52;	
			}

			a = a->Next;
		}
		while ( a != b );	


	}

	set_clip_rect(buffer,o->Absolute.a.x+1,o->Absolute.a.y+1,o->Absolute.b.x-1,o->Absolute.b.y-1);


	if ( (LISTVIEW(o)->Flags & LVF_COLUMS) && (LISTVIEW(o)->Style == LVS_LIST)) {

		if ( yy > o->Absolute.b.y-20 ) 
			ScrollBarSetMaxY(SLLWDG(o),yy-(o->Absolute.b.y-20));
		else
			ScrollBarSetMaxY(SLLWDG(o),0);
		
	} else if ( yy > o->Absolute.b.y ) 
		ScrollBarSetMaxY(SLLWDG(o),yy-o->Absolute.b.y);
	else
		ScrollBarSetMaxY(SLLWDG(o),0);
			

}
////////////////////////////////////////////////////////////////////////////////
void ListviewCalculateMax ( PWidget o ) {
	PListItem a = LISTVIEW(o)->Items->Last, b;
	l_int x = o->Relative.a.x+BORDEROFFSET;
	l_int yy= o->Relative.a.y+2;
	l_int width = 0;
	if ( a ) {
		a = a->Next;
		b = a;
	}
	
	if ( LISTVIEW(o)->Style == LVS_ICONS ) {
		if ( a ) {
		yy += ITEMHEIGHT;
		do
		{
			x += ITEMWIDTH;
			if ( x > o->Relative.b.x-BORDEROFFSET-ITEMWIDTH ) {
				x  = o->Relative.a.x+BORDEROFFSET;
				yy += ITEMHEIGHT;
			}

			a = a->Next;
		}
		while ( a != b );
		}
	} else if ( LISTVIEW(o)->Style == LVS_LIST ) {
		
		if ( a ) do {
			yy += 18;
			a = a->Next;
		} while ( a != b );
		
		if ( LISTVIEW(o)->Flags & LVF_COLUMS ) {
			l_int cw = 1;
			PListItem ca, cb;
			ca = cb = LISTVIEW(o)->Colums->Last->Next;
			if ( LISTVIEW(o)->Flags & LVF_DRAWICONS ) cw += 18;
			do {
				cw += LISTVIEWCOL(ca->Data)->Width+1;
				ca = ca->Next;
			} while ( ca != cb );
			width = max(width,cw+2);
		}
	} else if ( LISTVIEW(o)->Style == LVS_MOSC ) {
		if ( a ) do {
			x += 100;
			if ( x+100 > o->Absolute.b.x ) {
				x  = o->Absolute.a.x+BORDEROFFSET;
				yy += 50;	
			}
			a = a->Next;
		}
		while ( a != b );		
	}

	

	if ( width > (o->Relative.b.x-o->Relative.a.x) ) 
		ScrollBarSetMaxX(SLLWDG(o),width-(o->Relative.b.x-o->Relative.a.x));
	else
		ScrollBarSetMaxX(SLLWDG(o),0);
	
	if ( (LISTVIEW(o)->Flags & LVF_COLUMS) && (LISTVIEW(o)->Style == LVS_LIST) ) {

		if ( yy > o->Relative.b.y-20 ) 
			ScrollBarSetMaxY(SLLWDG(o),yy-o->Relative.b.y+20);
		else
			ScrollBarSetMaxY(SLLWDG(o),0);
		
	} else if ( yy > o->Relative.b.y ) 
		ScrollBarSetMaxY(SLLWDG(o),yy-o->Relative.b.y);
	else
		ScrollBarSetMaxY(SLLWDG(o),0);


}
////////////////////////////////////////////////////////////////////////////////
void ListviewUpdateScrollbars ( PListview o ) {

		ListviewCalculateMax(WIDGET(o));
		if ( SLLWDG(o)->Horizontal )
			WidgetDraw(WIDGET(SLLWDG(o)->Horizontal),NULL);
		if ( SLLWDG(o)->Vertical )
			WidgetDraw(WIDGET(SLLWDG(o)->Vertical),NULL);
	
}
////////////////////////////////////////////////////////////////////////////////
void ListviewUpdateAndReDraw ( PListview o ) {
	ListviewUpdateScrollbars(o);
	WidgetDraw(WIDGET(o),NULL);	
}
////////////////////////////////////////////////////////////////////////////////
void ListviewDraw ( PWidget o, p_bitmap buffer, PRect w )
{		if ( UseSkins && SkinListview )
			DrawSkin(buffer, SkinListview, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
		else {
			rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,COL_BOXFACE);
			Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y,COL_BOXLIGHT,COL_BOXDARK);
		     }

		ListviewDrawItems(o, buffer, *w);

	     
}


////////////////////////////////////////////////////////////////////////////////
PListviewItem  ListviewGetItemUnderPoint ( PListview o, TPoint p )
{
	PListItem a = LISTVIEW(o)->Items->Last;

	if ( (LISTVIEW(o)->Flags & LVF_COLUMS) && (LISTVIEW(o)->Style == LVS_LIST) )
		if ( p.x < WIDGET(o)->Absolute.a.y+19 ) return NULL;
	
	if ( a ) do
	{
		if (PointInRect(p, LISTVIEWITEM(a->Data)->BufAbs))
		{
			PListviewItem i  = LISTVIEWITEM(a->Data);
			if ( PointInRect(p, i->RLabel) )
				return i;
			if( o->Style == LVS_ICONS || o->Style == LVS_MOSC ) {
				if ( PointInRect(p, i->RIcon) )
					return i;
			} else {
				if ( LISTVIEW(o)->Flags & LVF_DRAWICONS )
					if ( PointInRect(p, i->RIcon) )
						return i;
				if ( LISTVIEW(o)->Flags & LVF_CHECKBOXES ) 
					if ( PointInRect(p, i->RChk) )
						return i;
			}
			return NULL;
		}

		a = a->Prev;
	}
	while ( a != LISTVIEW(o)->Items->Last );

	return 0;
}
////////////////////////////////////////////////////////////////////////////////
PListItem ListviewGetTopListItem ( PListview o ) {
	PListItem a = o->Items->Last, b = a;
	if ( !a ) return NULL;
	do {
		if (LISTVIEWITEM(a->Data) == o->TopItem ) return a;
		a = a->Prev;
	} while ( a != b );
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PListviewItem ListviewGetTopItem ( PListview o ) {

	return o->TopItem;
}
////////////////////////////////////////////////////////////////////////////////
l_bool ListviewEventHandler ( PWidget o, PEvent Ev )
{
	if (Ev->Type == EV_KEYBOARD) {
	
		if ( SCANCODE(Ev,KEY_RIGHT) || ( (LISTVIEW(o)->Style == LVS_LIST) && SCANCODE(Ev,KEY_DOWN) )) {
			PListItem a = ListviewGetTopListItem(LISTVIEW(o));
			if ( a ) {
				if ( a != LISTVIEW(o)->Items->Last ) 
					ListviewSelectItemAndTell(LISTVIEW(o),LISTVIEWITEM(a->Next->Data),true,(Ev->Keyb.Shift & KB_CTRL));
			} else if ( LISTVIEW(o)->Items->Last ) {
				ListviewSelectItemAndTell(LISTVIEW(o),LISTVIEWITEM(LISTVIEW(o)->Items->Last->Next->Data),true,(Ev->Keyb.Shift & KB_CTRL));
			}
			
		} else if ( SCANCODE(Ev,KEY_LEFT)  || ( (LISTVIEW(o)->Style == LVS_LIST) && SCANCODE(Ev,KEY_UP) )) {
			PListItem a = ListviewGetTopListItem(LISTVIEW(o));
			if ( a ) {
				if ( a != LISTVIEW(o)->Items->Last->Next )	
					ListviewSelectItemAndTell(LISTVIEW(o),LISTVIEWITEM(a->Prev->Data),true,(Ev->Keyb.Shift & KB_CTRL));
			} else if ( LISTVIEW(o)->Items->Last ) {
				ListviewSelectItemAndTell(LISTVIEW(o),LISTVIEWITEM(LISTVIEW(o)->Items->Last->Next->Data),true,(Ev->Keyb.Shift & KB_CTRL));
			}
					
					
		} else if ( SCANCODE(Ev,KEY_HOME)) {
			PListItem a = ListviewGetTopListItem(LISTVIEW(o));
			if ( a || LISTVIEW(o)->Items->Last  )
				if ( a != LISTVIEW(o)->Items->Last->Next )	
					ListviewSelectItemAndTell(LISTVIEW(o),LISTVIEWITEM(LISTVIEW(o)->Items->Last->Next->Data),true,(Ev->Keyb.Shift & KB_CTRL));
					
		} else if ( SCANCODE(Ev,KEY_END)) {
			PListItem a = ListviewGetTopListItem(LISTVIEW(o));
			if ( a || LISTVIEW(o)->Items->Last ) 
				if ( a != LISTVIEW(o)->Items->Last )	
					ListviewSelectItemAndTell(LISTVIEW(o),LISTVIEWITEM(LISTVIEW(o)->Items->Last->Data),true,(Ev->Keyb.Shift & KB_CTRL));
			
		} else if ( SCANCODE(Ev,KEY_ENTER)) {
			if ( LISTVIEW(o)->TopItem && LISTVIEW(o)->OnValMsg )
				WidgetSendSEvent(o, EV_MESSAGE, LISTVIEW(o)->OnValMsg, LISTVIEW(o)->TopItem);
				
		} else if ( !(Ev->Keyb.Shift & ~KB_CTRL) && LISTVIEW(o)->Items->Last ) {
			l_char sel = ToLower(ASCII(Ev));
			PListItem a = ListviewGetTopListItem(LISTVIEW(o)),b;
			if ( !a ) a = LISTVIEW(o)->Items->Last;
			a = a->Next;
			b = a;
			do {
				if ( ToLower(*LISTVIEWITEM(a->Data)->Caption) == sel ) {
					ListviewSelectItemAndTell(LISTVIEW(o),LISTVIEWITEM(a->Data),true,(Ev->Keyb.Shift & KB_CTRL));
					return true;	
				}
				a = a->Next;
			} while ( a != b);
		}	
	} else if (Ev->Type == EV_MESSAGE) {
 
		if ( Ev->Message == WEvNotifyLostFocus ) {
			WidgetDraw(o, NULL);
			return true;
		}
		if ( Ev->Message == WEvNotifyFocused ) {
			WidgetDraw(o, NULL);
			return true;
		}
		if ( Ev->Message == WEvNotifySameFocus ) {
			
			return true;
		}
		if ( Ev->Message == MSG_NOTIFY_AUTO_RESIZE ) {
			ListviewCalculateMax(o);
			return true;
		}	
	} else if (Ev->Type == EV_MOUSE) {
		
		if ( (Ev->Message == MSG_DRAGCANCEL) || ((Ev->Message == WEvMouseLeft) && (o->Flags & WF_DRAGFOCUS)) )
		{ 
			PListviewItem old = LISTVIEW(o)->DragItem;
			LISTVIEW(o)->DragItem = NULL;
			LISTVIEW(o)->TopItem = NULL;
			if ( old )
				WidgetDraw(WIDGET(o), &(old->BufTop));
			return true;
		}
	
		if (Ev->Message == WEvMouseLDClk) {
			PListviewItem p = ListviewGetItemUnderPoint(LISTVIEW(o), Mouse->State.p);
			
			if ( (LISTVIEW(o)->Flags & LVF_CHECKBOXES) && (LISTVIEW(o)->Style == LVS_LIST) )
				if ( PointInRect(Mouse->State.p, p->RChk) )
					return true;
			if ( p )
				if ( LISTVIEW(o)->OnValMsg ) 
					WidgetSendSEvent(o, EV_MESSAGE, LISTVIEW(o)->OnValMsg, p);
			return true;
		}

		if ( Ev->Message == WEvMouseLeft ) {
			LISTVIEW(o)->Flags &= ~(LVF_SIZINGCOL|LVF_ENDODRAG|LVF_HAVESELECT);
			return true;	
		}

		if (Ev->Message == WEvMouseLDown || Ev->Message == WEvMouseRDown)
		{
			if ( (LISTVIEW(o)->Flags & LVF_COLUMS) && (LISTVIEW(o)->Style == LVS_LIST) && ( Mouse->State.p.y < o->Absolute.a.y+18 ) ) {
				PListItem ca,cb;
				l_int cx = o->Absolute.a.x+1-SLLWDG(o)->Pos.x;
				if ( LISTVIEW(o)->Colums->Last ) {
					ca = cb = LISTVIEW(o)->Colums->Last->Next;
					if ( LISTVIEW(o)->Flags & LVF_DRAWICONS ) cx += 18;
					do {
						cx += LISTVIEWCOL(ca->Data)->Width+1;
						if ( (Mouse->State.p.x > cx-2) && (Mouse->State.p.x < cx+2) ) {
							LISTVIEW(o)->SelColum = LISTVIEWCOL(ca->Data);
							if ( LISTVIEW(o)->Flags & LVF_COLRESIZE )
								LISTVIEW(o)->Flags |= LVF_SIZINGCOL;
						}
						ca = ca->Next;
					} while ( ca != cb );
				}
			} else {
				PListviewItem p = 0;
				p = ListviewGetItemUnderPoint(LISTVIEW(o), Mouse->State.p);
				
				if ( p ) {
					
					if ( (LISTVIEW(o)->Flags & LVF_CHECKBOXES) && (LISTVIEW(o)->Style == LVS_LIST) )
						if ( PointInRect(Mouse->State.p, p->RChk) )
							return true;
					
					if ( !( p->Flags & LVI_SELECTED) ) {
						LISTVIEW(o)->Flags |= LVF_HAVESELECT;
						ListviewSelectItem(LISTVIEW(o),p,false,(Ev->Keyb.Shift & KB_CTRL));	
					}
					
				}
			}
			return true;
		}
				
		if ( (Ev->Message == WEvMouseMove) && (Mouse->State.b & BUTTON_LEFT) && !(o->Flags & WF_DRAGFOCUS) ) {
			
			if ( LISTVIEW(o)->Flags & LVF_SIZINGCOL ) {
			
				PListItem ca,cb;
				l_int cx = o->Absolute.a.x+1-SLLWDG(o)->Pos.x;
				ca = LISTVIEW(o)->Colums->Last;
				if ( LISTVIEW(o)->Flags & LVF_DRAWICONS ) cx += 18;
				do {
					ca = ca->Next;
					cx += LISTVIEWCOL(ca->Data)->Width+1;
				} while ( ca->Data != LISTVIEWCOL(LISTVIEW(o)->SelColum) );
			
				if ( (LISTVIEW(o)->SelColum->Width+Mouse->State.p.x-cx) > 20 ) {
					TRect r = o->Absolute;
					//r.a.x = cx-(LISTVIEWCOL(ca->Data)->Width+1);
					LISTVIEW(o)->SelColum->Width += Mouse->State.p.x-cx;
					
					WidgetDraw(o,&r);
					ListviewUpdateScrollbars(LISTVIEW(o));
				}
			} else if ( !(LISTVIEW(o)->Flags & LVF_ORDER) && (LISTVIEW(o)->Flags & LVF_ALLOWDRAG)) {
				PListviewItem p = ListviewGetItemUnderPoint(LISTVIEW(o), Mouse->State.p);
				if ( p ) 
					if ( p->Flags & LVI_SELECTED ) {
						LISTVIEW(o)->DragItem = NULL;
						if ( LISTVIEW(o)->ItemStartDrag ) 
							LISTVIEW(o)->ItemStartDrag(o);
						/*else // Testing purposes ONLY
							DragStart(o,0,0,DMETHOD_COPY|DMETHOD_MOVE);*/
					}
			} else if ( LISTVIEW(o)->Flags & LVF_ENDODRAG ) {
				PListviewItem p = ListviewGetItemUnderPoint(LISTVIEW(o), Mouse->State.p);					
				l_ulong LFlags = LISTVIEW(o)->Flags;
				if ( p ) {
					if ( Mouse->State.p.y < (p->BufAbs.a.y+p->BufAbs.b.y)/2 )
						LISTVIEW(o)->Flags |= LVF_POINTDOWN;
					else
						LISTVIEW(o)->Flags &= ~LVF_POINTDOWN;
					
				}
				if ( (LISTVIEW(o)->Flags != LFlags) || (p != LISTVIEW(o)->LPointed) ) {
					if ( p == LISTVIEW(o)->LPointed ) {	
						if ( p ) WidgetDraw(WIDGET(o), &p->BufAbs);
					} else { 
						if ( LISTVIEW(o)->LPointed ) WidgetDraw(WIDGET(o), &(LISTVIEW(o)->LPointed->BufAbs));
						if ( p ) WidgetDraw(WIDGET(o), &p->BufAbs);
						
				 	}
				}
				LISTVIEW(o)->LPointed = p;
			} else if ( (LISTVIEW(o)->Flags & LVF_ORDER) && (LISTVIEW(o)->Style == LVS_LIST)) {
				PListviewItem p = ListviewGetItemUnderPoint(LISTVIEW(o), Mouse->State.p);
				
				if ( p ) if ( p->Flags & LVI_SELECTED ) {
					LISTVIEW(o)->Flags |= LVF_ENDODRAG;
					if ( Mouse->State.p.y < (p->BufAbs.a.y+p->BufAbs.b.y)/2 )
						LISTVIEW(o)->Flags |= LVF_POINTDOWN;
					else
						LISTVIEW(o)->Flags &= ~LVF_POINTDOWN;
					LISTVIEW(o)->LPointed = p;
					WidgetDraw(o, NULL);
				}
			}
			
			
		}
		
		if ( Ev->Message == WEvMouseLUp || Ev->Message == WEvMouseRUp ) {
			
			PListviewItem s, p = ListviewGetItemUnderPoint(LISTVIEW(o), Mouse->State.p);
					
			if ( LISTVIEW(o)->Flags & LVF_SIZINGCOL ) { 
				
				LISTVIEW(o)->Flags &= ~LVF_SIZINGCOL;
				LISTVIEW(o)->Flags &= ~LVF_HAVESELECT;

			} else if ( LISTVIEW(o)->Flags & LVF_ENDODRAG ) {
				PListItem a = ListFoundItem(LISTVIEW(o)->Items,p);
				PListItem b = GetSelectedListItem(LISTVIEW(o));
				
				if ( a != b ) {
					ListRemoveItemEx(LISTVIEW(o)->Items,b);
					if ( !(LISTVIEW(o)->Flags & LVF_POINTDOWN) ) {
						if ( a == LISTVIEW(o)->Items->Last ) LISTVIEW(o)->Items->Last = b;
						ListAddItemBefore(LISTVIEW(o)->Items,b,a->Next);
					} else 
						ListAddItemBefore(LISTVIEW(o)->Items,b,a);
				}
				LISTVIEW(o)->Flags &= ~LVF_ENDODRAG;
				LISTVIEW(o)->Flags &= ~LVF_HAVESELECT;

				WidgetDraw(o, NULL);
			} else if ( p ) {
				
				if ( !(LISTVIEW(o)->Flags & LVF_HAVESELECT) )
					ListviewSelectItem(LISTVIEW(o),p,false,(Ev->Keyb.Shift & KB_CTRL));	
				
				LISTVIEW(o)->Flags &= ~LVF_HAVESELECT;

				
				if ( Ev->Message == WEvMouseLUp ) {
					
					if ( (LISTVIEW(o)->Flags & LVF_CHECKBOXES) && (LISTVIEW(o)->Style == LVS_LIST) ) {
						if ( PointInRect(Mouse->State.p, p->RChk) ) {
							p->Flags ^= LVI_CHECKED;
							WidgetDraw(WIDGET(o), &p->BufAbs);
							return true;
						}
					} 
					
					if ( o->Flags & WFExecuting ) {
						if ( LISTVIEW(o)->OnValMsg ) WidgetSendSEvent(o, EV_MESSAGE, LISTVIEW(o)->OnValMsg, p);
					} else
						if ( LISTVIEW(o)->OnSelMsg ) WidgetSendSEvent(o, EV_MESSAGE, LISTVIEW(o)->OnSelMsg, p);
				
				} else if ( Ev->Message == WEvMouseRUp && LISTVIEW(o)->OnRClkMsg ) 
					WidgetSendSEvent(o, EV_MESSAGE, LISTVIEW(o)->OnRClkMsg, p);
				return true;
			}

		}
	}
	return ScrollWidgetEventHandler(o,Ev);
}
////////////////////////////////////////////////////////////////////////////////
void FreeListviewItem ( PListviewItem p ) {
	if ( p->Caption ) free(p->Caption);
	if ( p->Icon ) FreeIcon(p->Icon);
	if ( p->ColumsData ) FreeList(p->ColumsData);
	free(p);	
}
////////////////////////////////////////////////////////////////////////////////
void ListviewItemPushCol ( PListviewItem p, l_text Text ) {
	ListAdd( p->ColumsData, NULL, TextDup(Text), free);
}
////////////////////////////////////////////////////////////////////////////////
void ListviewItemSetColCaption( PListviewItem p, l_int col, l_text Caption ) {
	PListItem a = ListAtItem(p->ColumsData,col+1);
	if ( a ) {
		if ( a->Data ) free(a->Data);	
		a->Data = TextDup(Caption);
		if ( col == 0 ) {
			if ( p->Caption ) free(p->Caption);	
			p->Caption = TextDup(Caption);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
PListviewItem ListviewInitItem ( PListviewItem p, l_text Caption, PIcon Icon ) {
	memset(p, 0, sizeof(TListviewItem));
	p->Caption = TextDup(Caption);
	p->Icon = DuplicateIcon(Icon);
	p->ColumsData = NewList();
	ListviewItemPushCol(p,Caption);
	return p;
}
////////////////////////////////////////////////////////////////////////////////
PListviewItem ListviewAddItem ( PListview o, char* Caption, PIcon Icon )
{
	PListviewItem p = malloc(sizeof(TListviewItem));
	if ( !p ) return NULL;
	ListviewInitItem(p,Caption,Icon);
	ListAdd (o->Items, Caption, p, (void*)&FreeListviewItem);
	return p;
}
////////////////////////////////////////////////////////////////////////////////
void ListviewItemRemoveItem ( PListview o, PListviewItem p ) {
	
	PListItem i = ListFoundItem(o->Items,p);
	if ( !i ) return;
	ListRemoveItem(o->Items,i); 
	
}
////////////////////////////////////////////////////////////////////////////////
void ListviewRemoveAllItems ( PListview o )
{
	ScrollBarSetMaxY(SLLWDG(o),0);
	ScrollBarSetMaxX(SLLWDG(o),0);

	if (o->Items) FreeList(o->Items);
	o->Items = NewList();
	o->TopItem = NULL;
}
////////////////////////////////////////////////////////////////////////////////
void ListviewFree ( PWidget o )
{
	FreeList(LISTVIEW(o)->Items);
	FreeList(LISTVIEW(o)->Colums);
}
////////////////////////////////////////////////////////////////////////////////
void Scroll ( PScrollWidget o, TPoint p )
{
	if ( WIDGET(o)->Flags & WFForceBuffer )
		WidgetDraw(WIDGET(o),NULL);
	else
	{
		WIDGET(o)->Flags |= WFForceBuffer;
		WidgetDraw(WIDGET(o),NULL);
		WIDGET(o)->Flags &= ~WFForceBuffer;
	}
}
////////////////////////////////////////////////////////////////////////////////
PListviewItem GetSelectedItem ( PListview o ) {
	PListItem a = o->Items->Last;
	PListviewItem p = 0;
	if ( !a ) return NULL;
	do
	{
		if (LISTVIEWITEM(a->Data)->Flags & LVI_SELECTED)return a->Data;
		a = a->Prev;
	}
	while ( a != o->Items->Last );
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PListItem GetSelectedListItem ( PListview o ) {
	PListItem a = o->Items->Last;
	PListviewItem p = 0;
	if ( !a ) return NULL;
	do
	{
		if (LISTVIEWITEM(a->Data)->Flags & LVI_SELECTED)return a;
		a = a->Prev;
	}
	while ( a != o->Items->Last );
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
l_bool ListviewScrollTo ( PListview o, PListviewItem i ) {
	TPoint Old = SLLWDG(o)->Pos;
	
	if ( !i ) return false;
	
	if ( (LISTVIEW(o)->Flags & LVF_COLUMS) && (LISTVIEW(o)->Style == LVS_LIST) ) {
		
		ScrollBarSetPosY(SLLWDG(o),i->BufAbs.a.y-WIDGET(o)->Absolute.a.y+SLLWDG(o)->Pos.y-20);

	} else {
		ScrollBarSetPosX(SLLWDG(o),i->BufAbs.a.x-WIDGET(o)->Absolute.a.x+SLLWDG(o)->Pos.x);
		ScrollBarSetPosY(SLLWDG(o),i->BufAbs.a.y-WIDGET(o)->Absolute.a.y+SLLWDG(o)->Pos.y);
	}
	
	return (Old.x != SLLWDG(o)->Pos.x)||(Old.y != SLLWDG(o)->Pos.y);
	
}
////////////////////////////////////////////////////////////////////////////////
void ListviewSelectItemAndTell ( PListview o, PListviewItem p, l_bool Scrol, l_bool Multi ) {
	ListviewSelectItem(o,p,Scrol,Multi);
	if ( o->OnSelMsg ) WidgetSendSEvent(WIDGET(o), EV_MESSAGE, o->OnSelMsg, p);
}
////////////////////////////////////////////////////////////////////////////////
void ListviewSelectItem ( PListview o, PListviewItem p, l_bool Scrol, l_bool Multi ) {
	l_bool RedrawAll = Scrol ? ListviewScrollTo(o,p) : false;
	
	PListItem a = o->Items->Last, b = a;

	if ( !a ) return;

	o->TopItem = NULL;
	if ( !Multi || !(o->Flags & LVF_MULTISELECT) ) {
		do {
			if ( LISTVIEWITEM(a->Data)->Flags & LVI_SELECTED ) {
				LISTVIEWITEM(a->Data)->Flags &= ~LVI_SELECTED;
				if ( !RedrawAll ) WidgetDraw(WIDGET(o), &LISTVIEWITEM(a->Data)->BufTop);
		 	}
			a = a->Prev;
		} while ( a != b );
	}
	if ( p ) {
		p->Flags |= LVI_SELECTED;
		o->TopItem = p;
		if ( RedrawAll ) {
			Scroll( SLLWDG(o), SLLWDG(o)->Pos);
			ScrollBarRedraw(SLLWDG(o));
		} else
			WidgetDraw(WIDGET(o), &p->BufTop);	
	}
}
////////////////////////////////////////////////////////////////////////////////
void ListviewSelectIndex ( PListview o, l_ulong Index, l_bool Scroll, l_bool Multi ) {
	if ( Index ) {
		PListviewItem a = ListAt(o->Items,Index);
		if ( a ) ListviewSelectItem(o,a,Scroll,Multi);
	} else
		ListviewSelectItem(o,NULL,Scroll,Multi);
}
////////////////////////////////////////////////////////////////////////////////
l_ulong ListviewItemIndex ( PListview o, PListviewItem a ) {
	return ListFoundIndex(o->Items,a);
}
////////////////////////////////////////////////////////////////////////////////
l_text ListviewRenameItemBox ( PListview o, PListviewItem i ) {
	
	PTextbox tb;
	TRect 		r;
	l_text ret = NULL;

	if ( o->Style == LVS_MOSC ) 
		RectAssign(&r,i->BufTop.a.x+50,(i->BufTop.a.y+i->BufTop.b.y-text_height(ListviewFont)-4)/2,i->BufTop.b.x,(i->BufTop.a.y+i->BufTop.b.y+text_height(ListviewFont)+4)/2);
	else if ( o->Style == LVS_LIST ) {
		RectAssign(&r,i->BufTop.a.x,i->BufTop.a.y,i->BufTop.b.x,i->BufTop.b.y);
		if ( LISTVIEW(o)->Flags & LVF_CHECKBOXES ) r.a.x += 14;
		if ( LISTVIEW(o)->Flags & LVF_DRAWICONS ) r.a.x += 18;
		if ( (LISTVIEW(o)->Flags & LVF_COLUMS) && (LISTVIEW(o)->Colums->Last) ) r.b.x = r.a.x + LISTVIEWCOL(LISTVIEW(o)->Colums->Last->Next->Data)->Width;
	} else
		RectAssign(&r,i->BufTop.a.x,i->BufTop.b.y-text_height(ListviewFont)-4,i->BufTop.b.x,i->BufTop.b.y);
	
	tb = CreateTextbox(&Me,r,TBF_EDITABLE);
	InsertWidget(DeskTop, WIDGET(tb));
	
	TextBoxSetTextEx(tb,i->Caption);

	tb->SelPos = tb->Text;
	tb->SelPosEnd = tb->Text+TextLen(i->Caption);
	WidgetDraw(WIDGET(tb), NULL);
	
	
	tb->ValidMsg = MSG_OK;

	WidgetSetFocus(WIDGET(tb));
	
	if ( WidgetExecute(WIDGET(tb)) != MSG_CANCEL ) {
		if ( TextCaseCompare(tb->Text,i->Caption) )
			ret = TextDup(tb->Text);
	}
	WidgetDispose(WIDGET(tb));

	return ret;	
}
////////////////////////////////////////////////////////////////////////////////
l_ulong ListviewCountSelectedFiles ( PListview o ) {
	l_ulong Nb = 0;
	PListItem a = o->Items->Last;
	if ( !a ) return 0;
	do
	{
		if (LISTVIEWITEM(a->Data)->Flags & LVI_SELECTED) Nb++;
		a = a->Prev;
	}
	while ( a != o->Items->Last );
	return Nb;
}
////////////////////////////////////////////////////////////////////////////////
l_ulong ListviewEstimateHeight ( PList Items ) {
	l_ulong H = 4;
	PListItem a = Items->Last;
	if ( !a ) return 0;
	do {
		H += 18;
		a = a->Prev;
	} while ( a != Items->Last );
	return H;	
}
////////////////////////////////////////////////////////////////////////////////
void FreeListviewCol ( PListviewCol o ) {
	
	if ( o->Caption ) free(o->Caption);
	if ( o->Icon ) FreeIcon(o->Icon);
	free(o);
}
////////////////////////////////////////////////////////////////////////////////
void ListviewSetColSize( PListview p, l_int col, l_ulong Width ) {
	PListviewCol c = ListAt(p->Colums,col+1);
	if ( c ) {
		c->Width = Width;
		ListviewCalculateMax(WIDGET(p));
	}
}
////////////////////////////////////////////////////////////////////////////////
/** Add a colum to a listview
	@param p Listview to affect
	@param Caption Caption of the new colum
	@param Icon Icon of the new colum
	@param Width Width in pixel of the new colum
	@param Flags Flags of the new colum 
*/
void ListviewAddColum ( PListview p, l_text Caption, PIcon Icon, l_int Width, l_ulong Flags ) {
	PListviewCol o = malloc(sizeof(TListviewCol));
	if ( !o ) return;
	memset(o,0,sizeof(TListviewCol));
	o->Caption = TextDup(Caption);
	o->Icon = DuplicateIcon(Icon);
	o->Width = Width;
	o->Flags = Flags;
	ListAdd(p->Colums,NULL,o,(void*)&FreeListviewCol);
	ListviewCalculateMax(WIDGET(p));
}
////////////////////////////////////////////////////////////////////////////////
/** Change the displaying style of a listview
	@param p Listview to affect
	@param Style New style
	@param Flags If set, change the flags to the specified value
	@note Redraw widget and scrollbars
*/
void ListviewSetStyle ( PListview p, l_int Style, l_int Flags ) {
	p->Style = Style;
	if ( Flags ) p->Flags = Flags;
	ScrollBarChangeType(SLLWDG(p),((p->Flags & LVF_COLUMS) && (p->Style == LVS_LIST))?3:1);
}
////////////////////////////////////////////////////////////////////////////////
/** Initialise a listview widget, or a listview derivated widget.
	@param p listview to initialise
	@param Class Class of widget (ListviewClass by default)
	@param App Application which want to allocate the widget( use &Me in most cases)
	@param r Relative coordinates of new widget
	@param CaptionColumName First colum caption, the one wich is associated to all item 'caption' field
	@param Style Displaying style of items
	@param Flags Flags
*/
PListview InitListview( PListview p, PWidgetClass Class, PApplication App, TRect r, l_text CaptionColumName, l_uchar Style, l_ulong Flags )
{
	if (!p) return NULL;

	memset(p, 0, sizeof(TListview));
	p->Items  = NewList();
	p->Colums = NewList();
	p->Style = Style;
	p->Flags = Flags;
	InitScrollWidget(SLLWDG(p), Class?Class:ListviewClass, App, r, ((Flags & LVF_COLUMS) && (Style == LVS_LIST))?3:1);
	WIDGET(p)->Flags |= WFFocusable;
	SLLWDG(p)->Delta.y = 20;
	SLLWDG(p)->Delta.x = 20;
	SLLWDG(p)->Scrolled = &Scroll;
	WIDGET(p)->Draw = &ListviewDraw;
	WIDGET(p)->EventHandler = &ListviewEventHandler;
	WIDGET(p)->FreeEx = &ListviewFree;
	WIDGET(p)->DropAccept 		= &ListviewDropAccept;
	WIDGET(p)->DragBeforeDrop 	= &ListviewDragBeforeDrop;
	WIDGET(p)->DropData 		= &ListviewDropData;
	WIDGET(p)->DragAfterDrop 	= &ListviewDragAfterDrop;	
	
	ListviewAddColum(p,CaptionColumName?CaptionColumName:"Caption",NULL,200,0);
	return p;
}
////////////////////////////////////////////////////////////////////////////////
/** Creates a listview widget.
	@param App Application which want to allocate the widget( use &Me in most cases)
	@param r Relative coordinates of new widget
	@param CaptionColumName First colum caption, the one wich is associated to all item 'caption' field
	@param Style Displaying style of items
	@param Flags Flags
	@see WidgetInsert	
*/
PListview CreateListview( PApplication App, TRect r, l_text CaptionColumName, l_uchar Style, l_ulong Flags )
{
	PListview p = malloc(sizeof(TListview));
	if (!p) return NULL;
	return InitListview(p,ListviewClass,App,r,CaptionColumName,Style,Flags);
}
////////////////////////////////////////////////////////////////////////////////
PWidget  StdCreateListview ( PApplication App, TRect r )
{
	return WIDGET(CreateListview( App, r, "Caption", 0, 0));
}
////////////////////////////////////////////////////////////////////////////////
l_bool LibMain ( int argc, l_text *argv )
{
	ListviewClass = CreateWidgetClass( ULONG_ID('L','s','t','V'), ScollWidgetClass, 0 );
	ListviewClass->StdCreate = &StdCreateListview;
	
	SkinListview = GetSkinItem("Listview");
	
	if ( UseSkins && SkinListview ) {
		ListviewFont = SkinListview->Font;
		ListviewFontColor = SkinListview->FontColor;
	} else {
		ListviewFont = default_font;	
		ListviewFontColor = makecol(0,0,0);
	}
	
	APPEXPORT(CreateListview);
	APPEXPORT(InitListview);
	APPEXPORT(ListviewAddItem);
	APPEXPORT(ListviewRemoveAllItems);
	APPEXPORT(ListviewEventHandler);
	APPEXPORT(GetSelectedItem);
	APPEXPORT(GetSelectedListItem);
	APPEXPORT(ListviewSelectItem);
	APPEXPORT(ListviewSelectIndex);
	APPEXPORT(ListviewItemIndex);
	APPEXPORT(ListviewSelectItemAndTell);
	APPEXPORT(ListviewRenameItemBox);
	APPEXPORT(FreeListviewItem);
	APPEXPORT(ListviewItemRemoveItem);
	APPEXPORT(ListviewCountSelectedFiles);
	APPEXPORT(ListviewGetItemUnderPoint);
	APPEXPORT(ListviewEstimateHeight);
	
	APPEXPORT(ListviewInitItem);
	APPEXPORT(ListviewItemPushCol);
	APPEXPORT(ListviewAddColum);
	APPEXPORT(ListviewItemSetColCaption);
	
	APPEXPORT(ListviewUpdateScrollbars);
	APPEXPORT(ListviewClass);
	APPEXPORT(ListviewSetStyle);
	APPEXPORT(ListviewFree);
	APPEXPORT(ListviewSetColSize);
	APPEXPORT(ListviewUpdateAndReDraw);
	APPEXPORT(ListviewCalculateMax);
	APPEXPORT(ListviewScrollTo);
	return true;
	
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{

}
////////////////////////////////////////////////////////////////////////////////
/**
@}
*/
