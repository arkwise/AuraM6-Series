/////////////////////////////////////////////////////////////////////////////////
//
//	Listbox Core File 
//
//	Aura Milestone 3 
//
//	(c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "listbox.h"

// TODO
//	+ Memory leaks
//	+ Document
//  + Messages

l_uid	nUID = "listbox";
l_ulong	AppVersion = ULONG_ID(0,0,0,1);
l_char	AppName[] = "Listbox Widget";

PWidgetClass ListboxClass;

PSkin SkinListbox = 0;

/**
*	NAME: LBAddItem
*	DESCRIPTION: Adds an item to a listbox (doesnt depend on row)
*	RETURN: Pointer to the added item
*/
PListboxItem  LBAddItem ( PListbox List, l_text Caption, void* Data, p_bitmap Icon )
{
	PListboxRow Row = 0;

	Row = List->AddRow(List, Data);
	Row->Icon = Icon;

	return Row->AddItem(Row, 0, Caption);
}

/**
*	NAME: RAddItem
*	DESCRIPTION:
*	RETURN:
*/
PListboxItem  RAddItem ( PListboxRow Row, l_int Offset, l_text Caption )
{
	PListboxItem o = NEW(TListboxItem);
	CLEAR(o);

	o->Offset	= Offset;
	o->Caption	= TextDup(Caption);

	ListAdd(Row->ItemList, NULL, o, NULL);

	return o;
}

/**
*	NAME: LBAddRow
*	DESCRIPTION:
*	RETURN:
*/
_PUBLIC PListboxRow  LBAddRow ( PListbox List, void* Data )
{
	PListboxRow o = NEW(TListboxRow);
	CLEAR(o);

	o->ItemList = NewList();
	o->AddItem	= &RAddItem;
	o->Data		= Data;

	ListAdd(List->RowList, NULL, o, NULL);

	return o;
}

/**
*	NAME: LBAddRow
*	DESCRIPTION:
*	RETURN:
*/
_PUBLIC PListboxItem  LBAddSeperator ( PListbox List )
{
	return List->AddItem(List, "\001\002\003", 0, 0);
}

void ListboxUpdateBufferAbs ( PListbox o )
{
	l_int y = 0;
	l_int yy = 0;

	PListItem a = LISTBOX(o)->RowList->Last->Next;
	if (!a) return;

	do
	{
		if ( ((PListboxRow)(a->Data))->Icon )
			yy = max(((PListboxRow)(a->Data))->Icon->w, text_height(default_font)+2);
		else
			yy = text_height(default_font) + 2;

		RectAssign(&((PListboxRow)(a->Data))->BufAbs, WIDGET(o)->Absolute.a.x + 5, WIDGET(o)->Absolute.a.y+5+y, WIDGET(o)->Absolute.b.x - 5, WIDGET(o)->Absolute.a.y + 5 + y + yy - 1);

		y += yy;

		a = a->Next;
	}
	while (a != LISTBOX(o)->RowList->Last->Next);
}

/**
*	NAME: LBDraw
*	DESCRIPTION: Default draw function
*/
void LBDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	if ( !w ) w = &o->Absolute;

	if ( UseSkins )
		DrawSkin(buffer, SkinListbox, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
	else {
		rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,COL_BOXFACE);
		Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y,COL_BOXLIGHT,COL_BOXDARK);
	}
	
	l_int y = 0;
	l_int yy = 0;

	if (!LISTBOX(o)->RowList) return;

	PListItem a = LISTBOX(o)->RowList->Last->Next;

	if (!a) return;

	ListboxUpdateBufferAbs(LISTBOX(o));

	do
	{
		PListItem b = ((PListboxRow)(a->Data))->ItemList->Last->Next;
		if (!b) return;

		if ( !TextCompare(((PListboxItem)(b->Data))->Caption, "\001\002\003") )
		{
			hline(buffer, o->Absolute.a.x+5, (((PListboxRow)(a->Data))->BufAbs.a.y+((PListboxRow)(a->Data))->BufAbs.b.y)/2, o->Absolute.b.x-5, makecol(0,0,0));
			yy = text_height(default_font) + 2;
		}
		else
		{
			if ( ((PListboxRow)(a->Data))->Icon )
				yy = max(((PListboxRow)(a->Data))->Icon->w, text_height(default_font)+2);
			else
				yy = text_height(default_font) + 2;

			l_int xx = 0;

			if (((PListboxRow)(a->Data))->State)
				rectfill(buffer, ((PListboxRow)(a->Data))->BufAbs.a.x, ((PListboxRow)(a->Data))->BufAbs.a.y, ((PListboxRow)(a->Data))->BufAbs.b.x, ((PListboxRow)(a->Data))->BufAbs.b.y, makecol(200,200,200));

			if (((PListboxRow)(a->Data))->Icon)
			{
				draw_sprite(buffer, ((PListboxRow)(a->Data))->Icon, o->Absolute.a.x+5, o->Absolute.a.y + 5 + y);
				xx = ((PListboxRow)(a->Data))->Icon->w + 5;
			}

			/**
			 * Draw all cells in this row
			 */
			do
			{
				if (b->Next != ((PListboxRow)(a->Data))->ItemList->Last->Next && ((PListboxItem)(b->Data))->Offset + text_length(default_font, ((PListboxItem)(b->Data))->Caption) >= ((PListboxItem)(b->Next->Data))->Offset)
					DrawNiceText(buffer, default_font, o->Absolute.a.x+5+xx+((PListboxItem)(b->Data))->Offset, o->Absolute.a.y + 5 + y, ((PListboxItem)(b->Data))->Offset-5, ((PListboxItem)(b->Data))->Caption, makecol(0,0,0));
				else
					textout_ex(buffer, default_font, ((PListboxItem)(b->Data))->Caption, o->Absolute.a.x+5+xx+((PListboxItem)(b->Data))->Offset, o->Absolute.a.y + 5 + y, makecol(0,0,0), -1);

				b = b->Next;
			}
			while (b != ((PListboxRow)(a->Data))->ItemList->Last->Next);
		}

		y += yy;

		a = a->Next;
	}
	while (a != LISTBOX(o)->RowList->Last->Next);
}

/**
*	NAME: LBEventHandler
*	DESCRIPTION: Default event handler
*/
l_bool LBEventHandler ( PWidget o, PEvent Ev )
{
	if (Ev->Type == EV_MOUSE)
	{
		if (Ev->Message == WEvMouseLDown || (Ev->Message == WEvMouseMove && Mouse->State.b & BUTTON_LEFT))
		{
			PListItem a = LISTBOX(o)->RowList->Last->Next;
			if (!a) return false;

			do
			{
				if (CURSOR_IN_RECT(((PListboxRow)(a->Data))->BufAbs))
				{
					if ( !( ((PListboxRow)(a->Data)) == LISTBOX(o)->SelectedRow ) )
					{
						if (LISTBOX(o)->SelectedRow)
						{
							LISTBOX(o)->SelectedRow->State = false;
							WidgetDraw(o, &LISTBOX(o)->SelectedRow->BufAbs);
						}

						if ( TextCompare( ((PListboxItem)(((PListboxRow)(a->Data))->ItemList->Last->Data))->Caption, "\001\002\003" ) )
						{
							LISTBOX(o)->SelectedRow = ((PListboxRow)(a->Data));
							LISTBOX(o)->SelectedRow->State = true;
							WidgetDraw(o, &LISTBOX(o)->SelectedRow->BufAbs);
						}
						else LISTBOX(o)->SelectedRow = 0;
					}

					return true;
				}
				a = a->Next;
			}
			while (a != LISTBOX(o)->RowList->Last->Next);

			if (LISTBOX(o)->SelectedRow)
			{
				LISTBOX(o)->SelectedRow->State = false;
				WidgetDraw(o, &LISTBOX(o)->SelectedRow->BufAbs);
				LISTBOX(o)->SelectedRow = 0;
			}
		}

		if ( Ev->Message == WEvMouseLUp )
		{
			if (LISTBOX(o)->OnSelect && LISTBOX(o)->SelectedRow)
				WidgetSendSEvent(o, EV_MESSAGE, LISTBOX(o)->OnSelect, LISTBOX(o)->SelectedRow->Data);
		}

		if ( Ev->Message == WEvMouseLDClk )
		{
			if (LISTBOX(o)->OnValidate && LISTBOX(o)->SelectedRow)
				WidgetSendSEvent(o, EV_MESSAGE, LISTBOX(o)->OnValidate, LISTBOX(o)->SelectedRow->Data);
		}
	}

	return false;
}

void ListboxFreeEx ( PWidget o )
{
	DebugMessage("LISTBOX::Leaking memory :P");
}

/**
*	NAME: CreateListbox
*	DESCRIPTION: Creates a new listbox widget
*	RETURN: Pointer to the new listbox
*/
PListbox CreateListbox ( PApplication App, TRect r )
{
	PListbox p = malloc(sizeof(TListbox));

	if (!p) return NULL;

	memset(p, 0, sizeof(TListbox));

	IntialiseWidget(App, ListboxClass, WIDGET(p), r );

	WIDGET(p)->Draw = &LBDraw;
	WIDGET(p)->EventHandler = &LBEventHandler;
	WIDGET(p)->FreeEx = &ListboxFreeEx;

	p->AddItem		= &LBAddItem;
	p->AddRow		= &LBAddRow;
	p->AddSeperator	= &LBAddSeperator;

	p->RowList = NewList();

	return p;
}

l_bool LibMain ( int argc, l_text *argv )
{	
	
	ListboxClass = CreateWidgetClass( ULONG_ID('L','s','t','B'), StdWidgetClass, 0 );
	ListboxClass->StdCreate = (void*)&CreateListbox;
	

	SkinListbox = GetSkinItem("Listbox");

	APPEXPORT(CreateListbox);

	/*TRect t = {20,50, 440, 280};

	PListbox l = 0;
	PListboxRow r = 0;

	l = CreateListbox(&Me, t);

	r = l->AddRow(l);
	r->AddItem(r, 0, "ASM", NULL); r->AddItem(r, 50, "hardcore", NULL);
	r = l->AddRow(l);
	r->AddItem(r, 0, "C", NULL); r->AddItem(r, 50, "medium", NULL);
	r = l->AddRow(l);
	r->AddItem(r, 0, "C++", NULL); r->AddItem(r, 50, "medium/hard", NULL);
	r = l->AddRow(l);
	r->AddItem(r, 0, "C#", NULL); r->AddItem(r, 50, "piece of cake", NULL);
	l->AddItem(l, "VB .NET", NULL, NULL);
	l->AddItem(l, "ASP", NULL, NULL);
	l->AddItem(l, "HTML", NULL, NULL);
	r = l->AddRow(l);
	r->AddItem(r, 0, "XML", NULL); r->AddItem(r, 50, "oZone GUI will be the best", NULL); r->AddItem(r, 100, "Screw me", NULL);
	l->AddItem(l, "Other ...", NULL, NULL);

	InsertWidget(DeskTop, WIDGET(l));
	WidgetDraw(WIDGET(l), NULL);*/

	return true;
}

void Close (void)
{

}
