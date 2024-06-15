////////////////////////////////////////////////////////////////////////////////
//
//	File Properties 0.01
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "button.h"
#include "window.h"
#include "label.h"
#include "fms2.h"
#include "checkbox.h"
#include "tabbook.h"
#include "tbox.h"
#include "iodlg.h"
l_ulong AppVersion	= ULONG_ID(0,0,0,1);
char AppName[]		= "File Properties";
l_uid	nUID = "app:fprop";
l_uid NeededLibs[] = { "label","checkbox","button","window","tabbook","textbox","iodlg","" };

l_text file = 0;
	PWindow w	= 0;
l_ulong Mode = 0;

PLabel l1;
PLabel l2;
PLabel l3;
PLabel l4;
PLabel l5;
PLabel l6;
PLabel l7;

PImageLabel icon;

PTextbox tl;

PCheckbox c1;
PCheckbox c2;
PCheckbox c3;
PCheckbox c4;

#define MSG_F_OK 				0x00010001
#define MSG_SELECTFILE 	0x00010002



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
			case MSG_SELECTFILE:
			{
				l_text lfile = IOBox("Select linked file", IOBOX_OPEN, NULL, NULL, true);

				if ( lfile ) {
					TextBoxSetTextEx(tl,lfile);
					free(lfile);
				}
			}
			break;
			case MSG_F_OK:
			{
				l_uchar Attr = 0;

				if ( c1->Checked ) Attr |= FA_RDONLY;
				if ( c2->Checked ) Attr |= FA_HIDDEN;
				if ( c3->Checked ) Attr |= FA_ARCH;
				if ( c4->Checked ) Attr |= FA_SYSTEM;

				if ( file ) {
					FileSetAttributes(file,Attr);
					if ( Mode == 1 ) SaveData2(file,tl->Text,TYPE_TEXT);
				}

				WidgetDispose(WIDGET(o));
				CloseApp(&Me);
				return true;
			}
			break;
			case WM_ABOUT:
			{
				MessageBox(&Me, "File properties", "File properties part of FMS 2\n\n(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.", MBB_OK);
				return true;
			}
			break;

		}
	}

	return false;
}

l_text format_size (l_ulong size)
{
	if (size < (l_ulong)1024)
		return TextArgs ("%d bytes", (l_ulong)size);
	else if (size < (l_ulong)1024*1024)
		return TextArgs ("%d KB", size / (1024));
	else if (size < (l_ulong)1024*1024*1024)
		return TextArgs ("%d MB", size / (1024*1024));
	else
		return TextArgs ("%d GB", size / (1024*1024*1024));
}

l_int Main ( int argc, l_text *argv )
{
	PLabel l;
	PButton b;
	TRect r;
	PFileInfo info;
	PTabBook bk;
	PTab t;
	PCodec c;
	
	struct tm *tme;

	if ( argc == 1 ) return false;
	
	file = TextDup(argv[1]);
	if ( TextRChr(argv[1],'.') )
	if ( !TextCompare(TextRChr(argv[1],'.'),".ln") )
	Mode = 1;
	info = FileGetInfo(file);
	if ( !info ) {
		free(file);
		MessageBox(&Me, "Error", "Unable to load file propreties.", MBB_OK|MBI_ERROR);
		return false;
	}

	c = GetCodecFileInfo(info);
	
	
	RectAssign(&r,0, 0, 285, 375);
	w = CreateWindow(&Me, r, "Properties", WF_NORMAL|WF_CENTERED);
	WIDGET(w)->AppEvHdl = &AppEventHandler;

	InsertWidget(WIDGET(DeskTop), WIDGET(w));

	RectAssign(&r, 5, 5, 45, 45);
	icon = CreateImageLabel(&Me,r,FileGetIcon2(info,32,NULL));
	WIDGET(icon)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(icon));

	RectAssign(&r, 50, 0, 250, 20);
	l1 = CreateLabel(&Me,r,"");
	WIDGET(l1)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l1));

	RectAssign(&r, 50, 20, 250, 40);
	l2 = CreateLabel(&Me,r,"");
	WIDGET(l2)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l2));


	RectAssign(&r, 5, 350, 95, 370);
	b = CreateButton(&Me,r,"Ok",MSG_F_OK);
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r, 100, 350, 190, 370);
	b = CreateButton(&Me,r,"Cancel",WM_CLOSE);
	InsertWidget(WIDGET(w), WIDGET(b));


	RectAssign(&r, 5, 50, 280, 345);
	bk = CreateTabBook(&Me,r);
	InsertWidget(WIDGET(w), WIDGET(bk));

	t = TabBookAddTab(bk,"Informations",NULL);

	RectAssign(&r, 0, 0, 250, 15);
	l3 = CreateLabel(&Me,r,"");
	WIDGET(l3)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l3));
	
	RectAssign(&r, 0, r.a.y+20, 250, r.b.y+20);
	l7 = CreateLabel(&Me,r,"File type : Unknown binary");
	WIDGET(l7)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l7));
	
	RectAssign(&r, 0, r.a.y+20, 250, r.b.y+20);
	l4 = CreateLabel(&Me,r,"Created  : (unavaible)");
	WIDGET(l4)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l4));

	RectAssign(&r, 0, r.a.y+20, 250, r.b.y+20);
	l5 = CreateLabel(&Me,r,"Modified : (unavaible)");
	WIDGET(l5)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l5));

	RectAssign(&r, 0, r.a.y+20, 250, r.b.y+20);
	l6 = CreateLabel(&Me,r,"Accessed : (unavaible)");
	WIDGET(l6)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l6));

	RectAssign(&r, 0, r.a.y+20, 120, r.b.y+20);
	c1 = CreateCheckbox(&Me,r,"Read only");
	InsertWidget(WIDGET(t), WIDGET(c1));

	RectAssign(&r, 125, r.a.y, 250, r.b.y);
	c2 = CreateCheckbox(&Me,r,"Hidden");
	InsertWidget(WIDGET(t), WIDGET(c2));

	RectAssign(&r, 0, r.a.y+20, 120, r.b.y+20);
	c3 = CreateCheckbox(&Me,r,"Archive");
	InsertWidget(WIDGET(t), WIDGET(c3));

	RectAssign(&r, 125, r.a.y, 250, r.b.y);
	c4 = CreateCheckbox(&Me,r,"System");
	InsertWidget(WIDGET(t), WIDGET(c4));

	if ( Mode ) {

		t = TabBookAddTab(bk,"Link",NULL);

		RectAssign(&r, 0, 0, 250, 15);
		l = CreateLabel(&Me,r,"Linked file");
		WIDGET(l)->BackgroundColor = COL_3DFACE;
		InsertWidget(WIDGET(t), WIDGET(l));

		RectAssign(&r, 0, r.a.y+20, 225, r.b.y+20);
		tl = CreateTextbox(&Me,r,0);
		InsertWidget(WIDGET(t), WIDGET(tl));

		RectAssign(&r, 230, r.a.y, 250, r.b.y);
		b = CreateButton(&Me,r,"...",MSG_SELECTFILE);
		InsertWidget(WIDGET(t), WIDGET(b));

	}

	c1->Checked = info->Attributes & FA_RDONLY;
	c2->Checked = info->Attributes & FA_HIDDEN;
	c3->Checked = info->Attributes & FA_ARCH;
	c4->Checked = info->Attributes & FA_SYSTEM;

	WidgetDrawAll(WIDGET(w));
	TabBookSelectTab(bk,t);

	if ( Mode == 1 ) {
		l_text link = LoadData2(file,TYPE_TEXT);
		TextBoxSetTextEx(tl,link);
		free(link);
	}

	LabelSetText(l1,"%s",info->Name);
	LabelSetText(l2,"%s",info->VPathName);
	LabelSetText(l3,"Size: %s", format_size(info->Size));

	tme = localtime((const time_t *)&info->CTime);
	if ( tme )
	LabelSetText(l4,"Created: %02d/%02d/%04d %02d:%02d:%02d",tme->tm_mday,tme->tm_mon+1,tme->tm_year+1900,tme->tm_hour,tme->tm_min,tme->tm_sec);
	tme = localtime((const time_t *)&info->MTime);
	if ( tme )
	LabelSetText(l5,"Modified: %02d/%02d/%04d %02d:%02d:%02d",tme->tm_mday,tme->tm_mon+1,tme->tm_year+1900,tme->tm_hour,tme->tm_min,tme->tm_sec);
	tme = localtime((const time_t *)&info->ATime);
	if ( tme )
	LabelSetText(l6,"Accessed: %02d/%02d/%04d %02d:%02d:%02d",tme->tm_mday,tme->tm_mon+1,tme->tm_year+1900,tme->tm_hour,tme->tm_min,tme->tm_sec);

	if ( c ) {
		if ( c->Name ) 
			LabelSetText(l7,"File type : %s",c->Name);
		else
			LabelSetText(l7,"File type : Id 0x%08x",c->CodecId);
	}
	
	FreeFileInfo(info);
	return true;
}

void Close (void)
{
	free(file);
}
