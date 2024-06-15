////////////////////////////////////////////////////////////////////////////////
//
//  Keyboard Settings - Core File
//
//  Copyright (c) 2003 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "label.h"
#include "button.h"
#include "window.h"
#include "combobox.h"

l_ulong AppVersion = ULONG_ID(0,0,1,0);
char    AppName[] = "Keyboard settings";
l_uid	nUID = "cp:keyb";
l_uid NeededLibs[] = { "widget","button","label","menu","window","combobox","" };

PCombobox clay;
#define BTNWIDTH 80
#define BTNHEIGHT 22
#define BTNSPACE 5

l_bool AppEventHandler ( PWidget o, PEvent Event )
{
	if ( Event->Type == EV_MESSAGE )
	{
		switch ( Event->Message )
		{
			case MSG_CANCEL:
			case WM_CLOSE:
			{
				WidgetDispose(WIDGET(o));
				CloseApp(&Me);
				
				return true;
			}
			break;


			case MSG_APPLY:
			case MSG_OK:
			{
				l_ulong idx = ComboboxItemIndex(clay,clay->Selected);
				
				if ( idx > 1 ) {
					l_text key = TextArgs("/SYSTEM/KEYBOARD/LAYOUTS/%s",clay->Selected->Caption);
					KeySetText("/SYSTEM/KEYBOARD/LAYOUT",KeyGetText(key,""));
					free(key);
				} else
					KeySetText("/SYSTEM/KEYBOARD/LAYOUT","");

				KeyboardInstallLayout();
				
				if ( Event->Message == MSG_OK ){
					WidgetDispose(WIDGET(o));
					CloseApp(&Me);
					
				}

				return true;
			}
			break;

		}
	}

	return false;
}



l_int Main ( int argc, l_text *argv )
{
	PWindow w	= 0;
	PButton b = 0;
	PLabel l = 0;
	l_ulong idx = 1, i = 2;
	PKeyboardLayout SysKL =  KeyboardGetLayout();
	PRegKey a, e;
	PRegKey p = ResolveKey("/SYSTEM/KEYBOARD/LAYOUTS");

	TRect r;

	RectAssign(&r,0, 0, 300, 150);

	w = CreateWindow(&Me, r, "Keyboard settings", WF_CAPTION|WF_FRAME|WF_CENTERED|WF_MINIMIZE);
	InsertWidget(WIDGET(DeskTop), WIDGET(w));


	WidgetSize(&r, 5, 20, 100, 20);
	l = CreateLabel(&Me,r,"Keyboard layout");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));

	WidgetSize(&r, 110, 20, 150, 20);
	clay = NewComboBox(&Me,r,NULL);
	InsertWidget(WIDGET(w), WIDGET(clay));
	ComboboxAddItem(clay,"United States",NULL);
  if ( p ) 
		if ( p->Last ) {
			a = e = p->Last->Next;
		  do {
			  ComboboxAddItem(clay,a->Name,NULL);
			  if ( SysKL ) if ( !TextCaseCompare(SysKL->Name,a->Name) ) idx = i;
			  i++;
			  a = a->Next;
		  } while ( a != e );
		}	
 	ComboboxSelectIndex(clay,idx);
	
	RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - BTNWIDTH/2 - BTNSPACE - BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5 - BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - BTNWIDTH/2 - BTNSPACE, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
	b = CreateButton(&Me, r, "OK", MSG_OK );
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) - BTNWIDTH/2, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5 - BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + BTNWIDTH/2, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
	b = CreateButton(&Me, r, "Cancel", MSG_CANCEL );
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r,((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + BTNWIDTH/2 + BTNSPACE, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y  - 5 - BTNHEIGHT, ((WIDGET(w)->ChildArea.b.x - WIDGET(w)->ChildArea.a.x)/2) + BTNSPACE + BTNWIDTH/2 + BTNWIDTH, WIDGET(w)->ChildArea.b.y - WIDGET(w)->ChildArea.a.y - 5);
	b = CreateButton(&Me, r, "Apply", MSG_APPLY );
	InsertWidget(WIDGET(w), WIDGET(b));

	WIDGET(w)->AppEvHdl = &AppEventHandler;

	WidgetDrawAll(WIDGET(w));

	return true;
}

void Close (void)
{

}
