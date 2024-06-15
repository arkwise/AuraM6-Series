#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "label.h"
#include "button.h"
#include "sound.h"
#include "listview.h"
#include "tabbook.h"
#include "menu.h"
#include "iodlg.h"

l_ulong AppVersion	= ULONG_ID(0,0,0,1);
char    AppName[]	= "Libraries setup";
l_uid	nUID		= "ap:lib";
l_uid NeededLibs[]	= { "widget","" };

PWindow w;
PListview lsta;
PListview lbdv;
PListview ldrv;
PListview llib;

#define MSG_REGISTERLIB 0x00060001
#define MSG_UNREGISTERLIB 0x00060002

PFileTypes Filter = 0;

////////////////////////////////////////////////////////////////////////////////
l_bool DynLdLibraryGetUid ( l_text file, l_uid *id ) {
	l_text key,n;
	TDynLdHeader DynHead;
	PFile f = FileOpen(file, "rb");
	l_uid r;
	
	if ( !f )
	{
		DebugError("DYNLD :: File not found - %s", file);
		return false;
	}

	FileRead(&DynHead, 1, sizeof(TDynLdHeader), f);
	if ( DynHead.Magic != ULONG_ID('D','n','L','d') )
	{
		FileClose(f);
		DebugError("DYNLD :: Library is not a valid DynLD. Library not installed.");
		return false;
	}

	if ( DynHead.FileFormatVersion > DYNLDVERSION )
	{
		FileClose(f);
		DebugError("DYNLD :: Library has invalid version number of DynLD linker. Library not installed.");
		return false;
	}

	FileClose(f);

	memcpy(id,&DynHead.UID,sizeof(l_uid));
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void RegenerateKeys ( PListview l, l_text key ) {
	PListItem a,b;
	if ( l->Items->Last ) {
		l_text t;
		DeleteKey(key);
		CreateKey(key);
		a = b = l->Items->Last->Next;
		do {
			if ( LISTVIEWITEM(a->Data)->Flags & LVI_CHECKED ) {
				t = TextArgs("%s/%s",key,LISTVIEWITEM(a->Data)->Caption);
				CreateKey(t);
				free(t);
			}
			a = a->Next;
		} while ( a != b );
	}
}
////////////////////////////////////////////////////////////////////////////////
l_bool AppEventHandler ( PWidget o, PEvent Ev )
{
	if ( Ev->Type == EV_MESSAGE )
	{
		if ( Ev->Message == WM_CLOSE )
		{
			CloseApp(&Me);

			return true;
		}
		
		if ( Ev->Message == MSG_OK )
		{
			
		  	RegenerateKeys(lsta,"/SYSTEM/AUTOLIB");
			RegenerateKeys(lbdv,"/SYSTEM/BASICDRIVERS");
			RegenerateKeys(ldrv,"/SYSTEM/DRIVERS");

			CloseApp(&Me);

			return true;
		}
		if ( Ev->Message == MSG_REGISTERLIB )
		{
				l_text file = IOBox("Register a library", IOBOX_OPEN, NULL, Filter, true);
				if ( file )
				{
					l_uid id = { 0 };
					if ( DynLdLibraryGetUid(file,&id ) ) {
						ListviewAddItem ( lsta, id, NULL );
						ListviewAddItem ( lbdv, id, NULL );
						ListviewAddItem ( ldrv, id, NULL );
						ListviewAddItem ( llib, id, NULL );

						DynLdInstallLibrary(file);
					}
					free(file);
				}
			return true;
		}
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
void GenerateItems ( PListview l, l_text Stuff ) {
	
	PRegKey o;
  	o =	ResolveKey("/SYSTEM/LIBRARIES");

  	if ( o )
		if ( o->Last ) {
			PRegKey a = o->Last->Next;
			PRegKey b = a;
			PListviewItem i;
			l_text t;
			do {
				i = ListviewAddItem ( l, a->Name, NULL );
				if ( Stuff ) {
					t = TextArgs("%s/%s",Stuff,a->Name);
					if ( KeyExists(t) ) i->Flags |= LVI_CHECKED;
					free(t);
				}	
				a = a->Next;
			} while ( a != b );
		}

}
////////////////////////////////////////////////////////////////////////////////
l_int Main ( int argc, l_text *argv )
{
	TRect r;
	PButton b = NULL;
	PRegKey o;
	PTabBook bk;
	PTab t;
	
	RectAssign(&r,0, 0, 250, 325);
	w = CreateWindow( &Me, r, "Libraries", WF_FRAME|WF_CAPTION|WF_MINIMIZE|WF_CENTERED );
	InsertWidget(DeskTop, WIDGET(w));
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	
	RectAssign(&r,0, 0, 250, 19);
	
	Filter = 	NewFileTypes("Libraries","dl",
						NewFileTypes("All files",NULL,
						NULL ));
	
	PMenu Menu = NewMenu(
     NewMenuItem( "Libraries", NULL, NULL, NULL,
     		 NewMenu (
     		 		NewMenuItem( "Install a new library",  NULL, MSG_REGISTERLIB, NULL, NULL,
     		 		NewMenuItem( "Remove selected", NULL, MSG_UNREGISTERLIB, NULL, NULL,
     		 		NULL ))
     		 ),
     NewMenuItem( "Help", NULL, NULL, NULL,
     		 NewMenu (
     		 		NewMenuItem( "Informations on startup phases",  NULL, 0, NULL, NULL,
     		 		NULL )
     		 ),
     NULL))
	);

	RectAssign(&r,0,0,540,20);
	PMenuView mv = NewMenuView(&Me,r,Menu,MenuViewStyleHorizontal,0);
	InsertWidget(WIDGET(w), WIDGET(mv));

	
	RectAssign(&r,0, 20, 250, 300);
	bk = CreateTabBook(&Me,r);
	InsertWidget(WIDGET(w), WIDGET(bk));
	
	t = TabBookAddTab(bk,"Libraries",NULL);
	RectAssign(&r,0, 0, 120, 240);
	llib = CreateListview(&Me,r,"Libraries",LVS_LIST,0);

	InsertWidget(WIDGET(t), WIDGET(llib));
		
	t = TabBookAddTab(bk,"Basic drivers",NULL);
	
	RectAssign(&r,0, 0, 240, 240);
	lbdv = CreateListview(&Me,r,"Libraries",LVS_LIST,LVF_CHECKBOXES);
	InsertWidget(WIDGET(t), WIDGET(lbdv));
	
	t = TabBookAddTab(bk,"Drivers",NULL);
	
	RectAssign(&r,0, 0, 240, 240);
	ldrv = CreateListview(&Me,r,"Libraries",LVS_LIST,LVF_CHECKBOXES);
	InsertWidget(WIDGET(t), WIDGET(ldrv));
	
	t = TabBookAddTab(bk,"Autolib",NULL);
	
	RectAssign(&r,0, 0, 240, 240);
	lsta = CreateListview(&Me,r,"Libraries",LVS_LIST,LVF_CHECKBOXES|LVF_COLUMS);
	InsertWidget(WIDGET(t), WIDGET(lsta));


	RectAssign(&r, 0, 305, 95, 325);
	b = CreateButton(&Me,r,"Ok",MSG_OK);
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r, 100, 305, 195, 325);
	b = CreateButton(&Me,r,"Cancel",WM_CLOSE);
	InsertWidget(WIDGET(w), WIDGET(b));
	
  	GenerateItems(llib,NULL);

  	GenerateItems(lsta,"/SYSTEM/AUTOLIB");
	GenerateItems(lbdv,"/SYSTEM/BASICDRIVERS");
	GenerateItems(ldrv,"/SYSTEM/DRIVERS");
	
	WidgetDrawAll(WIDGET(w));
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////
void Close (void)
{
	WidgetDispose(WIDGET(w));
}
////////////////////////////////////////////////////////////////////////////////
