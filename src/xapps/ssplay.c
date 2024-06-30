#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "label.h"
#include "button.h"
#include "sound.h"
#include "slider.h"
#include "listview.h"
#include "menu.h"
#include "iodlg.h"

/*
001 fixed menu generation errors


*/

l_ulong AppVersion = ULONG_ID(0,0,0,1);
char AppName[] = "Sound Stream Player";
l_uid nUID = "ap:ssplay";
l_uid NeededLibs[] = { "widget", "mp3","sound","" };

typedef struct TPlaylistItem *PPlaylistItem;
typedef struct TPlaylistItem {
	struct TListviewItem o;
	l_text File;
	l_int  Sec;
} TPlaylistItem;

PTimer t;
PWindow w;
PSStream s = NULL;
PSlider sl;
PFileTypes Filter = 0;
PFileTypes FilterPlay = 0;

PListview PlayList = 0;

l_bool UsePlayList = false;

l_text CurrentFile = NULL;
l_text CurrentName = NULL;
l_text CurrentList = NULL;
PPlaylistItem CurrentItem = NULL;
PLabel l;

void SStreamLoad ( l_text file, l_text name );

#define MSG_FILENEW    0xF0030001
#define MSG_FILEOPEN   0xF0030002

#define MSG_LISTOPEN   0xF0030003
#define MSG_LISTSAVE   0xF0030004
#define MSG_LISTSAVEAS 0xF0030005

#define MSG_PLAY       0xF0030006
#define MSG_STOP       0xF0030007
#define MSG_PLAYVAL    0xF0030008
#define MSG_NEXT       0xF0030009
#define MSG_PREV       0xF0030010
#define MSG_ADD        0xF0030011
#define MSG_REMOVE     0xF0030012

#define MSG_ADDFILE    0xF0030013
#define MSG_ADDCUR     0xF0030014
PButton btpp;

PImage IconPause, IconPlay, IconStop, IconNext, IconPrev, IconPlus, IconMinus;

l_int oldpos = 0;

#define PLITEM(o) ((PPlaylistItem)(o))

////////////////////////////////////////////////////////////////////////////////
void PlaylistItemFree(PPlaylistItem o) {
	free(o->File);
	FreeListviewItem(LISTVIEWITEM(o));
}
////////////////////////////////////////////////////////////////////////////////
PPlaylistItem PlaylistAddItem ( PListview o, l_text file, l_text name, l_int sec )
{
	PPlaylistItem p = malloc(sizeof(TPlaylistItem));
	if (!p) return;
	l_text l;
	memset(p, 0, sizeof(TPlaylistItem));
	ListviewInitItem(LISTVIEWITEM(p),name,NULL);
	if ( sec ) {
		l = TextArgs("%d:%02d",sec/60,sec%60);
		ListviewItemPushCol(LISTVIEWITEM(p),l);
		free(l);
	} else
		ListviewItemPushCol(LISTVIEWITEM(p),"???");

	p->File = TextDup(file);
	p->Sec = sec;
	ListAdd (o->Items, name, p, (void*)&PlaylistItemFree);
	return p;
}
////////////////////////////////////////////////////////////////////////////////
void LoadPlaylist( l_text file ) {
	char Buffer[1024];
	l_text dir = GetPathParent(file);
	PFile f = FileOpen(file,"rt");
	char Name[512], Len[512];
	int ext = 0;
	if ( CurrentList ) free(CurrentList);
	CurrentList = NULL;
	
	if ( !f ) return;
	ListviewRemoveAllItems(PlayList);
	while ( FileGets(Buffer,1024,f) ) {
		if ( !TextNCaseCompare(Buffer,"#EXTINF:",8) ) {
			char *s = TextChr(Buffer+8,',');
			if ( s ) {
				TextNCopy(Len,Buffer+8,(Buffer+8)-s);
				TextCopy(Name,s+1);
			}
			ext = 1;
		} else if ( TextLen(Buffer) > 0 && Buffer[0] != '#' ) {
			if ( Buffer[0] != '/' ) {
				l_text media = FileNameToPath(dir,Buffer);
				l_text rmedia = ResolveFileName(media);
				free(media);
				PlaylistAddItem(PlayList,rmedia,ext?Name:Buffer,ext?atoi(Len):0);
				free(rmedia);
			} else
				PlaylistAddItem(PlayList,Buffer,ext?Name:Buffer,ext?atoi(Len):0);
			ext = 0;
		
		}
	}

	FileClose(f);
	free(dir);
	if ( PlayList->Items->Last )
		CurrentItem = PLITEM(PlayList->Items->Last->Next->Data);
	else
		CurrentItem = NULL;
	UsePlayList = true;
	CurrentList = TextDup(file);
	WidgetDrawAll(WIDGET(PlayList));
	ScrollBarRedraw(SLLWDG(PlayList));
}
////////////////////////////////////////////////////////////////////////////////
void SavePlayList( l_text file ) {
	PListItem a,b;
	PPlaylistItem o;
	PFile f = FileOpen(file,"wt");
	l_text rf,dir;
	if ( !f ) return;
	if ( CurrentList ) free(CurrentList);
	CurrentList = TextDup(file);
	if ( !PlayList->Items->Last ) return;
	b = a = PlayList->Items->Last->Next;
	dir = GetPathParent(file);
	FilePrintf(f,"#EXTM3U\n");
	do {
		o = (PPlaylistItem)a->Data;
		FilePrintf(f,"#EXTINF:%d,%s\n",o->Sec,o->o.Caption);
		rf = RelativeName(dir,o->File);
		FilePrintf(f,"%s\n\n",rf);
		free(rf);
		a = a->Next;
	} while ( a != b );
	FileClose(f);
	free(dir);
}
////////////////////////////////////////////////////////////////////////////////
void PlayListUpdateElement ( PPlaylistItem i ) {
	l_ulong msec = 0;
	SStreamGetLengthMsec(s,&msec);
	if ( (msec/1000)+1 != i->Sec ) {
		l_text l;
		i->Sec = (msec/1000)+1;
		l = TextArgs("%d:%02d",i->Sec/60,i->Sec%60);
		ListviewItemSetColCaption(LISTVIEWITEM(i),1,l);
		free(l);
		WidgetDraw(WIDGET(PlayList), NULL);
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
void PlayListGoPrev ( void ) {
	PListItem a = CurrentItem?ListFoundItem(PlayList->Items,CurrentItem):GetSelectedListItem(PlayList);
	UsePlayList = true;
	if ( a ) {
		CurrentItem = PLITEM(a->Prev->Data);
		SStreamLoad(PLITEM(a->Prev->Data)->File,LISTVIEWITEM(a->Prev->Data)->Caption);
		ListviewSelectItem(PlayList,LISTVIEWITEM(a->Prev->Data),true,false);
		if ( s ) {
			PlayListUpdateElement(PLITEM(a->Prev->Data));
			PlaySoundStream(s);
			btpp->Icon = IconPause;
			WidgetDraw(WIDGET(btpp), NULL);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
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
void SlideNotify ( PSlider o, l_uchar Ctx, l_ulong Value ) {
	
	if ( Ctx == SCTX_SET ) {
		
		if ( s ) SStreamSetPosMsec(s,Value);
		
	} else if ( Ctx == SCTX_START ) {
		
		
	}
	
}
////////////////////////////////////////////////////////////////////////////////

l_bool AppEventHandler ( PWidget o, PEvent Ev )
{
	if ( Ev->Type == EV_MESSAGE )
	{
		if ( Ev->Message == WM_CLOSE )
		{
			CloseApp(&Me);

			return true;
		}
		if ( Ev->Message == MSG_PLAY )
		{
			if ( s ) {
				if ( s->Playing ) {
					btpp->Icon = IconPlay;
					PauseSoundStream(s);
				} else {
					btpp->Icon = IconPause;
					PlaySoundStream(s);
				}
				WidgetDraw(WIDGET(btpp), NULL);
			} else if ( UsePlayList ) {
				PPlaylistItem i = CurrentItem?CurrentItem:(PPlaylistItem)GetSelectedItem(PlayList);
				if ( i ) {
					SStreamLoad(i->File,LISTVIEWITEM(i)->Caption);
					if ( s ) {
						PlaySoundStream(s);
						btpp->Icon = IconPause;
						WidgetDraw(WIDGET(btpp), NULL);
					}
				}
			}
				
			return true;
		}
		if ( Ev->Message == MSG_PLAYVAL )
		{
			PPlaylistItem i = (PPlaylistItem)GetSelectedItem(PlayList);
			UsePlayList = true;
			if ( i ) {
				CurrentItem = i;
				SStreamLoad(i->File,LISTVIEWITEM(i)->Caption);
				if ( s ) {
					PlayListUpdateElement(i);
					PlaySoundStream(s);
					btpp->Icon = IconPause;
					WidgetDraw(WIDGET(btpp), NULL);
				}
			}
			return true;
		}
		
		if ( Ev->Message == MSG_NEXT )
		{
			PlayListGoNext();
			return true;
		}
		if ( Ev->Message == MSG_PREV )
		{
			PlayListGoPrev();
			return true;
		}
		if ( Ev->Message == MSG_STOP )
		{
			if ( s ) {
				if ( s->Playing ) PauseSoundStream(s);
				SStreamSetPosMsec(s,0);
				sl->Value = 0;
				btpp->Icon = IconPlay;
				WidgetDraw(WIDGET(btpp), NULL);
				WidgetDraw(WIDGET(sl), NULL);
			}
			return true;
		}
		if ( Ev->Message == MSG_FILEOPEN )
		{
			l_text newmedia = IOBox("MP3", IOBOX_OPEN, NULL, Filter, true);

			if ( newmedia ) {
				UsePlayList = false;
				SStreamLoad(newmedia,NULL);
				
				free(newmedia);	
			}
			
			return true;
		}
		if ( Ev->Message == MSG_LISTOPEN )
		{
			l_text newmedia = IOBox("Play lists", IOBOX_OPEN, NULL, FilterPlay, true);
			if ( newmedia ) {
				UsePlayList = true;
				LoadPlaylist(newmedia);
				free(newmedia);	
			}
			return true;
		}
		if ( Ev->Message == MSG_LISTSAVE )
		{
			if ( CurrentList ) {
				l_text newmedia = IOBox("Play lists", IOBOX_SAVE, NULL, FilterPlay, false);
				if ( newmedia ) {
					SavePlayList(newmedia);
					free(newmedia);	
				}
			} else
				SavePlayList(CurrentList);
			return true;
		}		
		if ( Ev->Message == MSG_LISTSAVEAS )
		{
			l_text newmedia = IOBox("Play lists", IOBOX_SAVE, NULL, FilterPlay, false);
			if ( newmedia ) {
				SavePlayList(newmedia);
				free(newmedia);	
			}
			return true;
		}
		if ( Ev->Message == WM_ABOUT )
		{
			MessageBox(&Me, "About oZone Music Player", "oZone Music Player 1.0\n\nCopyright (c) 2004 Point Mad, Lukas Lipka. All rights reserved.", MBB_OK);
			return true;
		}
		if ( Ev->Message == MSG_REMOVE )
		{
			PListItem i = GetSelectedListItem(PlayList);
			if ( i ) {
				if ( PLITEM(i->Data) == CurrentItem ) CurrentItem = NULL;
				if ( PlayList->TopItem == i->Data ) PlayList->TopItem = 0;
				ListRemoveItem(PlayList->Items,i);
				WidgetDrawAll(WIDGET(PlayList));
				ScrollBarRedraw(SLLWDG(PlayList));
			}			
			return true;
		}
		if ( Ev->Message == MSG_ADD )
		{
			PMenu m = NewMenu(
				NewMenuItemEx("Current", NULL, MSG_ADDCUR, UsePlayList || !s || !CurrentFile ? MI_DISABLE : 0, NULL, NULL, NULL,
				NewMenuItemEx("File", NULL, MSG_ADDFILE, 0, NULL, NULL, NULL, NULL))
			);
			l_ulong Msg = PopUpMenuWait ( &Me, Mouse->State.p, m, 0);
			FreeMenu(m);
			
			if ( Msg == MSG_ADDCUR ) {
				PlaylistAddItem(PlayList,CurrentFile,CurrentName,sl->Steps/1000);
			} else if ( Msg == MSG_ADDFILE ) {
				l_text newmedia = IOBox("MP3", IOBOX_OPEN, NULL, Filter, true);
				if ( newmedia ) {
					PFileInfo f = FileGetInfo(newmedia);
					if ( f ) {
						PlaylistAddItem(PlayList,newmedia,f->Name,0);
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

void GenerateSubMenu(PMenuItem itm, void* Args)
{
	l_text arg = (l_text)Args;

	if (TextCompare(arg, "File") == 0)
	{
		itm->SubMenu = NewMenu(
			NewMenuItem("New", NULL, MSG_FILENEW, 0, NULL, NULL)
		);
		AddMenuItem(itm->SubMenu, "Open", NULL, MSG_FILEOPEN, 0, NULL);
		AddMenuItemSeparator(itm->SubMenu);
		AddMenuItem(itm->SubMenu, "Open playlist", NULL, MSG_LISTOPEN, 0, NULL);
		AddMenuItem(itm->SubMenu, "Save playlist", NULL, MSG_LISTSAVE, 0, NULL);
		AddMenuItem(itm->SubMenu, "Save playlist as...", NULL, MSG_LISTSAVEAS, 0, NULL);
		AddMenuItemSeparator(itm->SubMenu);
		AddMenuItem(itm->SubMenu, "Exit", NULL, WM_CLOSE, 0, NULL);
	}
	else if (TextCompare(arg, "Help") == 0)
	{
		itm->SubMenu = NewMenu(
			NewMenuItem("About", NULL, WM_ABOUT, 0, NULL, NULL)
		);
	}
}

l_int Main ( int argc, l_text *argv )
{
	TRect r;
	PButton b = NULL;

	IconPause = ListKey(Me.Ressource,"PAUSE");
	IconPlay = ListKey(Me.Ressource,"PLAY");
	IconStop = ListKey(Me.Ressource,"STOP");
	IconNext = ListKey(Me.Ressource,"NEXT");
	IconPrev = ListKey(Me.Ressource,"PREV");
	IconPlus = ListKey(Me.Ressource,"PLUS");
	IconMinus = ListKey(Me.Ressource,"MINUS");

	RectAssign(&r,0, 0, 300, 230);
	w = CreateWindow( &Me, r, "oZone Music Player 1.0", WF_FRAME|WF_CAPTION|WF_MINIMIZE|WF_CENTERED );
	InsertWidget(DeskTop, WIDGET(w));
	WIDGET(w)->AppEvHdl = &AppEventHandler;

	Filter = 	NewFileTypes("MP3","mp3",
						NewFileTypes("All files",NULL,
						NULL ));
	FilterPlay = 	NewFileTypes("Play lists","m3u",
						NewFileTypes("All files",NULL,
						NULL ));
	
	PMenu Menu = NewMenu(
		NewMenuItemEx("File", NULL, 0, 0, NULL, &GenerateSubMenu, "File", NULL)
	);
	AddMenuItemEx(Menu, "Help", NULL, 0, 0, NULL, &GenerateSubMenu, "Help");

	RectAssign(&r,0,0,540,20);
	PMenuView o = CreateMenuView(&Me,r,Menu,MenuViewStyleHorizontal,0);
	InsertWidget(WIDGET(w), WIDGET(o));

	RectAssign(&r,0, 22, 300, 37);
	l = CreateLabel(&Me, r, NULL);
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));
	
	RectAssign(&r,0, 40, 250, 60);
	sl = CreateSlider(&Me,r,1);
	InsertWidget(WIDGET(w), WIDGET(sl));
	
	sl->Notify = &SlideNotify;
	
	RectAssign(&r, 255, 40, 275, 60);
	btpp = CreateButton( &Me,  r, NULL, MSG_PLAY );
	btpp->Icon = IconPlay;
	InsertWidget(WIDGET(w), WIDGET(btpp));
	
	RectAssign(&r, 280, 40, 300, 60);
	b = CreateButton( &Me,  r, NULL, MSG_STOP );
	b->Icon = IconStop;
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r,0, 80, 275, 230);
	PlayList = CreateListview(&Me,r,"Title",LVS_LIST,LVF_ORDER|LVF_COLUMS|LVF_COLRESIZE);
	ListviewAddColum ( PlayList, "Length",NULL, 75, 0 );
	PlayList->OnValMsg = MSG_PLAYVAL;
	InsertWidget(WIDGET(w), WIDGET(PlayList));
	
	RectAssign(&r, 280, 80, 300, 100);
	b = CreateButton( &Me,  r, NULL, MSG_NEXT );
	b->Icon = IconNext;
	InsertWidget(WIDGET(w), WIDGET(b));
	
	RectAssign(&r, 280, 105, 300, 125);
	b = CreateButton( &Me,  r, NULL, MSG_PREV );
	b->Icon = IconPrev;
	InsertWidget(WIDGET(w), WIDGET(b));
	
	RectAssign(&r, 280, 130, 300, 150);
	b = CreateButton( &Me,  r, NULL, MSG_ADD );
	b->Icon = IconPlus;
	InsertWidget(WIDGET(w), WIDGET(b));
	
	RectAssign(&r, 280, 155, 300, 175 );
	b = CreateButton( &Me,  r, NULL, MSG_REMOVE );
	b->Icon = IconMinus;
	InsertWidget(WIDGET(w), WIDGET(b));
	
	WidgetDrawAll(WIDGET(w));
	t = NewTimer(&Me,100, &TimerPoll,NULL);
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{
	KillTimer(t);
	WidgetDispose(WIDGET(w));
	if ( s ) SStreamFree(s);
	if ( CurrentFile ) free(CurrentFile);
	if ( CurrentName ) free(CurrentName);
	if ( CurrentList ) free(CurrentList);
}
////////////////////////////////////////////////////////////////////////////////
