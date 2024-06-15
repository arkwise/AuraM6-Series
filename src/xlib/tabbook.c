/////////////////////////////////////////////////////////////////////////////////
//
//	Tabbook Core File 
//
//	Aura Milestone 3 
//
//	(c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "tabbook.h"
#include "grfx.h"

l_ulong AppVersion	= ULONG_ID(0,2,0,0);
char    AppName[]	= "TabBook Widget";
l_uid	nUID		= "tabbook";
l_uid NeededLibs[]	= { "widget", "grfx", "skin", "" };

PSkin SkinTabOpen		= 0;
PSkin SkinTabClose		= 0;
PSkin SkinTabBackground	= 0;
PSkin SkinTabContainer	= 0;

PWidgetClass TabClass;
PWidgetClass TabBookClass;

void  NonSkinTabBookDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	l_int x = o->Absolute.a.x+2-TABBOOK(o)->Pos, y = o->Absolute.a.y;
	rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,COL_3DFACE);

	vline(buffer,o->Absolute.a.x,o->Absolute.a.y+20,o->Absolute.b.y,COL_3DLIGHT);
	vline(buffer,o->Absolute.b.x,o->Absolute.a.y+20,o->Absolute.b.y,COL_3DDARK);
	hline(buffer,o->Absolute.a.x,o->Absolute.b.y,o->Absolute.b.x,COL_3DDARK);
	hline(buffer,o->Absolute.a.x,o->Absolute.a.y+19,o->Absolute.b.x,COL_3DLIGHT);


	if ( o->Last ) {
		PWidget a = o->Last;
		PWidget b = a;

		do {
			RectAssign(&TAB(a)->BtCache,x,y+1,x+text_length(default_font,TAB(a)->Title)+6,y+18);


			if ( TAB(a)->BtCache.b.x  < o->Absolute.b.x-20 ) {

				textout_ex(buffer, default_font, TAB(a)->Title,x+3,y+2, COL_3DTEXT, -1);

				if ( TAB(a) == TABBOOK(o)->Current ) {
					hline(buffer,TAB(a)->BtCache.a.x-1,TAB(a)->BtCache.a.y-1,TAB(a)->BtCache.b.x+1,COL_3DLIGHT);
					vline(buffer,TAB(a)->BtCache.a.x-1,TAB(a)->BtCache.a.y-1,TAB(a)->BtCache.b.y,COL_3DLIGHT);
					vline(buffer,TAB(a)->BtCache.b.x+1,TAB(a)->BtCache.a.y-1,TAB(a)->BtCache.b.y,COL_3DDARK);
					hline(buffer,TAB(a)->BtCache.a.x,o->Absolute.a.y+19,TAB(a)->BtCache.b.x,COL_3DFACE);
				} else {
					hline(buffer,TAB(a)->BtCache.a.x+1,TAB(a)->BtCache.a.y,TAB(a)->BtCache.b.x,COL_3DLIGHT);
					if ( TAB(a->Next) != TABBOOK(o)->Current || a == b ) vline(buffer,TAB(a)->BtCache.a.x,TAB(a)->BtCache.a.y,TAB(a)->BtCache.b.y,COL_3DLIGHT);
					if ( TAB(a->Prev) != TABBOOK(o)->Current || a->Prev == b )vline(buffer,TAB(a)->BtCache.b.x,TAB(a)->BtCache.a.y,TAB(a)->BtCache.b.y,COL_3DDARK);
				}

			} else if ( x  < o->Absolute.b.x-20 ) {

				l_int w = o->Absolute.b.x-20-x;

				if ( w > 3 ) DrawNiceText(buffer, default_font,x+3,y+2, w-3, TAB(a)->Title, COL_3DTEXT);

				if ( TAB(a) == TABBOOK(o)->Current ) {
					hline(buffer,x,y,o->Absolute.b.x-20,COL_3DLIGHT);
					vline(buffer,x,y,y+18,COL_3DLIGHT);
					//vline(buffer,o->Absolute.b.x-20,y,y+18,COL_3DDARK);
					hline(buffer,x+1,o->Absolute.a.y+19,o->Absolute.b.x-20,COL_3DFACE);
				} else {
					hline(buffer,x+1,y+1,o->Absolute.b.x-20,COL_3DLIGHT);
					if ( TAB(a->Next) != TABBOOK(o)->Current || a == b )
						vline(buffer,x,y+1,y+18,COL_3DLIGHT);
					/*if ( TAB(a->Prev) != TABBOOK(o)->Current || a->Prev == b )
						vline(buffer,o->Absolute.b.x-20,y+1,y+18,COL_3DDARK);*/
				}
			}


			x = TAB(a)->BtCache.b.x + 1;

			a = a->Prev;
		} while ( a != b );

		TABBOOK(o)->Max = x-o->Absolute.a.x-2+TABBOOK(o)->Pos;

		if ( TABBOOK(o)->Max > o->Absolute.b.x-22-o->Absolute.a.x ) {
			if ( TABBOOK(o)->Flags & TBF_LDWN )
				Rect3D(buffer,o->Absolute.b.x-19,o->Absolute.a.y,o->Absolute.b.x-10,o->Absolute.a.y+18,COL_3DDARK,COL_3DLIGHT);
			else
				Rect3D(buffer,o->Absolute.b.x-19,o->Absolute.a.y,o->Absolute.b.x-10,o->Absolute.a.y+18,COL_3DLIGHT,COL_3DDARK);

			if ( TABBOOK(o)->Flags & TBF_RDWN )
				Rect3D(buffer,o->Absolute.b.x-9,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.a.y+18,COL_3DDARK,COL_3DLIGHT);
			else
				Rect3D(buffer,o->Absolute.b.x-9,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.a.y+18,COL_3DLIGHT,COL_3DDARK);

			textout_ex(buffer, default_font, "<",o->Absolute.b.x-17,o->Absolute.a.y+2, COL_3DTEXT, -1);
			textout_ex(buffer, default_font, ">",o->Absolute.b.x-7,o->Absolute.a.y+2, COL_3DTEXT, -1);

		}

	}
}

void SkinTabBookDraw (PWidget o, p_bitmap buffer, PRect w)
{
	l_int x = o->Absolute.a.x - TABBOOK(o)->Pos;
	l_int y = o->Absolute.a.y;

	DrawSkin(buffer, SkinTabBackground, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);

	if ( o->Last )
	{
		PWidget a = o->Last;
		PWidget b = a;

		DrawSkin(buffer, SkinTabContainer, o->Absolute.a.x, o->Absolute.a.y + SkinTabOpen->Skin->h, o->Absolute.b.x, o->Absolute.b.y);

		do
		{
			RectAssign(&TAB(a)->BtCache, x, y, x + text_length(default_font, TAB(a)->Title) + SkinTabOpen->Left + SkinTabOpen->Right, y + SkinTabOpen->Skin->h);


			if ( TAB(a)->BtCache.b.x  < o->Absolute.b.x-((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))*2) )
			{
				if ( TAB(a) == TABBOOK(o)->Current )
				{
					DrawSkin(buffer, SkinTabOpen, TAB(a)->BtCache.a.x, TAB(a)->BtCache.a.y, TAB(a)->BtCache.b.x, TAB(a)->BtCache.b.y);
				}
				else
				{
					DrawSkin(buffer, SkinTabClose, TAB(a)->BtCache.a.x, TAB(a)->BtCache.a.y, TAB(a)->BtCache.b.x, TAB(a)->BtCache.b.y);
				}

				textout_centre_ex(buffer, default_font, TAB(a)->Title, (TAB(a)->BtCache.a.x+TAB(a)->BtCache.b.x)/2, (TAB(a)->BtCache.a.y+TAB(a)->BtCache.b.y-text_height(default_font))/2, COL_3DTEXT, -1);
			}
			else if ( x  < o->Absolute.b.x - ((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))*2) )
			{
				l_int w = o->Absolute.b.x-((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))*2)-x;

				if ( TAB(a) == TABBOOK(o)->Current )
				{
					/*hline(buffer,x,y,o->Absolute.b.x-20,COL_3DLIGHT);
					vline(buffer,x,y,y+18,COL_3DLIGHT);
					//vline(buffer,o->Absolute.b.x-20,y,y+18,COL_3DDARK);
					hline(buffer,x+1,o->Absolute.a.y+19,o->Absolute.b.x-20,COL_3DFACE);*/
					DrawSkin(buffer, SkinTabOpen, x, y, o->Absolute.b.x, y + SkinTabOpen->Skin->h);
				}
				else
				{
					/*hline(buffer,x+1,y+1,o->Absolute.b.x-20,COL_3DLIGHT);
					if ( TAB(a->Next) != TABBOOK(o)->Current || a == b )
						vline(buffer,x,y+1,y+18,COL_3DLIGHT);*/
					DrawSkin(buffer, SkinTabClose, x, y, o->Absolute.b.x, y + SkinTabOpen->Skin->h);
				}

				if ( w > 3 ) DrawNiceText(buffer, default_font, x + SkinTabOpen->Left, y + (SkinTabOpen->Skin->h - text_height(default_font))/2, w - 3, TAB(a)->Title, COL_3DTEXT);
			}


			x = TAB(a)->BtCache.b.x;

			a = a->Prev;
		} while ( a != b );

		TABBOOK(o)->Max = x-o->Absolute.a.x - 2 + TABBOOK(o)->Pos;

		if ( TABBOOK(o)->Max > o->Absolute.b.x-o->Absolute.a.x - ((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))*2) )
		{
			if ( TABBOOK(o)->Flags & TBF_LDWN )
				DrawSkin(buffer, SkinTabOpen, o->Absolute.b.x - ((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))*2), y, o->Absolute.b.x - (SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">")), y + SkinTabOpen->Skin->h);
			else
				DrawSkin(buffer, SkinTabClose, o->Absolute.b.x - ((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))*2), y, o->Absolute.b.x - (SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">")), y + SkinTabOpen->Skin->h);

			textout_centre_ex(buffer, default_font, "<",((o->Absolute.b.x - ((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))*2))+(o->Absolute.b.x - (SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))))/2, y + (SkinTabOpen->Skin->h - text_height(default_font))/2, COL_3DTEXT, -1);

			if ( TABBOOK(o)->Flags & TBF_RDWN )
				DrawSkin(buffer, SkinTabOpen, o->Absolute.b.x - (SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">")), y, o->Absolute.b.x, y + SkinTabOpen->Skin->h);
			else
				DrawSkin(buffer, SkinTabClose, o->Absolute.b.x - (SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">")), y, o->Absolute.b.x, y + SkinTabOpen->Skin->h);

			textout_ex(buffer, default_font, ">",((o->Absolute.b.x - (SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">")))+(o->Absolute.b.x))/2, y + (SkinTabOpen->Skin->h - text_height(default_font))/2, COL_3DTEXT, -1);

		}
	}
}

void TabBookSelectTab ( PTabBook o, PTab t )
{
	if ( o->Current ) WIDGET(o->Current)->Flags &= ~WFVisible;
	o->Current = t;
	WIDGET(o->Current)->Flags |= WFVisible;
	WidgetDrawAll(WIDGET(o));
}

void TabBookRemoveTab ( PTabBook o, PTab t )
{
	RemoveWidget(WIDGET(o), WIDGET(t));
}

void TabBookDisposeTab ( PTabBook o, PTab t )
{
	WidgetDispose(WIDGET(t));
}

PTab TabBookAddTab ( PTabBook o, l_text Title, p_bitmap Icon )
{
	TRect r;
	PTab t = (PTab)malloc(sizeof(TTab));
	if ( !t ) return NULL;
	memset(t, 0, sizeof(TTab));

	r = WIDGET(o)->Relative;

	if (UseSkins)
		RectAssign(&r,SkinTabContainer->Left, SkinTabOpen->Skin->h + 1 + SkinTabContainer->Top, r.b.x-r.a.x-SkinTabContainer->Left, r.b.y-r.a.y-SkinTabContainer->Bottom);
	else
		RectAssign(&r,3,23,r.b.x-r.a.x-3,r.b.y-r.a.y-3);


	IntialiseWidget(WIDGET(o)->AppOwner,TabClass, WIDGET(t), r );

	WIDGET(t)->Flags &= ~WFVisible;
	WIDGET(t)->BackgroundColor = COL_3DFACE;

	t->Title	= TextDup(Title);
	t->Icon		= Icon;

	if (!WIDGET(o)->Last) TabBookSelectTab(o, t);

	InsertWidget(WIDGET(o), WIDGET(t));

	return t;
}

l_bool  NonSkinTabBookEventHandler ( PWidget o, PEvent Ev )
{
	if (Ev->Type == EV_MOUSE)
	{
		if (Ev->Message == WEvMouseLDown)
		{
			l_ulong w = o->Absolute.b.x-22-o->Absolute.a.x;

			if ( (TABBOOK(o)->Max > w) && (Mouse->State.p.y < o->Absolute.a.y+20) && (Mouse->State.p.x > o->Absolute.b.x-20) ) {
				if ( Mouse->State.p.x < o->Absolute.b.x-10 ) {
					TABBOOK(o)->Flags |= TBF_LDWN;
				} else {
					TABBOOK(o)->Flags |= TBF_RDWN;
				}

				WidgetDraw(o,NULL);
				return true;
			}
		}

		if (Ev->Message == WEvMouseLUp)
		{
			l_ulong w = o->Absolute.b.x-22-o->Absolute.a.x;

			if ( (TABBOOK(o)->Max > w) && (Mouse->State.p.y < o->Absolute.a.y+20) && (Mouse->State.p.x > o->Absolute.b.x-20) ) {
				if ( Mouse->State.p.x < o->Absolute.b.x-10 ) {
					if ( TABBOOK(o)->Pos > 0 )
						TABBOOK(o)->Pos -= 10;
				} else {
					if ( TABBOOK(o)->Pos < TABBOOK(o)->Max-w+10 )
						TABBOOK(o)->Pos += 10;
				}
				TABBOOK(o)->Flags &= ~(TBF_LDWN|TBF_RDWN);
				WidgetDraw(o,NULL);
				return true;
			}

			if ( o->Last ) {
				PWidget a = o->Last;
				PWidget b = a;
				do {
					if ( CURSOR_IN_RECT(TAB(a)->BtCache) ) {
						TabBookSelectTab(TABBOOK(o),TAB(a));
						return true;
					}
					a = a->Prev;
				} while ( a != b );
			}
			return true;
		}
	}

	return false;
}


l_bool SkinTabBookEventHandler ( PWidget o, PEvent Ev )
{
	if (Ev->Type == EV_MOUSE)
	{
		if (Ev->Message == WEvMouseLDown)
		{
			l_ulong w = o->Absolute.b.x-((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))*2)-o->Absolute.a.x;

			if ( (TABBOOK(o)->Max > w) && (Mouse->State.p.y < o->Absolute.a.y+((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))*2)) && (Mouse->State.p.x > o->Absolute.b.x-((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))*2)) )
			{
				if ( Mouse->State.p.x < o->Absolute.b.x-((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))) )
				{
					TABBOOK(o)->Flags |= TBF_LDWN;
				}
				else
				{
					TABBOOK(o)->Flags |= TBF_RDWN;
				}

				WidgetDraw(o,NULL);
				return true;
			}
		}

		if (Ev->Message == WEvMouseLUp)
		{
			l_ulong w = o->Absolute.b.x-((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))*2)-o->Absolute.a.x;

			if ( (TABBOOK(o)->Max > w) && (Mouse->State.p.y < o->Absolute.a.y+((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))*2)) && (Mouse->State.p.x > o->Absolute.b.x-((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))*2)) )
			{
				if ( Mouse->State.p.x < o->Absolute.b.x-((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))) )
				{
					if ( TABBOOK(o)->Pos > 0 )
						TABBOOK(o)->Pos -= 10;
				}
				else
				{
					if ( TABBOOK(o)->Pos < TABBOOK(o)->Max-w+((SkinTabOpen->Left + SkinTabOpen->Right + text_length(default_font, ">"))) )
						TABBOOK(o)->Pos += 10;
				}

				TABBOOK(o)->Flags &= ~(TBF_LDWN|TBF_RDWN);
				WidgetDraw(o,NULL);

				return true;
			}

			if ( o->Last )
			{
				PWidget a = o->Last;
				PWidget b = a;

				do
				{
					if ( CURSOR_IN_RECT(TAB(a)->BtCache) )
					{
						TabBookSelectTab(TABBOOK(o),TAB(a));
						return true;
					}
					a = a->Prev;
				}
				while ( a != b );
			}
			return true;
		}
	}

	return false;
}

PTabBook CreateTabBook ( PApplication App, TRect r )
{
	PTabBook o = (PTabBook)malloc(sizeof(TTabBook));
	if ( !o ) return NULL;
	memset(o, 0, sizeof(TTabBook));

	IntialiseWidget(App, TabBookClass, WIDGET(o), r);

	if (UseSkins)
	{
		WIDGET(o)->Draw	= &SkinTabBookDraw;
		WIDGET(o)->EventHandler = &SkinTabBookEventHandler;
	}
	else
	{
		WIDGET(o)->Draw	= &NonSkinTabBookDraw;
		WIDGET(o)->EventHandler = &NonSkinTabBookEventHandler;
	}

	return o;
}

l_bool LibMain ( int argc, l_text *argv )
{	
	TabBookClass = CreateWidgetClass( ULONG_ID('T','B','c','k'), StdWidgetClass, WCF_BOOK|WCF_CONTAINER );
	TabBookClass->StdCreate = (void*)&CreateTabBook;
	
	TabClass = CreateWidgetClass( ULONG_ID('T','a','b',' '), StdWidgetClass, WCF_PAGE|WCF_CONTAINER );
	

	SkinTabOpen			= GetSkinItem("TabbookTabOpened");
	SkinTabClose		= GetSkinItem("TabbookTabClosed");
	SkinTabBackground	= GetSkinItem("TabbookBackground");
	SkinTabContainer	= GetSkinItem("TabbookContainer");

	APPEXPORT(CreateTabBook);
	APPEXPORT(TabBookAddTab);
	APPEXPORT(TabBookDisposeTab);
	APPEXPORT(TabBookRemoveTab);
	APPEXPORT(TabBookSelectTab);

	/*TRect r;
	PTabBook o;
	RectAssign(&r,100,100,300,300);
	o = CreateTabBook(&Me,r);
	TabBookAddTab(o,"TAB 1",NULL);
	TabBookAddTab(o,"TAB 2",NULL);
	TabBookAddTab(o,"TAB 3",NULL);
	TabBookAddTab(o,"TAB 4",NULL);
	TabBookAddTab(o,"TAB 5",NULL);
	TabBookAddTab(o,"TAB 6",NULL);
	TabBookAddTab(o,"TAB 7",NULL);
	TabBookAddTab(o,"TAB 8",NULL);
	InsertWidget(WIDGET(Desktop),WIDGET(o));
	WidgetDrawAll(WIDGET(o));*/

	return true;
}

void Close (void)
{

}
