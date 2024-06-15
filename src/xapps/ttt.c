/**
 * TODO:
 *		+ timer for score
 */
#include "kernel.h"
#include "canvas.h"
#include "button.h"
#include "window.h"
#include "timer.h"
#include "menu.h"
#include "label.h"

l_uid 	nUID 		= "o3ttt";
l_ulong AppVersion	= ULONG_ID(0,0,1,0);
char AppName[]		= "03 TicTacToe";
l_uid NeededLibs[]	= { "widget","window","menu","canvas","" };

#define SIZE 48

#define MSG_NEWGAME 0xdeadbeef
PWindow w = 0;
PCanvas Surface = 0;
l_uchar GameSource[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

l_uchar BestPlaces[] = {4, 0, 2, 6, 8, 1, 3, 5, 7};

l_bool End = 0;

void SurfaceDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	l_int x = 0, xp;
	l_int y = 0, yp, n;

	rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DFACE);
	
	xp =o->Absolute.a.x;
	for ( x = 0; x < 2; x++ ) {
		xp += SIZE;
		vline(buffer,xp,o->Absolute.a.y,o->Absolute.b.y,COL_3DDARK);
	}
	
	yp = o->Absolute.a.y;
	for ( y = 0; y < 2; y++ ) {
		yp += SIZE;
		hline(buffer,o->Absolute.a.x,yp,o->Absolute.b.x,COL_3DDARK);
	}

	xp =o->Absolute.a.x;
	for ( x = 0; x < 3; x++ ) {
		yp = o->Absolute.a.y;
		for ( y = 0; y < 3; y++ ) {
			if ( GameSource[x][y] == 1 ) {
				line(buffer,xp+2,yp+2,xp+SIZE-2,yp+SIZE-2,makecol(0,255,0));
				line(buffer,xp+2,yp+3,xp+SIZE-2,yp+SIZE-1,makecol(0,255,0));
				
				
				line(buffer,xp+SIZE-2,yp+2,xp+2,yp+SIZE-2,makecol(0,255,0));
				line(buffer,xp+SIZE-2,yp+3,xp+2,yp+SIZE-1,makecol(0,255,0));
				
			} else if ( GameSource[x][y] == 2 ) {
				circle(buffer,xp+(SIZE/2),yp+(SIZE/2),(SIZE/2)-2,makecol(255,0,0));
				circle(buffer,xp+(SIZE/2),yp+(SIZE/2),(SIZE/2)-3,makecol(255,0,0));
			}
			yp += SIZE;
		}
		xp += SIZE;
	}
}

int Winner ( void ) {
	
	int x, y;
	
	for ( x = 0; x < 3; x++ )
		if ( (GameSource[x][0] == GameSource[x][1]) && (GameSource[x][1] == GameSource[x][2]) ) return GameSource[x][0];
		
	for ( y = 0; y < 3; y++ )
		if ( (GameSource[0][y] == GameSource[1][y]) && (GameSource[1][y] == GameSource[2][y]) ) return GameSource[0][y];

	if ( (GameSource[0][0] == GameSource[1][1]) && ( GameSource[1][1] == GameSource[2][2]) ) return GameSource[1][1];
	if ( (GameSource[0][2] == GameSource[1][1]) && ( GameSource[1][1] == GameSource[2][0]) ) return GameSource[1][1];
	
	return 0;	
}


l_bool IACompleteLines ( l_int p ) {
	
	int x, y, x1, x2, y1, y2;
	
	for ( x = 0; x < 3; x++ ) {
		x1 = x ? 0 : 1;
		x2 = x < 2 ? 2 : 1;
		for ( y = 0; y < 3; y++ ) {
			y1 = y ? 0 : 1;
			y2 = y < 2 ? 2 : 1;
			if ( !GameSource[x][y] ) {
				
				if ( ( GameSource[x1][y] == GameSource[x2][y] && (GameSource[x1][y] == p) )
					|| ( GameSource[x][y1] == GameSource[x][y2] && (GameSource[x][y1] == p) ) ) {
					GameSource[x][y] = 2;
					return true;
				}
			}
		}
	}
	
	for ( x = 0; x < 3; x++ ) {
		x1 = x ? 0 : 1;
		x2 = x < 2 ? 2 : 1;
		if ( !GameSource[x][x] ) {
			if ( GameSource[x1][x1] == GameSource[x2][x2] && (GameSource[x1][x1] == p) ) {
				GameSource[x][x] = 2;
				return true;
			}
		}
	}
	
	for ( x = 0; x < 3; x++ ) {
		x1 = x ? 0 : 1;
		x2 = x < 2 ? 2 : 1;
		if ( !GameSource[x][2-x] ) {
			if ( GameSource[x1][2-x1] == GameSource[x2][2-x2] && (GameSource[x1][2-x1] == p) ) {
				GameSource[x][2-x] = 2;
				return true;
			}
		}
	}
	return false;
}

void ComputerTurn ( void ) {
	int i,x,y;
	if ( IACompleteLines(2) ) return; // First : Try to win
	if ( IACompleteLines(1) ) return; // Second : Prevent player to win
		
	for ( i = 0; i < 9; i++ ) {
		x = BestPlaces[i]%3;
		y = BestPlaces[i]/3;
		if ( !GameSource[x][y] ) {
			GameSource[x][y] = 2;
			return;
		}
	}
	
}

l_bool SurfaceEH ( PWidget o, PEvent Event )
{
	if ( Event->Type == EV_MOUSE)
	{
		if ( Event->Message == WEvMouseLDown && !End )
		{
			l_int x,y;
			x = (Mouse->State.p.x-o->Absolute.a.x)/SIZE;
			y = (Mouse->State.p.y-o->Absolute.a.y)/SIZE;
			
			if ( !GameSource[x][y] ) GameSource[x][y] = 1;
			
			if ( Winner() ) {
				End = true;
				WidgetDraw(o,NULL);
				MessageBox(&Me, "O3 TicTacToe", "Well done !", MBB_OK);
			} else {
				ComputerTurn();
				if ( Winner() ) {
					End = true;
					WidgetDraw(o,NULL);
					MessageBox(&Me, "O3 TicTacToe", "Game Over : You lose", MBB_OK);
				} else
					WidgetDraw(o,NULL);
			}
			
			return true;
			
		}

	}
	return false;
}


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
				MessageBox(&Me, "About O3 TicTacToe", "O3 TicTacToe 0.1\nYet another cloned game for oZone GUI\n\nCopyright (c) 2004 Point Mad, Lukas Lipka. All rights reserved.", MBB_OK);
				return true;
			}
			break;

			case MSG_NEWGAME:
			{
				memset(&GameSource,0,sizeof(GameSource));
				
				End = false;
				WidgetDraw(WIDGET(Surface), NULL);


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
	
	
	RectAssign(&r,0, 0, 3*SIZE, 3*SIZE+25);
	w = CreateWindow(&Me, r, "TicTacToe", WF_CAPTION|WF_FRAME|WF_CENTERED|WF_MINIMIZE);
	InsertWidget(WIDGET(DeskTop), WIDGET(w));


	PMenu Menu = NewMenu(
     NewMenuItem( "Game", NULL, NULL, NULL,
     		 NewMenu (
     		 		NewMenuItem( "New game", NULL, MSG_NEWGAME, NULL, NULL,
     		 		NewMenuItemSeparator(
     		 		NewMenuItem( "Exit", NULL, WM_CLOSE, NULL, NULL,
     		 		NULL)))
     		 ),
     NewMenuItem( "Help", NULL, NULL, NULL,
     	NewMenu(
     		NewMenuItem( "About", NULL, WM_ABOUT, NULL, NULL, NULL)),
     NULL))
	);

	RectAssign(&r,0,0,3*SIZE,20);

	PMenuView o = NewMenuView(&Me,r,Menu,MenuViewStyleHorizontal,0);
	InsertWidget(WIDGET(w), WIDGET(o));
	
	WidgetSize(&r, 0, 25, 3*SIZE, 3*SIZE);
	Surface = CreateCanvas(&Me, r);
	WIDGET(Surface)->Draw = &SurfaceDraw;
	WIDGET(Surface)->EventHandler = &SurfaceEH;
	WIDGET(Surface)->Flags |= WFForceBuffer;
	InsertWidget(WIDGET(w), WIDGET(Surface));

	WidgetDrawAll(WIDGET(w));

	WIDGET(w)->AppEvHdl = &AppEventHandler;

	return true;
}

void Close (void)
{
	WidgetDispose(WIDGET(w));
}
