////////////////////////////////////////////////////////////////////////////////
//
//  Treeview Widget - Core File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "treeview.h"
#include "tbox.h"

////////////////////////////////////////////////////////////////////////////////
l_ulong	AppVersion = ULONG_ID(0,0,0,1);
l_char	AppName[] = "Treeview Widget";
l_uid	nUID = "treeview";
l_uid NeededLibs[] = { "widget", "skin","textbox","menu","" };

PSkin SkinTreeview = 0;

/** @defgroup treeview Treeview widget
	@ingroup widgets
	@{
*/

/**	@file
	Treeview core file.
*/
PWidgetClass TreeviewClass;

PTreeItem TreeviewInitItem ( PTreeItem i, l_text Caption, l_text Key, PIcon Icon, void (*GenerateSubs)(PTreeItem), void (*FreeEx) ( PTreeviewItem ) ) {
	memset(i,0,sizeof(TTreeItem));
	i->Caption = TextDup(Caption);
	i->Key = TextDup(Key);
	i->Icon = DuplicateIcon(Icon);
	i->GenerateSubs = GenerateSubs;
	i->FreeEx = FreeEx;
	return i;
}
////////////////////////////////////////////////////////////////////////////////
void TreeviewAddItem ( PTreeItem Parent, PTreeItem i ) {
	i->Parent = Parent;
	if ( Parent->Last ) {
		i->Next = Parent->Last->Next;
		i->Prev = Parent->Last;
		Parent->Last->Next->Prev = i;
		Parent->Last->Next = i;
		Parent->Last = i;
	} else {
		i->Next = i;
		i->Prev = i;
		Parent->Last = i;
	}
}
////////////////////////////////////////////////////////////////////////////////
PTreeItem AddTreeItem ( PTreeItem o, l_text Caption, l_text Key, PIcon Icon,void  (*GenerateSubs)(PTreeItem) ) {
	PTreeItem i = malloc(sizeof(TTreeItem));
	if ( !i ) return NULL;
	TreeviewInitItem(i,Caption,Key,Icon,GenerateSubs,NULL);
	TreeviewAddItem(o,i);
	return i;
}
////////////////////////////////////////////////////////////////////////////////
void DrawTreeItem ( p_bitmap Out, PTreeview t, PTreeItem o, TRect Abs, l_int *x, l_int *y, l_char Flags ,l_char Delta, l_char DeltaY, l_ulong *n, l_ulong ss,l_int SelColor ) {

	RectAssign(&o->BufAbs,*x,*y,(*x)+text_length(default_font,o->Caption)+Delta+2,(*y)+18);

	if ( _RectOverlay(o->BufAbs,Abs) ) {

		if ( ( o->Last || (o->GenerateSubs && !(o->Flags & TIF_EXPENDED)) ) && (Flags & TVF_DRAWPLUSMINUS) ) {
			rect(Out,o->BufAbs.a.x+3, o->BufAbs.a.y+3, o->BufAbs.a.x+11, o->BufAbs.a.y+11,makecol(128,128,128));
			hline(Out,o->BufAbs.a.x+5,o->BufAbs.a.y+7,o->BufAbs.a.x+9,makecol(0,0,0));
		    if ( !(o->Flags & TIF_EXPENDED) ) vline(Out,o->BufAbs.a.x+7,o->BufAbs.a.y+5,o->BufAbs.a.y+9,makecol(0,0,0));
		}

		if ( o->Icon && (Flags & TVF_DRAWICONS) ) {
			DrawIcon16(Out,o->Icon,o->BufAbs.a.x+Delta-16,o->BufAbs.a.y+1);
		}

	  if ( (o->Flags & TIF_SELECTED) || (t->DragItem == o) ) {
	    rectfill(Out, Delta+o->BufAbs.a.x+2, o->BufAbs.a.y+DeltaY, o->BufAbs.b.x, o->BufAbs.a.y+DeltaY+text_height(default_font), SelColor);
	    textout_ex(Out, default_font, o->Caption, Delta+3+o->BufAbs.a.x, o->BufAbs.a.y+DeltaY, makecol(255,255,255), -1);
	  } else
	    textout_ex(Out, default_font, o->Caption, Delta+3+o->BufAbs.a.x, o->BufAbs.a.y+DeltaY, makecol(0,0,0), -1);
	}

	if (Flags & TVF_DRAWICONS )
		(*y) += 19;
	else
		(*y) += text_height(default_font)+1;


	(*n)++;


	if ( o->Last && ( o->Flags & TIF_EXPENDED )  ) {
		PTreeItem a = o->Last->Next;
		PTreeItem b = a;
	  (*x) += 16;
		do {
			DrawTreeItem(Out,t,a,Abs,x,y,Flags,Delta,DeltaY,n,ss,SelColor);
			a = a->Next;
		} while ( a != b );
		(*x) -= 16;
	}
}
////////////////////////////////////////////////////////////////////////////////
void TreeViewDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	l_int x = o->Absolute.a.x-SLLWDG(o)->Pos.x, y = o->Absolute.a.y-(SLLWDG(o)->Pos.y);
	l_char Delta = 0, DeltaY = 0;
	l_int SelColor = ( o->Flags & WFFocused ) ? makecol(0,0,255) : makecol(215,215,215);

		if ( UseSkins && SkinTreeview )
			DrawSkin(buffer, SkinTreeview, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
		else {
			rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,COL_BOXFACE);
			Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y,COL_BOXLIGHT,COL_BOXDARK);
		     }
	


	if ( (TREEVIEW(o)->Flags & TVF_DRAWPLUSMINUS) && (TREEVIEW(o)->Flags & TVF_DRAWICONS) )
		Delta = 32;
	else
		Delta = 16;

	if ( TREEVIEW(o)->Flags & TVF_DRAWICONS ) DeltaY = (18-text_height(default_font))/2;

	if ( TREEVIEW(o)->Items->Last  ) {
		PTreeItem a = TREEVIEW(o)->Items->Last->Next;
		PTreeItem b = a;
		l_ulong n = 0;

		do {
			DrawTreeItem(buffer,TREEVIEW(o),a,*w,&x,&y,TREEVIEW(o)->Flags,Delta,DeltaY,&n, SLLWDG(o)->Pos.y,SelColor );

			a = a->Next;
		} while ( a != b );

  }
}
////////////////////////////////////////////////////////////////////////////////
void CalculateMaxEx ( PTreeItem o, l_long *n, l_long Delta, l_long *x, l_long *mx ) {

	*mx = max ( *mx, (*x)+text_length(default_font,o->Caption)+Delta+4 );

	(*n)++;
	if ( o->Last && ( o->Flags & TIF_EXPENDED )  ) {
		PTreeItem a = o->Last->Next;
		PTreeItem b = a;
		(*x) += 16;
		do {
			CalculateMaxEx(a,n,Delta,x,mx);
			a = a->Next;
		} while ( a != b );
		(*x) -= 16;
	}
}
////////////////////////////////////////////////////////////////////////////////
l_ulong CalculateMax ( PTreeView o ) {
	
	PTreeItem a,b;
	l_long n = 0;
	l_long x = 0;
	l_long mx = 0;

	if ( o->Items->Last ) {
		a = b = o->Items->Last->Next;
		do {
			CalculateMaxEx(a,&n,32,&x,(l_ulong*)&mx );
			a = a->Next;
		} while ( a != b );
	}
	
	ScrollBarSetMaxY(SLLWDG(o),max(0,(((l_long)n*19)-((WIDGET(o)->Absolute.b.y-WIDGET(o)->Absolute.a.y))-1)));
	ScrollBarSetMaxX(SLLWDG(o),max(0,(mx-(WIDGET(o)->Absolute.b.x-WIDGET(o)->Absolute.a.x))));


	//*******
	if ( SLLWDG(o)->Pos.x > SLLWDG(o)->Max.x  ) ScrollBarSetPosX(SLLWDG(o),SLLWDG(o)->Max.x);
	if ( SLLWDG(o)->Pos.y > SLLWDG(o)->Max.y  ) ScrollBarSetPosY(SLLWDG(o),SLLWDG(o)->Max.y);
	//*******
}
////////////////////////////////////////////////////////////////////////////////
void FreeTreeItem ( PTreeItem o ) {
	if ( o->Caption ) free(o->Caption);
	if ( o->Key ) free(o->Key);
	if ( o->Icon ) FreeIcon(o->Icon);
	if ( o->Last ) {
		PTreeItem a = o->Last->Next;
		PTreeItem b = a, n = a;
		do {
			n = a->Next;
			FreeTreeItem(a);
			a = n;
		} while ( a != b );
	}
	if ( o->FreeEx ) o->FreeEx(o);	
	free(o);
}
////////////////////////////////////////////////////////////////////////////////
void EmptySubTreeItems (PTreeItem o ) {
	if ( o->Last ) {
		PTreeItem a = o->Last->Next;
		PTreeItem b = a, n = a;
		do {
			n = a->Next;
			FreeTreeItem(a);
			a = n;
		} while ( a != b );
	}
	o->Last = NULL;
}
////////////////////////////////////////////////////////////////////////////////
void TreeViewFreeEx ( PWidget o ) {

	if ( TREEVIEW(o)->Items ) FreeTreeItem(TREEVIEW(o)->Items);


}

////////////////////////////////////////////////////////////////////////////////
l_bool FoundItemOnPoint ( PTreeItem o, TPoint p, PTreeItem *f ) {

	if ( PointInRect(p,o->BufAbs) ) {
	  *f = o;
	  return true;
	}

	if ( o->Last && ( o->Flags & TIF_EXPENDED ) ) {
		PTreeItem a = o->Last->Next;
		PTreeItem b = a;
		do {
			if ( FoundItemOnPoint(a,p,f) ) return true;
			a = a->Next;
		} while ( a != b );
	}

	return false;

}
////////////////////////////////////////////////////////////////////////////////
void TreeviewScrollBarRecalculate( PTreeView o ) {

	CalculateMax(o);

}
////////////////////////////////////////////////////////////////////////////////
void TreeviewExpendItem ( PTreeView o, PTreeItem i ) {

	if ( i->GenerateSubs ) {
		//EmptySubTreeItems(i);
		i->GenerateSubs(i);
	}
	i->Flags |= TIF_EXPENDED;

	CalculateMax(o);

	WidgetDraw(WIDGET(o), NULL);

	WidgetDraw(WIDGET(SLLWDG(o)->Vertical), NULL);
  WidgetDraw(WIDGET(SLLWDG(o)->Horizontal), NULL);


}

////////////////////////////////////////////////////////////////////////////////
void TreeviewUnExpendItem ( PTreeView o, PTreeItem i ) {

	i->Flags &= ~TIF_EXPENDED;

	CalculateMax(o);

	WidgetDraw(WIDGET(o), NULL);

	WidgetDraw(WIDGET(SLLWDG(o)->Vertical), NULL);
  WidgetDraw(WIDGET(SLLWDG(o)->Horizontal), NULL);

}
////////////////////////////////////////////////////////////////////////////////
void TreeViewSelectItem ( PTreeView o, PTreeItem i ) {

	if ( o->Sel != i ) {
		if ( o->Sel ) {
			o->Sel->Flags &= ~TIF_SELECTED;
		}
	}

  i->Flags |= TIF_SELECTED;
  o->Sel = i;

	if ( o->Flags &= NO_SCROLL ) {
	WidgetDraw(WIDGET(o), NULL);
	}

	else {

	WidgetDraw(WIDGET(o), NULL);
	ScrollBarSetPosX(SLLWDG(o),i->BufAbs.a.x-WIDGET(o)->Absolute.a.x+SLLWDG(o)->Pos.x);
	ScrollBarSetPosY(SLLWDG(o),i->BufAbs.a.y-WIDGET(o)->Absolute.a.y+SLLWDG(o)->Pos.y);
	WidgetDraw(WIDGET(o), NULL);
	ScrollBarRedraw(SLLWDG(o));
	}
}
////////////////////////////////////////////////////////////////////////////////
void TreeViewSelectItemAndTell ( PTreeView o, PTreeItem i ) {

	if ( o->Sel != i ) {
		if ( o->Sel ) {
			o->Sel->Flags &= ~TIF_SELECTED;
		}
	}

  i->Flags |= TIF_SELECTED;
  o->Sel = i;
	
	if ( o->Flags &= NO_SCROLL ){
	WidgetDraw(WIDGET(o), NULL);
	}
	else{
	WidgetDraw(WIDGET(o), NULL);
	ScrollBarSetPosX(SLLWDG(o),i->BufAbs.a.x-WIDGET(o)->Absolute.a.x+SLLWDG(o)->Pos.x);
	ScrollBarSetPosY(SLLWDG(o),i->BufAbs.a.y-WIDGET(o)->Absolute.a.y+SLLWDG(o)->Pos.y);
	WidgetDraw(WIDGET(o), NULL);
	ScrollBarRedraw(SLLWDG(o));
	}
	if ( TREEVIEW(o)->OnSelMsg )
		WidgetSendSEvent(WIDGET(o), EV_MESSAGE, o->OnSelMsg, o->Sel);


}
////////////////////////////////////////////////////////////////////////////////
PTreeItem TreeViewGetFirstFromByCharEx ( PTreeItem i, PTreeItem from, l_char c, l_bool *ok ) {
	PTreeItem r = NULL;
	if ( i->Caption ) if ( ToLower(*i->Caption) == c && *ok ) return i;
	if ( i == from ) *ok = true;
	if ( i->Last && ( i->Flags & TIF_EXPENDED )  ) {
		PTreeItem a = i->Last->Next;
		PTreeItem b = a;
		do {
			if ( r = TreeViewGetFirstFromByCharEx(a,from,c,ok) ) return r;
			a = a->Next;
		} while ( a != b );
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PTreeItem TreeViewGetFirstByCharEx ( PTreeItem i, l_char c ) {
	PTreeItem r = NULL;
	if ( i->Caption ) if ( ToLower(*i->Caption) == c ) return i;
	if ( i->Last && ( i->Flags & TIF_EXPENDED )  ) {
		PTreeItem a = i->Last->Next;
		PTreeItem b = a;
		do {
			if ( r = TreeViewGetFirstByCharEx(a,c) ) return r;
			a = a->Next;
		} while ( a != b );
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PTreeItem TreeViewGetNextEx ( PTreeItem i, PTreeItem who, l_bool *ok ) {
	PTreeItem r = NULL;
	if ( *ok ) return i;
	if ( i == who ) *ok = true;
	if ( i->Last && ( i->Flags & TIF_EXPENDED )  ) {
		PTreeItem a = i->Last->Next;
		PTreeItem b = a;
		do {
			if ( r = TreeViewGetNextEx(a,who,ok) ) return r;
			a = a->Next;
		} while ( a != b );
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PTreeItem TreeViewGetPrevEx ( PTreeItem i, PTreeItem who, PTreeItem *prev ) {
	PTreeItem r = NULL;
	if ( i == who ) return *prev;
	*prev = i;
	if ( i->Last && ( i->Flags & TIF_EXPENDED )  ) {
		PTreeItem a = i->Last->Next;
		PTreeItem b = a;
		do {
			if ( r = TreeViewGetPrevEx(a,who,prev) ) return r;
			a = a->Next;
		} while ( a != b );
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PTreeItem TreeViewGetNext ( PTreeView o, PTreeItem ii ) {
	if ( ii ) {
		l_bool ok = false;
		PTreeItem i = TreeViewGetNextEx(o->Items,ii,&ok);
		if ( !i ) i = ii;
		return i;
	} else if ( o->Items->Last )
		return o->Items->Last->Next;
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PTreeItem TreeViewGetPrev ( PTreeView o, PTreeItem ii ) {
	if ( ii ) {
		PTreeItem tmp = NULL;
		PTreeItem i = TreeViewGetPrevEx(o->Items,ii,&tmp);
		if ( i == o->Items || !i ) i = ii;
		return i;
	} else if ( o->Items->Last )
		return o->Items->Last->Next;
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PTreeItem TreeViewGetByCharFrom ( PTreeView o, PTreeItem ii, l_uchar c ) {
	l_bool ok = !(ii);
	PTreeItem i = TreeViewGetFirstFromByCharEx(o->Items,ii,c,&ok);
	if ( !i ) i = TreeViewGetFirstByCharEx(o->Items,c);
	if ( !i ) i = ii;
	return i;
}
////////////////////////////////////////////////////////////////////////////////
PTreeItem TreeViewGetLast ( PTreeView o ) {
	PTreeItem i = TREEVIEW(o)->Items->Last;
	if ( !i ) return NULL;
	while ( i->Last && ( i->Flags & TIF_EXPENDED ) ) {
		i = i->Last;
	}
	return i;
}
////////////////////////////////////////////////////////////////////////////////
PTreeItem TreeviewGetItemUnderPoint ( PTreeView o, TPoint p ) {
	PTreeItem i = NULL;
	if ( FoundItemOnPoint(TREEVIEW(o)->Items, p, &i) ) return i;
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
l_bool TreeviewDropAccept ( PWidget o, PWidget Caller, l_ulong Type, void *Data, l_ulong SupportedMethods, l_ulong *Method ) {
	if ( TREEVIEW(o)->Flags & TVF_ALLOWDRAG ) {

		PTreeviewItem old;
		PTreeviewItem p = TreeviewGetItemUnderPoint(TREEVIEW(o), Mouse->State.p);					
		
		if ( p != TREEVIEW(o)->DragItem ) {
			PTreeviewItem old = TREEVIEW(o)->DragItem;
			TREEVIEW(o)->DragItem = p;
			if ( old ) WidgetDraw(WIDGET(o), &(old->BufAbs));
			if ( TREEVIEW(o)->DragItem ) WidgetDraw(WIDGET(o), &(TREEVIEW(o)->DragItem->BufAbs));
		}
		
		if ( !p ) return false;
		
		if ( o == Caller )
			if ( p->Flags & TIF_SELECTED ) return false;
			
		if ( TREEVIEW(o)->ItemDropAccept ) 
			return TREEVIEW(o)->ItemDropAccept(o,Caller,TREEVIEW(o)->DragItem,Type,Data,SupportedMethods,Method);
	}
		
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool TreeviewDragBeforeDrop    ( PWidget o, PWidget Dest, void *Data, l_ulong Type, l_ulong Method ) {
	if ( o == Dest )
		if ( TREEVIEW(o)->DragItem )
			if ( TREEVIEW(o)->DragItem->Flags & TIF_SELECTED )
				return false;
	if ( TREEVIEW(o)->ItemDragBeforeDrop ) 
		return TREEVIEW(o)->ItemDragBeforeDrop(o,Dest,(o==Dest)?TREEVIEW(o)->DragItem:NULL,Data,Type,Method);
	
	return false;
}
////////////////////////////////////////////////////////////////////////////////
void TreeviewDropData    ( PWidget o, PWidget Caller, void *Data, l_ulong Type, l_ulong Method ) {
	PTreeviewItem old = TREEVIEW(o)->DragItem;
	
	TREEVIEW(o)->DragItem = NULL;
	
	TreeViewSelectItem(TREEVIEW(o),old);
	
	if ( TREEVIEW(o)->ItemDropData ) 
		TREEVIEW(o)->ItemDropData(o,Caller,old,Data,Type,Method);
	
}
////////////////////////////////////////////////////////////////////////////////
void TreeviewDragAfterDrop ( PWidget o, PWidget Dest, void*Data, l_ulong Type, l_ulong Method ) {

	if ( TREEVIEW(o)->ItemDragAfterDrop ) 
		TREEVIEW(o)->ItemDragAfterDrop(o,Dest,(o==Dest)?TREEVIEW(o)->DragItem:NULL,Data,Type,Method);
}

////////////////////////////////////////////////////////////////////////////////
l_bool TreeViewEventHandler ( PWidget o, PEvent Ev ) {

	if (Ev->Type == EV_KEYBOARD) {
		if ( SCANCODE(Ev,KEY_DOWN) ) {
			PTreeItem i = TreeViewGetNext(TREEVIEW(o),TREEVIEW(o)->Sel);
			if ( i )
				TreeViewSelectItemAndTell(TREEVIEW(o),i);
		} else if ( SCANCODE(Ev,KEY_UP) ) {
			PTreeItem i = TreeViewGetPrev(TREEVIEW(o),TREEVIEW(o)->Sel);
			if ( i )
				TreeViewSelectItemAndTell(TREEVIEW(o),i);
		} else if ( SCANCODE(Ev,KEY_HOME) ) {
			if ( TREEVIEW(o)->Items->Last )
				TreeViewSelectItemAndTell(TREEVIEW(o),TREEVIEW(o)->Items->Last->Next);
		} else if ( SCANCODE(Ev,KEY_END) ) {
			PTreeItem i = TreeViewGetLast(TREEVIEW(o));
			if ( i )
				TreeViewSelectItemAndTell(TREEVIEW(o),i);
		} else if ( SCANCODE(Ev,KEY_ENTER) ) {
			if ( TREEVIEW(o)->Sel ) {
				if ( TREEVIEW(o)->Sel->Flags & TIF_EXPENDED )
					TreeviewUnExpendItem(TREEVIEW(o),TREEVIEW(o)->Sel);
				else
					TreeviewExpendItem(TREEVIEW(o),TREEVIEW(o)->Sel);
			}
		} else if ( !Ev->Keyb.Shift ) {
			l_char sel = ToLower(ASCII(Ev));
			PTreeItem i = TreeViewGetByCharFrom(TREEVIEW(o),TREEVIEW(o)->Sel,sel);
			if ( i )
				TreeViewSelectItemAndTell(TREEVIEW(o),i);
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
			CalculateMax(TREEVIEW(o));
			return true;
		}


	} else if ( Ev->Type == EV_MOUSE )
	{
		
		if ( (Ev->Message == MSG_DRAGCANCEL) || ((Ev->Message == WEvMouseLeft) && (o->Flags & WF_DRAGFOCUS)) )
		{ 
			PTreeviewItem old = TREEVIEW(o)->DragItem;
			TREEVIEW(o)->DragItem = NULL;
			if ( old )
				WidgetDraw(WIDGET(o), &(old->BufAbs));
			return true;
		}
		
		if ( Ev->Message == WEvMouseLDClk )
		{
			PTreeItem i = NULL;
			//l_bool SendMsg = 1;

			if ( FoundItemOnPoint(TREEVIEW(o)->Items, Mouse->State.p, &i) )
			{
				if ( i->Flags & TIF_EXPENDED )
					TreeviewUnExpendItem(TREEVIEW(o), i);
				else
					TreeviewExpendItem(TREEVIEW(o), i);
			}

			return true;
		}
		else if ( (Ev->Message == WEvMouseMove) && (Mouse->State.b & BUTTON_LEFT) && !(o->Flags & WF_DRAGFOCUS) ) {
			
			if ( TREEVIEW(o)->Flags & TVF_ALLOWDRAG ) {
				PTreeviewItem p = TreeviewGetItemUnderPoint(TREEVIEW(o), Mouse->State.p);
				if ( p ) 
					if ( p->Flags & TIF_SELECTED ) {
						TREEVIEW(o)->DragItem = NULL;
						
						if ( TREEVIEW(o)->ItemStartDrag ) 
							TREEVIEW(o)->ItemStartDrag(o);
					}
			}		
		
		}
		else if ( Ev->Message == WEvMouseLDown || Ev->Message == WEvMouseRDown )
		{
			PTreeItem i = NULL;
			l_bool SendMsg = 1;

			if ( FoundItemOnPoint(TREEVIEW(o)->Items,Mouse->State.p,&i) )
			{
				
				TRect r;
				RectAssign (&r, i->BufAbs.a.x+3, i->BufAbs.a.y+3, i->BufAbs.a.x+11, i->BufAbs.a.y+11);
				if ( CURSOR_IN_RECT (r) )
				{
					if ( FoundItemOnPoint(TREEVIEW(o)->Items, Mouse->State.p, &i) )
					{
						if ( i->Flags & TIF_EXPENDED )
							TreeviewUnExpendItem(TREEVIEW(o), i);
						else
							TreeviewExpendItem(TREEVIEW(o), i);
					}
					SendMsg = 0;
			  	} else {
					if ( TREEVIEW(o)->Sel != i )
					{
						if ( TREEVIEW(o)->Sel )
						{
							TREEVIEW(o)->Sel->Flags &= ~TIF_SELECTED;
							WidgetDraw(o, &(TREEVIEW(o)->Sel->BufAbs));
						}
					}
					else
						SendMsg = 0;
					
					i->Flags |= TIF_SELECTED;
					WidgetDraw(o, &(i->BufAbs));
					TREEVIEW(o)->Sel = i;
		  		}

			}
			else if ( TREEVIEW(o)->Sel )
			{
				TREEVIEW(o)->Sel->Flags &= ~TIF_SELECTED;
				WidgetDraw(o, &(TREEVIEW(o)->Sel->BufAbs));
				TREEVIEW(o)->Sel = NULL;
			}

			if ( TREEVIEW(o)->OnSelMsg && SendMsg  )
					WidgetSendSEvent(o, EV_MESSAGE, TREEVIEW(o)->OnSelMsg, TREEVIEW(o)->Sel);

			if ( Ev->Message == WEvMouseRDown ) {
				if ( TREEVIEW(o)->OnMenuMsg ) {
					if ( TREEVIEW(o)->Sel )
						WidgetSendSEvent(o, EV_MESSAGE, TREEVIEW(o)->OnMenuMsg, TREEVIEW(o)->Sel);
				}
			}
			return true;
		}



	}
	return ScrollWidgetEventHandler(o,Ev);
}
////////////////////////////////////////////////////////////////////////////////
void Scroll ( PScrollWidget o, TPoint p ) {

	// To prevent blicking, force temporaly double buffer !

	if ( WIDGET(o)->Flags & WFForceBuffer )
		WidgetDraw(WIDGET(o),NULL);
	else {
		WIDGET(o)->Flags |= WFForceBuffer;
		WidgetDraw(WIDGET(o),NULL);
		WIDGET(o)->Flags &= ~WFForceBuffer;
	}


}
////////////////////////////////////////////////////////////////////////////////
l_text TreeViewRenameItemBox ( PTreeView t, PTreeItem i ) {

	PTextbox tb;
	TRect 		r;
	l_text ret = NULL;

	RectAssign(&r,i->BufAbs.a.x+32,i->BufAbs.a.y+2,WIDGET(t)->Absolute.b.x-32,i->BufAbs.b.y-2);
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
/**
	Changes the caption of a treeview item
	@param t Treeview owning the item
	@param i Item to affect
	@param Caption New caption
	@note Redraws scrolls bars and treeview
*/
void TreeViewRenameItem ( PTreeView t, PTreeItem i, l_text Caption ) {

	free(i->Caption);
	i->Caption = TextDup(Caption);

	WidgetDraw(WIDGET(t), NULL);

	WidgetDraw(WIDGET(SLLWDG(t)->Vertical), NULL);
  WidgetDraw(WIDGET(SLLWDG(t)->Horizontal), NULL);
}
////////////////////////////////////////////////////////////////////////////////
/**
	Removes the specified treeview item
	@param o Treeview owning the item
	@param i Item to remove
	@note Redraws scrolls bars and treeview
*/
void TreeViewDeleteItem ( PTreeView o, PTreeItem i ) {
	PTreeItem p = i->Parent;

	if ( o->Sel == i ) {
		if ( i->Next == i ) {
			if ( i->Parent == o->Items )
				o->Sel = NULL;
			else
				o->Sel = i->Parent;
		} else
			o->Sel = i->Next;

		if ( o->Sel ) o->Sel->Flags |= TIF_SELECTED;
	}

  if ( i->Next == i ) {
	  p->Last = NULL;
		if ( p->Flags & TIF_EXPENDED ) p->Flags &= ~TIF_EXPENDED;
  } else {
	  i->Next->Prev = i->Prev;
	  i->Prev->Next = i->Next;
	  if ( p->Last == i ) p->Last = i->Prev;
  }


	WidgetDraw(WIDGET(o), NULL);
	WidgetDraw(WIDGET(SLLWDG(o)->Vertical), NULL);
  WidgetDraw(WIDGET(SLLWDG(o)->Horizontal), NULL);
}

////////////////////////////////////////////////////////////////////////////////
/**
	Initialises a treeview widget
	@param b Treeview to initialise
	@param Class Class of widget (TreeviewClass by default)
	@param App Application which want to allocate the widget( use &Me in most cases)
	@param r Relative coordinates of new widget
	@param OnSelMsg Message send when an item is selected
	@return b
	@see WidgetInsert
*/
PTreeView InitTreeView( PTreeView b, PWidgetClass Class, PApplication App, TRect r, l_ulong OnSelMsg )
{

	b->Items = malloc(sizeof(TTreeItem));
	memset(b->Items,0,sizeof(TTreeItem));

	b->Items->Flags |= TIF_EXPENDED;

	b->OnSelMsg = OnSelMsg;
	b->Flags = TVF_DRAWPLUSMINUS|TVF_DRAWICONS;

	if( WIDGET(b)->Flags &= NO_SCROLL ){
	WIDGET(b)->Draw = &TreeViewDraw;
	WIDGET(b)->EventHandler = &TreeViewEventHandler;
	WIDGET(b)->FreeEx = &TreeViewFreeEx;
	WIDGET(b)->Flags |= WFFocusable;
	}
	else {
	WIDGET(b)->Draw = &TreeViewDraw;
	WIDGET(b)->EventHandler = &TreeViewEventHandler;
	WIDGET(b)->FreeEx = &TreeViewFreeEx;
	WIDGET(b)->Flags |= WFFocusable;
		
	InitScrollWidget(SLLWDG(b), Class?Class:TreeviewClass, App, r, 3);

	WIDGET(b)->Draw = &TreeViewDraw;
	WIDGET(b)->EventHandler = &TreeViewEventHandler;
	WIDGET(b)->FreeEx = &TreeViewFreeEx;
	WIDGET(b)->Flags |= WFFocusable;

	SLLWDG(b)->Scrolled = &Scroll;
	SLLWDG(b)->Delta.y = 19;
	SLLWDG(b)->Delta.x = 16;
	}
	
	WIDGET(b)->DropAccept 		= &TreeviewDropAccept;
	WIDGET(b)->DragBeforeDrop 	= &TreeviewDragBeforeDrop;
	WIDGET(b)->DropData 		= &TreeviewDropData;
	WIDGET(b)->DragAfterDrop 	= &TreeviewDragAfterDrop;	
	
	return b;
}
////////////////////////////////////////////////////////////////////////////////
/**
	Creates a new treeview widget
	@param App Application which want to allocate the widget( use &Me in most cases)
	@param r Relative coordinates of new widget
	@param OnSelMsg Message send when an item is selected
	@return Pointer to the new treeview
	@see WidgetInsert
*/
PTreeView CreateTreeView( PApplication App, TRect r, l_ulong OnSelMsg )
{
	PTreeView b = malloc(sizeof(TTreeView));

	if (!b) return NULL;

	//r.b.x -= 10;

	memset(b, 0, sizeof(TTreeView));

	InitTreeView(b,TreeviewClass,App,r,OnSelMsg);

	return b;
}
/**
@}
*/
////////////////////////////////////////////////////////////////////////////////
PWidget  StdCreateTreeView ( PApplication App, TRect r )
{
	return WIDGET(CreateTreeView( App, r, 0));
}
////////////////////////////////////////////////////////////////////////////////
l_bool LibMain ( int argc, l_text *argv )
{
	
	SkinTreeview = GetSkinItem("Treeview");
	
	TreeviewClass = CreateWidgetClass( ULONG_ID('T','r','e','V'), ScollWidgetClass, 0 );
	TreeviewClass->StdCreate = &StdCreateTreeView;

	APPEXPORT(CreateTreeView);
	APPEXPORT(EmptySubTreeItems);
	APPEXPORT(AddTreeItem);
	APPEXPORT(TreeviewExpendItem);
	APPEXPORT(TreeviewUnExpendItem);
	APPEXPORT(TreeViewRenameItemBox);
	APPEXPORT(TreeViewSelectItem);
	APPEXPORT(TreeViewRenameItem);
	APPEXPORT(TreeViewDeleteItem);

	APPEXPORT(TreeviewScrollBarRecalculate);
	APPEXPORT(InitTreeView);
	APPEXPORT(TreeViewEventHandler);
	APPEXPORT(TreeviewClass);
	
	APPEXPORT(TreeviewAddItem);
	APPEXPORT(TreeviewInitItem);
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{

}
////////////////////////////////////////////////////////////////////////////////
