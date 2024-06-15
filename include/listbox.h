#ifndef _LISTBOX_H_INCLUDED_
#define _LISTBOX_H_INCLUDED_

#include "widget.h"

typedef struct TListboxItem *PListboxItem;
typedef struct TListboxItem
{
	l_text			Caption;
	p_bitmap		Icon;

	l_bool			State;

	l_int			Offset;
} TListboxItem;

typedef struct TListboxRow *PListboxRow;
typedef struct TListboxRow
{
	PList ItemList;

	l_bool			State;
	TRect			BufAbs;

	l_ptr			Data;
	p_bitmap		Icon;

	PListboxItem	(*AddItem)(PListboxRow Row, l_int Offset, l_text Caption);
} TListboxRow;

typedef struct TListbox *PListbox;
typedef struct TListbox
{
	struct TWidget o;

	PList			RowList;

	PListboxRow		SelectedRow;

	PListboxRow		(*AddRow)(PListbox List, l_ptr Data);
	PListboxItem	(*AddItem)(PListbox List, l_text Caption, void* Data, p_bitmap Icon);
	PListboxItem	(*AddSeperator)( PListbox List );

	l_ulong			OnSelect;
	l_ulong			OnValidate;
} TListbox;

#define LISTBOX(o) ((PListbox)(o))

PListbox CreateListbox ( PApplication App, TRect r );

#endif /* _LISTBOX_H_INCLUDED_ */
