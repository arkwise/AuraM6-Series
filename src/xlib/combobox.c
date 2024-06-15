////////////////////////////////////////////////////////////////////////////////
//
//  Combobox - Core File
//
//  (c) Copyright 2003 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "combobox.h"
#include "grfx.h"

l_ulong AppVersion = ULONG_ID(0,0,0,1);
char    AppName[] = "Combobox widget";
l_uid	nUID = "combobox";
l_uid NeededLibs[] = { "widget","grfx","skin","listview","" };

PSkin SkinCombobox = 0;

PWidgetClass ComboboxClass;


void ComboboxDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	if ( UseSkins )
	{
		DrawSkin(buffer, SkinCombobox, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);

		hline(buffer,o->Absolute.b.x-SkinCombobox->Right-4,(o->Absolute.a.y+o->Absolute.b.y)/2-2,o->Absolute.b.x-SkinCombobox->Right-11,COL_3DTEXT);
		hline(buffer,o->Absolute.b.x-SkinCombobox->Right-5,(o->Absolute.a.y+o->Absolute.b.y)/2-1,o->Absolute.b.x-SkinCombobox->Right-10,COL_3DTEXT);
		hline(buffer,o->Absolute.b.x-SkinCombobox->Right-6,(o->Absolute.a.y+o->Absolute.b.y)/2,o->Absolute.b.x-SkinCombobox->Right-9,COL_3DTEXT);
		hline(buffer,o->Absolute.b.x-SkinCombobox->Right-7,(o->Absolute.a.y+o->Absolute.b.y)/2+1,o->Absolute.b.x-SkinCombobox->Right-8,COL_3DTEXT);
		hline(buffer,o->Absolute.b.x-SkinCombobox->Right-8,(o->Absolute.a.y+o->Absolute.b.y)/2+2,o->Absolute.b.x-SkinCombobox->Right-7,COL_3DTEXT);

		if ( COMBOBOX(o)->Selected )
			DrawNiceText (buffer, SkinCombobox->Font, o->Absolute.a.x+SkinCombobox->Left, (o->Absolute.a.y+o->Absolute.b.y-text_height(SkinCombobox->Font))/2, o->Absolute.b.x-o->Absolute.a.x-30,COMBOBOX(o)->Selected->Caption, SkinCombobox->FontColor);
			//textout(buffer, SkinCombobox->Font, COMBOBOX(o)->Selected->Caption,o->Absolute.a.x+SkinCombobox->Left, (o->Absolute.a.y+o->Absolute.b.y-text_height(SkinCombobox->Font))/2, SkinCombobox->FontColor);
	}
	else
	{
		rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,COL_3DFACE);

		if ( COMBOBOX(o)->State )
			Rect3D(buffer,o->Absolute.b.x-25,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.b.y,COL_3DDARK,COL_3DLIGHT);
		else
			Rect3D(buffer,o->Absolute.b.x-25,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.b.y,COL_3DLIGHT,COL_3DDARK);

		hline(buffer,o->Absolute.b.x-5-4,(o->Absolute.a.y+o->Absolute.b.y)/2-2,o->Absolute.b.x-5-11,COL_3DTEXT);
		hline(buffer,o->Absolute.b.x-5-5,(o->Absolute.a.y+o->Absolute.b.y)/2-1,o->Absolute.b.x-5-10,COL_3DTEXT);
		hline(buffer,o->Absolute.b.x-5-6,(o->Absolute.a.y+o->Absolute.b.y)/2,o->Absolute.b.x-5-9,COL_3DTEXT);
		hline(buffer,o->Absolute.b.x-5-7,(o->Absolute.a.y+o->Absolute.b.y)/2+1,o->Absolute.b.x-5-8,COL_3DTEXT);
		hline(buffer,o->Absolute.b.x-5-8,(o->Absolute.a.y+o->Absolute.b.y)/2+2,o->Absolute.b.x-5-7,COL_3DTEXT);

		rectfill(buffer,o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x-35,o->Absolute.b.y, COL_BOXFACE);
		Rect3D(buffer,o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x-35,o->Absolute.b.y, COL_BOXDARK,COL_BOXLIGHT);

		if ( COMBOBOX(o)->Selected )
			DrawNiceText (buffer, default_font, o->Absolute.a.x+5,(o->Absolute.a.y+o->Absolute.b.y-text_height(default_font))/2, o->Absolute.b.x-o->Absolute.a.x-35-5,COMBOBOX(o)->Selected->Caption, COL_3DTEXT);
			//textout(buffer, default_font, COMBOBOX(o)->Selected->Caption,o->Absolute.a.x+8,(o->Absolute.a.y+o->Absolute.b.y-text_height(default_font))/2, COL_3DTEXT);
	}
}
////////////////////////////////////////////////////////////////////////////////
l_bool ComboboxEventHandler ( PWidget o, PEvent Ev )
{
	if (Ev->Type == EV_MOUSE)
	{
		if ( Ev->Message == WEvMouseLDown )
		{
			TRect r;
			PListview l;
			l_ulong msg;

			COMBOBOX(o)->State = 1;

			WidgetDraw(o,NULL);

			RectAssign(&r,o->Absolute.a.x,o->Absolute.b.y+1,o->Absolute.b.x,o->Absolute.b.y+min(100,ListviewEstimateHeight(COMBOBOX(o)->Items)));

			l = CreateListview(o->AppOwner,r,NULL,LVS_LIST,LVF_DRAWICONS);

			FreeList(l->Items);

			l->Items = COMBOBOX(o)->Items;
			l->TopItem = COMBOBOX(o)->Selected;
			l->OnValMsg = 1;

			InsertWidget(DeskTop,WIDGET(l));

			WidgetDraw(WIDGET(l), NULL);
			WidgetDraw(WIDGET(SLLWDG(l)->Vertical), NULL);

			ListviewSelectItem(l, GetSelectedItem(l), true, false);

			WidgetSetFocus(WIDGET(l));

			if ( (msg = WidgetExecute(WIDGET(l))) == 1 )
				COMBOBOX(o)->Selected = GetSelectedItem(l);

			l->Items = NewList();

			WidgetDispose(WIDGET(SLLWDG(l)->Vertical));
			SLLWDG(l)->Vertical = NULL;
			WidgetDispose(WIDGET(l));

			COMBOBOX(o)->State = 0;

			WidgetDraw(o,NULL);

			if ( COMBOBOX(o)->OnUpdate ) WidgetSendSEvent(o, EV_MESSAGE, COMBOBOX(o)->OnUpdate, NULL);

			return true;
		}

	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////
void ComboboxSelectItem ( PCombobox o, PListviewItem p ) {
	PListItem a = o->Items->Last, b = a;
	if ( !a ) return;
	do {
		if (LISTVIEWITEM(a->Data)->Flags & LVI_SELECTED )
		{
			LISTVIEWITEM(a->Data)->Flags &= ~LVI_SELECTED;
		}
		a = a->Prev;
	} while ( a != b );

	if ( p ) {
		p->Flags |= LVI_SELECTED;
	}

	o->Selected	= p;
	WidgetDraw(WIDGET(o),NULL);
}
////////////////////////////////////////////////////////////////////////////////
void ComboboxSelectIndex ( PCombobox o, l_ulong Index ) {
	if ( Index ) {
		PListviewItem a = ListAt(o->Items,Index);
		if ( a ) ComboboxSelectItem(o,a);
	} else
		ComboboxSelectItem(o,NULL);
}
////////////////////////////////////////////////////////////////////////////////
l_ulong ComboboxItemIndex ( PCombobox o, PListviewItem a ) {
	return ListFoundIndex(o->Items,a);
}
////////////////////////////////////////////////////////////////////////////////
void ComboboxAddItem ( PCombobox o, char* Caption, PIcon Icon )
{
	PListviewItem p = malloc(sizeof(TListviewItem));
	if (!p) return;
	memset(p, 0, sizeof(TListviewItem));
	ListviewInitItem(LISTVIEWITEM(p),Caption,Icon);
	ListAdd (o->Items, Caption, p, (void*)&FreeListviewItem);
}
////////////////////////////////////////////////////////////////////////////////
void ComboBoxFree ( PWidget o )
{
	//FreeList(COMBOBOX(o)->Items);
}
////////////////////////////////////////////////////////////////////////////////
PCombobox NewComboBox ( PApplication App, TRect r, l_ulong OnUpdate ) {

	PCombobox o = NEW(TCombobox);
	memset(o,0,sizeof(TCombobox));

	IntialiseWidget(App,ComboboxClass,WIDGET(o),r);

	o->Items = NewList();

	WIDGET(o)->Draw         = &ComboboxDraw;
	WIDGET(o)->EventHandler = &ComboboxEventHandler;
	WIDGET(o)->FreeEx = &ComboBoxFree;
	o->OnUpdate = OnUpdate;
	return o;
}
////////////////////////////////////////////////////////////////////////////////
PWidget  StdCreateCombobox ( PApplication App, TRect r )
{
	return WIDGET(NewComboBox( App, r, 0 ));
}

////////////////////////////////////////////////////////////////////////////////
l_bool LibMain ( int argc, l_text *argv )
{
	ComboboxClass = CreateWidgetClass( ULONG_ID('C','m','b','B'), StdWidgetClass, 0 );
	ComboboxClass->StdCreate = (void*)&StdCreateCombobox;

	
	
	/*TRect r = { 100,100,300,120 };
	PCombobox cbx = NewComboBox(&Me,r);

	ComboboxAddItem(cbx,"Item a",NULL,NULL);
	ComboboxAddItem(cbx,"Item b",NULL,NULL);
	ComboboxAddItem(cbx,"Item c",NULL,NULL);
	ComboboxAddItem(cbx,"Item d",NULL,NULL);
	ComboboxAddItem(cbx,"Item e",NULL,NULL);
	ComboboxAddItem(cbx,"Item f",NULL,NULL);

	InsertWidget(DeskTop,WIDGET(cbx));

	WidgetDraw(WIDGET(cbx),NULL);*/

	SkinCombobox = GetSkinItem("Combobox");

	APPEXPORT(NewComboBox);
	APPEXPORT(ComboboxAddItem);
	APPEXPORT(ComboboxItemIndex);
	APPEXPORT(ComboboxSelectIndex);
	APPEXPORT(ComboboxSelectItem);

	return true;
}

void Close (void)
{

}
