////////////////////////////////////////////////////////////////////////////////
//
//	O3Snake
//		Snake game for codename Phoenix
//
//	Copyright (c) 2003,2004 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "button.h"
#include "window.h"
#include "timer.h"
#include "menu.h"
////////////////////////////////////////////////////////////////////////////////
l_uid 	nUID 				= "pm:o3snake";
l_ulong AppVersion	= ULONG_ID(0,0,1,0);
char AppName[]		= "O3Snake";
l_uid NeededLibs[] = { "widget","window","menu","" };
////////////////////////////////////////////////////////////////////////////////
PWidget Snk;

TPoint Snake[50] = {{2,1},{1,1}};
l_ulong Size = 2;
l_ulong Counter = 1;
l_ulong NGift = 0;
l_ulong NSGift = 0;

l_ulong Way = 1;

l_uchar c = 0;
TPoint Truc = { 5, 5 };
TPoint Gift = { 4, 4 };

l_bool Flags = 0;

#define F_MOVED 0x01
#define F_GAMEOVER 0x02
#define F_MORE 0x04
#define F_DONE 0x08
#define F_GIFT 0x10
#define F_SGIFT 0x20
#define F_NOGIFT 0x40

PTimer GameTimerId;


PWindow w	= 0;

#define GWIDTH 20
#define GHEIGHT 20

#define BWIDTH 10
#define BHEIGHT 10

l_ulong Score = 0;
l_ulong Speed = 100;

#define MSG_NEWGAME 0xFF010001

#define MSG_SPEED1 0xFF010002
#define MSG_SPEED2 0xFF010003
#define MSG_SPEED3 0xFF010004
#define MSG_SPEED4 0xFF010005
#define MSG_SPEED5 0xFF010006
#define MSG_SPEED6 0xFF010007
#define MSG_SPEED7 0xFF010008


l_text Message = "O3 Snake 0.1";
l_ulong CMsg = 150;

////////////////////////////////////////////////////////////////////////////////
void  GameDraw(PWidget o, p_bitmap buffer, PRect w)
{
	l_int n = 0,x,y;

	l_int Body = makecol(0,0,0);
	l_int Trc = makecol(255,0,0);
	l_int Text = makecol(0,0,0);
	l_int Gft = makecol(c,255,c);
	l_int SGft = makecol(c,c,255);
	rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, makecol(255,255,255));


	if ( !(o->Flags & WFFocused) ) {

	 Body = makecol(196,196,196);
	 Trc = makecol(255,196,196);
	 Text = makecol(196,196,196);
	 Gft = makecol(196,255,196);
	 SGft = makecol(196,196,255);

	}

	if ( NGift == 2 && c%10 ) Gft = makecol(255,255,255);
	if ( NSGift == 2 && c%20 ) SGft = makecol(255,255,255);

	if ( CMsg &&Message) {
		l_int col = Text;
		if ( CMsg < 0xFF && o->Flags & WFFocused )
			col = makecol(0xFF-CMsg,0xFF-CMsg,0xFF-CMsg);
		textout_ex (buffer, default_font, Message, o->Absolute.a.x, o->Absolute.a.y+text_height(default_font)+2, col, -1);
	}


	textprintf_ex (buffer,default_font,o->Absolute.a.x,o->Absolute.a.y,Text, -1, "%d Points",Score);

	if ( Flags & F_GAMEOVER ) {

		textprintf_ex (buffer, default_font, (o->Absolute.a.x+o->Absolute.b.x)/2, (o->Absolute.b.y+o->Absolute.a.y-text_height(default_font))/2, makecol(c,c,c), -1, "GAME OVER");
		textout_ex (buffer, default_font, "(i) To restart the game press [Enter]", o->Absolute.a.x, o->Absolute.b.y-text_height(default_font)-1, Text, -1);
	} else {



		while ( n < Size ) {

			x = o->Absolute.a.x + ( Snake[n].x * BWIDTH );
			y = o->Absolute.a.y + ( Snake[n].y * BHEIGHT );
			rectfill(buffer, x, y, x+BWIDTH-1, y+BHEIGHT-1, Body);
			n++;
		}

		x = o->Absolute.a.x + ( Truc.x * BWIDTH );
		y = o->Absolute.a.y + ( Truc.y * BHEIGHT );
		rectfill(buffer, x, y, x+BWIDTH-1, y+BHEIGHT-1, Trc);

		if ( Flags & F_GIFT ) {
			x = o->Absolute.a.x + ( Gift.x * BWIDTH );
			y = o->Absolute.a.y + ( Gift.y * BHEIGHT );
			rectfill(buffer, x, y, x+BWIDTH-1, y+BHEIGHT-1, Gft);
		} else if ( Flags & F_SGIFT ) {
			x = o->Absolute.a.x + ( Gift.x * BWIDTH );
			y = o->Absolute.a.y + ( Gift.y * BHEIGHT );
			rectfill(buffer, x, y, x+BWIDTH-1, y+BHEIGHT-1, SGft);
		}


		if ( (o->Flags & WFFocused) && !(Flags & F_MOVED) ) {

			if ( Flags & F_DONE ) {
				rectfill(buffer,o->Absolute.a.x+2,o->Absolute.b.y-text_height(default_font)-10,o->Absolute.b.x-2,o->Absolute.b.y-2,makecol(255,255,128));
				textout_ex (buffer, default_font, "(i) Use the arrow keys to control the snake", o->Absolute.a.x+5, o->Absolute.b.y-(1*(text_height(default_font)+2))-5, Text, -1);

			} else {
				rectfill(buffer,o->Absolute.a.x+2,o->Absolute.b.y-(6*(text_height(default_font)+2))-10,o->Absolute.b.x-2,o->Absolute.b.y-2,makecol(255,255,128));
				textout_ex (buffer, default_font, "[i] Welcome to O3 Snake!", o->Absolute.a.x+5, o->Absolute.b.y-(6*(text_height(default_font)+2))-5, makecol(255,0,0), -1);
				textout_ex (buffer, default_font, "+ The goal is to collect the color", o->Absolute.a.x+5, o->Absolute.b.y-(5*(text_height(default_font)+2))-5, Text, -1);
				textout_ex (buffer, default_font, " squares with the snake.", o->Absolute.a.x+5, o->Absolute.b.y-(4*(text_height(default_font)+2))-5, Text, -1);
				textout_ex (buffer, default_font, " (Red: 5pts Green: 20pts Blue: 50pts )", o->Absolute.a.x+5, o->Absolute.b.y-(3*(text_height(default_font)+2))-5, Text, -1);
				textout_ex (buffer, default_font, "+ Control the snake using  the arrow keys", o->Absolute.a.x+5, o->Absolute.b.y-(2*(text_height(default_font)+2))-5, Text, -1);
				textout_ex (buffer, default_font, "(To hide this help, start playing)", o->Absolute.a.x+5, o->Absolute.b.y-(1*(text_height(default_font)+2))-5, Text, -1);
			}
		}

	}

	if ( !(o->Flags & WFFocused) )
	{
		//textout_ex (buffer, default_font, "(i) Click here to play", o->Absolute.a.x, o->Absolute.b.y-(2*text_height(default_font))-5, makecol(0,0,0), -1);
	}


}

////////////////////////////////////////////////////////////////////////////////
GameEH(PWidget o, PEvent Event)
{
	if (Event->Type == EV_KEYBOARD)
	{
		if (SCANCODE(Event, KEY_RIGHT))
		{
			if ( Way != 2 ) Way = 1; Flags |= F_MOVED|F_DONE;

			return true;
		}

		if (SCANCODE(Event, KEY_LEFT))
		{
			if ( Way != 1 ) Way = 2; Flags |= F_MOVED|F_DONE;
			return true;
		}

		if (SCANCODE(Event, KEY_DOWN))
		{
			if ( Way != 4 ) Way = 3; Flags |= F_MOVED|F_DONE;
			return true;
		}

		if (SCANCODE(Event, KEY_UP))
		{
			if ( Way != 3 ) Way = 4; Flags |= F_MOVED|F_DONE;
			return true;
		}
		if (SCANCODE(Event, KEY_I))
		{
			Flags &= ~(F_MOVED|F_DONE);
			return true;
		}
		if (SCANCODE(Event, KEY_ENTER))
		{
			TimerSetPeriod(GameTimerId,Speed);
			Size = 2;
			Way = 1;
			Flags = F_DONE;
			Truc.x = Truc.y =  5;
			Snake[0].x = 2;
			Snake[0].y = 1;
			Snake[0].x = 1;
			Snake[0].y = 1;
				Score = 0; NGift = 0; Counter = 1;NSGift = 0;
			return true;
		}
	}

	if (Event->Type == EV_MESSAGE) {

		if ( Event->Message == WEvNotifyLostFocus ) {
			Flags &= ~F_MOVED;
			WidgetDraw(o, NULL);
			return true;
		}
		if ( Event->Message == WEvNotifyFocused ) {
			Flags &= ~F_MOVED;
			WidgetDraw(o, NULL);
			return true;
		}
		if ( Event->Message == WEvNotifySameFocus ) {
			WidgetDraw(o, NULL);
			return true;
		}

	}
	return false;
}

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
			case MSG_SPEED1:
			{
				Speed = 250;
				if ( !(Flags & F_GAMEOVER) ) TimerSetPeriod(GameTimerId,Speed);
				return true;
			}
			break;
			case MSG_SPEED2:
			{
				Speed = 170;
				if ( !(Flags & F_GAMEOVER) ) TimerSetPeriod(GameTimerId,Speed);
				return true;
			}
			break;
			case MSG_SPEED3:
			{
				Speed = 130;
				if ( !(Flags & F_GAMEOVER) ) TimerSetPeriod(GameTimerId,Speed);
				return true;
			}
			break;
			case MSG_SPEED4:
			{
				Speed = 100;
				if ( !(Flags & F_GAMEOVER) ) TimerSetPeriod(GameTimerId,Speed);
				return true;
			}
			break;
			case MSG_SPEED5:
			{
				Speed = 80;
				if ( !(Flags & F_GAMEOVER) ) TimerSetPeriod(GameTimerId,Speed);
				return true;
			}
			break;
			case MSG_SPEED6:
			{
				Speed = 60;
				if ( !(Flags & F_GAMEOVER) ) TimerSetPeriod(GameTimerId,Speed);
				return true;
			}
			break;
			case MSG_SPEED7:
			{
				Speed = 40;
				if ( !(Flags & F_GAMEOVER) ) TimerSetPeriod(GameTimerId,Speed);
				return true;
			}
			break;
			case WM_ABOUT:
			{
				MessageBox(&Me, "About O3 Snake", "O3 Snake 0.1\nA small game for oZone\n\nCopyright (c) 2003,2004 Point Mad. All rights reserved.", MBB_OK);
				return true;
			}
			case MSG_NEWGAME:
			{
				TimerSetPeriod(GameTimerId,Speed);
				Size = 2;
				Way = 1;
				Flags = F_DONE;
				Truc.x = Truc.y =  5;
				Snake[0].x = 2;
				Snake[0].y = 1;
				Snake[0].x = 1;
				Snake[0].y = 1;
				Score = 0; NGift = 0; Counter = 1; NSGift = 0;
				WidgetDraw(Snk,NULL);

				return true;
			}
			break;
		}
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////
void NewRamdonPoint ( TPoint *p ) {
	l_long n = 0;

	p->x = rand()%GWIDTH;
	p->y = rand()%GHEIGHT;

	if ( p == &Truc ) if ( (p->x == Gift.x) && (p->y == Gift.y) ) { NewRamdonPoint(p); return; }
	if ( p == &Gift ) if ( (p->x == Truc.x) && (p->y == Truc.y) ) { NewRamdonPoint(p); return; }

	while ( n < Size ) {
		if ( (p->x == Snake[n].x) && (p->y == Snake[n].y) ) {
			NewRamdonPoint(p);
			return;
		}
		n++;
	}


}
////////////////////////////////////////////////////////////////////////////////
void GameTimer ( void *Arg ) {

	l_int n = Size;

	if ( CMsg ) CMsg -= 10;


	if ( Flags & F_GAMEOVER ) {
		if ( Flags & F_MORE ) c+=10;
		else c-=10;
		if ( !c ) Flags |= F_MORE;
		if ( c == 250 ) Flags &= ~F_MORE;
		TimerSetPeriod(GameTimerId,100);
		WidgetDraw(Snk,NULL);
		return;
	}

	if ( WIDGET(Snk)->Flags & WFFocused ) {

		if ( Flags & F_GIFT ) {

			c += 5;

			if ( c == 250 ) {
				Flags &= ~F_GIFT;
				Message="Too late...";
				CMsg = 250;
			}

		} else if ( Flags & F_SGIFT ) {

			c += 10;

			if ( c == 250 ) {
				Flags &= ~F_SGIFT;
				Message="Too late...";
				CMsg = 250;

			}

		} else if ( !(Flags & F_NOGIFT)) {


			if ( !(Counter%25) ) {
				/*Gift.x = random()%GWIDTH;
				Gift.y = random()%GHEIGHT;*/
				NewRamdonPoint(&Gift);
				c = 0;
				Flags |= F_SGIFT|F_NOGIFT;
				if ( NSGift == 2 ) {
					Message="You MUST get this bonus !";
					CMsg = 250;
				}
			} else if ( !(Counter%5)  ) {
				/*Gift.x = random()%GWIDTH;
				Gift.y = random()%GHEIGHT;*/
				NewRamdonPoint(&Gift);
				c = 0;
				Flags |= F_GIFT|F_NOGIFT;
				if ( NGift == 2 ) {
					Message="You should get this bonus !";
					CMsg = 500;
				}
			}
		}




	while ( n ) {
		Snake[n] = Snake[n-1];
		n--;
	}

	if ( (Snake[0].x == Truc.x) && (Snake[0].y == Truc.y) ) {
		Size++;
		Counter++;
		if ( Size == 49 ) Flags |= F_GAMEOVER;
		Flags &=~F_NOGIFT;
		/*Truc.x = random()%GWIDTH;
		Truc.y = random()%GHEIGHT;*/
		NewRamdonPoint(&Truc);
		Score += 5;
	}

	if ( Flags & F_GIFT ) {
		if ( (Snake[0].x == Gift.x) && (Snake[0].y == Gift.y) ) {
			Flags &= ~F_GIFT;
			Score += 20;
			NGift++;
			if ( NGift == 3 ) {
				NGift =	0;
				Size = max(Size-3,3);
				Message="Bonus 20pts and 3 weight less";

			} else
				Message="Bonus 20pts";
			CMsg = 250;

		}
	} else if ( Flags & F_SGIFT ) {
		if ( (Snake[0].x == Gift.x) && (Snake[0].y == Gift.y) ) {
			Flags &= ~F_SGIFT;
			NGift++;
			NSGift++;
			if ( NSGift == 3 ) {
				Score += 100;
				Size = max(Size-20,3);
				NSGift = 0;
				Message="ULTRA BONUS 100pts and 20 weight less";
			} else {
				Score += 50;
				Size = max(Size-5,3);
				Message="Bonus 50pts and 5 weight less";
			}
			CMsg = 250;
		}
	}


	if ( Way == 1 ) {

		Snake[0].x++;
		if ( Snake[0].x >= GWIDTH ) Snake[0].x = 0;



	} else if ( Way == 2 ) {

		Snake[0].x--;
		if ( Snake[0].x < 0 ) Snake[0].x = GWIDTH-1;


	} else if ( Way == 3 ) {

		Snake[0].y++;
		if ( Snake[0].y >= GHEIGHT ) Snake[0].y = 0;

	} else if ( Way == 4 ) {

		Snake[0].y--;
		if ( Snake[0].y < 0 ) Snake[0].y = GHEIGHT-1;

	}

	n = 1;
	while ( n < Size ) {
		if ( (Snake[0].y == Snake[n].y) && (Snake[0].x == Snake[n].x) ) {
			Flags |= F_GAMEOVER;
			Message="COLLISION ON BODY !!!";
			CMsg = 500;

			//MessageBox(&Me, "O3 Snake", "Collision on body !", MBB_OK|MBI_WARNING);
			return;
		}
		n++;
	}

	WidgetDraw(Snk,NULL);


	}


}
////////////////////////////////////////////////////////////////////////////////
l_int Main ( int argc, l_text *argv )
{
	TRect r;

	Speed = KeyGetInt("/USER/GAMES/O3SNAKE/SPEED",100);
	Flags = KeyGetInt("/USER/GAMES/O3SNAKE/FLAGS",0)&(F_GAMEOVER|F_DONE);
	srand(time(NULL));

	RectAssign(&r,0, 0, GWIDTH*BWIDTH, GHEIGHT*BHEIGHT+25);

	w = CreateWindow(&Me, r, "O3 Snake", WF_CAPTION|WF_FRAME|WF_CENTERED|WF_MINIMIZE);
	InsertWidget(WIDGET(DeskTop), WIDGET(w));
	WidgetPreCenter(WIDGET(w));


	PMenu Menu = NewMenu(
     NewMenuItem( "File", NULL, 0, 0,
     		 NewMenu (
     		 		NewMenuItem( "New game", NULL, MSG_NEWGAME, 0, NULL,
     		 		NewMenuItemSeparator(
     		 		NewMenuItem( "Exit", NULL, WM_CLOSE, 0, NULL,
     		 		NULL)))
     		 ),
     NewMenuItem( "Speed", NULL, 0, 0,
				NewMenu (
     		 		NewMenuItem( "Slowest", NULL, MSG_SPEED1, 0, NULL,
     		 		NewMenuItem( "Very slow", NULL, MSG_SPEED2, 0, NULL,
     		 		NewMenuItem( "Slow", NULL, MSG_SPEED3, 0, NULL,
     		 		NewMenuItem( "Normal", NULL, MSG_SPEED4, 0, NULL,
     		 		NewMenuItem( "Fast", NULL, MSG_SPEED5, 0, NULL,
     		 		NewMenuItem( "Very fast", NULL, MSG_SPEED6, 0, NULL,
     		 		NewMenuItem( "Fastest", NULL, MSG_SPEED7, 0, NULL,
     		 		NULL)))))))
     		 ),
     NewMenuItem( "Help", NULL, 0, 0,
     	NewMenu(
     		NewMenuItem( "About", NULL, WM_ABOUT, 0, NULL, NULL)),
     NULL)))
	);

	RectAssign(&r,0,0,GWIDTH*BWIDTH,20);

	PMenuView o = NewMenuView(&Me,r,Menu,MenuViewStyleHorizontal,0);
	InsertWidget(WIDGET(w), WIDGET(o));



	RectAssign(&r, 0, 25, GWIDTH*BWIDTH, GHEIGHT*BHEIGHT+25);
	Snk = CreateWidget(&Me, r);
	Snk->Draw = &GameDraw;
	Snk->EventHandler = &GameEH;
	Snk->Flags |= WFFocusable;
	Snk->Flags |= WFForceBuffer;
	InsertWidget(WIDGET(w), Snk);
	WidgetDrawAll(WIDGET(w));
	WidgetSetFocus(Snk);

	WIDGET(w)->AppEvHdl = &AppEventHandler;

	GameTimerId = NewTimer(&Me,Speed,&GameTimer,NULL);


	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{
	KillTimer(GameTimerId);
	WidgetDispose(WIDGET(w));
	KeySetInt("/USER/GAMES/O3SNAKE/SPEED",Speed);
}
////////////////////////////////////////////////////////////////////////////////
