////////////////////////////////////////////////////////////////////////////////
//
//  Treeview Widget - Header File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _TREEVIEW_H_INCLUDED_
#define _TREEVIEW_H_INCLUDED_

#include "sbar.h"
#include "menu.h"

/**	
	@addtogroup treeview
	@{
*/
/**
	@file
	Treeview widget header file.
*/


typedef struct TTreeItem *PTreeItem, *PTreeviewItem;
/**
	Treeview item structure
*/
typedef struct TTreeItem {

	l_text  Caption; ///< Caption of item
	l_text	Key; ///< Iternal string to identify item 
	l_char  Flags;
	PIcon Icon; ///< Icon of item
	TRect   BufAbs; 

	void  (*GenerateSubs)(PTreeItem);

	void (*FreeEx) ( PTreeviewItem i );

	PTreeItem Last;
	PTreeItem Next;
	PTreeItem Prev;
	PTreeItem Parent;
	
} TTreeItem, TTreeviewItem;

#define TREEVIEWITEM(o) ((PTreeviewItem)(o))


#define NO_SCROLL 0x10

#define TIF_EXPENDED 0x01
#define TIF_SELECTED 0x02
#define TIF_FREEICON 0x04
#define TIF_FREEARGS 0x08

typedef struct TTreeView *PTreeView, *PTreeview;
/**
	Treeview widget structure
*/
typedef struct TTreeView
{
	struct TScrollWidget o;

	PTreeItem Items; ///< root item (not displayed)
	PTreeItem Sel; ///< Selected item

	l_ulong OnSelMsg; ///< Message send when an item is selected
	l_ulong OnMenuMsg; ///< Message send when there is a right click on item
	
	l_ulong Flags;
	
	
	PTreeItem DragItem; 
	
	void 	(*ItemStartDrag) ( PWidget o );
	l_bool  (*ItemDropAccept) ( PWidget o, PWidget Caller, PTreeItem Item, l_ulong Type, void *Data, l_ulong SupportedMethods, l_ulong *Method );
	l_bool  (*ItemDragBeforeDrop)    ( PWidget o, PWidget Dest, PTreeItem Item, void *Data, l_ulong Type, l_ulong Method );
	void    (*ItemDropData)    ( PWidget o, PWidget Caller, PTreeItem Item, void *Data, l_ulong Type, l_ulong Method );
	void    (*ItemDragAfterDrop) ( PWidget o, PWidget Dest, PTreeItem Item, void*Data, l_ulong Type, l_ulong Method );
	
} TTreeView, TTreeview;

#define TVF_DRAWPLUSMINUS 0x1
#define TVF_DRAWICONS 0x2
#define TVF_ALLOWDRAG 0x4


#define TREEVIEW(o) ((PTreeView)(o))

PTreeView CreateTreeView( PApplication App, TRect r, l_ulong OnSelMsg );

void EmptySubTreeItems ( PTreeItem o );
PTreeItem AddTreeItem ( PTreeItem o, l_text Caption, l_text Key, PIcon Icon,void  (*GenerateSubs)(PTreeItem) );

void TreeviewExpendItem ( PTreeView o, PTreeItem i ) ;
void TreeviewUnExpendItem ( PTreeView o, PTreeItem i );

l_text TreeViewRenameItemBox ( PTreeView t, PTreeItem i );
void TreeViewRenameItem ( PTreeView t, PTreeItem i, l_text Caption );
void TreeViewDeleteItem ( PTreeView o, PTreeItem i );

void TreeViewSelectItem ( PTreeView o, PTreeItem i );
void TreeviewScrollBarRecalculate( PTreeView o );

PTreeView InitTreeView( PTreeView b, PWidgetClass Class, PApplication App, TRect r, l_ulong OnSelMsg );
l_bool TreeViewEventHandler ( PWidget o, PEvent Ev );


void TreeviewAddItem ( PTreeItem Parent, PTreeItem i );
PTreeItem TreeviewInitItem ( PTreeItem i, l_text Caption, l_text Key, PIcon Icon, void (*GenerateSubs)(PTreeItem),  void (*FreeEx) ( PTreeviewItem ) );


extern PWidgetClass TreeviewClass;

/**
@}
*/

#endif /* _TREEVIEW_H_INCLUDED_ */
