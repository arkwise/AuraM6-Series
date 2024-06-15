#ifndef _TOOLBAR_H_INCLUDED_
#define _TOOLBAR_H_INCLUDED_

#include "widget.h"
#include "button.h"

typedef struct TToolbar *PToolbar;
typedef struct TToolbar
{
	struct TWidget o;
	PList Items;
} TToolbar;

#define TOOLBAR(o) ((PToolbar)(o))

typedef struct TToolbarItem *PToolbarItem;
typedef struct TToolbarItem {
	l_text		Caption;
	p_bitmap	Icon;
	l_ulong		Message;

	l_ulong		Flags;

	TRect		BufAbs;

	PButton		Button;
} TToolbarItem;

#define TOOLBARITEM(o) ((PToolbarItem)(o))

#define TI_DISABLED			0x00000001
#define	TI_TEXTUNDERICON	0x00000002

PToolbar CreateToolbar( PApplication App, TRect r );
void ToolbarRemoveItem ( PToolbar b, PToolbarItem i );
void ToolbarAddItem ( PToolbar b, PToolbarItem i );
PToolbarItem CreateToolbarItem ( l_text Caption, p_bitmap Icon, l_ulong Message, l_ulong Flags );
void ToolbarItemEnable ( PToolbar t, PToolbarItem p );
void ToolbarItemDisable ( PToolbar t, PToolbarItem p );

#endif /* _TOOLBAR_H_INCLUDED_ */
