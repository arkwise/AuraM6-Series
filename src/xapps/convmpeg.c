/**
*	oZone example
*		(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
* Flamme algorithm by Kostas Michalopoulos aka Bad Sector
* 	Copyright (C) 2001,2002,2003 Kostas Michalopoulos
*
* Licenses
*   Algorithm : GNU General Public License (TimerPoll,NormalFlame,ColdFlame,GreenFlame)
*   Software  : Public domain
*
**/
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "label.h"
#include "button.h"
#include "iodlg.h"
#include "console.h"
#include "sound.h"
#include "tbox.h"
#include "time.h"
#include "slider.h"

#include "mpeg2.h"
#include "canvas.h"

#define MSG_COLNO		0x20104
#define MSG_COLRED		0x20101
#define MSG_COLBLUE		0x20102
#define MSG_COLGREEN	0x20103
#define MSG_REDRAW		0x20001

#define FWIDTH  324
#define FHEIGHT 210

#define  MSG_PLAY	0xF0030006

#define PLITEM(o) ((PPlaylistItem)(o))

typedef struct TPlaylistItem *PPlaylistItem;
typedef struct TPlaylistItem {
	struct	TListviewItem o;
	l_text File;
	l_int  Sec;
} TPlaylistItem;

PCanvas	c 	= 0;
PLabel l = 0;
PTimer t = 0;
PWindow w = 0;
PSStream s = NULL;
PFileTypes Filter = 0;
char temp[400];
PButton btpp;

l_color	Col[256]; // Color palette
l_uchar	Scr[FWIDTH][FHEIGHT+2]; // was 160,100
PListview PlayList = 0;
PSlider sl;
l_bool UsePlayList = false;
PPlaylistItem CurrentItem = NULL;

////////////////////////////////////////////////////////////////////////////////
l_ulong AppVersion = ULONG_ID(0,1,0,0);
char    AppName[] = "Convert MPEG";
l_uid	nUID = "convmpeg";
l_uid NeededLibs[] = { "widget", "button","label","window","mpeg2","conlib","mpg","textbox","" };

PImage IconPause, IconPlay, IconStop, IconNext, IconPrev, IconPlus, IconMinus;
///////////////////////////////////////////////////////////////////////////////
l_bool AppEventHandler ( PWidget o, PEvent Ev ) {
	if ( Ev->Type == EV_MESSAGE ) {
		if ( Ev->Message == WM_CLOSE ) {
			CloseApp(&Me);
			return true;
		}
//		if ( Ev->Message == MSG_COLRED ) NormalFlame();
//		if ( Ev->Message == MSG_COLBLUE ) ColdFlame();
//		if ( Ev->Message == MSG_COLGREEN ) GreenFlame();
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
void SStreamLoad ( l_text file, l_text name ) {
	//CurrentName
	btpp->Icon = IconPlay;
	if ( s ) SStreamFree(s);
	if ( CurrentFile ) free(CurrentFile);
	if ( CurrentName ) free(CurrentName);
	CurrentFile = NULL;
	CurrentName = NULL;
	if ( s = LoadData2(file,DTYPE_SOUNDSTREAM) ) {
		if ( !name ) {
			PFileInfo f = FileGetInfo(file);
			if ( f ) {
				CurrentName = TextDup(f->Name);
				FreeFileInfo(f);
			} else
				CurrentName = TextDup("Error!");
		} else
			CurrentName = TextDup(name);
		LabelSetText(l,"%s",CurrentName);
		CurrentFile = TextDup(file);
		SStreamGetLengthMsec(s,&sl->Steps);
		WidgetDraw(WIDGET(sl), NULL);
	} else {
		LabelSetText(l,"Error loading %s",name);
		s = NULL;
		sl->Steps = sl->Value = 0;
		WidgetDraw(WIDGET(btpp), NULL);
	}

}

////////////////////////////////////////////////////////////////////////////////

static void save_pgm (int width, int height,
		      int chroma_width, int chroma_height,
		      uint8_t * const * buf, int num)
{
    char filename[100];
    PFile * pgmfile;
    int i;
    static uint8_t black[16384] = { 0 };

    sprintf (filename, "%d.pgm", num);
    pgmfile = FileOpen (filename, "wb");
    if (!pgmfile) {
     sprintf (temp, "Could not open file \"%s\".\n", filename);
     MessageBox(&Me, "Error", temp, MBB_OK);
     return;
    }
    else {
     //sprintf (temp, "Writing to tile %s...\n", filename);
     //MessageBox(&Me, "Info", temp, MBB_OK);
    }

    FilePrintf (pgmfile, "P5\n%d %d\n255\n",
	     2 * chroma_width, height + chroma_height);
    for (i = 0; i < height; i++) {
	FileWrite (buf[0] + i * width, width, 1, pgmfile);
	FileWrite (black, 2 * chroma_width - width, 1, pgmfile);
    }
    for (i = 0; i < chroma_height; i++) {
	FileWrite (buf[1] + i * chroma_width, chroma_width, 1, pgmfile);
	FileWrite (buf[2] + i * chroma_width, chroma_width, 1, pgmfile);
    }
    FileClose (pgmfile);
}

void NormalFlame(void) {
	l_int  i, r;
	for (i=0;i<64;i++)
		Col[i] = RGB(i*3, 0, 0);
	r = 192;
	for (i=64;i<128;i++) {
		Col[i] = RGB(r, (i-64)*4, 0);
		if (r < 255) r++;
	}
	for (i=128;i<255;i++)
		Col[i] = RGB(255, 255, i-128);
}

static void sample1 (PFile * mpgfile)
{

#define BUFFER_SIZE 4096
    uint8_t buffer[BUFFER_SIZE];
    mpeg2dec_t * decoder;
    const mpeg2_info_t * info;
    const mpeg2_sequence_t * sequence;
    mpeg2_state_t state;
    size_t size;
    int framenum = 0;

    decoder = mpeg2_init ();
    if (decoder == NULL) {
     sprintf (temp, "Could not allocate a decoder object.\n");
     MessageBox(&Me, "Error", temp, MBB_OK);
    return;
    }
    else {
//     sprintf (temp, "Started...\n");
//     MessageBox(&Me, "Info", temp, MBB_OK);
    }
    info = mpeg2_info (decoder);

    size = (size_t)-1;
    do {
	state = mpeg2_parse (decoder);
	sequence = info->sequence;
	switch (state) {
	case STATE_BUFFER:
	    size = FileRead (buffer, 1, BUFFER_SIZE, mpgfile);
	    mpeg2_buffer (decoder, buffer, buffer + size);
	    break;
	case STATE_SLICE:
	case STATE_END:
	case STATE_INVALID_END:
	    if (info->display_fbuf)
		save_pgm (sequence->width, sequence->height,
			  sequence->chroma_width, sequence->chroma_height,
			  info->display_fbuf->buf, framenum++);
	    break;
	default:
	    break;
	}
    } while (size);

    mpeg2_close (decoder);
}

void FlameDraw (PWidget o, p_bitmap buffer, PRect w) {
	TPoint p;
	l_int x, y;
	for (y=w->a.y-o->Absolute.a.y;y<w->b.y-o->Absolute.a.y+1;y++) 
		for (x=w->a.x-o->Absolute.a.x;x<w->b.x-o->Absolute.a.x+1;x++)
		{
			l_int c = Scr[x+1][y+1];
			p.x = o->Absolute.a.x+x;
			p.y = o->Absolute.a.y+y;
			if ( _PointInRect( p,Mouse->CursorPosition) ) {
				if ( getpixel(Mouse->Cur->Image,p.x-Mouse->CursorPosition.a.x,p.y-Mouse->CursorPosition.a.y) == Mouse->Cur->Image->vtable->mask_color )
					putpixel(buffer, p.x, p.y, Col[c]);
			} else
				putpixel(buffer, p.x, p.y, Col[c]);
		}
}

void TimerPoll(void* A)
{
	if ( s ) {
		if ( s->Finished && !s->Playing ) {
			btpp->Icon = IconPlay;
			WidgetDraw(WIDGET(btpp), NULL);

			if ( UsePlayList ) PlayListGoNext();

		} else if ( s->Playing ) {
			SStreamGetPosMsec(s,&sl->Value);
			WidgetDraw(WIDGET(sl), NULL);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
void PlayListGoNext ( void ) {
	PListItem a = CurrentItem?ListFoundItem(PlayList->Items,CurrentItem):GetSelectedListItem(PlayList);
	UsePlayList = true;
	if ( a ) {
		CurrentItem = PLITEM(a->Next->Data);
		SStreamLoad(PLITEM(a->Next->Data)->File,LISTVIEWITEM(a->Next->Data)->Caption);
		ListviewSelectItem(PlayList,LISTVIEWITEM(a->Next->Data),true,false);
		if ( s ) {
			PlayListUpdateElement(PLITEM(a->Next->Data));
			PlaySoundStream(s);
			btpp->Icon = IconPause;
			WidgetDraw(WIDGET(btpp), NULL);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
l_int Main( int argc, l_text *argv ) {
	TRect r;
	PButton b = NULL;

	IconPlay = ListKey(Me.Ressource,"PLAY");

	RectAssign(&r,0, 0, FWIDTH+110, FHEIGHT);
	w = CreateWindow( &Me, r, "oZone example : Fire Effect", WF_FRAME|WF_CAPTION|WF_MINIMIZE|WF_CENTERED );
	InsertWidget(DeskTop, WIDGET(w));
	WIDGET(w)->AppEvHdl = &AppEventHandler;

	WidgetSize(&r,FWIDTH+5, 0, 100, 15);
	l = CreateLabel(&Me, r,"Color");
	InsertWidget(WIDGET(w), WIDGET(l));

	WidgetSize(&r,FWIDTH+15, r.b.y+2, 90, 20);
	b = CreateButton(&Me, r,"Normal",MSG_COLRED);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,FWIDTH+15, r.b.y+2, 90, 20);
	b = CreateButton(&Me, r,"Blue",MSG_COLBLUE);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,FWIDTH+15, r.b.y+2, 90, 20);
	b = CreateButton(&Me, r,"Green",MSG_COLGREEN);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,FWIDTH+5, r.b.y+12, 100, 15);
	l = CreateLabel(&Me, r,"Statistics");
	InsertWidget(WIDGET(w), WIDGET(l));

	WidgetSize(&r,FWIDTH+15, r.b.y+2, 90, 15);
	l = CreateLabel(&Me, r,"");
	InsertWidget(WIDGET(w), WIDGET(l));

	NormalFlame();

	WidgetSize(&r,2, 1, FWIDTH-3, FHEIGHT-4);
	c = CreateCanvas(&Me, r);
	WIDGET(c)->Draw = &FlameDraw;
	WIDGET(c)->Flags |= WFForceNonBuffer;
	InsertWidget(WIDGET(w), WIDGET(c));

	WidgetDrawAll(WIDGET(w));

	RectAssign(&r, 5, 274, 25, 294);
	btpp = CreateButton( &Me,  r, NULL, MSG_PLAY );
	btpp->Icon = IconPlay;
	InsertWidget(WIDGET(w), WIDGET(btpp));

	t = NewTimer(&Me,0, &TimerPoll,NULL);


Filter =        NewFileTypes("MPEG files (*.mpg)","mpg",
                                                NewFileTypes("All files",NULL,
                                                NULL ));


l_text file = IOBox("Open file",IOBOX_OPEN,NULL,Filter,true);
        if ( file ) {
          PFile f = FileOpen(file,"rb");
//          FILE *f = fopen(file,"rb");
          if ( f ) {
          sample1 (f);
          }
          else {
          sprintf (temp, "Cannot open %s...\n", file);
          MessageBox(&Me, "Error", temp, MBB_OK);
          }

          free(file);
        }

	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close( void ) {
WidgetDispose(WIDGET(w));
}
////////////////////////////////////////////////////////////////////////////////
