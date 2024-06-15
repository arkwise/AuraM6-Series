/////////////////////////////////////////////////////////////////////////////////
//
//	Wallpaper Settings
//
//	Aura Milestone 3 
//
//	(c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "label.h"
#include "button.h"
#include "window.h"
#include "combobox.h"
#include "iodlg.h"
#include "tbox.h"
#include "colsel.h"
#include "prevscr.h"

void WallpaperReLoad ( void );


l_ulong AppVersion = ULONG_ID(0,0,1,0);
char    AppName[] = "Desktop wallpaper";
l_uid	nUID = "cp:setwall";
l_uid NeededLibs[] = { "widget","textbox","button","label","menu","window","combobox","iodlg","prevscr","colsel","" };

#define BTNWIDTH 80
#define BTNHEIGHT 22
#define BTNSPACE 5

PCombobox Al;

PPrevScr PS;
PTextbox Wll;

p_bitmap Scr;
p_bitmap Img = NULL;

l_int BGColor;

#define MSG_UPDATESCR 0x00FF0001
#define MSG_SELECTFILE 0x00FF0002
#define MSG_SELECTNONE 0x00FF0003
#define MSG_CHGBGCOLOR 0x00FF0004

PFileTypes Filter = 0;

p_bitmap LoadResizedImg ( l_text file ) {
	l_ulong w,h;
	p_bitmap I,O = NULL;
	I = LoadImage(file);
	if ( I ) {
		w = I->w*152/GSScreenWidth;
		h = I->h*112/GSScreenHeight;
		O = create_bitmap(w, h);
		stretch_blit(I, O, 0, 0, I->w, I->h, 0, 0, w, h);
		destroy_bitmap(I);
	}
	return O;
}

void RegeneratePreview ( void ) {



	rectfill(Scr,0,0,Scr->w,Scr->h,BGColor);
	if ( Img ) {
		l_ulong i = ComboboxItemIndex(Al,Al->Selected);
		if ( i == 1 ) {
			blit(Img, Scr, 0,0, (Scr->w-Img->w)/2, (Scr->h-Img->h)/2, Img->w, Img->h );
		} else if ( i == 3 ) {
			stretch_blit(Img, Scr, 0, 0, Img->w, Img->h, 0, 0, Scr->w, Scr->h);
		} else {
				l_int x;
				l_int y;
				for ( x = 0; x < Scr->w; x += Img->w)
					for ( y = 0; y < Scr->h; y += Img->h)
						blit(Img, Scr, 0,0, x, y, Img->w, Img->h  );
		}
	}
	WidgetDraw(WIDGET(PS),NULL);

}

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

			case MSG_UPDATESCR:
			{

				RegeneratePreview();
			}
			break;

			case MSG_SELECTFILE:
			{
				l_text file = IOBox("Select wallpaper image",IOBOX_OPEN,NULL,Filter,true);
				if ( file ) {
					TextBoxSetTextEx(Wll,file);
					free(file);

					if ( Img ) destroy_bitmap(Img);
					Img = LoadResizedImg(Wll->Text);
					RegeneratePreview();
				}
			}
			break;
			
			case MSG_CHGBGCOLOR:
			{
			
				DialogColor(NULL,&BGColor);
				RegeneratePreview();
				
			}
			break;

			case MSG_SELECTNONE:
			{
				TextBoxSetTextEx(Wll,"");
				if ( Img ) destroy_bitmap(Img);
				Img = NULL;
				RegeneratePreview();
			}
			break;

			case MSG_APPLY:
			case MSG_OK:
			{
				l_text ColCode = TextArgs("%02x%02x%02x",getr(BGColor),getg(BGColor),getb(BGColor));
				
				KeySetText("/USER/DESKTOP/color", ColCode);
				free(ColCode);
				KeySetText("/USER/DESKTOP/wallpaper", Wll->Text);
				KeySetInt("/USER/DESKTOP/alignment", ComboboxItemIndex(Al,Al->Selected)-1);

				WallpaperReLoad();
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

	TRect r;

	BGColor = DeskTop->BackgroundColor;
	
	
	Filter = 	NewFileTypes("Supported images files (*.png;*.bmp)","png,bmp",
						NewFileTypes("All files",NULL,
						NULL ));


	Scr = create_bitmap(152, 112);
	rectfill(Scr,0,0,Scr->w,Scr->h,makecol(0,0,0));
	textout_centre_ex(Scr,default_font,"Loading...",Scr->w/2,Scr->h/2,makecol(255,255,255), -1);


	RectAssign(&r,0, 0, 380, 290);

	w = CreateWindow(&Me, r, "Wallpaper settings", WF_CAPTION|WF_FRAME|WF_CENTERED|WF_MINIMIZE);
	InsertWidget(WIDGET(DeskTop), WIDGET(w));


	WidgetSize(&r,190-(182/2), 5, 182, 163);
	PS = CreatePrevScr(&Me,r,Scr);
	InsertWidget(WIDGET(w), WIDGET(PS));

	WidgetSize(&r, 15, 180, 135, 20);
	l = CreateLabel(&Me,r,"Wallpaper");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));

	//WidgetSize(&r, 25, 200, 135, 20);

	WidgetSize(&r, 15, 200, 170, 20);
	Wll = CreateTextbox(&Me,r,0);
	InsertWidget(WIDGET(w), WIDGET(Wll));

	TextBoxSetTextEx(Wll,KeyGetText("/USER/DESKTOP/wallpaper", ""));

	WidgetSize(&r, 190, 200, 20, 20);
	b = CreateButton(&Me, r, "...", MSG_SELECTFILE );
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r, 215, 200, 40, 20);
	b = CreateButton(&Me, r, "none", MSG_SELECTNONE );
	InsertWidget(WIDGET(w), WIDGET(b));


	WidgetSize(&r, 265, 180, 100, 20);
	l = CreateLabel(&Me,r,"Alignement");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));


	WidgetSize(&r, 265, 200, 100, 20);
	Al = NewComboBox(&Me,r,MSG_UPDATESCR);
	ComboboxAddItem(Al,"Center",NULL);
	ComboboxAddItem(Al,"Pattern",NULL);
	ComboboxAddItem(Al,"Stretch",NULL);
	InsertWidget(WIDGET(w), WIDGET(Al));

	
	
	WidgetSize(&r, 15, 225, 170, 20);
	b = CreateButton(&Me, r, "Change background color", MSG_CHGBGCOLOR );
	InsertWidget(WIDGET(w), WIDGET(b));
	
	ComboboxSelectIndex(Al,1+KeyGetInt("/USER/DESKTOP/alignment", 0));


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

	if ( TextLen(Wll->Text) > 0 ) Img = LoadResizedImg(Wll->Text);
	RegeneratePreview();

	return true;
}

void Close (void)
{
	if ( Img ) destroy_bitmap(Img);
	if ( Scr ) destroy_bitmap(Scr);
}
