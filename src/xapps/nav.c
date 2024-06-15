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
#include "menu.h"
#include "button.h"
#include "window.h"
#include "treeview.h"
#include "iodlg.h"
#include "fms2.h"
#include "toolbar.h"

l_ulong AppVersion	= ULONG_ID(0,0,1,0);
char AppName[]		= "File Navigateur";
l_uid	nUID		= "app:nav";
l_uid NeededLibs[]	= { "menu", "treeview", "iodlg", "window", "" };

/**
 * Messages
 */
#define MSG_TREEVIEW_ONSEL	0x1256
#define MSG_DBLCLK			0x2266
#define MSG_TB_GOUP			0x4266

#define MSG_VIEWICONS		0x4267
#define MSG_VIEWLIST		0x4268
#define MSG_VIEWMOSC		0x4269
/**
 * Widgets
 */
PTreeDirview t	= 0;
PDirview s	= 0;
PToolbar b	= 0;
PWindow w	= 0;

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
				MessageBox(&Me, "About Navigateur", "Navigateur 0.1\nThe oZone GUI file manager\n\nCopyright (c) 2003 Point Mad, Lukas Lipka. All rights reserved.", MBB_OK);
				return true;
			}
			break;

			case MSG_IODLG_ITEMFILE_VAL:
			{
				if ( DIRVIEWITEM(Event->Extra)->f->Attributes & FA_DIREC )
				{
					TreeDirViewOpenDir(t,FileNameToPath(DIRVIEW(s)->Directory, LISTVIEWITEM(Event->Extra)->Caption));
				}
				else
				{
					RunFile2(DIRVIEWITEM(Event->Extra)->f,0,NULL);
					return true;
				}
			}
			break;

			case MSG_TB_GOUP:
			{
				DirviewOpenParentDirectory(s);
				TreeDirViewOpenDir(t,s->Directory);
				return true;
			}
			break;
			
			case MSG_VIEWICONS:
			{
				ListviewSetStyle(LISTVIEW(s),LVS_ICONS,0);
				WidgetDrawAll(WIDGET(w));
				return true;
			}
			break;
			case MSG_VIEWLIST:
			{
				ListviewSetStyle(LISTVIEW(s),LVS_LIST,0);
				WidgetDrawAll(WIDGET(w));
				return true;
			}
			break;
			case MSG_VIEWMOSC:
			{
				ListviewSetStyle(LISTVIEW(s),LVS_MOSC,0);
				WidgetDrawAll(WIDGET(w));
				return true;
			}
			break;

		}
	}

	return false;
}

l_int Main ( int argc, l_text *argv )
{
	TRect r;
	PWidget spl;
	
	RectAssign(&r,0, 0, 520, 380);
	w = CreateWindow(&Me, r, "Navigateur", WF_NORMAL|WF_CENTERED|WF_RESIZE);
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	InsertWidget(WIDGET(DeskTop), WIDGET(w));

	PMenu Menu = NewMenu(
     NewMenuItem( "File", NULL, NULL, NULL,
     		 NewMenu (
     		 		/*NewMenuItem( "Open", NULL, NULL, NULL,
     		 			NewMenu (
     		 				NewMenuItem( "From HDD", NULL, NULL, NULL, NULL,
     		 				NewMenuItem( "From WWW", NULL, NULL, NULL, NULL,
     		 				NewMenuItem( "From FTP", NULL, NULL, NULL, NULL,
     		 				NewMenuItem( "From RES", NULL, NULL, NULL, NULL,
     		 				NULL))))
     					 ),*/
     		 		NewMenuItem( "Exit", NULL, WM_CLOSE, NULL, NULL, NULL)//)
     		 ),
     NewMenuItem( "Edit", NULL, NULL, NULL, NULL,
     NewMenuItem( "View", NULL, NULL, NULL, 
     	NewMenu (
     		 		NewMenuItem( "Icons", NULL, MSG_VIEWICONS, NULL, NULL, 
     		 		NewMenuItem( "List", NULL, MSG_VIEWLIST, NULL, NULL, 
     		 		NewMenuItem( "Mosaic", NULL, MSG_VIEWMOSC, NULL, NULL, 
     		 		
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

	PToolbarItem ti;
	RectAssign(&r, 10,30,510,75);
	b = CreateToolbar(&Me, r);
	InsertWidget(WIDGET(w), WIDGET(b));

	ti = CreateToolbarItem("Back", LoadImage("./SYSTEM/ICONS/back.bmp"), 0, TI_DISABLED|TI_TEXTUNDERICON);
	ToolbarAddItem(b,ti);
	ti = CreateToolbarItem("Forward", LoadImage("./SYSTEM/ICONS/forward.bmp"), 0, TI_DISABLED|TI_TEXTUNDERICON);
	ToolbarAddItem(b,ti);
	ti = CreateToolbarItem("Up", LoadImage("./SYSTEM/ICONS/up.bmp"), MSG_TB_GOUP, TI_TEXTUNDERICON);
	ToolbarAddItem(b,ti);

	RectAssign(&r,5, 90, 160, 375);
	t = CreateTreeDirview(&Me,r);
	WIDGET(t)->Flags |= WF_AUTORESIZE|WF_MAGNETBOTTOM|WF_MAGNETRIGHT;
	InsertWidget(WIDGET(w), WIDGET(t));

	RectAssign(&r,161, 90, 164, 380);
	spl = CreateVSpliter(&Me,r);
	InsertWidget(WIDGET(w), spl);

	RectAssign(&r,165, 90, 515, 375);
	t->Linked = s = CreateDirview(&Me,r,"/",NULL,DVF_NOPARICON|DVF_MULTISELECT);
	WIDGET(s)->AppEvHdl = &AppEventHandler;
	WIDGET(s)->Flags |= WF_AUTORESIZE|WF_MAGNETRIGHT|WF_MAGNETBOTTOM;
	/*if ( SLLWDG(s)->Horizontal )
	WIDGET(SLLWDG(s)->Horizontal)->Flags |= WF_AUTORESIZE|WF_MAGNETRIGHT|WF_MAGNETBOTTOM|WF_FIXEDHEIGHT;
	if ( SLLWDG(s)->Vertical )
	WIDGET(SLLWDG(s)->Vertical)->Flags |= WF_AUTORESIZE|WF_MAGNETRIGHT|WF_MAGNETBOTTOM|WF_FIXEDWIDTH;
	*/
	LISTVIEW(s)->Style = LVS_ICONS;
	InsertWidget(WIDGET(w), WIDGET(s));

	if ( argc > 1 ) {
		TreeDirViewOpenDir(t,argv[1]);
		DirviewOpenDirectory(s,argv[1]);
	}


	WidgetDrawAll(WIDGET(w));

	return true;
}

void Close (void)
{
	WidgetDispose(WIDGET(w));
}
