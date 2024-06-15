#include"widget.h"

#ifndef _menu_h_included_
#define _menu_h_included_

typedef struct TMenu *PMenu;
typedef struct TMenuItem *PMenuItem;

typedef struct TMenu {

	PMenuItem Items;
	PMenuItem Current;

} TMenu;

typedef struct TMenuItem {

	l_text    Caption;
	l_text 	  Name;
	p_bitmap  Icon;
	l_ulong   Message;

	l_char    State; // 0 -> Nothing, 1 -> Selected, 2 -> Down

	l_ulong   Flags;

	PMenuItem Next;
	PMenu     SubMenu;

	TRect    BufAbs;


	void *Args;
	void (*FreeArgs)(void*);

	void (*GenerateSubMenu)(PMenuItem,void*);

} TMenuItem;

#define MIF_FREEICON   0x08


#define MENUITEM(o) ((PMenuItem)(o))



typedef struct TMenuView *PMenuView;

typedef struct TMenuView {

	struct TWidget o;

	l_char Style;

	PMenu Menu;

	PMenuView Child;
	PMenuView Parent;

	l_ulong   Flags;

	PWidget   MsgDest;

} TMenuView;

#define MENUVIEW(o) ((PMenuView)(o))

#define MenuViewStyleHorizontal 0x00
#define MenuViewStyleVertical   0x01
#define MenuViewStyleDock 0x00

#define MNF_DISPOSEONEVENT   0x01
#define MVF_FREEMENU		0x02

#define MI_DISABLE   0x01

PMenuView PopUpMenu ( PApplication App, TPoint p, PMenu M, PWidget MsgDest,l_ulong Options );
l_ulong   PopUpMenuWait ( PApplication App, TPoint p, PMenu M,l_ulong Options );

#define NewMenuView CreateMenuView

PMenuView CreateMenuView ( PApplication A, TRect r, PMenu M, l_char S, l_ulong F );
PMenu     NewMenu ( PMenuItem Itm );
PMenuItem NewMenuItem ( l_text Caption, p_bitmap Icon, l_ulong Message, l_ulong Flags, PMenu SubMenu, PMenuItem Nxt );
PMenuItem NewMenuItemEx ( l_text Caption, p_bitmap Icon, l_ulong Message, l_ulong Flags, PMenu SubMenu, void (*GenerateSubMenu)(PMenuItem,void*), void *Args, PMenuItem Nxt );

PMenuItem NewMenuItemSeparator ( PMenuItem Next );

PMenuItem AddMenuItemEx ( PMenu m, l_text Caption, p_bitmap Icon, l_ulong Message, l_ulong Flags, PMenu SubMenu, void (*GenerateSubMenu)(PMenuItem,void*), void *Args );
PMenuItem AddMenuItem ( PMenu m, l_text Caption, p_bitmap Icon, l_ulong Message, l_ulong Flags, PMenu SubMenu );
PMenuItem AddMenuItemSeparator ( PMenu m );

PMenuItem DuplicateMenuItem ( PMenuItem i );
PMenu DuplicateMenu ( PMenu m );

void FreeMenu ( PMenu o );

#endif /*_menu_h_included_*/
