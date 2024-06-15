/////////////////////////////////////////////////////////////////////////////////
//
//	Sticky Note
//
//	Aura Milestone 4 
//
//	(c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "label.h"
#include "window.h"
#include "tbox.h"
#include "button.h"

l_ulong UID			= ULONG_ID('S','T','N','O');
l_ulong AppVersion	= ULONG_ID(0,0,1,0);
char AppName[]		= "StickyNote";

typedef struct TNOTEHEAD *PNOTEHEAD;
typedef struct TNOTEHEAD
{
	l_ulong Magic;
	l_int Notes;
} TNOTEHEAD;

typedef struct TNOTE *PNOTE;
typedef struct TNOTE
{
	char Note[1024];

	TPoint Where;
} TNOTE;

typedef struct TWGTNOTE *PWGTNOTE;
typedef struct TWGTNOTE
{
	struct TWidget o;

	PNOTE Note;
} TWGTNOTE;

#define NOTE(x) ((PWGTNOTE)(x))

/**
*	Global variables
*/
//FILE *NOTEFile	= 0;
PMenu NOTEMenu	= 0;
l_int NOTEDoing = 0;
PList NOTEList	= 0;
TPoint NOTEPoint;
PTextbox NOTETextbox = 0;
PWidget NOTEEventHandler = 0;

/**
*	NOTE MENU MESSAGES
*/
#define MSG_NOTE_REMOVE 0xFAFA0001
#define MSG_NOTE_HIDE	0xFAFA0002

#define MSG_TRAY_NEW	0xFAFB0001
#define MSG_TRAY_EXIT	0xFAFB0002

#define BTN_ADDNOTE		0xBBBB0001
#define BTN_CANCELNOTE	0xBBBB0002

/**
*	Prototypes
*/
void NOTENew ( void );

/**
*	NAME: WidgetNoteEH
*	DESCRIPTION: Handles events for each note
*/
l_bool WidgetNoteEH ( PWidget o, PEvent Ev )
{
	if ( Ev->Type == EV_MESSAGE )
	{
		if ( Ev->Message == MSG_NOTE_REMOVE )
		{
			ListRemoveItem(NOTEList, ListFoundItem(NOTEList, NOTE(o)));
			return true;
		}
	}
	else if ( Ev->Type == EV_MOUSE )
	{
		if ( Ev->Message == WEvMouseLDown )
		{
			MasterWidget = o;
			NOTEPoint = Mouse->State.p;
			return true;
		}

		if ( Ev->Message == WEvMouseLUp )
		{
			MasterWidget = NULL;

			/**
			*	If moving, move the window to the new position.
			*/
			if ( NOTEDoing )
			{
				WidgetUnOSD(); // Unactivate OSD
				WidgetMove(o,Mouse->State.p.x-NOTEPoint.x,Mouse->State.p.y-NOTEPoint.y);
			}

			return true;
		}

		if (Ev->Message == WEvMouseMove && Mouse->State.b & BUTTON_LEFT )
		{
			NOTEDoing = 1;
			TRect r;
			RectAssign(&r,o->Absolute.a.x+(Mouse->State.p.x-NOTEPoint.x),o->Absolute.a.y+(Mouse->State.p.y-NOTEPoint.y),o->Absolute.b.x+(Mouse->State.p.x-NOTEPoint.x),o->Absolute.b.y+(Mouse->State.p.y-NOTEPoint.y));
			WidgetSetOSDRect(r); // Set/Move OSD rect (xor)
		}

		if ( Ev->Message == WEvMouseRDown )
		{
			PopUpMenu(&Me, Mouse->State.p, NOTEMenu, o, 0);
			return true;
		}
	}

	return false;
}

/**
*	NAME: WidgetNoteDraw
*	DESCRIPTION: Default draw function for each note
*/
void WidgetNoteDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	rectfill(buffer, o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.b.y, makecol(255,255,176));
	rect(buffer, o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x,o->Absolute.b.y, makecol(225,225,146));
	rect(buffer, o->Absolute.a.x,o->Absolute.a.y,o->Absolute.b.x-1,o->Absolute.b.y-1, makecol(200,200,120));

	if ( NOTE(o)->Note->Note )
	{
		l_text tempt = (char*)strdup(NOTE(o)->Note->Note);
		l_text t  = tempt;
		l_text lt = tempt;
		l_char tmp = 0;
		l_ulong y = o->Absolute.a.y+5;
		l_ulong x = o->Absolute.a.x+5;
		l_ulong ln = 0;
		l_ulong h = text_height(default_font);

		while ( t )
		{
			t = TextChr(t,'\n');
			if ( t ) t++;
			ln++;
		}

		t  = tempt;

		while ( t )
		{
			lt = t;

			t = TextChr(t,'\n');

  		if ( t ) {
				DrawTextEx(buffer,default_font,x, y,lt,t,makecol(0,0,0),0);
				t++;
				y += h;
			} else
			  textout_ex(buffer, default_font, lt, x, y,makecol(0,0,0),-1);
		}
	}
}

void NOTEFree ( void* Data )
{
	WidgetDispose(WIDGET(Data));
}

/**
*	NAME: CreateNote
*	DESCRIPTION: Creates a new note on the screen
*/
PWGTNOTE CreateNote ( PNOTE Note )
{
	PWGTNOTE p = malloc(sizeof(TWGTNOTE));
	TRect r;

	if (!p) return NULL;

	memset(p, 0, sizeof(TWGTNOTE));

	p->Note = Note;

	RectAssign(&r,Note->Where.x, Note->Where.y,Note->Where.x+140, Note->Where.y+160);
	IntialiseWidget(&Me, StdWidgetClass, WIDGET(p), r);
	WIDGET(p)->Draw = &WidgetNoteDraw;
	WIDGET(p)->EventHandler = &WidgetNoteEH;
	InsertWidget(DeskTop, WIDGET(p));
	WidgetDraw(WIDGET(p), NULL);

	ListAdd(NOTEList, NULL, p, &NOTEFree);

	return p;
}

/**
*	NAME: NOTELoadFile
*	DESCRIPTION: Loads a note file
*/
void NOTELoadFile ( l_text szFile )
{
	TNOTEHEAD NoteHead;
	PNOTE Note;
	l_int Entry = 0;

	FILE* NOTEFile = fopen(szFile, "rb");
	if (!NOTEFile) return;

	fread(&NoteHead, 1, sizeof(TNOTEHEAD), NOTEFile);

	if (NoteHead.Notes)
	{
		while ( Entry != NoteHead.Notes )
		{
			Note = NEW(TNOTE);
			CLEAR(Note);

			fread(Note, 1, sizeof(TNOTE), NOTEFile);
			CreateNote(Note);

			Entry++;
		}
	}

	fclose(NOTEFile);
}

/**
*	NAME: NOTESaveFile
*	DESCRIPTION: Saves all notes to the file
*/
void NOTESaveFile ( l_text szFile )
{
	TNOTEHEAD NoteHead;
	TNOTE Note;
	l_int Entries = 0;

	FILE* NOTEFile = fopen(szFile, "wb");

	if (!NOTEFile) return;

	//Head
	NoteHead.Magic = ULONG_ID('S','N','G','S');
	NoteHead.Notes = Entries;
	fwrite(&NoteHead, 1, sizeof(TNOTEHEAD), NOTEFile);

	PListItem a = NOTEList->Last;

	if (a)
	{
		do
		{
			strcpy(Note.Note, NOTE(a->Data)->Note->Note);
			Note.Where = WIDGET(a->Data)->Absolute.a;
			fwrite(&Note, 1, sizeof(TNOTE), NOTEFile);

			Entries++;
			a = a->Prev;
		}
		while ( a != NOTEList->Last );

		// Head once more
		fseek(NOTEFile, 0, SEEK_SET);
		NoteHead.Notes = Entries;
		fwrite(&NoteHead, 1, sizeof(TNOTEHEAD), NOTEFile);
	}

	fclose(NOTEFile);
}

l_bool AppEventHandler ( PWidget o, PEvent Event )
{
	if ( Event->Type == EV_MESSAGE )
	{
		switch ( Event->Message )
		{
			case MSG_TRAY_NEW:
				NOTENew();
			break;

			case MSG_TRAY_EXIT:
				CloseApp(&Me);
			break;
		}
	}

	return false;
}

l_bool NewNoteEventHandler ( PWidget o, PEvent Event )
{
	if ( Event->Type == EV_MESSAGE )
	{
		switch ( Event->Message )
		{
			case BTN_ADDNOTE:
			{
				PNOTE p = NEW(TNOTE);
				CLEAR(p);

				strcpy(p->Note, (char*)strdup(NOTETextbox->Text));
				PointAssign(&p->Where, 20, 20);

				CreateNote(p);
				CloseApp(&Me);

				//FIXME: Close window
			}
			break;

			case BTN_CANCELNOTE:

				CloseApp(&Me);
				//FIXME: Close window
			break;
		}
	}

	return false;
}

void NOTENew ( void )
{
	TRect r;
	PWindow w;

	RectAssign(&r,0,0,250,290);
	w = CreateWindow(&Me, r, "StickyNote - Add new note", WF_CAPTION|WF_FRAME|WF_MINIMIZE|WF_CENTERED);
	WIDGET(w)->AppEvHdl = &NewNoteEventHandler;
	InsertWidget(DeskTop, WIDGET(w));

	RectAssign(&r,5,5,220,210);
	NOTETextbox = CreateTextbox(&Me, r, TBF_MULTILINE|TBF_EDITABLE);
	InsertWidget(WIDGET(w), WIDGET(NOTETextbox));

	RectAssign(&r,20,235,120,258);
	PButton b = CreateButton(&Me, r, "Add Note", BTN_ADDNOTE);
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r,125,235,225,258);
	b = CreateButton(&Me, r, "Cancel", BTN_CANCELNOTE);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetDrawAll(WIDGET(w));
}

l_int Main ( int argc, l_text *argv )
{
	TRect r;
	RectAssign(&r,0,0,0,0);

	NOTEEventHandler = CreateWidget(&Me, r);
	NOTEEventHandler->AppEvHdl = &AppEventHandler;

	NOTEList = NewList();


	NOTELoadFile("notes.snt");


	NOTEMenu = NewMenu(
		NewMenuItem("Remove", NULL, MSG_NOTE_REMOVE, NULL, NULL,
		NewMenuItem("Hide", NULL, MSG_NOTE_HIDE, NULL, NULL, NULL
		))
	);

	PMenu NOTETraybarMenu =
	 NewMenu(
	  NewMenuItem("New note", NULL, MSG_TRAY_NEW, NULL, NULL,
	  NewMenuItem("Exit", NULL, MSG_TRAY_EXIT, NULL, NULL, NULL
	  ))
	 );

	TraybarAdd(LoadImage("SYSTEM/ICONS/pc16.bmp"), "StickyNote", NOTETraybarMenu, NOTEEventHandler);
	return true;
}

void Close (void)
{
	TraybarRemove("StickyNote");
	WidgetDispose(NOTEEventHandler);

	NOTESaveFile("notes.snt");
	FreeList(NOTEList);
}
