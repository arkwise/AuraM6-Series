////////////////////////////////////////////////////////////////////////////////
//
//	Keyboard Editor
//
//	Copyright (c) 2004 Point Mad. All rights  reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "button.h"
#include "window.h"
#include "label.h"
#include "checkbox.h"
#include "tabbook.h"
#include "tbox.h"
#include "iodlg.h"
#include <allegro/internal/aintern.h>
////////////////////////////////////////////////////////////////////////////////
l_ulong AppVersion	= ULONG_ID(0,0,0,1);
char AppName[]		= "Keyboard editor";
l_uid	nUID = "app:kedit";
l_uid NeededLibs[] = { "label","checkbox","button","window","tabbook","menu","textbox","" };
////////////////////////////////////////////////////////////////////////////////
PWindow w	= 0;
l_short *SelKey = 0;
TKeyboardLayout Layout;
TKeyboardLayout SysLayout;

PTabBook bk;

PTextbox tacc[4];
PTextbox taccf[4];
PTextbox tname;
PTextbox tcode;

PFileTypes Filter = 0;
PFileTypes Filter1 = 0;
////////////////////////////////////////////////////////////////////////////////
// Standart allegro stuff
////////////////////////////////////////////////////////////////////////////////
static unsigned short standard_key_ascii_table[KEY_MAX] = {
   0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   27, '`', '-', '=', 8, 9, '[', ']', 13, ';', '\'', '\\', '\\', ',', '.', '/', ' ',
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   '/', '*', '-', '+', '.', 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned short standard_key_capslock_table[KEY_MAX] = {
   0, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   27, '`', '-', '=', 8, 9, '[', ']', 13, ';', '\'', '\\', '\\', ',', '.', '/', ' ',
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   '/', '*', '-', '+', '.', 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned short standard_key_shift_table[KEY_MAX] = {
   0, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
   ')', '!', '@', '#', '$', '%', '^', '&', '*', '(', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   27, '~', '_', '+', 8, 9, '{', '}', 13, ':', '"', '|', '|', '<', '>', '?', ' ',
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   '/', '*', '-', '+', '.', 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned short standard_key_control_table[KEY_MAX] = {
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   27, 2, 2, 2, 127, 127, 2, 2, 10, 2, 2, 2, 2, 2, 2, 2, 2,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   2, 2, 2, 2, 2, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned short standard_key_empty_table[KEY_MAX];
////////////////////////////////////////////////////////////////////////////////
l_uchar VK[] =  {

KEY_ESC, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, 0xFE, 0xFE,
KEY_PRTSCR, KEY_SCRLOCK, KEY_PAUSE,
0xFF,

KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6,
KEY_7, KEY_8, KEY_9, KEY_0, KEY_MINUS, KEY_EQUALS, KEY_BACKSPACE, 0xFE,
KEY_INSERT,KEY_HOME,KEY_PGUP,0xFE,
KEY_NUMLOCK,KEY_SLASH_PAD,KEY_ASTERISK,KEY_MINUS_PAD,
0xFF,

KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y,
KEY_U, KEY_I, KEY_O, KEY_P, KEY_OPENBRACE, KEY_CLOSEBRACE, KEY_ENTER, 0xFE,
KEY_DEL,KEY_END,KEY_PGDN,0xFE,
KEY_7_PAD,KEY_8_PAD,KEY_9_PAD,KEY_PLUS_PAD,
0xFF,

KEY_CAPSLOCK, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H,
KEY_J, KEY_K, KEY_L, KEY_COLON, KEY_QUOTE, KEY_BACKSLASH, 0xFE, 0xFE,
0xFE,0xFE,0xFE,0xFE,
KEY_4_PAD,KEY_5_PAD,KEY_6_PAD,
0xFF,

KEY_LSHIFT, KEY_BACKSLASH2, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N,
KEY_M, KEY_COMMA, KEY_STOP, KEY_SLASH, KEY_RSHIFT, 0xFE, 0xFE,
0xFE,KEY_UP,0xFE,0xFE,
KEY_1_PAD,KEY_2_PAD,KEY_3_PAD, KEY_ENTER_PAD,
0xFF,

KEY_LCONTROL, KEY_ALT, KEY_LWIN, KEY_SPACE, KEY_ALTGR, KEY_RWIN, KEY_MENU, KEY_RCONTROL,
0xFE, 0xFE, 0xFE,
KEY_LEFT,KEY_DOWN,KEY_RIGHT,0xFE,
KEY_0_PAD,KEY_DEL_PAD,
0xFF,

0x00

};
////////////////////////////////////////////////////////////////////////////////
typedef struct TKeybEd {
	struct		TWidget o;
	l_short *Table;
	l_uchar   SelKey;
} TKeybEd, *PKeybEd;

#define KEYED(o) ((PKeybEd)(o))

////////////////////////////////////////////////////////////////////////////////
l_bool KeybEdEventHandler ( PWidget o, PEvent Event )
{
	if (Event->Type == EV_KEYBOARD) {
		KEYED(o)->SelKey = Event->Keyb.Key >> 8;
		WidgetDraw(o,NULL);
		return true;
	}

	if ((Event->Type == EV_MOUSE) && (Event->Message == WEvMouseLDClk)) {
		TRect r;
		l_uchar *g = VK;
		l_long n = 0, x = o->Absolute.a.x, y = o->Absolute.a.y,h=0,w=0;
		l_short *Table = KEYED(o)->Table, KeyCode = 0,*KeyPtr = 0;

		while ( n = *g ) {
			h = 20; w = 22;
			if ( *g == 0xFF ) {
				x = o->Absolute.a.x;
				y += h+2;
			} else if ( *g == 0xFE ) {
				x += w+2;
			} else {
				if( n == KEY_ENTER ||  n == KEY_ENTER_PAD ||  n == KEY_PLUS_PAD ) h += h+2;
				if( n == KEY_SPACE ) w += (w+2)*4;
				if( n == KEY_0_PAD ) w += (w+2);
				RectAssign(&r,x,y,x+w,y+h);
				if ( PointInRect(Mouse->State.p, r) ) {
					KeyCode = n;
					KeyPtr = &Table[KeyCode];
					break;
				}
				x += w+2;
			}
			g++;
		}

		if ( KeyCode ) {

			PTextbox tb;
			PLabel l;
			TRect 		rr;
			l_text ret = NULL;
			RectAssign(&rr,r.a.x-o->Absolute.a.x,r.a.y+15-o->Absolute.a.y,r.a.x-o->Absolute.a.x+60,r.a.y+30-o->Absolute.a.y);
			tb = CreateTextbox(&Me,rr,TBF_EDITABLE);
			InsertWidget(o, WIDGET(tb));

			RectAssign(&rr,r.a.x-o->Absolute.a.x,r.a.y-o->Absolute.a.y,r.a.x-o->Absolute.a.x+60,r.a.y+14-o->Absolute.a.y);
			l = CreateLabel(&Me,rr,"");
		  WIDGET(l)->BackgroundColor = COL_3DFACE;
			InsertWidget(o, WIDGET(l));

			TextBoxSetText(tb,"%d",*KeyPtr & 0xFFFF);
			LabelSetText(l,"Key: %d",KeyCode );

			tb->SelPos = tb->Text;
			tb->SelPosEnd = tb->Text+TextLen(tb->Text);
			WidgetDraw(WIDGET(tb), NULL);
			WidgetDraw(WIDGET(l), NULL);

			tb->ValidMsg = MSG_OK;

			WidgetSetFocus(WIDGET(tb));

			if ( WidgetExecute(WIDGET(tb)) != MSG_CANCEL ) {
				*KeyPtr = atoi(tb->Text) & 0xFFFF;
			}
			WidgetDispose(WIDGET(tb));
			WidgetDispose(WIDGET(l));

		}
		return true;

	} else if ((Event->Type == EV_MOUSE) && (Event->Message == WEvMouseLUp)) {
		TRect r;
		l_uchar *g = VK;
		l_long n = 0, x = o->Absolute.a.x, y = o->Absolute.a.y,h=0,w=0;
		l_short *Table = KEYED(o)->Table, KeyCode = 0;

		while ( n = *g ) {

			h = 20; w = 22;

			if ( *g == 0xFF ) {
				x = o->Absolute.a.x;
				y += h+2;
			} else if ( *g == 0xFE ) {
				x += w+2;
			} else {

				if( n == KEY_ENTER ||  n == KEY_ENTER_PAD ||  n == KEY_PLUS_PAD ) h += h+2;
				if( n == KEY_SPACE ) w += (w+2)*4;
				if( n == KEY_0_PAD ) w += (w+2);
				RectAssign(&r,x,y,x+w,y+h);

				if ( PointInRect(Mouse->State.p, r) ) {
					KeyCode = n;
				}
				x += w+2;
			}
			g++;
		}
		if ( KeyCode != KEYED(o)->SelKey ) {
			KEYED(o)->SelKey = KeyCode;
			WidgetDraw(o,NULL);
		}

		return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
void  KeybEdDraw(PWidget o, p_bitmap buffer, PRect where) {
	l_uchar *g = VK;
	l_long n = 0, x = o->Absolute.a.x, y = o->Absolute.a.y, vx = 0,h=0,w=0;
	l_short *Table = KEYED(o)->Table; l_text t; l_char vc = 0;
	rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, makecol(255,255,255));

	while ( n = *g ) {
			h = 20; w = 22;

		if ( *g == 0xFF ) {
			x = o->Absolute.a.x;
			y += h+2;
		} else if ( *g == 0xFE ) {
			x += w+2;
		} else {

				if( n == KEY_ENTER ||  n == KEY_ENTER_PAD ||  n == KEY_PLUS_PAD ) h += h+2;
				if( n == KEY_SPACE ) w += (w+2)*4;
				if( n == KEY_0_PAD ) w += (w+2);



			if ( !Table[n] ) {

				line(buffer,x+2,y+1,x+w-1,y+h,makecol(128,128,128));

				line(buffer,x+w-1,y+1,x+2,y+h,makecol(128,128,128));
			} else if ( (Table[n] >= 32) && (Table[n] < 127) ) {
				t = TextArgs("%c",0xFF & Table[n]);
				textout_ex(buffer, default_font, t, x+2, y+2, makecol(0,0,0), -1);
				free(t);
			} else {
				t = TextArgs("%x",0xFFFF & Table[n]);
				textout_ex(buffer, default_font, t, x+2, y+2, makecol(0,0,0), -1);
				free(t);
			}

			rect(buffer,x,y,x+w,y+h,(KEYED(o)->SelKey==n)?makecol(255,128,128):makecol(128,128,128));
			x += w+2;
		}
		g++;
	}
}
////////////////////////////////////////////////////////////////////////////////
PKeybEd NewKeybEdit ( TRect r, l_short *Table ) {
	PKeybEd o = malloc(sizeof(TKeybEd));
	if (!o) return NULL;
	memset(o, 0, sizeof(TKeybEd));
	IntialiseWidget(&Me, StdWidgetClass, WIDGET(o), r);
	o->Table = Table;
	WIDGET(o)->Draw = &KeybEdDraw;
	WIDGET(o)->EventHandler = KeybEdEventHandler;
	WIDGET(o)->Flags |= WFFocusable;
	return o;
}
////////////////////////////////////////////////////////////////////////////////
#define MSG_OPENFILE 0xF0020000
#define MSG_SAVEFILE 0xF0020001
#define MSG_TEST 0xF0020002
#define MSG_RESTAURE 0xF0020003
#define MSG_OPENALLEG 0xF0020004
#define MSG_SAVEALLEG 0xF0020005
////////////////////////////////////////////////////////////////////////////////
// Allegro imported function to load from .cfg files
static void read_key_table(unsigned short *out, unsigned short *in, char *section)
{
   char tmp1[64], tmp2[128], name[128];
   char *fmt = uconvert_ascii("key%d", tmp1);
   char *sec = uconvert_ascii(section, tmp2);
   int i;

   for (i=0; i<KEY_MAX; i++) {
      uszprintf(name, sizeof(name), fmt, i);
      out[i] = get_config_int(sec, name, in[i]);
   }
}
////////////////////////////////////////////////////////////////////////////////
void WriteCfgSection ( PFile f, l_short *Sec, l_short *Sys, l_text Name ) {
	int i,n=0;
	for (i=0; i<KEY_MAX; i++) {
	  if ( Sec[i] != Sys[i] ) n++;
	}
	if ( n ) {
		FilePrintf(f,"[%s]\n",Name);
		for (i=0; i<KEY_MAX; i++) {
			if ( Sec[i] != Sys[i] ) FilePrintf(f,"key%d = %d\n",i,Sec[i] & 0xFFFF);
		}
		FilePrintf(f,"\n");
	}
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
				WidgetDispose(WIDGET(o));
				CloseApp(&Me);
				return true;
			}
			break;

			case WM_ABOUT:
			{
				MessageBox(&Me, "Keyboard editor", "Keyboard Editor 0.1\nKeyboard layout editor for ozone/allegro.\nSupport OKL(ozone keyboard layout) 1.0\nSupport allegro cfg files (/allegro/ressource/keyboard/))\n\nCopyright (c) 2004 Point Mad. All rights reserved.", MBB_OK);
				return true;
			}
			break;

			case MSG_TEST:
			{
				Layout.accent1 = atoi(tacc[0]->Text);
				Layout.accent2 = atoi(tacc[1]->Text);
				Layout.accent3 = atoi(tacc[2]->Text);
				Layout.accent4 = atoi(tacc[3]->Text);
				Layout.accent1_flag = atoi(taccf[0]->Text);
				Layout.accent2_flag = atoi(taccf[1]->Text);
				Layout.accent3_flag = atoi(taccf[2]->Text);
				Layout.accent4_flag = atoi(taccf[3]->Text);
				KeyboardSetLayout(&Layout);
				return true;
			}
			break;
			case MSG_RESTAURE:
			{
				KeyboardSetLayout(&SysLayout);


				return true;
			}
			break;
			case MSG_OPENFILE:
			{
				l_text file = IOBox("Open file",IOBOX_OPEN,NULL,Filter,true);
				if ( file ) {
					PFile f = FileOpen(file,"rb");
					if ( f ) {
						FileRead(&Layout,1,sizeof(TKeyboardLayout),f);
						FileClose(f);
					}
					free(file);
					TextBoxSetText(tacc[0],"%d",Layout.accent1);
					TextBoxSetText(tacc[1],"%d",Layout.accent2);
					TextBoxSetText(tacc[2],"%d",Layout.accent3);
					TextBoxSetText(tacc[3],"%d",Layout.accent4);
					TextBoxSetText(taccf[0],"%d",Layout.accent1_flag);
					TextBoxSetText(taccf[1],"%d",Layout.accent2_flag);
					TextBoxSetText(taccf[2],"%d",Layout.accent3_flag);
					TextBoxSetText(taccf[3],"%d",Layout.accent4_flag);
					TextBoxSetText(tname,"%s",Layout.Name);
					TextBoxSetText(tcode,"%s",Layout.Code);
				}
				return true;
			}
			break;
			case MSG_SAVEFILE:
			{
				l_text file = IOBox("Save file",IOBOX_SAVE,NULL,Filter,false);
				if ( file ) {
					PFile f = FileOpen(file,"wb");
					if ( f ) {
						Layout.accent1 = atoi(tacc[0]->Text);
						Layout.accent2 = atoi(tacc[1]->Text);
						Layout.accent3 = atoi(tacc[2]->Text);
						Layout.accent4 = atoi(tacc[3]->Text);
						Layout.accent1_flag = atoi(taccf[0]->Text);
						Layout.accent2_flag = atoi(taccf[1]->Text);
						Layout.accent3_flag = atoi(taccf[2]->Text);
						Layout.accent4_flag = atoi(taccf[3]->Text);
						TextNCopy(Layout.Name,tname->Text,31);
						TextNCopy(Layout.Code,tcode->Text,4);
						FileWrite(&Layout,1,sizeof(TKeyboardLayout),f);
						FileClose(f);
					}
					free(file);
				}
				return true;
			}
			break;
			case MSG_OPENALLEG:
			{
				l_text file = IOBox("Open Allegro CFG file",IOBOX_OPEN,NULL,Filter1,true);
				if ( file ) {
					l_ulong size = 0;
					void *buffer = LoadData2(file,TYPE_TEXT);
					size = TextLen((l_text)buffer);
					if ( buffer ) {
						char tmp1[128],tmp2[128];
						set_config_data(buffer,size);
						free(buffer);
						read_key_table(Layout.ascii_table,          standard_key_ascii_table,     "key_ascii");
						read_key_table(Layout.capslock_table,       standard_key_capslock_table,  "key_capslock");
						read_key_table(Layout.shift_table,          standard_key_shift_table,     "key_shift");
						read_key_table(Layout.control_table,        standard_key_control_table,   "key_control");
						read_key_table(Layout.altgr_lower_table,    standard_key_empty_table,     "key_altgr");
						read_key_table(Layout.altgr_upper_table,    standard_key_empty_table,     "key_altgr");
						read_key_table(Layout.altgr_lower_table,    standard_key_empty_table, "key_altgr_lower");
						read_key_table(Layout.altgr_upper_table,    standard_key_empty_table, "key_altgr_upper");
						read_key_table(Layout.accent1_lower_table,  standard_key_empty_table,     "key_accent1_lower");
						read_key_table(Layout.accent1_upper_table,  standard_key_empty_table,     "key_accent1_upper");
						read_key_table(Layout.accent2_lower_table,  standard_key_empty_table,     "key_accent2_lower");
						read_key_table(Layout.accent2_upper_table,  standard_key_empty_table,     "key_accent2_upper");
						read_key_table(Layout.accent3_lower_table,  standard_key_empty_table,     "key_accent3_lower");
						read_key_table(Layout.accent3_upper_table,  standard_key_empty_table,     "key_accent3_upper");
						read_key_table(Layout.accent4_lower_table,  standard_key_empty_table,     "key_accent4_lower");
						read_key_table(Layout.accent4_upper_table,  standard_key_empty_table,     "key_accent4_upper");
						Layout.accent1 = get_config_int(uconvert_ascii("key_escape", tmp1), uconvert_ascii("accent1", tmp2), 0);
						Layout.accent2 = get_config_int(uconvert_ascii("key_escape", tmp1), uconvert_ascii("accent2", tmp2), 0);
						Layout.accent3 = get_config_int(uconvert_ascii("key_escape", tmp1), uconvert_ascii("accent3", tmp2), 0);
						Layout.accent4 = get_config_int(uconvert_ascii("key_escape", tmp1), uconvert_ascii("accent4", tmp2), 0);
						Layout.accent1_flag = get_config_int(uconvert_ascii("key_escape", tmp1), uconvert_ascii("accent1_flag", tmp2), 0);
						Layout.accent2_flag = get_config_int(uconvert_ascii("key_escape", tmp1), uconvert_ascii("accent2_flag", tmp2), 0);
						Layout.accent3_flag = get_config_int(uconvert_ascii("key_escape", tmp1), uconvert_ascii("accent3_flag", tmp2), 0);
						Layout.accent4_flag = get_config_int(uconvert_ascii("key_escape", tmp1), uconvert_ascii("accent4_flag", tmp2), 0);
						TextBoxSetText(tacc[0],"%d",Layout.accent1);
						TextBoxSetText(tacc[1],"%d",Layout.accent2);
						TextBoxSetText(tacc[2],"%d",Layout.accent3);
						TextBoxSetText(tacc[3],"%d",Layout.accent4);
						TextBoxSetText(taccf[0],"%d",Layout.accent1_flag);
						TextBoxSetText(taccf[1],"%d",Layout.accent2_flag);
						TextBoxSetText(taccf[2],"%d",Layout.accent3_flag);
						TextBoxSetText(taccf[3],"%d",Layout.accent4_flag);
						TextBoxSetText(tname,"%s",get_config_string(NULL,"keyboard_name","unknown"));
						TextBoxSetText(tcode,"????");
					}
					free(file);
				}
				return true;
			}
			break;
			case MSG_SAVEALLEG:
			{
				l_text file = IOBox("Save Allegro CFG file",IOBOX_SAVE,NULL,Filter1,false);
				if ( file ) {
					PFile f = FileOpen(file,"wt");
					if ( f ) {
						FilePrintf(f,"# Generated by oZone keyboard editor. http://ozonegui.net/.\n\n");
						FilePrintf(f,"keyboard_name = %s\n\n",Layout.Name);
						FilePrintf(f,"[key_escape]\naccent1 = %d\naccent2 = %d\naccent3 = %d\naccent4 = %d\naccent1_flag = %d\naccent2_flag = %d\naccent3_flag = %d\naccent4_flag = %d\n\n",
						Layout.accent1,Layout.accent2,Layout.accent3,Layout.accent4,Layout.accent1_flag,Layout.accent2_flag,Layout.accent3_flag,Layout.accent4_flag);

						WriteCfgSection(f,Layout.ascii_table,          standard_key_ascii_table,     "key_ascii");
						WriteCfgSection(f,Layout.capslock_table,       standard_key_capslock_table,  "key_capslock");
						WriteCfgSection(f,Layout.shift_table,          standard_key_shift_table,     "key_shift");
						WriteCfgSection(f,Layout.control_table,        standard_key_control_table,   "key_control");
						WriteCfgSection(f,Layout.altgr_lower_table,    standard_key_empty_table,     "key_altgr");
						WriteCfgSection(f,Layout.altgr_upper_table,    standard_key_empty_table,     "key_altgr");
						WriteCfgSection(f,Layout.altgr_lower_table,    standard_key_empty_table, "key_altgr_lower");
						WriteCfgSection(f,Layout.altgr_upper_table,    standard_key_empty_table, "key_altgr_upper");
						WriteCfgSection(f,Layout.accent1_lower_table,  standard_key_empty_table,     "key_accent1_lower");
						WriteCfgSection(f,Layout.accent1_upper_table,  standard_key_empty_table,     "key_accent1_upper");
						WriteCfgSection(f,Layout.accent2_lower_table,  standard_key_empty_table,     "key_accent2_lower");
						WriteCfgSection(f,Layout.accent2_upper_table,  standard_key_empty_table,     "key_accent2_upper");
						WriteCfgSection(f,Layout.accent3_lower_table,  standard_key_empty_table,     "key_accent3_lower");
						WriteCfgSection(f,Layout.accent3_upper_table,  standard_key_empty_table,     "key_accent3_upper");
						WriteCfgSection(f,Layout.accent4_lower_table,  standard_key_empty_table,     "key_accent4_lower");
						WriteCfgSection(f,Layout.accent4_upper_table,  standard_key_empty_table,     "key_accent4_upper");


						FileClose(f);
					}
					free(file);
				}
				return true;
			}

		}
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_int Main ( int argc, l_text *argv )
{
	PLabel l;
	PButton b;
	TRect r;
	PFileInfo info;
	PKeyboardLayout SysKL =  KeyboardGetLayout ( );

	l_ulong n = 0;

	PTab t, t1;
	PKeybEd Ed;

	Filter = 	NewFileTypes("Ozone keyboard layout file (*.okl)","okl",
						NewFileTypes("All files",NULL,
						NULL ));

	Filter1 = 	NewFileTypes("Allegro keyboard layout file (*.cfg)","cfg",
						NewFileTypes("All files",NULL,
						NULL ));

	if ( SysKL ) {

		memcpy(&Layout,SysKL,sizeof(TKeyboardLayout));

	} else {

		Layout.accent1 = _key_accent1;
		Layout.accent2 = _key_accent2;
		Layout.accent3 = _key_accent3;
		Layout.accent4 = _key_accent4;

		Layout.accent1_flag = _key_accent1_flag;
		Layout.accent2_flag = _key_accent2_flag;
		Layout.accent3_flag = _key_accent3_flag;
		Layout.accent4_flag = _key_accent4_flag;

		memcpy(Layout.ascii_table,				_key_ascii_table,sizeof(Layout.ascii_table));
		memcpy(Layout.capslock_table,			_key_capslock_table,sizeof(Layout.capslock_table));
		memcpy(Layout.shift_table,				_key_shift_table,sizeof(Layout.shift_table));
		memcpy(Layout.control_table,			_key_control_table,sizeof(Layout.control_table));
		memcpy(Layout.altgr_lower_table,	_key_altgr_lower_table,sizeof(Layout.altgr_lower_table));
		memcpy(Layout.altgr_upper_table,	_key_altgr_upper_table,sizeof(Layout.altgr_upper_table));
		memcpy(Layout.accent1_lower_table,_key_accent1_lower_table,sizeof(Layout.accent1_lower_table));
		memcpy(Layout.accent1_upper_table,_key_accent1_upper_table,sizeof(Layout.accent1_upper_table));
		memcpy(Layout.accent2_lower_table,_key_accent2_lower_table,sizeof(Layout.accent2_lower_table));
		memcpy(Layout.accent2_upper_table,_key_accent2_upper_table,sizeof(Layout.accent2_upper_table));
		memcpy(Layout.accent3_lower_table,_key_accent3_lower_table,sizeof(Layout.accent3_lower_table));
		memcpy(Layout.accent3_upper_table,_key_accent3_upper_table,sizeof(Layout.accent3_upper_table));
		memcpy(Layout.accent4_lower_table,_key_accent4_lower_table,sizeof(Layout.accent4_lower_table));
		memcpy(Layout.accent4_upper_table,_key_accent4_upper_table,sizeof(Layout.accent4_upper_table));

	}


	memcpy(&SysLayout,&Layout,sizeof(TKeyboardLayout));

	RectAssign(&r,0, 0, 640, 226);
	w = CreateWindow(&Me, r, "Keyboard editor", WF_NORMAL|WF_CENTERED);
	WIDGET(w)->AppEvHdl = &AppEventHandler;

	InsertWidget(WIDGET(DeskTop), WIDGET(w));

	PMenu Menu = NewMenu(
     NewMenuItem( "File", NULL, NULL, NULL,
     		 NewMenu (
     		 		NewMenuItem( "Open", NULL, MSG_OPENFILE, NULL, NULL,
     		 		NewMenuItem( "Save", NULL, MSG_SAVEFILE, NULL, NULL,
     		 		NewMenuItemSeparator(
     		 		NewMenuItem( "Import from allegro cfg", NULL, MSG_OPENALLEG, NULL, NULL,
     		 		NewMenuItem( "Export to allegro cfg", NULL, MSG_SAVEALLEG, NULL, NULL,
     		 		NewMenuItemSeparator(
     		 		NewMenuItem( "Exit", NULL, WM_CLOSE, NULL, NULL,
     		 		NULL)))))))
     		 ),
     NewMenuItem( "Layout", NULL, NULL, NULL,
     		 NewMenu (
     		 		NewMenuItem( "Test", NULL, MSG_TEST, NULL, NULL,
     		 		NewMenuItem( "Restaure system layout", NULL, MSG_RESTAURE, NULL, NULL,
     		 		NULL))
     		 ),
     NewMenuItem( "Help", NULL, NULL, NULL,
     	NewMenu(
     		NewMenuItem( "About", NULL, WM_ABOUT, NULL, NULL, NULL)),
     NULL)))
	);

	RectAssign(&r,0,0,500,20);

	PMenuView o = NewMenuView(&Me,r,Menu,MenuViewStyleHorizontal,0);
	InsertWidget(WIDGET(w), WIDGET(o));

	RectAssign(&r, 2, 25, 632, 200);
	bk = CreateTabBook(&Me,r);
	InsertWidget(WIDGET(w), WIDGET(bk));




	t1 = t = TabBookAddTab(bk,"Options",NULL);


	RectAssign(&r, 0, 0, 100, 15);
	l = CreateLabel(&Me,r,"Name : ");
 	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l));

	RectAssign(&r, 100, r.a.y, 305, r.b.y);
	tname = CreateTextbox(&Me,r,TBF_EDITABLE);
	InsertWidget(WIDGET(t), WIDGET(tname));

	RectAssign(&r, 0, r.a.y+20, 100, r.b.y+20);
	l = CreateLabel(&Me,r,"Code : ");
 	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l));

	RectAssign(&r, 100, r.a.y, 305, r.b.y);
	tcode = CreateTextbox(&Me,r,TBF_EDITABLE);
	InsertWidget(WIDGET(t), WIDGET(tcode));

	while ( n < 4 ) {

		RectAssign(&r, 0, r.a.y+20, 100, r.b.y+20);
		l = CreateLabel(&Me,r,"Accent");
	 	WIDGET(l)->BackgroundColor = COL_3DFACE;
		InsertWidget(WIDGET(t), WIDGET(l));

		LabelSetText(l,"Accent %d",n+1);

		RectAssign(&r, 100, r.a.y, 200, r.b.y);
		tacc[n] = CreateTextbox(&Me,r,TBF_EDITABLE);
		InsertWidget(WIDGET(t), WIDGET(tacc[n]));

		RectAssign(&r, 205, r.a.y, 305, r.b.y);
		taccf[n] = CreateTextbox(&Me,r,TBF_EDITABLE);
		InsertWidget(WIDGET(t), WIDGET(taccf[n]));

		n++;
	}

	TextBoxSetText(tacc[0],"%d",Layout.accent1);
	TextBoxSetText(tacc[1],"%d",Layout.accent2);
	TextBoxSetText(tacc[2],"%d",Layout.accent3);
	TextBoxSetText(tacc[3],"%d",Layout.accent4);

	TextBoxSetText(taccf[0],"%d",Layout.accent1_flag);
	TextBoxSetText(taccf[1],"%d",Layout.accent2_flag);
	TextBoxSetText(taccf[2],"%d",Layout.accent3_flag);
	TextBoxSetText(taccf[3],"%d",Layout.accent4_flag);

	TextBoxSetText(tname,"%s",Layout.Name);
	TextBoxSetText(tcode,"%s",Layout.Code);

	RectAssign(&r, 0, 0, 625, 200);


	t = TabBookAddTab(bk,"Ascii",NULL);
	Ed = NewKeybEdit(r,Layout.ascii_table);
	InsertWidget(WIDGET(t), WIDGET(Ed));

	t = TabBookAddTab(bk,"Caps",NULL);
	Ed = NewKeybEdit(r,Layout.capslock_table);
	InsertWidget(WIDGET(t), WIDGET(Ed));

	t = TabBookAddTab(bk,"Shift",NULL);
	Ed = NewKeybEdit(r,Layout.shift_table);
	InsertWidget(WIDGET(t), WIDGET(Ed));

	t = TabBookAddTab(bk,"Control",NULL);
	Ed = NewKeybEdit(r,Layout.control_table);
	InsertWidget(WIDGET(t), WIDGET(Ed));

	t = TabBookAddTab(bk,"AltGr",NULL);
	Ed = NewKeybEdit(r,Layout.altgr_lower_table);
	InsertWidget(WIDGET(t), WIDGET(Ed));

	t = TabBookAddTab(bk,"AltGr+Up",NULL);
	Ed = NewKeybEdit(r,Layout.altgr_upper_table);
	InsertWidget(WIDGET(t), WIDGET(Ed));

	t = TabBookAddTab(bk,"Accent1",NULL);
	Ed = NewKeybEdit(r,Layout.accent1_lower_table);
	InsertWidget(WIDGET(t), WIDGET(Ed));

	t = TabBookAddTab(bk,"Accent1+Up",NULL);
	Ed = NewKeybEdit(r,Layout.accent1_upper_table);
	InsertWidget(WIDGET(t), WIDGET(Ed));

	t = TabBookAddTab(bk,"Accent2",NULL);
	Ed = NewKeybEdit(r,Layout.accent2_lower_table);
	InsertWidget(WIDGET(t), WIDGET(Ed));

	t = TabBookAddTab(bk,"Accent2+Up",NULL);
	Ed = NewKeybEdit(r,Layout.accent2_upper_table);
	InsertWidget(WIDGET(t), WIDGET(Ed));

	t = TabBookAddTab(bk,"Accent3",NULL);
	Ed = NewKeybEdit(r,Layout.accent3_lower_table);
	InsertWidget(WIDGET(t), WIDGET(Ed));

	t = TabBookAddTab(bk,"Accent3+Up",NULL);
	Ed = NewKeybEdit(r,Layout.accent3_upper_table);
	InsertWidget(WIDGET(t), WIDGET(Ed));

	t = TabBookAddTab(bk,"Accent4",NULL);
	Ed = NewKeybEdit(r,Layout.accent4_lower_table);
	InsertWidget(WIDGET(t), WIDGET(Ed));

	t = TabBookAddTab(bk,"Accent4+Up",NULL);
	Ed = NewKeybEdit(r,Layout.accent4_upper_table);
	InsertWidget(WIDGET(t), WIDGET(Ed));

	WidgetDrawAll(WIDGET(w));
	//TabBookSelectTab(bk,t1);

	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{

	FreeFileTypes(Filter);
	FreeFileTypes(Filter1);

}
////////////////////////////////////////////////////////////////////////////////
