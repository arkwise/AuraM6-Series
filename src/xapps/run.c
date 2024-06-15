/////////////////////////////////////////////////////////////////////////////////
//
//	Run
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
#include "fms2.h"
#include "tbox.h"
#include "iodlg.h"
////////////////////////////////////////////////////////////////////////////////
l_ulong AppVersion	= ULONG_ID(0,1,0,0);
char AppName[]		= "Run";
l_uid	nUID		= "run";
l_uid NeededLibs[]	= { "menu", "iodlg", "window","fms2","textbox", "" };
////////////////////////////////////////////////////////////////////////////////
PWindow w	= 0;
l_text 		File;
PFileInfo 	FFile;
PTextbox ext;
#define MSG_SELECTFILE 	0x00010002
PFileTypes Filter = 0;
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
				MessageBox(&Me, "About ''Run''", "Copyright (c) 2004 Point Mad, Lukas Lipka. All rights reserved.", MBB_OK);
				return true;
			}
			break;
			case MSG_SELECTFILE:
			{
				l_text afile = IOBox("Select applications", IOBOX_OPEN, NULL, Filter, true);
				if ( afile ) {
					TextBoxSetText(ext,"\"%s\"",afile);
					free(afile);
				}
			}
			break;
			case MSG_OK:
			{
				PFileInfo f;
				l_int argc = 0;
				l_text *argv = 0;
				_ParseArgs(ext->Text,NULL,&argc,&argv);
				if ( argc ) {
					f = FileGetInfo(argv[0]);
					if ( f ) {
						if ( !RunFile2(f,argc-1,&argv[1]) )
							MessageBox(&Me, "Run : Error", "Unable to open or run file", MBB_OK|MBI_ERROR);
						else {
							FreeArgs(argc,argv);	
							CloseApp(&Me);
							return true;
						}
					} else 
						MessageBox(&Me, "Run : Error", "Unable to found file", MBB_OK|MBI_ERROR);
				} else 
					MessageBox(&Me, "Run : Error", "Please enter a command", MBB_OK|MBI_ERROR);
				FreeArgs(argc,argv);
				return true;
			}
			break;

		}
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_int Main ( int argc, l_text *argv )
{
	TRect r, wr;
	PButton b;
	PLabel l;
	PiWidget iw = ListKey(Me.Ressource,"run");
	
	Filter = 	NewFileTypes("DynLd executable","app,dl",
				NewFileTypes("All files",NULL,
				NULL ));
				
	w = (PWindow)ComplieiWidget(&Me,iw);
	
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	
	InsertWidget(WIDGET(DeskTop), WIDGET(w));
	
	ext = (PTextbox)WidgetGetName(WIDGET(w),"command");
		
	WidgetDrawAll(WIDGET(w));
/*	
	RectAssign(&wr,0, 0, 350, 100);
	w = CreateWindow(&Me, wr, "Run", WF_NORMAL|WF_CENTERED|WF_RESIZE);
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	InsertWidget(WIDGET(DeskTop), WIDGET(w));

	
	RectAssign(&r, 5, wr.b.y-30, 95, wr.b.y-5);
	b = CreateButton(&Me,r,"Ok",MSG_OK);
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r, 100, wr.b.y-30, 190, wr.b.y-5);
	b = CreateButton(&Me,r,"Cancel",WM_CLOSE);
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r, 5, 5, wr.b.x-40, 15);
	l = CreateLabel(&Me,r,"Command to run, or file to open");
	InsertWidget(WIDGET(w), WIDGET(l));

	RectAssign(&r, 5, r.a.y+20, wr.b.x-40, r.b.y+30);
	ext = CreateTextbox(&Me,r,TBF_EDITABLE);
	InsertWidget(WIDGET(w), WIDGET(ext));

	RectAssign(&r, wr.b.x-35, r.a.y, wr.b.x-5, r.b.y);
	b = CreateButton(&Me,r,"...",MSG_SELECTFILE);
	InsertWidget(WIDGET(w), WIDGET(b));
*/
	

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
