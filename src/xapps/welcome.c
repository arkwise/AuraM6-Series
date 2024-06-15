//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																																//
//	Welcome																											//
//																																//
//	Aura Milestone 4 																								//
//																																//
//	(c) Copyright 2013 Finn Technologies and Chase Finn. All rights reserved.	//
//																																//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 
#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "label.h"
#include "button.h"
#include "window.h"
#include "desktop/desktop.h"
////////////////////////////////////////////////////////////////////////////////
l_ulong AppVersion	= ULONG_ID(1,0,0,0);
char AppName[]		= "Welcome";
l_uid	nUID		= "app:welcome";
l_uid NeededLibs[]	= { "button","label", "window","widget","" };
////////////////////////////////////////////////////////////////////////////////

PWindow w	= 0;
PButton b	= 0;
#define BTN_DESKTOP 	0x00010002
#define BTN_SETUP 	0x00010003
////////////////////////////////////////////////////////////////////////////////
l_bool AppEventHandler ( PWidget o, PEvent Event )
{ 
	if ( Event->Type == EV_MESSAGE )
	{
		switch ( Event->Message )
		{
			case WM_CLOSE:
			{
			MessageBox(&Me, "Sorry", "You must choose an action to continue", MBB_OK);
						Authenticate();
						WidgetDispose ( WIDGET(w) );
						//CloseApp(&Me);
			return true;
			}
			break;
			

			case WM_ABOUT:
			{
				MessageBox(&Me, "About ''Welcome''", "Copyright (c) 2013 Finn Technologies, Chase Finn. All rights reserved.", MBB_OK);
				return true;
			}
			break;
 
			case BTN_DESKTOP:
               		{
						Authenticate();
						WidgetDispose ( WIDGET(w) );
						//CloseApp(&Me); 
			
                        return true;
                	}
                	break;

		    	case BTN_SETUP:
                	{
						WidgetDispose ( WIDGET(w) );
                        FileRun("applications/setup.app", NULL); 
                        return true;
                	}
	                break;

		}
	}

	return true;
}
  


////////////////////////////////////////////////////////////////////////////////
l_int Main ( int argc, l_text *argv )
{
	TRect r, wr;
	PButton b;
	PLabel l;

	RectAssign(&r,0, 0, 553, 294);
	w = CreateWindow( &Me, r, "Welcome", WF_NORMAL|WF_CENTERED|WF_RESIZE );
	WINDOW(w)->Background = ListKey(Me.Ressource,"WINFACE");
	WINDOW(w)->TitleActive = ListKey(Me.Ressource,"WINTACT");
	WINDOW(w)->TitlePassive = ListKey(Me.Ressource,"WINTPAS");
	InsertWidget(DeskTop, WIDGET(w));
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	  

	RectAssign(&r,99, 112, 499, 132);
	l = CreateLabel(&Me, r, "Welcome to Aura OS, ");
	WIDGET(l)->BackgroundColor = makecol(255,255,255);
	InsertWidget(WIDGET(w), WIDGET(l));
	
	RectAssign(&r,99, 152, 499, 172);
	l = CreateLabel(&Me, r, "What would you like to do next?");
	WIDGET(l)->BackgroundColor = makecol(255,255,255);
	InsertWidget(WIDGET(w), WIDGET(l));
	
	RectAssign(&r,362,241, 472, 270);
	b = CreateButton(&Me,r,"Live",BTN_DESKTOP);
	InsertWidget(WIDGET(w), WIDGET(b));
	
	RectAssign(&r,215,241, 325, 270);
	b = CreateButton(&Me,r,"Tour",BTN_DESKTOP);
	InsertWidget(WIDGET(w), WIDGET(b));
		
	RectAssign(&r,68, 241, 178, 270);
	b = CreateButton(&Me,r,"Install",BTN_SETUP);
	InsertWidget(WIDGET(w), WIDGET(b));
	
	WidgetDrawAll(WIDGET(w));


	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{
	Authenticate();
	WidgetDispose ( WIDGET(w) );
						//CloseApp(&Me);
    return;

}
////////////////////////////////////////////////////////////////////////////////
