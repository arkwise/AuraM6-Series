/////////////////////////////////////////////////////////////////////////////////
//
//	Open With
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
#include "tabbook.h"
#include "fms2.h"
#include "tbox.h"
#include "iodlg.h"
////////////////////////////////////////////////////////////////////////////////
l_ulong AppVersion	= ULONG_ID(0,0,1,0);
char AppName[]		= "Open with...";
l_uid	nUID		= "openwth";
l_uid NeededLibs[]	= { "menu", "treeview", "iodlg", "window","fms2","tabbook","textbox", "" };
////////////////////////////////////////////////////////////////////////////////
PWindow w	= 0;
l_text 		File;
PFileInfo 	FFile;
PTreeView   tv;
PDataTypeEx DTEBin;
PTabBook bk;

PTextbox ext;
#define MSG_SELECTFILE 	0x00010002

PFileTypes Filter = 0;
////////////////////////////////////////////////////////////////////////////////
typedef struct TTreeviewActionItem *PTreeviewActionItem;
typedef struct TTreeviewActionItem
{
	struct TTreeItem o;
	
	PDTAction Act;

} TTreeviewActionItem;

#define TREEVIEWACTIONITEM(o) ((PTreeviewActionItem)(o))
////////////////////////////////////////////////////////////////////////////////
PTreeItem AddTreeActionItem ( PTreeItem o, l_text Caption, l_text Key, PIcon Icon,void  (*GenerateSubs)(PTreeItem), PDTAction Act  ) {
	PTreeviewActionItem i = malloc(sizeof(TTreeviewActionItem));
	if ( !i ) return NULL;
	TreeviewInitItem(TREEVIEWITEM(i),Caption,Key,Icon,GenerateSubs,NULL);
	i->Act = Act;
	TreeviewAddItem(o,TREEVIEWITEM(i));
	return TREEVIEWITEM(i);
}
////////////////////////////////////////////////////////////////////////////////
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
				MessageBox(&Me, "About Open With", "Open With \nAura File Managment System\n\nCopyright (c) 2008 Finn Technologies. All rights reserved.", MBB_OK);
				return true;
			}
			break;
			case MSG_SELECTFILE:
			{
				l_text afile = IOBox("Select applications", IOBOX_OPEN, NULL, Filter, true);
				if ( afile ) {
					TextBoxSetTextEx(ext,afile);
					free(afile);
				}
			}
			break;
			case MSG_OK:
			{
				if ( bk->Current ) 
					if ( WidgetGetIndex(WIDGET(bk->Current)) ) {
						PDTAction act;
						
						if ( !tv->Sel ) return true;
						if ( !TREEVIEWACTIONITEM(tv->Sel)->Act ) return true;
						
						act = TREEVIEWACTIONITEM(tv->Sel)->Act;
						
						ExecutePDAction(act,FFile,0,NULL);
						
						CloseApp(&Me);
						
					} else {
						
						DynLdRun(ext->Text,FFile->VPathName);
						
						
						CloseApp(&Me);
						
					}
				
				return true;
			}
			break;

		}
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////
void MakeDTEItems ( PTreeItem i, PDataTypeEx dte ) {
	PTreeItem si;
	if ( dte->Actions->Last ) {
		PListItem a = dte->Actions->Last->Next, b = a;
		do {
			AddTreeActionItem(i,DTACTION(a->Data)->Name,NULL,NULL,0,DTACTION(a->Data));
			
			a = a->Next;
		} while ( a != b );
	}
}
////////////////////////////////////////////////////////////////////////////////
l_int Main ( int argc, l_text *argv )
{
	TRect r, wr;
	PTab t;
	PButton b;
	PLabel l;
	
	DTEBin = _GetDataTypeEx(DTYPE_BINDATA);

	if ( argc == 1 ) {
		MessageBox(&Me, "Error", "This app can be only called by internal functions !", MBB_OK|MBI_ERROR);
		return false;
	}
	
	File = TextDup(argv[1]);
	
	FFile = FileGetInfo(File);
	
	if ( !FFile ) {
		free(File);
		MessageBox(&Me, "Error", "Unable to found specified file... (check system configuration)", MBB_OK|MBI_ERROR);
		return false;
	}
	
	Filter = 	NewFileTypes("DynLd executable","app,dl",
				NewFileTypes("All files",NULL,
				NULL ));
	
	RectAssign(&wr,0, 0, 300, 380);
	w = CreateWindow(&Me, wr, "Open with ...", WF_NORMAL|WF_CENTERED|WF_RESIZE);
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	InsertWidget(WIDGET(DeskTop), WIDGET(w));

	
	RectAssign(&r, 0, wr.b.y-30, 95, wr.b.y-5);
	b = CreateButton(&Me,r,"Ok",MSG_OK);
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r, 100, wr.b.y-30, 190, wr.b.y-5);
	b = CreateButton(&Me,r,"Cancel",WM_CLOSE);
	InsertWidget(WIDGET(w), WIDGET(b));
	
	
	RectAssign(&r, 0, 0, wr.b.x, wr.b.y-35);
	bk = CreateTabBook(&Me,r);
	InsertWidget(WIDGET(w), WIDGET(bk));

	t = TabBookAddTab(bk,"System actions",NULL);
	
	RectAssign(&r,0, 0, (WIDGET(t)->ChildArea.b.x - WIDGET(t)->ChildArea.a.x), (WIDGET(t)->ChildArea.b.y - WIDGET(t)->ChildArea.a.y));
	tv = CreateTreeView(&Me,r,0);
	InsertWidget(WIDGET(t), WIDGET(tv));

	if ( DTEs->Last ) {
		PListItem a = DTEs->Last->Next, b = a;
		PTreeItem i;
		do {
			
			if ( !(DATATYPEEX(a->Data)->Flags & DTEF_STRICTINTERNAL) ) {
				PIcon Icon = 0;
				if ( DATATYPEEX(a->Data)->Default ) Icon = DATATYPEEX(a->Data)->Default;
				if ( !Icon ) Icon = DTEBin?DTEBin->Default:0;
				//i = AddTreeItem(tv->Items,DATATYPEEX(a->Data)->TypeName,NULL,Icon,0);
				i = AddTreeActionItem(tv->Items,DATATYPEEX(a->Data)->TypeName,NULL,Icon,0,NULL);

				MakeDTEItems(i,DATATYPEEX(a->Data));
			}
			a = a->Next;
		} while ( a != b );	
	}
	
	
	t = TabBookAddTab(bk,"External application",NULL);
	
	RectAssign(&r, 0, 0, 250, 15);
	l = CreateLabel(&Me,r,"Select application to open the file");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l));

	RectAssign(&r, 0, r.a.y+20, 225, r.b.y+25);
	ext = CreateTextbox(&Me,r,0);
	InsertWidget(WIDGET(t), WIDGET(ext));

	RectAssign(&r, 230, r.a.y, 250, r.b.y);
	b = CreateButton(&Me,r,"...",MSG_SELECTFILE);
	InsertWidget(WIDGET(t), WIDGET(b));

	
	
	WidgetDrawAll(WIDGET(w));

	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{
	WidgetDispose(WIDGET(w));
	if ( File ) free(File);
	if ( FFile ) FreeFileInfo(FFile);
	if ( Filter ) FreeFileTypes(Filter);

}
////////////////////////////////////////////////////////////////////////////////
