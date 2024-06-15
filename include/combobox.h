////////////////////////////////////////////////////////////////////////////////
//
//  combobox - Header File
//
//  (c) Copyright 2003 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include"listview.h"

#ifndef _COMBOBOX_H_INCLUDED_
#define _COMBOBOX_H_INCLUDED_

typedef struct TCombobox *PCombobox;
typedef struct TCombobox
{
	struct TWidget o;

	PList Items;
	
	PListviewItem Selected;
	
	l_uchar State;
	l_ulong OnUpdate;
	
} TCombobox;

#define COMBOBOX(o) ((PCombobox)(o))

PCombobox NewComboBox ( PApplication App, TRect r, l_ulong OnUpdate );
void ComboboxAddItem ( PCombobox o, char* Caption, PIcon Icon );

l_ulong ComboboxItemIndex 	( PCombobox o, PListviewItem a );
void 		ComboboxSelectIndex ( PCombobox o, l_ulong Index );
void 		ComboboxSelectItem 	( PCombobox o, PListviewItem p );

#endif /* _COMBOBOX_H_INCLUDED_ */
