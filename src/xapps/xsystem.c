////////////////////////////////////////////////////////////////////////////////
//
//	Navigateur 0.10
//		Codename Phoenix file explorer
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "button.h"
#include "window.h"
#include "treeview.h"

l_ulong AppVersion	= ULONG_ID(0,0,0,0);
char AppName[]		= "xSystem Manager";
l_uid	nUID		= "app:xsystem";
l_uid NeededLibs[]	= { "treeview", "button", "window", "" };

extern PList DynLdApplications;

/**
 * Messages
 */
#define MSG_PROCESS_KILL	0x1256
#define MSG_VIEW_ONSELECT	0x1258

/**
 * Widgets
 */
PTreeView t	= 0;
PButton b	= 0;
PWindow w	= 0;

////////////////////////////////////////////////////////////////////////////////
typedef struct TTreeviewAppItem *PTreeviewAppItem;
typedef struct TTreeviewAppItem
{
	struct TTreeItem o;
	
	PApplication App;

} TTreeviewAppItem;

#define TREEVIEWACTIONITEM(o) ((PTreeviewAppItem)(o))
////////////////////////////////////////////////////////////////////////////////
PTreeItem AddTreeAppItem ( PTreeItem o, l_text Caption, l_text Key, PIcon Icon,void  (*GenerateSubs)(PTreeItem), PApplication App  ) {
	PTreeviewAppItem i = malloc(sizeof(TTreeviewAppItem));
	if ( !i ) return NULL;
	TreeviewInitItem(TREEVIEWITEM(i),Caption,Key,Icon,GenerateSubs,NULL);
	i->App = App;
	TreeviewAddItem(o,TREEVIEWITEM(i));
	return TREEVIEWITEM(i);
}

l_bool AppEventHandler ( PWidget o, PEvent Event )
{
	if ( Event->Type == EV_MESSAGE )
	{
		switch ( Event->Message )
		{
			case WM_CLOSE:
			{
				CloseApp(&Me);
				return true;
			}
			break;

			case WM_ABOUT:
			{
				MessageBox (&Me, "About xSystem", "xSystem 1.0\nA system runtime manager.\n\nCopyright (c) 2004 Lukas Lipka. All rights reserved.", MBB_OK);
				return true;
			}
			break;

		        case MSG_PROCESS_KILL:
			{
				if (t->Sel)
				{
					if ( TREEVIEWACTIONITEM(t->Sel)->App ) {
						DebugMessage ("Killing app - %s", TREEVIEWACTIONITEM(t->Sel)->App->Name);
						BreakApp (TREEVIEWACTIONITEM(t->Sel)->App);
					}
				}

				return true;
			}  
			break;

		}
	}

	return false;
}

void GenerateProcessList (PTreeItem o)
{
	PListItem a = DynLdApplications->Last->Next;
	if (!a) return;

	do
	{
		AddTreeAppItem (o, APPLICATION(a->Data)->Name, NULL, APPLICATION(a->Data)->Icon, 0, APPLICATION(a->Data));

		a = a->Next;
	}
	while (a != DynLdApplications->Last->Next);

	WidgetDrawAll (WIDGET(t));
}

l_int Main ( int argc, l_text *argv )
{
	TRect r;

	RectAssign (&r, 0, 0, 380, 420);
	w = CreateWindow(&Me, r, "xSystem", WF_NORMAL|WF_CENTERED);
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	InsertWidget(WIDGET(DeskTop), WIDGET(w));

	RectAssign(&r,10, 10, 370, 350);
	t = CreateTreeView(&Me, r, MSG_VIEW_ONSELECT);
	InsertWidget(WIDGET(w), WIDGET(t));

	AddTreeAppItem(t->Items, "DynLD", NULL, NULL, &GenerateProcessList, NULL);

	RectAssign(&r, 190, 375, 275, 410);
	b = CreateButton(&Me, r, "Kill", MSG_PROCESS_KILL );
	//b->Icon = LoadImage("./SYSTEM/ICONS/close.bmp");
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r, 285, 375, 370, 410);
	b = CreateButton(&Me, r, "Close", WM_CLOSE );
	b->Icon = LoadImage("./SYSTEM/ICONS/close.bmp");
	InsertWidget(WIDGET(w), WIDGET(b));


	WidgetDrawAll(WIDGET(w));

	return true;
}

void Close (void)
{
	WidgetDispose(WIDGET(w));
}
