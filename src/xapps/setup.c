/////////////////////////////////////////////////////////////////////////////////
//
//	System Installer
//
//	Aura Milestone 3 
//
//	(c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "button.h"
#include "label.h"
#include "canvas.h"
#include "window.h"
#include "timer.h"
#include "iodlg.h"
#include "tbox.h"

#define BTN_fdisk                  0x4263
#define BTN_fdos                  0x4264
#define BTN_aura                  0x4265
#define BTN_update                  0x4266
#define BTN_Ok                  0x4267
#define BTN_Ok2                 0x4268
#define WBB_OK                  0x4269
#define MSG_TREEVIEW_ONSEL      0x1256
#define MSG_select		        0x1257
#define MSG_doinstall		        0x1258
#define CB_SEL		        0x1259
#define CB_FILTER       0xFF00FFB0
PDirview s      = 0;
        PWidget spl;

l_ulong UID			= ULONG_ID('O','R','C','H');
l_ulong AppVersion	= ULONG_ID(0,0,0,1);
char AppName[]		= "Aura Setup";
l_uid	nUID		= "app:aossetup";

p_bitmap Bmp1;
PCanvas	c 	= 0;

_PUBLIC PCanvas Frame	= 0;
	TRect r, wr;
	PButton b;
	PButton b1;
	PButton b2;
	PButton b3;
	PButton b4;
	PButton b5;
	PButton a = 0;

	PLabel j;
	PLabel j11;
	PLabel j2;
	PLabel j3;
	PLabel j4;
	PWindow w	= 0;
	PTreeDirview t  = 0;
	PTextbox ext;
        PProgressPop p;

void Draw1 (PWidget o, p_bitmap buffer, PRect w)
{
	draw_sprite(buffer,Bmp1, (o->Absolute.a.x+o->Absolute.b.x-Bmp1->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp1->h)/2);
}

l_bool SysAppEventHandler ( PWidget o, PEvent Event )
{
	if ( Event->Type == EV_MESSAGE )
	{
		switch ( Event->Message )
		{
                	case BTN_Ok2:
              		{
				WidgetDispose ( WIDGET(w) );
                        	return true;
                	}
                	break;
									
		}
	}

	return false;
}

void CollectingData( void )
{

	RectAssign(&r,0, 0, 400, 255);
	w = CreateWindow( &Me, r, "Aura Setup", WF_FRAME|WF_CAPTION|WF_MINIMIZE|WF_CENTERED );
	InsertWidget(DeskTop, WIDGET(w));
	WIDGET(w)->AppEvHdl = &SysAppEventHandler;


	WidgetSize(&r,0, 0, 400, 41);
	c = CreateCanvas(&Me, r);
	WIDGET(c)->Draw = &Draw1;
	WIDGET(c)->Flags |= WFForceBuffer;
	InsertWidget(WIDGET(w), WIDGET(c));


	RectAssign(&r,5, 45, 250, 60);
	j = CreateLabel(&Me, r, "Where do you want Aura Installed?");
	WIDGET(j)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(j));
	

        RectAssign(&r,5, 65, 200, 190);
        s = CreateDirview(&Me,r,"/system/System/install/",NULL,DVF_NOPARICON|DVF_MULTISELECT);
        WIDGET(s)->Flags |= WF_AUTORESIZE|WF_MAGNETRIGHT|WF_MAGNETBOTTOM;
        LISTVIEW(s)->Style = LVS_ICONS;
        InsertWidget(WIDGET(w), WIDGET(s));

	RectAssign(&r,201, 65, 204, 190);
        spl = CreateVSpliter(&Me,r);
        InsertWidget(WIDGET(w), spl);

        RectAssign(&r,205, 65, 395, 190);
        t = CreateTreeDirview(&Me,r);
        WIDGET(t)->Flags |= WF_AUTORESIZE|WF_MAGNETBOTTOM|WF_MAGNETRIGHT;
        InsertWidget(WIDGET(w), WIDGET(t));

	RectAssign(&r,5, 195, 195, 225);
	j = CreateLabel(&Me, r, "Drag Aura to the desired diretory");
	WIDGET(j)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(j));

	RectAssign(&r,350, 230, 400, 250);
	b = CreateButton(&Me,r,"Done",BTN_Ok2);
	InsertWidget(WIDGET(w), WIDGET(b));
	
	WidgetDrawAll(WIDGET(w));

	return;
}

l_bool AppEventHandler ( PWidget o, PEvent Event )
{
	if ( Event->Type == EV_MESSAGE )
	{
		switch ( Event->Message )
		{
			case WM_CLOSE:
			{
				WidgetDispose(WIDGET(o));
				CloseApp(&Me);
				return true;
			}
			break;
			case BTN_Ok:
			{

                       		if ( MessageBox(&Me, "Its That Easy!", "Setup is now complete, You must now reboot.", MBB_OK) == MSG_OK)
				{
					TEvent Event;
								
					EventAssign(&Event,EV_MESSAGE, MSG_QUIT, NULL, KState, Mouse->State);
					HandleEvent(&Event);
	
				}
				return true;
			}
			break;
			case BTN_fdisk:
			{
				FileRun("xapps/rundos.app","/v/system/websurf/fdisk.bat");
				return true;
			}
			break;
			case BTN_fdos:
			{
				
				FileRun("xapps/rundos.app","/v/system/websurf/install.bat");
				return true;
			}
			break;
			case BTN_aura:
			{
				
				CollectingData();
				return true;
			}
			break;
			case BTN_update:
			{
				
				FileRun("xapps/rundos.app","/v/system/websurf/update.bat");
				return true;
			}
			break;

									
		}
	}

	return false;
}




l_int Main ( int argc, l_text *argv )
{

	//if ( Me.Ressource ) {
	//	Bmp1 = ListKey(Me.Ressource,"SETUP");
	//}
	if ( ListKey(Me.Ressource,"1" )) {
		Bmp1 = ListKey(Me.Ressource,"SETUP");
		MessageBox(&Me, "YAYY", "Verified and initiated", MBB_OK);
	}
	else {
	Bmp1 = ListKey(Me.Ressource,"SETUP");
	MessageBox(&Me, "BOO", "This didnt work", MBB_OK);
	}	

        l_ulong BHeight = 20;
        l_ulong BWidth = 50;
        l_ulong yspace = 35;

	RectAssign(&r,0, 0, 400, 255);
	w = CreateWindow( &Me, r, "Aura Setup", WF_FRAME|WF_CAPTION|WF_MINIMIZE|WF_CENTERED );
	InsertWidget(DeskTop, WIDGET(w));
	WIDGET(w)->AppEvHdl = &AppEventHandler;


	WidgetSize(&r,0, 0, 400, 41);
	c = CreateCanvas(&Me, r);
	WIDGET(c)->Draw = &Draw1;
	WIDGET(c)->Flags |= WFForceBuffer;
	InsertWidget(WIDGET(w), WIDGET(c));



	RectAssign(&r,5, 55, 5 + BWidth, 55 + BHeight);
	b1 = CreateButton(&Me,r,"fdisk",BTN_fdisk);
	InsertWidget(WIDGET(w), WIDGET(b1));

	RectAssign(&r,10 + BWidth, 55, 400, 55 + BHeight);
	j11 = CreateLabel(&Me, r, "Prepare your partition for installation with fdisk");
	WIDGET(j11)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(j11));




	RectAssign(&r,5, 55 + yspace, 5 + BWidth, (55 + BHeight) + yspace);
	b2 = CreateButton(&Me,r,"freeDOS",BTN_fdos);
	InsertWidget(WIDGET(w), WIDGET(b2));

	RectAssign(&r,10 + BWidth, 55 + yspace, 400, (55 + BHeight) + yspace);
	j2 = CreateLabel(&Me, r, "Install freedos 1.1 with doscore networking and filesharing settings after optional format");
	WIDGET(j2)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(j2));



	RectAssign(&r,5, 55 + (2 * yspace), 5 + BWidth, (55 + BHeight) + (2 * yspace));
	b3 = CreateButton(&Me,r,"Aura m3",BTN_aura);
	InsertWidget(WIDGET(w), WIDGET(b3));

	RectAssign(&r,10 + BWidth, 55 + (2 * yspace), 400, (55 + BHeight) + (2 * yspace));
	j3 = CreateLabel(&Me, r, "Install the Aura m3 System");
	WIDGET(j3)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(j3));


	RectAssign(&r,5, 55 + (3 * yspace), 5 + BWidth, (55 + BHeight) + (3 * yspace));
	b4 = CreateButton(&Me,r,"Update",BTN_update);
	InsertWidget(WIDGET(w), WIDGET(b4));

	RectAssign(&r,10 + BWidth, 55 + (3 * yspace), 400, (55 + BHeight) + (3 * yspace));
	j4 = CreateLabel(&Me, r, "Check for and install updates");
	WIDGET(j4)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(j4));


	RectAssign(&r,350, 230, 400, 250);
	b = CreateButton(&Me,r,"All Done",BTN_Ok);
	InsertWidget(WIDGET(w), WIDGET(b));
	
	WidgetDrawAll(WIDGET(w));

	return true;
}

void Close (void)
{

}
