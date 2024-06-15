#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "label.h"
#include "button.h"
#include "canvas.h"

l_ulong AppVersion	= ULONG_ID(0,0,0,1);
char    AppName[]	= "About/Welcome";
l_uid	nUID		= "shw:abtwel";
l_uid NeededLibs[]	= { "widget", "button","label","window","" };

#define FWIDTH  420
#define FHEIGHT 220

PTask	MainTask = NULL;
PCanvas	c 	= 0;
PLabel l = 0;
PTimer t = 0;
l_ulong nb = 0, show = 2;
l_ubig ltime = 0;
PWindow w = 0;

p_bitmap Bmp1;
p_bitmap Bmp2;
p_bitmap Bmp3;
p_bitmap Bmp4;

void TimerPoll(void* A)
{
	nb++;
	if ( show == 1 && nb == 0x1300 ) nb = 0;
	if ( show == 2 && nb > 0xD00 ) {
		/*WIDGET(w)->Alpha = (nb & 0xFF);
		if ( WIDGET(w)->Alpha == 0xFF ) {*/
			CloseWindow(w);
		/*} else
			WidgetDrawAll(WIDGET(w));*/
		return;
	}
	WidgetDraw(WIDGET(c), NULL);
}

void DrawCenteredText (PWidget o, p_bitmap buffer, l_text text ) {

		textout(buffer, default_font, text,
		(o->Absolute.a.x+o->Absolute.b.x-text_length(default_font,text))/2,
		(o->Absolute.a.y+o->Absolute.b.y-text_height(default_font))/2, makecol(0,0,0));

}
void DrawCenteredTextIn (PWidget o, p_bitmap buffer, l_text text ) {

	l_ulong n = (0xFF-(nb & 0xFF))*(((o->Absolute.b.x-o->Absolute.a.x)/2)+text_length(default_font,text))/0xFF;

		textout(buffer, default_font, text,
		(o->Absolute.a.x+o->Absolute.b.x-text_length(default_font,text))/2 - n,
		(o->Absolute.a.y+o->Absolute.b.y-text_height(default_font))/2, makecol(128,128,128));
		textout(buffer, default_font, text,
		(o->Absolute.a.x+o->Absolute.b.x-text_length(default_font,text))/2 + n,
		(o->Absolute.a.y+o->Absolute.b.y-text_height(default_font))/2, makecol(128,128,128));
}

void DrawCenteredTextOut (PWidget o, p_bitmap buffer, l_text text ) {

	l_ulong n = (nb & 0xFF)*(((o->Absolute.b.x-o->Absolute.a.x)/2)+text_length(default_font,text))/0xFF;

		textout(buffer, default_font, text,
		(o->Absolute.a.x+o->Absolute.b.x-text_length(default_font,text))/2 - n,
		(o->Absolute.a.y+o->Absolute.b.y-text_height(default_font))/2, makecol(128,128,128));
		textout(buffer, default_font, text,
		(o->Absolute.a.x+o->Absolute.b.x-text_length(default_font,text))/2 + n,
		(o->Absolute.a.y+o->Absolute.b.y-text_height(default_font))/2, makecol(128,128,128));
}

void DrawSubTitleText (PWidget o, p_bitmap buffer, l_text text, p_bitmap bmp ) {
		textout(buffer, default_font, text,
		(o->Absolute.a.x+o->Absolute.b.x-text_length(default_font,text))/2,
		(o->Absolute.a.y+o->Absolute.b.y+bmp->h)/2, makecol(0,0,0));
}

void FlameDraw (PWidget o, p_bitmap buffer, PRect w)
{
	rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,makecol(255,255,255));

	if ( show == 1 ) {


		if ( ( nb >> 8 ) == 0 ) {

			set_trans_blender(255, 255, 255, nb & 0xFF);
		  draw_trans_sprite(buffer,Bmp1, (o->Absolute.a.x+o->Absolute.b.x-Bmp1->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp1->h)/2);


		} else if ( ( nb >> 8 ) == 1 ) {
		  draw_sprite(buffer,Bmp1, (o->Absolute.a.x+o->Absolute.b.x-Bmp1->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp1->h)/2);
			set_trans_blender(255, 255, 255, nb & 0xFF);
		  draw_trans_sprite(buffer,Bmp2, (o->Absolute.a.x+o->Absolute.b.x-Bmp2->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp2->h)/2);
		} else if ( ( nb >> 8 ) == 2 ) {


			draw_sprite(buffer,Bmp2, (o->Absolute.a.x+o->Absolute.b.x-Bmp2->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp2->h)/2);

			drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
			set_trans_blender(255, 255, 255, nb & 0xFF);
			rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,makecol(255,255,255));
			solid_mode();
		} else if ( ( nb >> 8 ) == 3 ) {
			DrawCenteredText(o,buffer,"and");
		} else if ( ( nb >> 8 ) == 4 ) {
			DrawCenteredTextIn(o,buffer,"Lukas Lipka");
		} else if ( ( nb >> 8 ) == 5 ) {
			DrawCenteredText(o,buffer,"Lukas Lipka");
		} else if ( ( nb >> 8 )  == 6 ) {
			DrawCenteredTextOut(o,buffer,"Lukas Lipka");
		} else if ( ( nb >> 8 ) == 7 ) {
			DrawCenteredText(o,buffer,"present");
		} else if ( ( nb >> 8 ) == 8 ) {
			set_trans_blender(255, 255, 255, nb & 0xFF);
		  draw_trans_sprite(buffer,Bmp3, (o->Absolute.a.x+o->Absolute.b.x-Bmp3->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp3->h)/2);
		} else if ( ( nb >> 8 ) == 9 ) {
			set_trans_blender(255, 255, 255, -((nb & 0xFF)*0xDD/0xFF)+0xFF );
		  draw_trans_sprite(buffer,Bmp3, (o->Absolute.a.x+o->Absolute.b.x-Bmp3->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp3->h)/2);
		} else if ( ( nb >> 8 ) < 12 ) {
			set_trans_blender(255, 255, 255, 0x22);
		  draw_trans_sprite(buffer,Bmp3, (o->Absolute.a.x+o->Absolute.b.x-Bmp3->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp3->h)/2);

			DrawCenteredText(o,buffer,"a multiplatform 32 bit GUI");
		} else if ( ( nb >> 8 ) < 14 ) {
			set_trans_blender(255, 255, 255, 0x22);
		  draw_trans_sprite(buffer,Bmp3, (o->Absolute.a.x+o->Absolute.b.x-Bmp3->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp3->h)/2);

			DrawCenteredText(o,buffer,"Programmed by Julien Etelain and Lukas Lipka");
		} else if ( ( nb >> 8 ) < 16 ) {
			set_trans_blender(255, 255, 255, 0x22);
		  draw_trans_sprite(buffer,Bmp3, (o->Absolute.a.x+o->Absolute.b.x-Bmp3->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp3->h)/2);
			DrawCenteredText(o,buffer,"Graphic themes by Lukas \"RIPPER\" Lipka");
		} else if ( ( nb >> 8 ) < 18 ) {
			set_trans_blender(255, 255, 255, 0x22);
		  draw_trans_sprite(buffer,Bmp3, (o->Absolute.a.x+o->Absolute.b.x-Bmp3->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp3->h)/2);

			DrawCenteredText(o,buffer,"'O3' logo by Cedric Dagherir");
		} else if ( ( nb >> 8 ) < 20 ) {
			set_trans_blender(255, 255, 255, 0x22);
		  draw_trans_sprite(buffer,Bmp3, (o->Absolute.a.x+o->Absolute.b.x-Bmp3->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp3->h)/2);

			DrawCenteredText(o,buffer,"http://www.ozonegui.net");

		}
	} else {

		if ( ( nb >> 8 ) == 0 ) {

			set_trans_blender(255, 255, 255, nb & 0xFF);
		  draw_trans_sprite(buffer,Bmp1, (o->Absolute.a.x+o->Absolute.b.x-Bmp1->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp1->h)/2);


		} else if ( ( nb >> 8 ) == 1 ) {
		  draw_sprite(buffer,Bmp1, (o->Absolute.a.x+o->Absolute.b.x-Bmp1->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp1->h)/2);
			set_trans_blender(255, 255, 255, nb & 0xFF);
		  draw_trans_sprite(buffer,Bmp2, (o->Absolute.a.x+o->Absolute.b.x-Bmp2->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp2->h)/2);
		} else if ( ( nb >> 8 ) == 2 ) {


			draw_sprite(buffer,Bmp2, (o->Absolute.a.x+o->Absolute.b.x-Bmp2->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp2->h)/2);

			drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
			set_trans_blender(255, 255, 255, nb & 0xFF);
			rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,makecol(255,255,255));
			solid_mode();
		} else if ( ( nb >> 8 ) == 3 ) {
			DrawCenteredText(o,buffer,"and");
		} else if ( ( nb >> 8 ) == 4 ) {
			DrawCenteredTextIn(o,buffer,"Lukas Lipka");
		} else if ( ( nb >> 8 ) == 5 ) {
			DrawCenteredText(o,buffer,"Lukas Lipka");
		} else if ( ( nb >> 8 )  == 6 ) {
			DrawCenteredTextOut(o,buffer,"Lukas Lipka");
		} else if ( ( nb >> 8 ) == 7 ) {
			DrawCenteredText(o,buffer,"present");
		} else if ( ( nb >> 8 ) == 8 ) {
			set_trans_blender(255, 255, 255, nb & 0xFF);
		  draw_trans_sprite(buffer,Bmp3, (o->Absolute.a.x+o->Absolute.b.x-Bmp3->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp3->h)/2);

		} else if ( ( nb >> 8 ) == 9 ) {
			draw_sprite(buffer,Bmp3, (o->Absolute.a.x+o->Absolute.b.x-Bmp3->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp3->h)/2);
		  DrawSubTitleText(o,buffer,"oZone GUI (codename Phoenix)",Bmp3);

		} else if ( ( nb >> 8 ) == 10 ) {
			set_trans_blender(255, 255, 255, 0xFF-(nb & 0xFF));
		  draw_trans_sprite(buffer,Bmp3, (o->Absolute.a.x+o->Absolute.b.x-Bmp3->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp3->h)/2);
			set_trans_blender(255, 255, 255, nb & 0xFF);
		  draw_trans_sprite(buffer,Bmp4, (o->Absolute.a.x+o->Absolute.b.x-Bmp4->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp4->h)/2);


		} else /*if ( ( nb >> 8 ) == 11 )*/ {
						draw_sprite(buffer,Bmp4, (o->Absolute.a.x+o->Absolute.b.x-Bmp4->w)/2, (o->Absolute.a.y+o->Absolute.b.y-Bmp4->h)/2);

			DrawCenteredText(o,buffer,"Click on 'O3' button to start");
		}



	}

}

l_bool AppEventHandler ( PWidget o, PEvent Ev )
{
	if ( Ev->Type == EV_MESSAGE )
	{
		if ( Ev->Message == WM_CLOSE )
		{
			CloseApp(&Me);

			return true;
		}

	}

	return false;
}

l_int Main ( int argc, l_text *argv )
{
	TRect r;
	PButton b = NULL;

	if ( GSScreenDepth == 8 ) return false;
	
	if ( Me.Ressource ) {
		Bmp1 = ListKey(Me.Ressource,"1");
		Bmp2 = ListKey(Me.Ressource,"2");
		Bmp3 = ListKey(Me.Ressource,"3");
		Bmp4 = ListKey(Me.Ressource,"4");
	}

	if ( argc > 1 ) if ( !TextCompare(argv[1],"about") ) show = 1;

	RectAssign(&r,0, 0, FWIDTH, FHEIGHT);
	w = CreateWindow( &Me, r, show==1?"About":"Welcome", WF_FRAME|WF_CAPTION|WF_MINIMIZE|WF_CENTERED );
	InsertWidget(DeskTop, WIDGET(w));
	WIDGET(w)->AppEvHdl = &AppEventHandler;


	WidgetSize(&r,0, 0, FWIDTH, FHEIGHT);
	c = CreateCanvas(&Me, r);
	WIDGET(c)->Draw = &FlameDraw;
	WIDGET(c)->Flags |= WFForceBuffer;
	InsertWidget(WIDGET(w), WIDGET(c));

	WidgetDrawAll(WIDGET(w));


	t = NewTimer(&Me,10, &TimerPoll,NULL);
	return true;
}

void Close (void)
{
	KillTimer(t);
	WidgetDispose(WIDGET(w));
}
