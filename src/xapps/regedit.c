/////////////////////////////////////////////////////////////////////////////////
//
//	Registry Editor
//
//	Aura Milestone 3 
//
//	(c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "button.h"
#include "window.h"
#include "treeview.h"
#include "combobox.h"
#include "label.h"
#include "tbox.h"
#include "fms2.h"

l_ulong AppVersion	= ULONG_ID(0,0,0,1);
char AppName[]		= "Registry Editor";
l_uid	nUID = "app:regedit";
l_uid NeededLibs[] = { "textbox","combobox","label","button","menu","treeview","window","" };


#define MSG_RELOADREG 	0x00010001
#define MSG_SAVEREG   	0x00010002
#define MSG_NEWKEY   		0x00010003
#define MSG_DELETEKEY   0x00010004
#define MSG_RENAMEKEY   0x00010005

#define MSG_TREESEL     0x00010006
#define MSG_SAVEVALUE   0x00010007

#define MSG_KEYMENU   0x00010008


PTreeView t	= 0;
	PWindow w	= 0;
PLabel    lbl = 0;
PTextbox  val = 0;
PCombobox typ = 0;
PIcon DirIcon = NULL;


void  GenerateSubs ( PTreeItem o ) {
	PRegKey a, b;
	PRegKey p = ResolveKey(o->Key);
	l_text Path;
	
	if ( !p ) return;
	
	EmptySubTreeItems(o);
	
	if ( !p->Last ) return;
	
	a = b = p->Last->Next;
	do {
		Path = FileNameToPath(o->Key,a->Name);
		AddTreeItem(o,a->Name,Path,DirIcon,&GenerateSubs);
		free(Path);
		a = a->Next;
	} while ( a != b );
	
	WidgetDrawAll(WIDGET(t));

}
PMenu GenerateItemMenu ( PTreeView o, PTreeItem i ) {

	if ( o->Items->Last != i )


	return NewMenu(
     NewMenuItem( "New", NULL, MSG_NEWKEY, NULL, NULL,
     NewMenuItem( "Delete", NULL, MSG_DELETEKEY, NULL, NULL,
     NewMenuItem( "Rename", 	NULL, MSG_RENAMEKEY, NULL, NULL,
     NULL )))
	);

	else

	return NewMenu(
     	NewMenuItem( "Reload", NULL, MSG_RELOADREG, NULL, NULL,
     	NewMenuItem( "Save", NULL, MSG_SAVEREG, NULL, NULL,
     	NULL))
	);

}

void ShowKey ( l_text K ) {

	if ( K ) {
		PRegKey Key = ResolveKey(K);

		LabelSetText(lbl,K);

		if ( Key->Type == RKT_NODATA )
			TextBoxSetText(val,"(none)");
		else if ( Key->Type == RKT_CHAR || Key->Type == RKT_BOOL )
			TextBoxSetText(val,"%d",*((l_char*)Key->Data));
		else if ( Key->Type == RKT_SHORT )
			TextBoxSetText(val,"%d",*((l_short*)Key->Data));
		else if ( Key->Type == RKT_LONG )
			TextBoxSetText(val,"%d",*((l_long*)Key->Data));
		else if ( Key->Type == RKT_TEXT )
			TextBoxSetText(val,Key->Data);
		else
			TextBoxSetText(val,"(unsupported)");

		ComboboxSelectIndex(typ,Key->Type+1);

	} else {
		LabelSetText(lbl,"...");
		TextBoxSetText(val,"...");
		ComboboxSelectIndex(typ,0);
	}


}


l_bool AppEventHandler ( PWidget o, PEvent Ev )
{
	switch (Ev->Message)
	{
		case MSG_KEYMENU:
		{
			if ( t->Sel ) {
				l_ulong Message;
				PMenu m = GenerateItemMenu(t,t->Sel);
				Message = PopUpMenuWait(o->AppOwner,Mouse->State.p,m,MVF_FREEMENU);
				WidgetSendSEvent(o, EV_MESSAGE, Message, NULL);
			}
			return true;
		}
		break;

		case MSG_SAVEREG:
		{
			RegistrySave();
			return true;
		}
		break;

		case MSG_RELOADREG:
		{
			RegistryReLoad();
			EmptySubTreeItems(t->Items);
			AddTreeItem(t->Items,"Registry","/",NULL,&GenerateSubs);

			WidgetDrawAll(WIDGET(w));

			return true;
		}
		break;

		case WM_ABOUT:
		{

			MessageBox ( &Me, "Registry Editor","Registry Editor 0.1\n\nPart of oZone (codename phoenix)\n\n(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.", MBB_OK );


			return true;
		}
		break;

		case MSG_NEWKEY:
		{

			if ( t->Sel ) {
				l_text Path;

				NewKey((l_text)t->Sel->Key,"New Key");
				
				Path = FileNameToPath((l_text)t->Sel->Key,"New Key");
				AddTreeItem(t->Sel,"New Key",Path,DirIcon,&GenerateSubs);
				free(Path);
				
				TreeviewExpendItem(t,t->Sel);
				

			}
			return true;
		}
		break;

		case MSG_RENAMEKEY:
		{

			if ( t->Sel ) {
				PTreeItem Itm = t->Sel;
				l_text txt = TreeViewRenameItemBox(t,Itm);

				if ( txt ) {
					l_text Parent = GetParentKeyName((l_text)Itm->Key);
					EmptySubTreeItems(Itm);
					RenameKey(Itm->Key,txt);
					TreeViewRenameItem(t,Itm,txt);
					free(Itm->Key);
					Itm->Key = FileNameToPath(Parent,txt);
					free(Parent);
					free(txt);
					if ( t->Sel == Itm ) ShowKey((l_text)Itm->Key);
				}
			}
			return true;
		}

		break;
		case MSG_DELETEKEY:
		{
			if ( t->Sel ) {
				DeleteKey((l_text)t->Sel->Key);
				TreeViewDeleteItem(t,t->Sel);

				if ( t->Sel )
					ShowKey((l_text)t->Sel->Key);
				else
					ShowKey(NULL);
			}
			return true;
		}

		break;

		case MSG_TREESEL:
		{

			if ( t->Sel ) {
				ShowKey((l_text)t->Sel->Key);
			} else {
				ShowKey(NULL);
			}

			return true;
		}

		break;
		case MSG_SAVEVALUE:
		{

			if ( t->Sel && typ->Selected ) {
				l_text Value = TextDup(val->Text);
				l_text Key = (l_text)t->Sel->Key;
				l_ulong Type = ComboboxItemIndex(typ,typ->Selected)-1;

				if ( Type == RKT_NODATA )
					KeySetNothing(Key);
				else if ( Type == RKT_TEXT )
					KeySetText(Key,Value);
				else if ( Type == RKT_INT )
					KeySetInt(Key,atoi(Value));
				else if ( Type == RKT_CHAR )
					KeySetChar(Key,atoi(Value));
				else if ( Type == RKT_BOOL )
					KeySetBool(Key,atoi(Value));
				else if ( Type == RKT_SHORT )
					KeySetShort(Key,atoi(Value));
				else
					TextBoxSetText(val,"UNSUPORTED!");
				free(Value);

			}
			return true;
		}
		break;
	}

	return false;
}

l_int Main ( int argc, l_text *argv )
{
	PLabel l;
	PButton b;
	PDataTypeEx dirdte;

	TRect r;

	
	
	if ( dirdte = _GetDataTypeEx(DTYPE_DIRECTORY) ) DirIcon = DuplicateIcon(dirdte->Default);
	
	RectAssign(&r,0, 0, 510, 340);
	w = CreateWindow(&Me, r, "Registry Editor", WF_NORMAL|WF_CENTERED);
	//WIDGET(w)->AppEvHdl = &AppEventHandler;
	InsertWidget(WIDGET(DeskTop), WIDGET(w));

	PMenu Menu = NewMenu(
     NewMenuItem( "File", NULL, NULL, NULL,
     		 NewMenu (
     		 		NewMenuItem( "Exit", NULL, WM_CLOSE, NULL, NULL,
     		 		NULL)
     		 ),
     NewMenuItem( "Registry", NULL, NULL, NULL,
     		 NewMenu (
     		 		NewMenuItem( "Reload", NULL, MSG_RELOADREG, NULL, NULL,
     		 		NewMenuItem( "Save", NULL, MSG_SAVEREG, NULL, NULL,
     		 		NULL))
     		 ),
     NewMenuItem( "Key", NULL, NULL, NULL,
     		 NewMenu (
     		 		NewMenuItem( "New", NULL, MSG_NEWKEY, NULL, NULL,
     		 		NewMenuItem( "Delete", NULL, MSG_DELETEKEY, NULL, NULL,
     		 		NewMenuItem( "Rename", NULL, MSG_RENAMEKEY, NULL, NULL,
     		 		NULL)))
     		 ),
     NewMenuItem( "Help", NULL, NULL, NULL,
     	NewMenu(
     		NewMenuItem( "About", NULL, WM_ABOUT, NULL, NULL, NULL)),
     NULL))))
	);

	RectAssign(&r,0,0,500,20);

	PMenuView o = NewMenuView(&Me,r,Menu,MenuViewStyleHorizontal,0);
	InsertWidget(WIDGET(w), WIDGET(o));

	RectAssign(&r,10, 30, 300, 330);
	t = CreateTreeView(&Me,r,MSG_TREESEL);
	t->OnMenuMsg = MSG_KEYMENU;
	InsertWidget(WIDGET(w), WIDGET(t));

	AddTreeItem(t->Items,"Registry","/",DirIcon,&GenerateSubs);

	RectAssign(&r, 305, 30, 500, 50);
	lbl = CreateLabel(&Me,r,"Welcome!");
	WIDGET(lbl)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(lbl));

	RectAssign(&r, 305, 55, 350, 75);
	l = CreateLabel(&Me,r,"Type :");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));

	RectAssign(&r, 355, 55, 500, 75);
	typ = NewComboBox(&Me,r,0);
	ComboboxAddItem(typ,"RKT_NODATA",NULL);
	ComboboxAddItem(typ,"RKT_CHAR",NULL);
	ComboboxAddItem(typ,"RKT_BOOL",NULL);
	ComboboxAddItem(typ,"RKT_SHORT",NULL);
	ComboboxAddItem(typ,"RKT_INT/RKT_LONG",NULL);
	ComboboxAddItem(typ,"RKT_BIG",NULL);
	ComboboxAddItem(typ,"RKT_FLOAT",NULL);
	ComboboxAddItem(typ,"RKT_DOUBLE",NULL);
	ComboboxAddItem(typ,"RKT_RECT",NULL);
	ComboboxAddItem(typ,"RKT_POINT",NULL);
	ComboboxAddItem(typ,"RKT_TEXT",NULL);
	InsertWidget(WIDGET(w), WIDGET(typ));


	RectAssign(&r, 305, 80, 350, 100);
	l = CreateLabel(&Me,r,"Value :");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));

	RectAssign(&r, 355, 80, 500, 100);
	val = CreateTextbox(&Me,r,TBF_EDITABLE);
	InsertWidget(WIDGET(w), WIDGET(val));


	RectAssign(&r, 355, 130, 500, 150);
	b = CreateButton(&Me,r,"Save",MSG_SAVEVALUE);
	InsertWidget(WIDGET(w), WIDGET(b));

	WIDGET(w)->AppEvHdl = &AppEventHandler;

	WidgetDrawAll(WIDGET(w));
	return true;
}

void Close (void)
{

	if ( DirIcon ) FreeIcon(DirIcon);

}
