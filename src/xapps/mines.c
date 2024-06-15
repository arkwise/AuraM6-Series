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

l_uid 	nUID 		= "o3mines";
l_ulong AppVersion	= ULONG_ID(0,0,1,0);
char AppName[]		= "03 Mines";
l_uid NeededLibs[]	= { "widget","window","menu","canvas","label","" };

int FWIDTH = 10;
int FHEIGHT = 10;
int MINES = 10;

#define BSIZE 15


#define Q_BOMB		0x00000010
#define Q_QUESTION	0x00000008
#define Q_FLAGED	0x00000004
#define Q_VISITED	0x00000002

#define MSG_NEWGAME 0xdeadbeef

#define MSG_EASY 0x0F0A0001
#define MSG_MEDIUM 0x0F0A0002
#define MSG_HARD 0x0F0A0003
PWindow w = 0;
PCanvas MineField = 0;
l_uchar MineSource[64][64];
l_bool End = 0;

PLabel Count, Info;

void CountMines ( int *total, int *marked, int *good ) {
	
	l_int x,y;	
	
	(*total) = 0;
	(*marked) = 0;
	(*good) = 0;
	
	for(y=0;y<FHEIGHT;y++)
		for(x=0;x<FHEIGHT;x++) {
			
			if ( MineSource[x][y] & Q_BOMB ) (*total)++;
			
			if ( MineSource[x][y] & Q_FLAGED ) (*marked)++;
			
			if ( (MineSource[x][y] & Q_FLAGED) && (MineSource[x][y] & Q_BOMB)  ) 
				(*good)++;
			else if ( !(MineSource[x][y] & Q_FLAGED) && (MineSource[x][y] & Q_BOMB) )
				(*good)--;
		}
	
	
}


l_int GetMinesAround ( l_int x, l_int y )
{
	l_int i = 0;

	if ( x > 0 && y > 0 )
		if (MineSource[x - 1][y - 1] & Q_BOMB) i++;

	if ( y > 0 )
		if (MineSource[x][y - 1] & Q_BOMB) i++;

	if ( x < (FWIDTH-1) && y >= 0 )
		if (MineSource[x + 1][y - 1] & Q_BOMB) i++;

	if ( x > 0 )
		if (MineSource[x - 1][y] & Q_BOMB) i++;

	if ( x < (FWIDTH-1) )
		if (MineSource[x + 1][y] & Q_BOMB) i++;

	if ( x > 0 && y < (FHEIGHT-1) )
		if (MineSource[x - 1][y + 1] & Q_BOMB) i++;

	if ( y < (FHEIGHT-1) )
		if (MineSource[x][y + 1] & Q_BOMB) i++;

	if ( x < (FWIDTH-1) && y < (FHEIGHT-1) )
		if (MineSource[x + 1][y + 1] & Q_BOMB) i++;

	return i;
}


void OpenMinesAround ( l_int x, l_int y )
{
	
	if ( MineSource[x][y] & Q_BOMB ) return;
	if ( MineSource[x][y] & Q_VISITED ) return;
	
	MineSource[x][y] |= Q_VISITED;
	MineSource[x][y] &= ~(Q_FLAGED|Q_QUESTION);
	
	if ( GetMinesAround(x,y) ) return;
	
	if ( x > 0 && y > 0 )
		OpenMinesAround(x - 1,y - 1);

	if ( y > 0 )
		OpenMinesAround(x,y - 1);

	if ( x < (FWIDTH-1) && y > 0 )
		OpenMinesAround(x + 1,y - 1);

	if ( x > 0 )
		OpenMinesAround(x - 1,y);

	if ( x < (FWIDTH-1) )
		OpenMinesAround(x + 1,y);

	if ( x > 0 && y < (FHEIGHT-1) )
		OpenMinesAround(x - 1,y + 1);

	if ( y < (FHEIGHT-1) )
		OpenMinesAround(x,y + 1);

	if ( x < (FWIDTH-1) && y < (FHEIGHT-1) )
		OpenMinesAround(x + 1,y + 1);
}


void FieldDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	l_int x = 0, xp;
	l_int y = 0, yp, n;

	rectfill(buffer, WIDGET(MineField)->Absolute.a.x, WIDGET(MineField)->Absolute.a.y, WIDGET(MineField)->Absolute.b.x, WIDGET(MineField)->Absolute.b.y, COL_3DFACE);

	xp = WIDGET(MineField)->Absolute.a.x;
	do
	{
		yp = WIDGET(MineField)->Absolute.a.y;
		do
		{
			
			

			if ( MineSource[x][y] & Q_VISITED || ((MineSource[x][y] & Q_BOMB) && End)  )
			{
				rect(buffer, xp, yp ,xp+BSIZE-1, yp+BSIZE-1, COL_3DLIGHT);
				//rectfill(buffer, WIDGET(MineField)->Absolute.a.x+x*BSIZE, WIDGET(MineField)->Absolute.a.y+y*BSIZE , WIDGET(MineField)->Absolute.a.x+-1+(x+1)*BSIZE, WIDGET(MineField)->Absolute.a.y-1+(y+1)*BSIZE, makecol(255,255,255));
				
				if ( (MineSource[x][y] & Q_BOMB) && End )
					rectfill(buffer, xp+3, yp+3, xp+BSIZE-4, yp+BSIZE-4, makecol(255,0,0));
				else if ( n = GetMinesAround(x,y)) {
					l_text t = TextArgs("%d", n);
					textout_ex(buffer, default_font,t, xp+((BSIZE-text_length(default_font,t))/2), yp+((BSIZE-text_height(default_font))/2), COL_3DTEXT, -1);
					free(t);
				}
			}
			else 
			{
				Rect3D(buffer, xp, yp ,xp+BSIZE-1, yp+BSIZE-1, COL_3DLIGHT, COL_3DDARK);

				if ( MineSource[x][y] & Q_FLAGED )
					textout_ex(buffer, default_font,"X", xp+((BSIZE-text_length(default_font,"X"))/2), yp+((BSIZE-text_height(default_font))/2), COL_3DTEXT, -1);
				
				if ( MineSource[x][y] & Q_QUESTION )
					textout_ex(buffer, default_font,"?", xp+((BSIZE-text_length(default_font,"?"))/2), yp+((BSIZE-text_height(default_font))/2), COL_3DTEXT, -1);

			}

			y++;
			yp+= BSIZE;
		}
		while ( y != FHEIGHT );

		y = 0; x++;
		xp+= BSIZE;
	}
	while ( x != FWIDTH );
}

void GetFieldUnder ( TPoint *p , TPoint m )
{
	l_int x = 0;
	l_int y = 0;

	do
	{
		do
		{
			TRect r;
			RectAssign(&r, WIDGET(MineField)->Absolute.a.x+x*BSIZE, WIDGET(MineField)->Absolute.a.y+y*BSIZE , WIDGET(MineField)->Absolute.a.x+(x+1)*BSIZE, WIDGET(MineField)->Absolute.a.y+(y+1)*BSIZE);

			if ( PointInRect(m, r) )
			{
				p->x = x;
				p->y = y;
				return;
			}

			y++;

		}
		while ( y != FHEIGHT );

		y = 0; x++;
	}
	while ( x != FWIDTH );
}

FieldEH ( PWidget o, PEvent Event )
{
	if ( Event->Type == EV_MOUSE)
	{
		if ( Event->Message == WEvMouseLDown && !End )
		{
			TPoint p;
			GetFieldUnder(&p, Mouse->State.p);

			if (MineSource[p.x][p.y] & Q_FLAGED)
				return false;

			if (MineSource[p.x][p.y] & Q_BOMB)
			{
				End = true;
				LabelSetText(Info,"BOOM! Game Over");
				WidgetDraw(WIDGET(MineField), NULL);
			}
			else if ( !MineSource[p.x][p.y] )
			{
				//MineSource[p.x][p.y] |= Q_VISITED;
				OpenMinesAround(p.x, p.y);
				WidgetDraw(WIDGET(MineField), NULL);
			}
		}

		if ( Event->Message == WEvMouseRDown && !End )
		{
			TPoint p;
			TRect r;

			int total, marked, good;
			
			
			GetFieldUnder(&p, Mouse->State.p);

			if (MineSource[p.x][p.y] & Q_FLAGED)
				MineSource[p.x][p.y] &= ~Q_FLAGED;
			else
				MineSource[p.x][p.y] |= Q_FLAGED;

			RectAssign(&r, WIDGET(MineField)->Absolute.a.x+p.x*BSIZE, WIDGET(MineField)->Absolute.a.y+p.y*BSIZE , WIDGET(MineField)->Absolute.a.x+(p.x+1)*BSIZE, WIDGET(MineField)->Absolute.a.y+(p.y+1)*BSIZE);
			WidgetDraw(WIDGET(MineField), &r);
			
			CountMines ( &total, &marked, &good );
			
			if ( total == good ) {
				LabelSetText(Info,"Well done !");
				End = true;
			}
			
			LabelSetText(Count,"%d",total-marked);
			
			
		}
	}
	return false;
}

void NewRandomPoint ( TPoint *p )
{
	p->x = rand()%FWIDTH;
	p->y = rand()%FHEIGHT;

	if ( MineSource[p->x][p->y] == 1 )
	{
		NewRandomPoint(p);
		return;
	}
}

void GenerateMineSource ( void )
{
	l_int n = 0;
	TPoint p;

	memset(MineSource,0,sizeof(MineSource));

	while ( n != MINES )
	{
		NewRandomPoint(&p);

		MineSource[p.x][p.y] |= Q_BOMB;

		n++;
	}
}

void ResetCounter ( void ) {
	int total, marked, good;
	CountMines ( &total, &marked, &good );
	LabelSetText(Count,"%d",total-marked);
}


void ChangeSize ( l_int nw, l_int nh ) {
	int dw, dh;
	
	dw = (nw-FWIDTH)*BSIZE;
	dh = (nh-FHEIGHT)*BSIZE;
	DebugMessage("%d %d",dw,dh);
	FWIDTH = nw;
	FHEIGHT = nh;
	
	GenerateMineSource();
	WidgetResize(WIDGET(MineField),dw,dh);
	WidgetResize(WIDGET(w),dw,dh);	
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
				MessageBox(&Me, "About O3 Mines", "O3 Mines 0.1\nYet another cloned game for oZone GUI\n\nCopyright (c) 2004 Point Mad, Lukas Lipka. All rights reserved.", MBB_OK);
				return true;
			}
			break;
			case MSG_EASY:
			{

				End = false;
				MINES = 10;
				ChangeSize(10,10);
				
				LabelSetText(Info,"Let's go !");
				ResetCounter();
				return true;
			}
			break;
			case MSG_MEDIUM:
			{

				End = false;
				MINES = 40;
				ChangeSize(16,16);
				
				LabelSetText(Info,"Let's go !");
				ResetCounter();
				return true;
			}
			break;
			case MSG_HARD:
			{

				End = false;
				MINES = 100;
				ChangeSize(30,16);

				LabelSetText(Info,"Let's go !");
				ResetCounter();

				return true;
			}
			break;

			case MSG_NEWGAME:
			{

				GenerateMineSource();
				End = false;
				WidgetDraw(WIDGET(MineField), NULL);

				LabelSetText(Info,"Let's go !");
				ResetCounter();

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
	
	
	RectAssign(&r,0, 0, FWIDTH*BSIZE, FHEIGHT*BSIZE+50);
	w = CreateWindow(&Me, r, "O3 Mines", WF_CAPTION|WF_FRAME|WF_CENTERED|WF_MINIMIZE);
	InsertWidget(WIDGET(DeskTop), WIDGET(w));


	PMenu Menu = NewMenu(
     NewMenuItem( "Game", NULL, 0, 0,
     		 NewMenu (
     		 		NewMenuItem( "New game", NULL, MSG_NEWGAME, 0, NULL,
     		 		NewMenuItemSeparator(
     		 		NewMenuItem( "Easy", NULL, MSG_EASY, 0, NULL,
     		 		NewMenuItem( "Medium", NULL, MSG_MEDIUM, 0, NULL,
     		 		NewMenuItem( "Hard", NULL, MSG_HARD, 0, NULL,
     		 		NewMenuItemSeparator(
     		 		NewMenuItem( "Exit", NULL, WM_CLOSE, 0, NULL,
     		 		NULL)))))))
     		 ),
     NewMenuItem( "Help", NULL, 0, 0,
     	NewMenu(
     		NewMenuItem( "About", NULL, WM_ABOUT, 0, NULL, NULL)),
     NULL))
	);

	RectAssign(&r,0,0,FWIDTH*BSIZE,20);

	PMenuView o = NewMenuView(&Me,r,Menu,MenuViewStyleHorizontal,0);
	InsertWidget(WIDGET(w), WIDGET(o));

	RectAssign(&r,5, 25, 30, 45);
	Count = CreateLabel(&Me, r,"");
	InsertWidget(WIDGET(w), WIDGET(Count));	
	
	
	RectAssign(&r,35,25, FWIDTH*BSIZE, 45);
	Info = CreateLabel(&Me, r,"Welcome");
	InsertWidget(WIDGET(w), WIDGET(Info));	
	
	
	WidgetSize(&r, 0, 50, FWIDTH*BSIZE, FHEIGHT*BSIZE);
	MineField = CreateCanvas(&Me, r);
	WIDGET(MineField)->Draw = &FieldDraw;
	WIDGET(MineField)->EventHandler = &FieldEH;
	WIDGET(MineField)->Flags |= WFForceBuffer;
	InsertWidget(WIDGET(w), WIDGET(MineField));
	
	srand(time(0));
	
	GenerateMineSource();

	WidgetDrawAll(WIDGET(w));
	
	ResetCounter();

	WIDGET(w)->AppEvHdl = &AppEventHandler;

	return true;
}

void Close (void)
{
	WidgetDispose(WIDGET(w));
}
