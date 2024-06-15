#include "kernel.h"
#include "widget.h"
#include "toolbar.h"
#include "button.h"

l_ulong	AppVersion = ULONG_ID(0,0,0,1);
l_char	AppName[] = "Toolbar Widget";
l_uid	nUID = "toolbar";
l_uid NeededLibs[] = { "widget", "skin", "button", "" };

PSkin SkinToolbar	= 0;

PWidgetClass ToolBarClass;

#define ITEMSPACE 5

void ToolbarDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	if (UseSkins)
		DrawSkin (buffer, SkinToolbar, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
	else
		rectfill (buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DFACE);
}

l_bool ToolbarEventHandler ( PWidget o, PEvent Ev )
{
	return false;
}

PToolbarItem CreateToolbarItem ( l_text Caption, p_bitmap Icon, l_ulong Message, l_ulong Flags )
{
	PToolbarItem p = malloc(sizeof(TToolbarItem));
	if (!p) return NULL;
	memset(p, 0, sizeof(TToolbarItem));

	p->Caption = TextDup(Caption);
	p->Icon = Icon;
	p->Message = Message;
	p->Flags = Flags;

	return p;
}

void UpdateBufferAbs ( PToolbar c )
{
	PListItem a = c->Items->Last->Next, b = a;
	l_int x = 0;

	if (!a) return;

	do
	{
		RectAssign(&TOOLBARITEM(a->Data)->BufAbs, x, 0, x + (WIDGET(c)->Absolute.b.y - WIDGET(c)->Absolute.a.y), (WIDGET(c)->Absolute.b.y - WIDGET(c)->Absolute.a.y));
		WidgetMoveToPoint(WIDGET(TOOLBARITEM(a->Data)->Button), TOOLBARITEM(a->Data)->BufAbs.a.x, TOOLBARITEM(a->Data)->BufAbs.a.y);

		x += (WIDGET(c)->Absolute.b.y - WIDGET(c)->Absolute.a.y)+ITEMSPACE;

		a = a->Next;
	}
	while (a != b);

	WidgetDraw(WIDGET(c), NULL);
}

void ToolbarAddItem ( PToolbar b, PToolbarItem i )
{
	TRect r;
	PButton c;

	if ( b->Items->Last )
		RectAssign(&r, TOOLBARITEM(b->Items->Last->Data)->BufAbs.b.x+ITEMSPACE, 0, TOOLBARITEM(b->Items->Last->Data)->BufAbs.b.x + ITEMSPACE + (WIDGET(b)->Absolute.b.y - WIDGET(b)->Absolute.a.y), (WIDGET(b)->Absolute.b.y - WIDGET(b)->Absolute.a.y));
	else
		RectAssign(&r, 0, 0, (WIDGET(b)->Absolute.b.y - WIDGET(b)->Absolute.a.y), (WIDGET(b)->Absolute.b.y - WIDGET(b)->Absolute.a.y));

	c = CreateButton(WIDGET(b)->AppOwner, r, i->Caption, i->Message);
	InsertWidget(WIDGET(b), WIDGET(c));
	if ( i->Flags & TI_DISABLED) c->Flags |= BF_DISABLED;
	if ( i->Flags & TI_TEXTUNDERICON) c->Flags |= BF_TEXTUNDERICON;
	WidgetDraw(WIDGET(c), NULL);

	i->BufAbs = r;
	i->Button = c;

	if ( i->Icon )
		c->Icon = i->Icon;

	ListAdd(b->Items, i->Caption, i, &free);
}

void ToolbarRemoveItem ( PToolbar b, PToolbarItem i )
{
	WidgetDispose(WIDGET(i->Button));
	ListRemoveItem( b->Items, ListFoundItem( b->Items, i ) );
	UpdateBufferAbs(b);
}

void ToolbarFreeEx ( PWidget o )
{
	FreeList(TOOLBAR(o)->Items);
}

void ToolbarItemEnable ( PToolbar t, PToolbarItem p )
{
	p->Flags &=~ TI_DISABLED;
	p->Button->Flags &=~ BF_DISABLED;
	UpdateBufferAbs(t);
}

void ToolbarItemDisable ( PToolbar t, PToolbarItem p )
{
	p->Flags |= TI_DISABLED;
	p->Button->Flags |= BF_DISABLED;
	UpdateBufferAbs(t);
}

PToolbar CreateToolbar( PApplication App, TRect r )
{
	PToolbar p = malloc(sizeof(TToolbar));

	if (!p) return NULL;

	memset(p, 0, sizeof(TToolbar));

	IntialiseWidget(App, ToolBarClass, WIDGET(p), r );

	WIDGET(p)->Draw = &ToolbarDraw;

	p->Items = NewList();

	return p;
}

l_bool LibMain ( int argc, l_text *argv )
{
	ToolBarClass = CreateWidgetClass( ULONG_ID('T','l','B','r'), StdWidgetClass, WCF_BOOK );
	ToolBarClass->StdCreate = (void*)&CreateToolbar;
	
	
	/**
	*	Skin items
	*/
	SkinToolbar = GetSkinItem("Toolbar");

	APPEXPORT(CreateToolbar);
	APPEXPORT(ToolbarRemoveItem);
	APPEXPORT(ToolbarAddItem);
	APPEXPORT(CreateToolbarItem);
	APPEXPORT(ToolbarItemEnable);
	APPEXPORT(ToolbarItemDisable);

	/*TRect r;
	PToolbarItem p1, p2, p3, p4;
	RectAssign(&r, 50,70,400,110);
	PToolbar b = CreateToolbar(&Me, r);
	InsertWidget(WIDGET(DeskTop), WIDGET(b));

	p1 = CreateToolbarItem("Open", LoadImage("SYSTEM/ICONS/pc32.bmp"), 0, 0);
	ToolbarAddItem(b,p1);
	p2 = CreateToolbarItem("Close", LoadImage("SYSTEM/ICONS/pc32.bmp"), 0, 0);
	ToolbarAddItem(b,p2);
	p3 = CreateToolbarItem("Help", LoadImage("SYSTEM/ICONS/pc32.bmp"), 0, 0);
	ToolbarAddItem(b,p3);
	p4 = CreateToolbarItem("Exit", LoadImage("SYSTEM/ICONS/pc32.bmp"), 0, 0);
	ToolbarAddItem(b,p4);

	ToolbarRemoveItem(b,p2);

	WidgetDraw(WIDGET(b), NULL);*/

	return true;
}

void Close (void)
{

}
