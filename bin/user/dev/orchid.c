////////////////////////////////////////////////////////////////////////////////
//
//	Orchid Web Browser
//
//	Copyright (c) 2010 Chase Finn, Finn Technologies. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "button.h"
#include "window.h"
#include "tbox.h"
#include "iodlg.h"
#include "toolbar.h"

char AppName[]		= "Orchid";
l_uid	nUID = "app:Orchid";
l_uid NeededLibs[] = { "menu","button","iodlg","window","textbox","toolbar","" };


#define  MSG_FILENEW	0xF0020001
#define  MSG_FILEOPEN	0xF0020002
#define  MSG_FILESAVE	0xF0020003
#define  MSG_FILESAVEAS	0xF0020004
#define  MSG_GO	0xF0020005


PTextbox tb = 0;
PButton b = 0;
PTextbox addtb = 0;
PWindow w = 0;
PToolbar bar = 0;
PFileTypes Filter = 0;
l_text OpenedFile = 0;


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

			case MSG_FILENEW:
			{
				TextBoxSetTextEx(tb,"");
				WindowSetCaptionEx(w,"Type Writer");

				return true;
			}
			break;

			case MSG_GO:
			{
				l_text tmp = OpenedFile;

				OpenedFile = TextDup(addtb->Text);

				if ( OpenedFile )
				{
					l_text txt = LoadData2(OpenedFile,TYPE_TEXT);

					if ( txt )
					{
						TextBoxSetTextEx(tb,txt);
						free(txt);
						
					}

					free(tmp);
				}
				return true;
			}
			break;
			case MSG_FILEOPEN:
			{
				l_text tmp = OpenedFile;

				OpenedFile = IOBox("Open text file (UTF-8)", IOBOX_OPEN, NULL, Filter, true);

				if ( OpenedFile )
				{
					l_text txt = LoadData2(OpenedFile,TYPE_TEXT);

					if ( txt )
					{
						TextBoxSetTextEx(addtb,OpenedFile);
						TextBoxSetTextEx(tb,txt);
						free(txt);
						
					}

					free(tmp);
				}
				return true;
			}
			break;
			case MSG_FILESAVE:
			case MSG_FILESAVEAS:
			{
				l_text File;

				if (Event->Message == MSG_FILESAVEAS)
				{
					free(OpenedFile);
					OpenedFile = IOBox("Save text file (UTF-8)", IOBOX_SAVE, NULL, Filter, false);
				}

				if ( OpenedFile )
				{
					l_text txt = TextDup(tb->Text);

					if ( SaveData2(OpenedFile, txt, TYPE_TEXT ) )
					{
						WindowSetCaption(w,"Type Writer");
					}

					free(txt);
				}
				return true;
			}
			break;

			case WM_ABOUT:
			{
				MessageBox(&Me, "About Type Writer", "Type Writer 2.2\nThe Aura Word Processer\n\nCopyright (c) 2008 Finn Technologies. All rights reserved.", MBB_OK);
				return true;
			}
			break;
		}
	}

	return false;
}

void GenerateEditSubMenu ( PMenuItem o ,void* Args) {
	o->SubMenu = NewMenu(
		NewMenuItem( "Copy", NULL, MSG_COPY, (TEXTBOX(tb)->SelPos!=TEXTBOX(tb)->SelPosEnd)?0:MI_DISABLE,NULL,
     	NewMenuItem( "Cut",	NULL, MSG_CUT, (TEXTBOX(tb)->SelPos!=TEXTBOX(tb)->SelPosEnd)&&(TEXTBOX(tb)->Flags & TBF_EDITABLE)?0:MI_DISABLE,NULL,
     	NewMenuItem( "Paste", 	NULL, 	MSG_PASTE, 	((TEXTBOX(tb)->Flags & TBF_EDITABLE) && ClipboardGet(TYPE_TEXT,NULL))?0:MI_DISABLE,NULL,
     	NewMenuItemSeparator(
     	NewMenuItem( "Delete", 	NULL, MSG_DELETE, (TEXTBOX(tb)->SelPos!=TEXTBOX(tb)->SelPosEnd)&&(TEXTBOX(tb)->Flags & TBF_EDITABLE)?0:MI_DISABLE,NULL,
     	NULL))))));
}

l_int Main ( int argc, l_text *argv )
{

	TRect r;
	PToolbarItem ti;

	RectAssign(&r,0, 0, 540, 380);
	w = CreateWindow(&Me, r, "Orchid Browser", WF_NORMAL|WF_MAXIMIZE|WF_CENTERED|WF_RESIZE);
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	InsertWidget(WIDGET(DeskTop), WIDGET(w));


	Filter = 	NewFileTypes("HTML files","htm,html",
						NewFileTypes("Text files","txt",
						NewFileTypes("All files",NULL,
						NULL )));

	PMenu Menu = NewMenu(
     NewMenuItem( "File", NULL, NULL, NULL,
     		 NewMenu (
     		 		NewMenuItem( "New",  NULL, MSG_FILENEW, NULL, NULL,
     		 		NewMenuItem( "Open", NULL, MSG_FILEOPEN, NULL, NULL,
     		 		NewMenuItem( "Save", NULL, MSG_FILESAVE, NULL, NULL,
     		 		NewMenuItem( "Save as...", NULL, MSG_FILESAVEAS, NULL, NULL,
     		 		NewMenuItem( "Exit", NULL, WM_CLOSE, NULL, NULL, NULL)))))
     		 ),
     NewMenuItemEx("Edit",NULL,0,0,NULL,&GenerateEditSubMenu,NULL,
     NewMenuItem( "View", NULL, NULL, NULL, NULL,
     NewMenuItem( "Help", NULL, NULL, NULL,
     	NewMenu(
     		NewMenuItem( "About", NULL, WM_ABOUT, NULL, NULL, NULL)),
     NULL))))
	);

	RectAssign(&r,0,0,540,20);
	PMenuView o = NewMenuView(&Me,r,Menu,MenuViewStyleHorizontal,0);
	InsertWidget(WIDGET(w), WIDGET(o));

	RectAssign(&r, 510, 25, 540, 40);
	b = CreateButton(&Me,r,"Go",MSG_GO);
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r,0,25,505,40);
	addtb = CreateTextbox(&Me, r, TBF_EDITABLE);
	WIDGET(addtb)->Flags |= WF_AUTORESIZE|WF_MAGNETRIGHT;
	InsertWidget(WIDGET(w), WIDGET(addtb));

	RectAssign(&r,10,45,530,370);
	tb = CreateTextbox(&Me, r, TBF_EDITABLE|TBF_MULTILINE);
	WIDGET(tb)->Flags |= WF_AUTORESIZE|WF_MAGNETRIGHT|WF_MAGNETBOTTOM;
	InsertWidget(WIDGET(w), WIDGET(tb));


	if ( argc > 1 )
	{
		l_text txt = LoadData2(argv[1],TYPE_TEXT);
		OpenedFile = TextDup(argv[1]);

		if ( txt )
		{
			TextBoxSetTextEx(addtb,argv[1]);
			TextBoxSetTextEx(tb,txt);
			free(txt);
		}
	}

	WidgetDrawAll(WIDGET(w));
	return true;
}

void Close (void)
{
	if ( OpenedFile ) free(OpenedFile);

	WidgetDispose(WIDGET(w));
	FreeFileTypes(Filter);
}
