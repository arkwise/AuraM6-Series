////////////////////////////////////////////////////////////////////////////////
//
//  Menu Widget - Core File
//
//  (c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "window.h"
////////////////////////////////////////////////////////////////////////////////
l_ulong	UID = ULONG_ID('M','E','N','U');
l_ulong	AppVersion = ULONG_ID(0,3,0,0);
l_char	AppName[] = "Menu Widget";
l_uid	nUID = "menu";
l_uid NeededLibs[] = { "widget", "" };

PWidgetClass MenuClass;



PSkin SkinMenuviewBackground	= 0;
PSkin SkinMenuviewSelect		= 0;
PSkin SkinMenuBackground		= 0;
PSkin SkinMenuSelect			= 0;

PMenuView PopUpMenuEx ( PApplication App, PMenuView Parent, TPoint p, PMenu M, l_ulong F, l_ulong Options );

/*
	Notice : Menu duplicators support only basics menus

*/

PMenu DuplicateMenu ( PMenu m ) {
	if ( m ) {
		PMenu n = malloc(sizeof(TMenu));
		if ( !n ) return NULL;
		memset(n,0,sizeof(TMenu));	
		n->Items = DuplicateMenuItem(m->Items);
		return n;
	}
	return NULL;	
}

PMenuItem DuplicateMenuItem ( PMenuItem i ) {
	PMenuItem n = malloc(sizeof(TMenuItem));
	if ( !n ) return NULL;
	memset(n,0,sizeof(TMenuItem));	
	
	n->Caption = TextDup(i->Caption);
	n->Name = TextDup(i->Name);
	n->Message = i->Message;
	
	if ( i->Next ) n->Next = DuplicateMenuItem(i->Next);
	if ( i->SubMenu ) n->SubMenu = DuplicateMenu(i->SubMenu);
	return n;
}


void FreeMenuItem ( PMenuItem o ) {
	if ( o->Caption ) free(o->Caption);
	if ( o->FreeArgs && o->Args ) o->FreeArgs(o->Args);
	if ( o->Flags & MIF_FREEICON ) destroy_bitmap(o->Icon);
	free(o);	
}

void FreeMenu ( PMenu o ) {
	PMenuItem a = o->Items, n;	
	while ( a ) {
		n = a->Next;	
		if ( a->SubMenu ) FreeMenu(a->SubMenu);
		FreeMenuItem(a);
		a = n;
	}
	free(o);
}

#define MSG_TREEVIEW_ONSEL	0x1256
#define MSG_DBLCLK			0x2266
#define MSG_TB_GOUP			0x4266

#define MSG_VIEWICONS		0x4267
#define MSG_VIEWLIST		0x4268
#define MSG_VIEWMOSC		0x4269

void WriteMenu ( PFile f, PMenu m );

void WriteMenuItem ( PFile f, PMenuItem i ) {
	FileWriteBinString(f,i->Caption?i->Caption:"");
	FileWriteBinString(f,i->Name?i->Name:"");
	file_write_long(f,i->Message);
	if ( i->Next ) {
		file_write_char(f,1);
		WriteMenuItem(f,i->Next);
	} else 
		file_write_char(f,0);
		
	if ( i->SubMenu ) {
		file_write_char(f,1);
		WriteMenu(f,i->SubMenu);
	} else 
		file_write_char(f,0);
}

void WriteMenu ( PFile f, PMenu m ) {
	if ( m->Items ) {
		file_write_char(f,1);
		WriteMenuItem(f,m->Items);
	} else 
		file_write_char(f,0);
}

PMenu ReadMenu ( PFile f );

PMenuItem ReadMenuItem ( PFile f ) {
	PMenuItem n = malloc(sizeof(TMenuItem));
	if ( !n ) return NULL;
	memset(n,0,sizeof(TMenuItem));	
	n->Caption = FileReadBinString(f);
	n->Name = FileReadBinString(f);
	if ( !TextCompare("",n->Name) ) {
		free(n->Name);
		n->Name = NULL;	
	}
	FileRead(&n->Message,1,4,f);
	if ( file_get_char(f) )
		n->Next = ReadMenuItem(f);
	if ( file_get_char(f) )
		n->SubMenu = ReadMenu(f);
	return n;
}

PMenu ReadMenu ( PFile f ) {
	PMenu n = malloc(sizeof(TMenu));
	if ( !n ) return NULL;
	memset(n,0,sizeof(TMenu));	
	if ( file_get_char(f) )
		n->Items = ReadMenuItem(f);
	return n;
}

PMenu NewMenu ( PMenuItem Itm )
{
	PMenu o = malloc(sizeof(TMenu));

	if ( !o ) return NULL;

	memset(o, 0, sizeof(TMenu));

	o->Items = Itm;
	o->Current = NULL;

	return o;
}

PMenuItem MenuGetLatest ( PMenu m ) {
	PMenuItem i = m->Items;
	if ( i ) while ( i->Next ) i = i->Next;
	return i;
}

PMenuItem NewMenuItem ( l_text Caption, p_bitmap Icon, l_ulong Message, l_ulong Flags, PMenu SubMenu, PMenuItem Nxt )
{
	PMenuItem o = malloc(sizeof(TMenuItem));

	if ( !o ) return NULL;

	memset(o, 0, sizeof(TMenuItem));

	o->Caption  = TextDup(Caption);
	o->Icon     = Icon;
	o->Message  = Message;
	o->Flags    = Flags;
	o->State = 0;
	o->SubMenu  = SubMenu;
	o->Next = Nxt;

	return o;
}

PMenuItem AddMenuItem ( PMenu m, l_text Caption, p_bitmap Icon, l_ulong Message, l_ulong Flags, PMenu SubMenu )
{
	PMenuItem l = MenuGetLatest(m);
	PMenuItem o = NewMenuItem(Caption,Icon,Message,Flags,SubMenu,NULL);
	if ( l ) 
		l->Next = o;
	else
		m->Items = o;
	return o;
}

PMenuItem NewMenuItemEx ( l_text Caption, p_bitmap Icon, l_ulong Message, l_ulong Flags, PMenu SubMenu, void (*GenerateSubMenu)(PMenuItem,void*), void *Args, PMenuItem Nxt )
{
	PMenuItem o = malloc(sizeof(TMenuItem));

	if ( !o ) return NULL;

	memset(o, 0, sizeof(TMenuItem));

	o->Caption  = TextDup(Caption);
	o->Icon     = Icon;
	o->Message  = Message;
	o->Flags    = Flags;
	o->State = 0;
	o->SubMenu  = SubMenu;
	o->Next = Nxt;
	o->GenerateSubMenu = GenerateSubMenu;
	o->Args = Args;

	return o;
}

PMenuItem AddMenuItemEx ( PMenu m, l_text Caption, p_bitmap Icon, l_ulong Message, l_ulong Flags, PMenu SubMenu, void (*GenerateSubMenu)(PMenuItem,void*), void *Args )
{
	PMenuItem l = MenuGetLatest(m);
	PMenuItem o = NewMenuItemEx(Caption,Icon,Message,Flags,SubMenu,GenerateSubMenu,Args,NULL);
	if ( l ) 
		l->Next = o;
	else
		m->Items = o;
	return o;
}

PMenuItem NewMenuItemSeparator ( PMenuItem Next )
{
	return NewMenuItem("---", NULL, 0, 0, NULL, Next);
}

PMenuItem AddMenuItemSeparator ( PMenu m )
{
	PMenuItem l = MenuGetLatest(m);
	PMenuItem o = NewMenuItemSeparator(NULL);
	if ( l ) 
		l->Next = o;
	else
		m->Items = o;
	return o;
}

void DrawSubMenuArrow (p_bitmap buffer, TPoint p)
{
	putpixel(buffer, p.x, p.y, makecol(0,0,0));
	putpixel(buffer, p.x, p.y+1, makecol(0,0,0)); putpixel(buffer, p.x+1, p.y+1, makecol(0,0,0));
	putpixel(buffer, p.x, p.y+2, makecol(0,0,0)); putpixel(buffer, p.x+1, p.y+2, makecol(0,0,0)); putpixel(buffer, p.x+2, p.y+2, makecol(0,0,0));
	putpixel(buffer, p.x, p.y+3, makecol(0,0,0)); putpixel(buffer, p.x+1, p.y+3, makecol(0,0,0));
	putpixel(buffer, p.x, p.y+4, makecol(0,0,0));
}

void MenuDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	PMenu m = MENUVIEW(o)->Menu;

	l_color FontColor;

	if ( !m ) return;
	if ( !w )
	{
		DebugWarning("MENU::w is NULL!");
		w = &o->Absolute;
	}

	if ( UseSkins )
		FontColor = SkinMenuviewBackground->FontColor;
	else
		FontColor = COL_3DTEXT;

	rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,o->BackgroundColor);

	if ( MENUVIEW(o)->Style == MenuViewStyleHorizontal )
	{
		PMenuItem i = m->Items;
		l_ulong x = o->Absolute.a.x;
		l_ulong y = o->Absolute.a.y;
		l_ulong ln;

		while ( i )
		{
			ln = text_length(default_font,i->Caption);
			RectAssign(&i->BufAbs,x,y,x+ln+9,y+text_height(default_font)+6);

			if ( _RectOverlay(i->BufAbs,*w) )
			{
				if ( i->State == 1 )
				{
					if (UseSkins)
					{
						FontColor = SkinMenuviewBackground->FontColor;
						DrawSkin(buffer, SkinMenuviewBackground, i->BufAbs.a.x,i->BufAbs.a.y,i->BufAbs.b.x,i->BufAbs.b.y);
					}
					else
					{
						FontColor = COL_3DTEXT;
						Rect3D(buffer,i->BufAbs.a.x,i->BufAbs.a.y,i->BufAbs.b.x,i->BufAbs.b.y,makecol(255,255,255),COL_3DDARK);
					}
				}
				else if ( i->State == 2 )
				{
					if (UseSkins)
					{
						FontColor = SkinMenuviewSelect->FontColor;
						DrawSkin(buffer, SkinMenuviewSelect, i->BufAbs.a.x,i->BufAbs.a.y,i->BufAbs.b.x,i->BufAbs.b.y);
					}
					else
					{
						FontColor = COL_3DTEXT;
						Rect3D(buffer,i->BufAbs.a.x,i->BufAbs.a.y,i->BufAbs.b.x,i->BufAbs.b.y,makecol(128,128,128),COL_3DLIGHT);
					}
				}

				if ( i->Flags & MI_DISABLE )
				{
					textout_ex(buffer, default_font, i->Caption, i->BufAbs.a.x+6, i->BufAbs.a.y+4, COL_3DLIGHT, -1 );
					textout_ex(buffer, default_font, i->Caption, i->BufAbs.a.x+5, i->BufAbs.a.y+3, COL_3DDARK, -1 );
		  		}
		  		else
		  		{
			  		textout_ex(buffer, default_font, i->Caption, i->BufAbs.a.x+5, i->BufAbs.a.y+3, FontColor, -1 );
		  		}
			}

			x += ln+10;
			i = i->Next;
		}
	}
	else
	{
		PMenuItem i = m->Items;
		l_ulong x = o->Absolute.a.x+3;
		l_ulong y = o->Absolute.a.y+3;
		l_ulong ln,dx=0,dy=0;

		if ( UseSkins )
		{
			DrawSkin(buffer, SkinMenuBackground, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
		}
		else
		{
			Rect3D(buffer,o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.b.y,COL_3DLIGHT,COL_3DDARK);
		}


		while ( i )
		{
			if ( i->Icon ) dx = max(dx,i->Icon->w+2);
			i = i->Next;
		}

		i = m->Items;

		while ( i )
		{
			if ( !TextCompare(i->Caption,"---" ) )
			{
				RectAssign(&i->BufAbs,0,0,0,0);
				hline(buffer,x,y+3,o->Absolute.b.x-3,COL_3DDARK);
				hline(buffer,x,y+4,o->Absolute.b.x-3,COL_3DLIGHT);
				y += 6;
			}
			else
			{
				dy=text_height(default_font)+2;
				if ( i->Icon ) dy = i->Icon->h+2;
				ln = text_length(default_font,i->Caption);
				RectAssign(&i->BufAbs,x,y,o->Absolute.b.x-3,y+dy);
				if ( _RectOverlay(i->BufAbs,*w) )
				{
					if ( i->State )
					{
						if ( UseSkins )
						{
							FontColor = SkinMenuSelect->FontColor;
							DrawSkin(buffer, SkinMenuSelect, i->BufAbs.a.x,i->BufAbs.a.y,i->BufAbs.b.x,i->BufAbs.b.y);
						}
						else
						{
							FontColor = COL_3DTEXT;
							rectfill(buffer,i->BufAbs.a.x,i->BufAbs.a.y,i->BufAbs.b.x,i->BufAbs.b.y,COL_3DOVER);
						}
					}
					else
					{
						if ( UseSkins )
						{
							FontColor = SkinMenuBackground->FontColor;
						}
						else
						{
							FontColor = COL_3DTEXT;
						}
					}

					if ( i->Icon )
						draw_sprite(buffer,i->Icon, i->BufAbs.a.x+1, i->BufAbs.a.y+1);

					if ( i->Flags & MI_DISABLE )
					{
						textout_ex(buffer, default_font, i->Caption, i->BufAbs.a.x+6+dx, i->BufAbs.a.y+2, COL_3DLIGHT, -1 );
						textout_ex(buffer, default_font, i->Caption, i->BufAbs.a.x+5+dx, i->BufAbs.a.y+1, COL_3DDARK, -1 );
					}
					else textout_ex(buffer, default_font, i->Caption, i->BufAbs.a.x+5+dx, i->BufAbs.a.y+1, FontColor, -1 );

					if ( i->SubMenu || i->GenerateSubMenu )
					{
						TPoint pp;
				    	PointAssign(&pp,i->BufAbs.b.x-6, i->BufAbs.a.y+(i->BufAbs.b.y-i->BufAbs.a.y)/2 - 1);
						DrawSubMenuArrow(buffer, pp);
					}
			  	}

			  	y += dy+1;
			}

			i = i->Next;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void MenuDisposeBackMe ( PMenuView o ) {
  if ( o->Parent ) {
	  PMenuView Parent = o->Parent;
	  WidgetDispose((PWidget)o);
	  Parent->Child = NULL;
	  MenuDisposeBackMe(Parent);
  }
}
////////////////////////////////////////////////////////////////////////////////
void MenuDisposeFromMe ( PMenuView o ) {
	PMenuView Child = o->Child;
	PMenuView Parent = o->Parent;
  if ( o->Parent ) {
	  o->Parent->Child = NULL;
	  WidgetDispose((PWidget)o);
	  WidgetSetFocusExNoNotify(WIDGET(Parent));
  } else {
	  o->Child = NULL;
	  if ( Child ) WidgetDispose((PWidget)Child);
	  if ( o->Flags & MNF_DISPOSEONEVENT )
	  	WidgetDispose((PWidget)o);
	  else
	  	WidgetSendSEvent(WIDGET(o),EV_MESSAGE,MSG_MSGCANCEL,NULL);
  }
}
////////////////////////////////////////////////////////////////////////////////
PMenuItem MenuGetActiveItem ( PMenuView o ) {
	PMenu m = MENUVIEW(o)->Menu;
	if ( m ) {
		PMenuItem i = m->Items;
		while ( i ) {
	    if ( i->State == 1 ) return i;
		  i = i->Next;
    }

	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PMenuItem MenuGetBeforeActiveItem ( PMenuView o ) {
	PMenu m = MENUVIEW(o)->Menu;
	if ( m ) {
		PMenuItem p = 0;
		PMenuItem i = m->Items;
		while ( i ) {
	    if ( i->State == 1 ) return p;
	    p = i;
		  i = i->Next;
    }
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void MenuRunItem ( PMenuView o,PMenuItem i ) {
	if ( !i->SubMenu && !i->GenerateSubMenu ) {
		PMenuView SM = o;
    if ( i->Flags & MI_DISABLE ) return;
    while ( SM->Parent ) SM = SM->Parent;
	  if ( i->Message ) {
		  l_ulong Msg  = i->Message;
		  if ( SM->MsgDest ) {
			  PWidget Dest = SM->MsgDest;
			  MenuDisposeFromMe(SM);
		  	WidgetSendSEvent(Dest,EV_MESSAGE,Msg,i);
		  } else if ( !(SM->Flags & MNF_DISPOSEONEVENT) ) {
			  MenuDisposeFromMe(SM);
		  	WidgetSendSEvent(WIDGET(SM),EV_MESSAGE,Msg,i);
	    } else { // This should never happen !
		    DebugMessage("TIP : Disposing on Event Menus should have a widget (MsgDest) to recolt messages");
			  WidgetMakeSync((PWidget*)&SM);
	    	WidgetSendSEvent(WIDGET(SM),EV_MESSAGE,Msg,i);
        if ( SM ) MenuDisposeFromMe(SM); // Oups... and if handler killed me ????
        WidgetUnMakeSync((PWidget*)&SM);
    	}
	  }
	}

}
////////////////////////////////////////////////////////////////////////////////
void MenuExpendItem ( PMenuView o,PMenuItem i, l_bool delay ) {
	if ( i->SubMenu || i->GenerateSubMenu ) {
	  TPoint pp;
    if ( i->Flags & MI_DISABLE ) return;
	  if ( i->GenerateSubMenu ) {
	  	i->GenerateSubMenu(i,i->Args);
	  	i->GenerateSubMenu = NULL;
  	}
    if ( o->Style == MenuViewStyleVertical ) {
      PointAssign(&pp,i->BufAbs.b.x,i->BufAbs.a.y);
	    PopUpMenuEx(WIDGET(o)->AppOwner,o,pp,i->SubMenu,0,0);
  	} else if ( !delay   ){
	    PointAssign(&pp,i->BufAbs.a.x,i->BufAbs.b.y);
    	PopUpMenuEx(WIDGET(o)->AppOwner,o,pp,i->SubMenu,0,0);
    }
  }
}
////////////////////////////////////////////////////////////////////////////////
l_bool MenuEventHandler ( PWidget o, PEvent Ev )
{
	if (Ev->Type == EV_KEYBOARD) {

		if ( SCANCODE(Ev,KEY_DOWN) ) {

			PMenuItem i = MenuGetActiveItem(MENUVIEW(o));

			if ( i ) {
				if ( i->Next ) {

					i->State = 0;
					i->Next->State = 1;
					WidgetDraw(o, &i->BufAbs);
					WidgetDraw(o, &i->Next->BufAbs);

				}
			} else {
				MENUVIEW(o)->Menu->Items->State = 1;
				WidgetDraw(o, &MENUVIEW(o)->Menu->Items->BufAbs);
			}
			return true;
		} else if ( SCANCODE(Ev,KEY_UP) ) {
			PMenuItem i = MenuGetActiveItem(MENUVIEW(o));
			PMenuItem p = MenuGetBeforeActiveItem(MENUVIEW(o));
			if ( i && p ) {
				i->State = 0;
				p->State = 1;
				WidgetDraw(o, &i->BufAbs);
				WidgetDraw(o, &p->BufAbs);
			} else if ( !i ) {
				MENUVIEW(o)->Menu->Items->State = 1;
				WidgetDraw(o, &MENUVIEW(o)->Menu->Items->BufAbs);
			}
			return true;
		} else if ( SCANCODE(Ev,KEY_LEFT) ) {
			PMenuView Parent = MENUVIEW(o)->Parent;
			if ( Parent ) {
				MenuDisposeFromMe(MENUVIEW(o));
			}
			return true;
		} else if ( SCANCODE(Ev,KEY_RIGHT) ) {
			PMenuItem i = MenuGetActiveItem(MENUVIEW(o));
			if ( i ) MenuExpendItem(MENUVIEW(o),i,false);
			return true;
		} else if ( SCANCODE(Ev,KEY_ENTER)) {
			PMenuItem i = MenuGetActiveItem(MENUVIEW(o));
			if ( i ) MenuRunItem(MENUVIEW(o),i);
			return true;
 		}
	}

	if ( Ev->Type == EV_MESSAGE )
	{
		if ( Ev->Message == WEvNotifySameFocus  ) {
			PMenuView SM = (PMenuView)o;
			while ( SM->Parent ) SM = SM->Parent;
			MenuDisposeFromMe(SM);
		  return true;
	  }
		if ( Ev->Message == WEvNotifyLostFocus ) {
			PMenuView SM = (PMenuView)o;
			while ( SM->Parent && WIDGET(SM->Parent) != FocusedWidget ) SM = SM->Parent;
			MenuDisposeFromMe(SM);
		  return true;
	  }
	}

	if (Ev->Type == EV_MOUSE)
	{

		PMenu m = MENUVIEW(o)->Menu;
		if ( !m ) return false;

		if (Ev->Message == WEvMouseMove)
		{
			PMenuItem i = m->Items;

			while ( i )
			{
				if ( PointInRect(Mouse->State.p,i->BufAbs) )
				{
					if ( i->State != 1 )
					{
						i->State = 1;
						WidgetDraw(o, &i->BufAbs);
					}
				}
				else if ( i->State == 1 )
				{
					i->State = 0;
					WidgetDraw(o, &i->BufAbs);
				}

				i = i->Next;
			}

			return true;
		}

		if (Ev->Message == WEvMouseLeft)
		{
			PMenuItem i = m->Items;

			while ( i )
			{
		    	if ( i->State != 0 )
		    	{
					i->State = 0;
					WidgetDraw(o, &i->BufAbs);
				}

				i = i->Next;
			}

			return true;
		}

		if ( Ev->Message == WEvMouseLDown || Ev->Message == WEvMouseLUp || Ev->Message == WEvMouseEnterDelay )
		{
			PMenuItem i = m->Items;

			while ( i )
			{
				if ( PointInRect(Mouse->State.p,i->BufAbs) )
				{
					if ( Ev->Message == WEvMouseLDown )
						i->State = 2;
					else if ( Ev->Message == WEvMouseLUp )
						i->State = 1;

					WidgetDraw(o, &i->BufAbs);

					if ( Ev->Message == WEvMouseLUp || Ev->Message == WEvMouseEnterDelay )
					{
						if ( i->SubMenu || i->GenerateSubMenu )
						{
							// PopUp Menu !
							MenuExpendItem(MENUVIEW(o),i,Ev->Message==WEvMouseEnterDelay);
							return true;
						}
						else
						{
							if ( Ev->Message == WEvMouseLUp )
							{
								MenuRunItem(MENUVIEW(o),i);
							}
							else
							{
								if ( MENUVIEW(o)->Child ) MenuDisposeFromMe(MENUVIEW(o)->Child);
							}

							return true;
						}
					}
				}
				else if ( i->State != 0 )
				{
					i->State = 0;
					WidgetDraw(o, &i->BufAbs);
				}

				i = i->Next;
			}

			return true;
		}
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////
void MenuViewFreeEx ( PWidget o ) {
	if ( MENUVIEW(o)->Child ) WidgetDispose(WIDGET(MENUVIEW(o)->Child));
	if( (MENUVIEW(o)->Flags & MVF_FREEMENU) && !MENUVIEW(o)->Parent ) FreeMenu(MENUVIEW(o)->Menu);
}
////////////////////////////////////////////////////////////////////////////////
PMenuView CreateMenuView ( PApplication A, TRect r, PMenu M, l_char S, l_ulong F ) {

	PMenuView v = malloc(sizeof(TMenuView));
	if (!v) return NULL;
	memset(v,0,sizeof(TMenuView));

	IntialiseWidget(A, MenuClass, WIDGET(v), r );

	v->Menu = M;
	v->Style = S;
	v->Flags = F;
	v->Child = NULL;

	WIDGET(v)->Draw = &MenuDraw;
	WIDGET(v)->EventHandler = &MenuEventHandler;
	WIDGET(v)->FreeEx = &MenuViewFreeEx;
	WIDGET(v)->BackgroundColor = COL_3DFACE;

	if ( M )
	{
		PMenuItem i = M->Items;

		while ( i )
		{
			i->State = 0;
			i = i->Next;
	  }
	}

	return v;
}
////////////////////////////////////////////////////////////////////////////////
PMenuView PopUpMenuEx ( PApplication App, PMenuView Parent, TPoint p, PMenu M, l_ulong F, l_ulong Options ) {

	l_ulong w = 10, h = 6, t = 0, d = 0;
	PMenuView o;
	PMenuItem i = M->Items;
	TRect r;

	while ( i )
	{
		if ( i->Icon ) d = max(d,i->Icon->w);
		if ( i->SubMenu || i->GenerateSubMenu ) t = 1;
		i = i->Next;
	}

	/**
	 * Submenu arrow width
	 */
	if ( t ) d += 6;

	i = M->Items;

	while ( i )
	{
		w = max(w, text_length(default_font,i->Caption)+16+d);

		if ( i->Icon )
			h += i->Icon->h+3;
		else if ( !TextCompare(i->Caption,"---" ) )
			h += 6;
		else
			h += text_height(default_font)+3;

		i = i->Next;
	}




	if ( Options == 1 ) {
		if ( p.y > GSScreenHeight)
			RectAssign(&r,p.x,GSScreenHeight-h,p.x+w,GSScreenHeight);
		else
			RectAssign(&r,p.x,p.y-h,p.x+w,p.y); 	
	
	} else { 
		if ( p.y+h > GSScreenHeight)
			RectAssign(&r,p.x,GSScreenHeight-h,p.x+w,GSScreenHeight);
		else
			RectAssign(&r,p.x,p.y,p.x+w,p.y+h); 
	}



	o = NewMenuView(App,r,M,MenuViewStyleVertical,F);

	if ( Parent )
	{
		o->Parent = Parent;
		if ( Parent->Child ) MenuDisposeFromMe(Parent->Child );
		Parent->Child = o;
	}

	InsertWidget(DeskTop, WIDGET(o));

	WIDGET(o)->Flags |= WFFocusable;

	if ( Parent )
	{
		if ( Parent->Style == MenuViewStyleVertical )
			WidgetSetFocusExNoNotify(WIDGET(o));
		else
			WidgetSetFocusEx(WIDGET(o));
	}
	else
		WidgetSetFocusEx(WIDGET(o));

	WidgetDraw(WIDGET(o), NULL);

	return o;
}
////////////////////////////////////////////////////////////////////////////////
l_ulong PopUpMenuWait ( PApplication App, TPoint p, PMenu M, l_ulong Options ) {

	PMenuView m = PopUpMenuEx(App,NULL,p,M,0,Options);

	l_ulong Message = WidgetExecute((PWidget)m);

	WidgetDispose((PWidget)m);

	if ( Message == MSG_MSGCANCEL )
		return 0;
	else
		return Message;
}

////////////////////////////////////////////////////////////////////////////////
PMenuView PopUpMenu ( PApplication App, TPoint p, PMenu M, PWidget MsgDest, l_ulong Options )
{
	PMenuView o = PopUpMenuEx(App,NULL,p,M,MNF_DISPOSEONEVENT,Options);
	o->MsgDest = MsgDest; // Tell menu to send his messages to widget MsgDest and not desktop...

	return o;
}
////////////////////////////////////////////////////////////////////////////////
PWidget  StdCreateMenu ( PApplication App, TRect r )
{
	return WIDGET(CreateMenuView( App, r, NULL, 0, 0));
}
////////////////////////////////////////////////////////////////////////////////
l_bool	MenuSetRProp ( PWidget o, l_text Name, void *Value, l_bool Redraw ) {
	
	if ( !TextCompare(Name,"menu") ) {
		if ( MENUVIEW(o)->Menu )  FreeMenu(MENUVIEW(o)->Menu);
		if ( Value )
			MENUVIEW(o)->Menu = DuplicateMenu((PMenu)Value);
		else 
			MENUVIEW(o)->Menu = NULL;
		if ( Redraw ) WidgetDraw(WIDGET(o), NULL);
		return true;
	}	
	
	return false;
}
////////////////////////////////////////////////////////////////////////////////
void	*MenuGetRProp ( PWidget o, l_text Name ) {
	if ( !TextCompare(Name,"menu") && MENUVIEW(o)->Menu) return DuplicateMenu(MENUVIEW(o)->Menu);
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
l_bool LibMain ( int argc, l_text *argv )
{
	MenuClass = CreateWidgetClass( ULONG_ID('M','e','n','u'), StdWidgetClass, 0 );
	MenuClass->StdCreate = &StdCreateMenu;
	MenuClass->SetRProp = &MenuSetRProp;
	MenuClass->GetRProp = &MenuGetRProp;

	CreatePropType(PTYPE_MENU,(void*)&DuplicateMenu,(void*)&FreeMenu,(void*)&WriteMenu,(void*)&ReadMenu);
	
	
	WClassRegisterProp(MenuClass,PTYPE_MENU,"menu");
	
	SkinMenuviewBackground	= GetSkinItem("MenuviewBackground");
	SkinMenuviewSelect		= GetSkinItem("MenuviewSelect");
	SkinMenuBackground		= GetSkinItem("MenuBackground");
	SkinMenuSelect			= GetSkinItem("MenuSelect");


	APPEXPORT(PopUpMenu);
	APPEXPORT(PopUpMenuWait);
	APPEXPORT(CreateMenuView);
	APPEXPORT(NewMenu);
	APPEXPORT(NewMenuItem);
	APPEXPORT(NewMenuItemEx);
	APPEXPORT(NewMenuItemSeparator);
	
	APPEXPORT(AddMenuItem);
	APPEXPORT(AddMenuItemEx);
	APPEXPORT(AddMenuItemSeparator);
	APPEXPORT(FreeMenu);
	
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{

}
