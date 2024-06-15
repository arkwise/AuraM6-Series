/////////////////////////////////////////////////////////////////////////////////
//
//	Media Box
//
//	Aura Milestone 3
//
//	(c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////
#include <JUDAS.H>

#include "kernel.h"
#include "internal.h"
#include "widget.h"
#include "window.h"
#include "label.h"
#include "button.h"
#include "sound.h"
#include "slider.h"
#include "listview.h"
#include "menu.h"
#include "label.h"
#include "iodlg.h"
#include "canvas.h"
#include "tabbook.h"
#include "debug.h"

l_ulong AppVersion = ULONG_ID(0, 0, 0, 1);
char AppName[] = "Media Box";
l_uid nUID = "app:mbox";
l_uid NeededLibs[] = {"widget", "mp3", "tabbook", "sound", ""};

typedef struct TPlaylistItem *PPlaylistItem;
typedef struct TPlaylistItem
{
	struct TListviewItem o;
	l_text File;
	l_int Sec;
} TPlaylistItem;

enum JUDASFILE
{
	UNDEF,
	XM,
	MOD,
	S3M
};


PTimer tim;
PWindow w;
PSStream s = NULL;
PSlider sl;
PFileTypes Filter = 0;
PFileTypes FilterPlay = 0;
PTabBook bk;

PListview PlayList = 0;

l_bool UsePlayList = false;

l_text CurrentFile = NULL;
l_text CurrentName = NULL;
l_text CurrentList = NULL;
PPlaylistItem CurrentItem = NULL;
PLabel l;

#define FWIDTH 324
#define FHEIGHT 210

l_color Col[256];									// Color palette
l_uchar Scr[FWIDTH][FHEIGHT + 2]; // was 160,100
l_int cnt = 0;
PCanvas c = 0;
PLabel l = 0;
PTimer t = 0;
l_ulong nb = 0;
l_ubig ltime = 0;
PWindow w = 0;

/* JUDAS variables */
int mixer = QUALITYMIXER;
int filetype = 0;
int interpolation = 1;
int songrounds = 1;
int channels = 80;
int vumeterwidth = 1;
int vumeterson = 1;
char filename[254];
int mastervolume = 50;
int filewriter = 0;
int isJudasError = UNDEF;

#define MSG_COLNO 0x20104
#define MSG_COLRED 0x20101
#define MSG_COLBLUE 0x20102
#define MSG_COLGREEN 0x20103
#define MSG_REDRAW 0x20001

void SStreamLoad(l_text file, l_text name);

#define MSG_FILENEW 0xF0030001
#define MSG_FILEOPEN 0xF0030002

#define MSG_LISTOPEN 0xF0030003
#define MSG_LISTSAVE 0xF0030004
#define MSG_LISTSAVEAS 0xF0030005

#define MSG_PLAY 0xF0030006
#define MSG_STOP 0xF0030007
#define MSG_PLAYVAL 0xF0030008
#define MSG_NEXT 0xF0030009
#define MSG_PREV 0xF0030010
#define MSG_ADD 0xF0030011
#define MSG_REMOVE 0xF0030012

#define MSG_ADDFILE 0xF0030013
#define MSG_ADDCUR 0xF0030014
PButton btpp;

PImage IconPause, IconPlay, IconStop, IconNext, IconPrev, IconPlus, IconMinus;

l_int oldpos = 0;

#define PLITEM(o) ((PPlaylistItem)(o))
////////////////////////////////////////////////////////////////////////////////
void PlaylistItemFree(PPlaylistItem o)
{
	free(o->File);
	FreeListviewItem(LISTVIEWITEM(o));
}
////////////////////////////////////////////////////////////////////////////////
PPlaylistItem PlaylistAddItem(PListview o, l_text file, l_text name, l_int sec)
{
	PPlaylistItem p = malloc(sizeof(TPlaylistItem));
	if (!p)
		return;
	l_text l;
	memset(p, 0, sizeof(TPlaylistItem));
	ListviewInitItem(LISTVIEWITEM(p), name, NULL);
	if (sec)
	{
		l = TextArgs("%d:%02d", sec / 60, sec % 60);
		ListviewItemPushCol(LISTVIEWITEM(p), l);
		free(l);
	}
	else
		ListviewItemPushCol(LISTVIEWITEM(p), "???");

	p->File = TextDup(file);
	p->Sec = sec;
	ListAdd(o->Items, name, p, (void *)&PlaylistItemFree);
	return p;
}
////////////////////////////////////////////////////////////////////////////////
void LoadPlaylist(l_text file)
{
	char Buffer[1024];
	l_text dir = GetPathParent(file);
	PFile f = FileOpen(file, "rt");
	char Name[512], Len[512];
	int ext = 0;
	if (CurrentList)
		free(CurrentList);
	CurrentList = NULL;

	if (!f)
		return;
	ListviewRemoveAllItems(PlayList);
	while (FileGets(Buffer, 1024, f))
	{
		if (!TextNCaseCompare(Buffer, "#EXTINF:", 8))
		{
			char *s = TextChr(Buffer + 8, ',');
			if (s)
			{
				TextNCopy(Len, Buffer + 8, (Buffer + 8) - s);
				TextCopy(Name, s + 1);
			}
			ext = 1;
		}
		else if (TextLen(Buffer) > 0 && Buffer[0] != '#')
		{
			if (Buffer[0] != '/')
			{
				l_text media = FileNameToPath(dir, Buffer);
				l_text rmedia = ResolveFileName(media);
				free(media);
				PlaylistAddItem(PlayList, rmedia, ext ? Name : Buffer, ext ? atoi(Len) : 0);
				free(rmedia);
			}
			else
				PlaylistAddItem(PlayList, Buffer, ext ? Name : Buffer, ext ? atoi(Len) : 0);
			ext = 0;
		}
	}

	FileClose(f);
	free(dir);
	if (PlayList->Items->Last)
		CurrentItem = PLITEM(PlayList->Items->Last->Next->Data);
	else
		CurrentItem = NULL;
	UsePlayList = true;
	CurrentList = TextDup(file);
	WidgetDrawAll(WIDGET(PlayList));
	ScrollBarRedraw(SLLWDG(PlayList));
}
////////////////////////////////////////////////////////////////////////////////
void SavePlayList(l_text file)
{
	PListItem a, b;
	PPlaylistItem o;
	PFile f = FileOpen(file, "wt");
	l_text rf, dir;
	if (!f)
		return;
	if (CurrentList)
		free(CurrentList);
	CurrentList = TextDup(file);
	if (!PlayList->Items->Last)
		return;
	b = a = PlayList->Items->Last->Next;
	dir = GetPathParent(file);
	FilePrintf(f, "#EXTM3U\n");
	do
	{
		o = (PPlaylistItem)a->Data;
		FilePrintf(f, "#EXTINF:%d,%s\n", o->Sec, o->o.Caption);
		rf = RelativeName(dir, o->File);
		FilePrintf(f, "%s\n\n", rf);
		free(rf);
		a = a->Next;
	} while (a != b);
	FileClose(f);
	free(dir);
}
////////////////////////////////////////////////////////////////////////////////
void PlayListUpdateElement(PPlaylistItem i)
{
	l_ulong msec = 0;
	SStreamGetLengthMsec(s, &msec);
	if ((msec / 1000) + 1 != i->Sec)
	{
		l_text l;
		i->Sec = (msec / 1000) + 1;
		l = TextArgs("%d:%02d", i->Sec / 60, i->Sec % 60);
		ListviewItemSetColCaption(LISTVIEWITEM(i), 1, l);
		free(l);
		WidgetDraw(WIDGET(PlayList), NULL);
	}
}
////////////////////////////////////////////////////////////////////////////////
void PlayListGoNext(void)
{
	PListItem a = CurrentItem ? ListFoundItem(PlayList->Items, CurrentItem) : GetSelectedListItem(PlayList);
	UsePlayList = true;
	if (a)
	{
		CurrentItem = PLITEM(a->Next->Data);
		SStreamLoad(PLITEM(a->Next->Data)->File, LISTVIEWITEM(a->Next->Data)->Caption);
		ListviewSelectItem(PlayList, LISTVIEWITEM(a->Next->Data), true, false);
		if (s)
		{
			PlayListUpdateElement(PLITEM(a->Next->Data));
			PlaySoundStream(s);
			btpp->Icon = IconPause;
			WidgetDraw(WIDGET(btpp), NULL);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void PlayListGoPrev(void)
{
	PListItem a = CurrentItem ? ListFoundItem(PlayList->Items, CurrentItem) : GetSelectedListItem(PlayList);
	UsePlayList = true;
	if (a)
	{
		CurrentItem = PLITEM(a->Prev->Data);
		SStreamLoad(PLITEM(a->Prev->Data)->File, LISTVIEWITEM(a->Prev->Data)->Caption);
		ListviewSelectItem(PlayList, LISTVIEWITEM(a->Prev->Data), true, false);
		if (s)
		{
			PlayListUpdateElement(PLITEM(a->Prev->Data));
			PlaySoundStream(s);
			btpp->Icon = IconPause;
			WidgetDraw(WIDGET(btpp), NULL);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void TimerPoll(void *A)
{
	if (s)
	{
		if (s->Finished && !s->Playing)
		{
			btpp->Icon = IconPlay;
			WidgetDraw(WIDGET(btpp), NULL);

			if (UsePlayList)
				PlayListGoNext();
		}
		else if (s->Playing)
		{
			SStreamGetPosMsec(s, &sl->Value);
			WidgetDraw(WIDGET(sl), NULL);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void SStreamLoad(l_text file, l_text name)
{
	//CurrentName
	btpp->Icon = IconPlay;
	if (s)
		SStreamFree(s);
	if (CurrentFile)
		free(CurrentFile);
	if (CurrentName)
		free(CurrentName);
	CurrentFile = NULL;
	CurrentName = NULL;
	if (s = LoadData2(file, DTYPE_SOUNDSTREAM))
	{
		if (!name)
		{
			PFileInfo f = FileGetInfo(file);
			if (f)
			{
				CurrentName = TextDup(f->Name);
				FreeFileInfo(f);
			}
			else
				CurrentName = TextDup("Error!");
		}
		else
			CurrentName = TextDup(name);
		LabelSetText(l, "%s", CurrentName);
		CurrentFile = TextDup(file);
		SStreamGetLengthMsec(s, &sl->Steps);
		WidgetDraw(WIDGET(sl), NULL);
	}
	else
	{
		LabelSetText(l, "Error loading %s", name);
		s = NULL;
		sl->Steps = sl->Value = 0;
		WidgetDraw(WIDGET(btpp), NULL);
	}
}
////////////////////////////////////////////////////////////////////////////////
void SlideNotify(PSlider o, l_uchar Ctx, l_ulong Value)
{

	if (Ctx == SCTX_SET)
	{

		if (s)
			SStreamSetPosMsec(s, Value);
	}
	else if (Ctx == SCTX_START)
	{
	}
}
////////////////////////////////////////////////////////////////////////////////
void TimerPolll(void *A)
{
	l_int i, j;
	for (i = 1; i < FWIDTH; i++)
		Scr[i][FHEIGHT] = rand() % 255;
	for (j = 1; j < FHEIGHT; j++)
		for (i = 1; i < FWIDTH - 2; i++)
		{
			int c = (Scr[i - 1][j - 1] + Scr[i + 1][j - 1] + Scr[i - 1][j + 1] + Scr[i + 1][j + 1]) / 4;
			if (c < 0)
				c = 0;
			if (cnt == 0)
				Scr[i - 1][j - 1] = c;
			if (cnt == 1)
				Scr[i][j - 1] = c;
			else if (cnt == 2)
				Scr[i + 1][j - 1] = c;
			else if (cnt == 3)
				Scr[i][j - 1] = c;
			Scr[i][j] = c;
		}
	cnt++;
	if (cnt == 4)
		cnt = 0;
	nb++;
	if (ATime - ltime >= 1000)
	{
		ltime = ATime;
		nb = 0;
	}
	WidgetDraw(WIDGET(c), NULL);
}

void NormalFlame(void)
{
	l_int i, r;
	for (i = 0; i < 64; i++)
		Col[i] = RGB(i * 3, 0, 0);
	r = 192;
	for (i = 64; i < 128; i++)
	{
		Col[i] = RGB(r, (i - 64) * 4, 0);
		if (r < 255)
			r++;
	}
	for (i = 128; i < 255; i++)
		Col[i] = RGB(255, 255, i - 128);
}

void GreenFlame(void)
{
	l_int i, r;
	for (i = 0; i < 64; i++)
		Col[i] = RGB(0, i * 3, 0);
	r = 192;
	for (i = 64; i < 128; i++)
	{
		Col[i] = RGB((i - 64) * 4, r, 0);
		if (r < 255)
			r++;
	}
	for (i = 128; i < 255; i++)
		Col[i] = RGB(255, 255, i - 128);
}

void ColdFlame(void)
{
	l_int i, r;
	for (i = 0; i < 64; i++)
		Col[i] = RGB(0, 0, i * 3);
	r = 192;
	for (i = 64; i < 128; i++)
	{
		Col[i] = RGB(0, (i - 64) * 4, r);
		if (r < 255)
			r++;
	}
	for (i = 128; i < 255; i++)
		Col[i] = RGB(i - 128, 255, 255);
}

void NoFlame(void)
{
	l_int i, r;
	for (i = 0; i < 64; i++)
		Col[i] = RGB(0, 0, 0);
	r = 192;
	for (i = 64; i < 128; i++)
	{
		Col[i] = RGB(0, 0, 0);
		if (r < 255)
			r++;
	}
	for (i = 128; i < 255; i++)
		Col[i] = RGB(0, 0, 0);
}

void FlameDraw(PWidget o, p_bitmap buffer, PRect w)
{
	TPoint p;
	l_int x, y;
	for (y = w->a.y - o->Absolute.a.y; y < w->b.y - o->Absolute.a.y + 1; y++)
		for (x = w->a.x - o->Absolute.a.x; x < w->b.x - o->Absolute.a.x + 1; x++)
		{
			l_int c = Scr[x + 1][y + 1];
			p.x = o->Absolute.a.x + x;
			p.y = o->Absolute.a.y + y;
			if (_PointInRect(p, Mouse->CursorPosition))
			{
				if (getpixel(Mouse->Cur->Image, p.x - Mouse->CursorPosition.a.x, p.y - Mouse->CursorPosition.a.y) == Mouse->Cur->Image->vtable->mask_color)
					putpixel(buffer, p.x, p.y, Col[c]);
			}
			else
				putpixel(buffer, p.x, p.y, Col[c]);
		}
}

l_bool AppEventHandler(PWidget o, PEvent Ev)
{
	if (Ev->Type == EV_MESSAGE)
	{
		if (Ev->Message == WM_CLOSE)
		{
			CloseApp(&Me);

			return true;
		}

		if (Ev->Message == MSG_COLNO)
			NoFlame();
		if (Ev->Message == MSG_COLRED)
			NormalFlame();
		if (Ev->Message == MSG_COLBLUE)
			ColdFlame();
		if (Ev->Message == MSG_COLGREEN)
			GreenFlame();

		if (Ev->Message == MSG_PLAY)
		{
			if (s)
			{
				if (s->Playing)
				{
					btpp->Icon = IconPlay;
					PauseSoundStream(s);
					NoFlame();
				}
				else
				{
					btpp->Icon = IconPause;
					PlaySoundStream(s);
					NormalFlame();
				}
				WidgetDraw(WIDGET(btpp), NULL);
			}
			else if (UsePlayList)
			{
				PPlaylistItem i = CurrentItem ? CurrentItem : (PPlaylistItem)GetSelectedItem(PlayList);
				if (i)
				{
					SStreamLoad(i->File, LISTVIEWITEM(i)->Caption);
					if (s)
					{
						PlaySoundStream(s);
						btpp->Icon = IconPause;
						WidgetDraw(WIDGET(btpp), NULL);
					}
				}
			}

			return true;
		}
		if (Ev->Message == MSG_PLAYVAL)
		{
			PPlaylistItem i = (PPlaylistItem)GetSelectedItem(PlayList);
			UsePlayList = true;
			if (i)
			{
				CurrentItem = i;
				SStreamLoad(i->File, LISTVIEWITEM(i)->Caption);
				if (s)
				{
					PlayListUpdateElement(i);
					PlaySoundStream(s);
					btpp->Icon = IconPause;
					WidgetDraw(WIDGET(btpp), NULL);
				}
			}
			return true;
		}

		if (Ev->Message == MSG_NEXT)
		{
			PlayListGoNext();
			return true;
		}
		if (Ev->Message == MSG_PREV)
		{
			PlayListGoPrev();
			return true;
		}
		if (Ev->Message == MSG_STOP)
		{
			NoFlame();
			if (s)
			{
				if (s->Playing)
					PauseSoundStream(s);
				SStreamSetPosMsec(s, 0);
				sl->Value = 0;
				btpp->Icon = IconPlay;
				WidgetDraw(WIDGET(btpp), NULL);
				WidgetDraw(WIDGET(sl), NULL);
			}
			return true;
		}
		if (Ev->Message == MSG_FILEOPEN)
		{
			l_text newmedia = IOBox("MP3", IOBOX_OPEN, NULL, Filter, true);

			if (newmedia)
			{
				UsePlayList = false;
				SStreamLoad(newmedia, NULL);

				free(newmedia);
			}

			return true;
		}
		if (Ev->Message == MSG_LISTOPEN)
		{
			l_text newmedia = IOBox("Play lists", IOBOX_OPEN, NULL, FilterPlay, true);
			if (newmedia)
			{
				UsePlayList = true;
				LoadPlaylist(newmedia);
				free(newmedia);
			}
			return true;
		}
		if (Ev->Message == MSG_LISTSAVE)
		{
			if (CurrentList)
			{
				l_text newmedia = IOBox("Play lists", IOBOX_SAVE, NULL, FilterPlay, false);
				if (newmedia)
				{
					SavePlayList(newmedia);
					free(newmedia);
				}
			}
			else
				SavePlayList(CurrentList);
			return true;
		}
		if (Ev->Message == MSG_LISTSAVEAS)
		{
			l_text newmedia = IOBox("Play lists", IOBOX_SAVE, NULL, FilterPlay, false);
			if (newmedia)
			{
				SavePlayList(newmedia);
				free(newmedia);
			}
			return true;
		}
		if (Ev->Message == WM_ABOUT)
		{
			MessageBox(&Me, "About Music Player", "Music Player\n\nOrigionaly written by Point Mad and Lukas Lipka for use with Ozone GUI.", MBB_OK);
			return true;
		}
		if (Ev->Message == MSG_REMOVE)
		{
			PListItem i = GetSelectedListItem(PlayList);
			if (i)
			{
				if (PLITEM(i->Data) == CurrentItem)
					CurrentItem = NULL;
				if (PlayList->TopItem == i->Data)
					PlayList->TopItem = 0;
				ListRemoveItem(PlayList->Items, i);
				WidgetDrawAll(WIDGET(PlayList));
				ScrollBarRedraw(SLLWDG(PlayList));
			}
			return true;
		}
		if (Ev->Message == MSG_ADD)
		{
			PMenu m = NewMenu(
					NewMenuItem("Current", NULL, MSG_ADDCUR, UsePlayList || !s || !CurrentFile ? MI_DISABLE : 0, NULL,
											NewMenuItem("File", NULL, MSG_ADDFILE, 0, NULL,
																	NULL)));
			l_ulong Msg = PopUpMenuWait(&Me, Mouse->State.p, m, 0);
			FreeMenu(m);

			if (Msg == MSG_ADDCUR)
			{
				PlaylistAddItem(PlayList, CurrentFile, CurrentName, sl->Steps / 1000);
			}
			else if (Msg == MSG_ADDFILE)
			{
				l_text newmedia = IOBox("MP3", IOBOX_OPEN, NULL, Filter, true);
				if (newmedia)
				{
					PFileInfo f = FileGetInfo(newmedia);
					if (f)
					{
						PlaylistAddItem(PlayList, newmedia, f->Name, 0);
						FreeFileInfo(f);
					}
				}
			}
			WidgetDrawAll(WIDGET(PlayList));
			ScrollBarRedraw(SLLWDG(PlayList));
			return true;
		}
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_int Main(int argc, l_text *argv)
{
	TRect r;
	PButton b = NULL;

	/* judas library init */
	int mixrate = 48000;
	int mixmode = SIXTEENBIT | STEREO;

	// judas_config();
	// if (!judas_init(mixrate, mixer, mixmode, interpolation))
	// 	DebugFatal("JUDAS ERROR: %s\n", judas_errortext[judas_error]);
	
	// /* Set mastervolume of channels */
	// judas_setmusicmastervolume(CHANNELS, mastervolume);

	IconPause = ListKey(Me.Ressource, "PAUSE");
	IconPlay = ListKey(Me.Ressource, "PLAY");
	IconStop = ListKey(Me.Ressource, "STOP");
	IconNext = ListKey(Me.Ressource, "NEXT");
	IconPrev = ListKey(Me.Ressource, "PREV");
	IconPlus = ListKey(Me.Ressource, "PLUS");
	IconMinus = ListKey(Me.Ressource, "MINUS");

	RectAssign(&r, 0, 0, 327, 303);
	w = CreateWindow(&Me, r, "Media Box", WF_FRAME | WF_CAPTION | WF_MINIMIZE | WF_CENTERED | WF_RESIZE);

	InsertWidget(DeskTop, WIDGET(w));
	WIDGET(w)->AppEvHdl = &AppEventHandler;

	Filter = NewFileTypes("MP3", "mp3",
												NewFileTypes("All files", NULL,
																		 NULL));
	FilterPlay = NewFileTypes("Aura Playlist", "mbpl",
														NewFileTypes("Windows Play list", "m3u",
																				 NewFileTypes("All files", NULL,
																											NULL)));

	PMenu Menu = NewMenu(
			NewMenuItem("File", NULL, 0, 0,
									NewMenu(
											NewMenuItem("New", NULL, MSG_FILENEW, 0, NULL,
																	NewMenuItem("Open", NULL, MSG_FILEOPEN, 0, NULL,
																							NewMenuItemSeparator(
																									NewMenuItem("Open playlist", NULL, MSG_LISTOPEN, 0, NULL,
																															NewMenuItem("Save playlist", NULL, MSG_LISTSAVE, 0, NULL,
																																					NewMenuItem("Save playlist as...", NULL, MSG_LISTSAVEAS, 0, NULL,
																																											NewMenuItemSeparator(
																																													NewMenuItem("Exit", NULL, WM_CLOSE, 0, NULL, NULL))))))))),
									NewMenuItem("View", NULL, 0, 0,
															NewMenu(
																	NewMenuItem("No Effect", NULL, MSG_COLNO, 0, NULL,
																							NewMenuItem("Normal Fire", NULL, MSG_COLRED, 0, NULL,
																													NewMenuItem("Blue Fire", NULL, MSG_COLBLUE, 0, NULL,
																																			NewMenuItem("Green Fire", NULL, MSG_COLGREEN, 0, NULL, NULL))))),
															NewMenuItem("Help", NULL, 0, 0,
																					NewMenu(
																							NewMenuItem("About", NULL, WM_ABOUT, 0, NULL, NULL)),
																					NULL))));

	RectAssign(&r, 0, 0, 540, 20);
	PMenuView o = NewMenuView(&Me, r, Menu, MenuViewStyleHorizontal, 0);
	InsertWidget(WIDGET(w), WIDGET(o));

	RectAssign(&r, 105, 278, 327, 294);
	l = CreateLabel(&Me, r, NULL);
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));

	RectAssign(&r, 0, 249, 327, 269);
	sl = CreateSlider(&Me, r, 1);
	InsertWidget(WIDGET(w), WIDGET(sl));

	sl->Notify = &SlideNotify;

	RectAssign(&r, 5, 274, 25, 294);
	btpp = CreateButton(&Me, r, NULL, MSG_PLAY);
	btpp->Icon = IconPlay;
	InsertWidget(WIDGET(w), WIDGET(btpp));

	RectAssign(&r, 30, 274, 50, 294);
	b = CreateButton(&Me, r, NULL, MSG_STOP);
	b->Icon = IconStop;
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r, 55, 274, 75, 294);
	b = CreateButton(&Me, r, NULL, MSG_PREV);
	b->Icon = IconPrev;
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r, 80, 274, 100, 294);
	b = CreateButton(&Me, r, NULL, MSG_NEXT);
	b->Icon = IconNext;
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r, 1, 20, 326, 226);
	bk = CreateTabBook(&Me, r);
	InsertWidget(WIDGET(w), WIDGET(bk));

	t = TabBookAddTab(bk, "Visualizations", NULL);
	NoFlame();

	WidgetSize(&r, 0, 0, FWIDTH - 3, FHEIGHT - 4);
	c = CreateCanvas(&Me, r);
	WIDGET(c)->Draw = &FlameDraw;
	WIDGET(c)->Flags |= WFForceNonBuffer | WF_AUTORESIZE;
	InsertWidget(WIDGET(t), WIDGET(c));

	t = TabBookAddTab(bk, "Playlist", NULL);
	RectAssign(&r, 0, 0, 275, 150);
	PlayList = CreateListview(&Me, r, "Title", LVS_LIST, LVF_ORDER | LVF_COLUMS | LVF_COLRESIZE);
	ListviewAddColum(PlayList, "Lenght", NULL, 75, 0);
	PlayList->OnValMsg = MSG_PLAYVAL;

	if (argc > 0 && argv[1])
	{
		PFileInfo f = FileGetInfo(argv[1]);
		if (f)
		{
			UsePlayList = true;
			PlaylistAddItem(PlayList, argv[1], f->Name, 0);
			FreeFileInfo(f);
		}
	}
	InsertWidget(WIDGET(t), WIDGET(PlayList));

	RectAssign(&r, 280, 0, 300, 20);
	b = CreateButton(&Me, r, NULL, MSG_ADD);
	b->Icon = IconPlus;
	InsertWidget(WIDGET(t), WIDGET(b));

	RectAssign(&r, 280, 25, 300, 45);
	b = CreateButton(&Me, r, NULL, MSG_REMOVE);
	b->Icon = IconMinus;
	InsertWidget(WIDGET(t), WIDGET(b));

	WidgetDrawAll(WIDGET(w));
	tim = NewTimer(&Me, 100, &TimerPoll, NULL);
	tim = NewTimer(&Me, 0, &TimerPolll, NULL);

	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close(void)
{
	// judas_uninit();
	KillTimer(tim);
	WidgetDispose(WIDGET(w));
	if (s)
		SStreamFree(s);
	if (CurrentFile)
		free(CurrentFile);
	if (CurrentName)
		free(CurrentName);
}
////////////////////////////////////////////////////////////////////////////////
