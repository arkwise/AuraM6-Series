////////////////////////////////////////////////////////////////////////////////
//
//	DMS2 & FMS2 Info 0.1
//
//	(c) Copyright 2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "button.h"
#include "window.h"
#include "treeview.h"
#include "fms2.h"

l_ulong AppVersion	= ULONG_ID(0,1,0,0);
char AppName[]		= "DMS & FMS Info";
l_uid	nUID		= "app:nav";
l_uid NeededLibs[]	= { "menu", "treeview", "iodlg", "window","fms2", "" };

PTreeView t	= 0;
PWindow w	= 0;
PDataTypeEx DTEBin;

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
				MessageBox(&Me, "About ''DMS & FMS  info 0.1'' part of FMS2", "Copyright (c) 2004 Point Mad, Lukas Lipka. All rights reserved.", MBB_OK);
				return true;
			}
			break;

		}
	}

	return false;
}
void MakeDTEItems ( PTreeItem i, PDataTypeEx dte ) {
	
	PTreeItem si;

	if ( dte->Actions->Last ) {
		PListItem a = dte->Actions->Last->Next, b = a;
		do {
			AddTreeItem(i,DTACTION(a->Data)->Name,NULL,NULL,0);
			a = a->Next;
		} while ( a != b );
	}
	
}

void MakeCodecItems ( PTreeItem i, PCodec c ) {
	
	PTreeItem si;

	si = AddTreeItem(i,"Supported data types",NULL,NULL,NULL);
	
	if ( c->SubCodecs->Last ) {
		PListItem a = c->SubCodecs->Last->Next, b = a;
		do {
			PDataTypeEx dte = _GetDataTypeEx(SUBCODEC(a->Data)->TypeId);
			l_text Name;
			PIcon Icon = NULL;
			if ( dte ) {
				if ( dte->Default ) Icon = dte->Default;
				Name = TextDup(dte->TypeName);
			} else
				Name = TextArgs("Type : %08x",SUBCODEC(a->Data)->TypeId);
			if ( !Icon ) Icon = DTEBin?DTEBin->Default:NULL;	
			AddTreeItem(si,Name,NULL,Icon,0);
			free(Name);
			a = a->Next;
		} while ( a != b );
	}
	
	si = AddTreeItem(i,"Files extentions",NULL,NULL,NULL);
	if ( FExts->Last ) {
		PListItem a = FExts->Last->Next, b = a;
		do {
			if ( FEXT(a->Data)->CodecId == c->CodecId ) {
				AddTreeItem(si,a->Key,NULL,NULL,0);
			}
			a = a->Next;
		} while ( a != b );
	}
	
	si = AddTreeItem(i,"MIME types",NULL,NULL,NULL);
	if ( MimeDats->Last ) {
		PListItem a = MimeDats->Last->Next, b = a;
		do {
			if ( MIMEDAT(a->Data)->CodecId == c->CodecId ) {
				AddTreeItem(si,a->Key,NULL,NULL,0);
			}
			a = a->Next;
		} while ( a != b );
	}
}

l_int Main ( int argc, l_text *argv )
{
	TRect r;
	PTreeItem iDTEs, iCodecs,i;
	 DTEBin = _GetDataTypeEx(DTYPE_BINDATA);
	
	
	RectAssign(&r,0, 0, 520, 380);
	w = CreateWindow(&Me, r, "DMS/FMS Info", WF_NORMAL|WF_CENTERED|WF_RESIZE);
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	InsertWidget(WIDGET(DeskTop), WIDGET(w));

	PMenu Menu = NewMenu(
     NewMenuItem( "File", NULL, NULL, NULL,
     		 NewMenu (
     		 		NewMenuItem( "Exit", NULL, WM_CLOSE, NULL, NULL, NULL)//)
     		 ),
     NewMenuItem( "Edit", NULL, NULL, NULL, NULL,
     NewMenuItem( "View", NULL, NULL, NULL, NULL,
     NewMenuItem( "Help", NULL, NULL, NULL,
     	NewMenu(
     		NewMenuItem( "About", NULL, WM_ABOUT, NULL, NULL, NULL)),
     NULL))))
	);

	RectAssign(&r,0,0,(WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x),20);
	PMenuView o = NewMenuView(&Me,r,Menu,MenuViewStyleHorizontal,0);
	InsertWidget(WIDGET(w), WIDGET(o));

	RectAssign(&r,0, 25, (WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x), (WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y));
	t = CreateTreeView(&Me,r,0);
	InsertWidget(WIDGET(w), WIDGET(t));

	iDTEs = AddTreeItem(t->Items,"Data types extenders",NULL,NULL,NULL);
	
	if ( DTEs->Last ) {
		PListItem a = DTEs->Last->Next, b = a;
		do {
			PIcon Icon = 0;
			if ( DATATYPEEX(a->Data)->Default ) Icon = DATATYPEEX(a->Data)->Default;
			if ( !Icon ) Icon = DTEBin?DTEBin->Default:0;

			i = AddTreeItem(iDTEs,DATATYPEEX(a->Data)->TypeName,NULL,Icon,0);
			MakeDTEItems(i,DATATYPEEX(a->Data));
			a = a->Next;
		} while ( a != b );
		
	}
	
	iCodecs = AddTreeItem(t->Items,"Codecs",NULL,NULL,NULL);
	
	if ( Codecs->Last ) {
		PListItem a = Codecs->Last->Next, b = a;
		do {
			PIcon Icon = 0;
			
			if ( CODEC(a->Data)->Default ) {
				Icon = CODEC(a->Data)->Default;
			}  
			
			if ( !Icon && CODEC(a->Data)->FavoriteType ) {
				PDataTypeEx dte = _GetDataTypeEx(CODEC(a->Data)->FavoriteType);
				if ( dte ) if ( dte->Default ) Icon = dte->Default;
			}
			
			if ( !Icon ) Icon = DTEBin?DTEBin->Default:0;
			
			if ( !CODEC(a->Data)->Name ) {
				l_text name = TextArgs("Id: %08x",CODEC(a->Data)->CodecId);
				i = AddTreeItem(iCodecs,name,NULL,Icon,0);
				free(name);
			} else 
				i = AddTreeItem(iCodecs,CODEC(a->Data)->Name,NULL,Icon,0);

			MakeCodecItems(i,CODEC(a->Data));
				
			a = a->Next;
		} while ( a != b );
		
	}
	
	WidgetDrawAll(WIDGET(w));

	return true;
}

void Close (void)
{
	WidgetDispose(WIDGET(w));
}
