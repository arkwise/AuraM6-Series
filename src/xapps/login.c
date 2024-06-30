////////////////////////////////////////////////////////////////////////////////
//
//	Login
//
//	(c) Copyright 2010 Finn Technologies and Chase Finn. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "button.h"
#include "window.h"
#include "fms2.h"
#include "tbox.h"
#include "treeview.h"
#include "listview.h"
////////////////////////////////////////////////////////////////////////////////
l_ulong AppVersion = ULONG_ID(0, 0, 1, 0);
char AppName[] = "Login";
l_uid nUID = "app:login";
l_uid NeededLibs[] = { "menu", "window", "fms2", "textbox", "treeview", "widget", "listview", "" };
////////////////////////////////////////////////////////////////////////////////
PWindow w = 0;
PTreeview t;
PListview s;
PTextbox ext;
PTextbox sext;
PIcon UserIcon = NULL;

l_text x;
l_text Value;
long Userkey = 0;
l_text y;
PRegKey thekey;

#define MSG_SELECTFILE 0x00010002
#define MSG_LOGIN 0x00010003
#define MSG_TREESEL 0x00010004

////////////////////////////////////////////////////////////////////////////////
l_bool AppEventHandler(PWidget o, PEvent Event)
{
	if (Event->Type == EV_MESSAGE)
	{
		switch (Event->Message)
		{
		case WM_CLOSE:
		{
			MessageBox(&Me, "Error", "You must be logged in to do that.", MBB_OK);
			return true;
		}
		break;

		case WM_ABOUT:
		{
			MessageBox(&Me, "About 'Login'", "Copyright (c) 2010 Finn Technologies, Chase Finn. All rights reserved.", MBB_OK);
			return true;
		}
		break;

		case MSG_TREESEL:
		{
			if (t->Sel) {
				PRegKey thekey = ResolveKey((l_text)t->Sel->Key);
				if (thekey->Type == RKT_NODATA)
				{
					Userkey = 1;
				}
				else
				{
					Value = thekey->Data;
					TextBoxSetText(sext, Value);
					Userkey = 2;
				}
			}
			else {
				Userkey = 0;
			}
			return true;
		}
		break;

		case MSG_LOGIN:
		{
			if (Userkey == 0)
			{
				MessageBox(&Me, "Select Username", "You must select a username from the list", MBB_OK);
			}
			else {
				x = ext->Text;
				y = sext->Text;
				if (TextCompare(x, y) == 0) {
					MessageBox(&Me, "YAY", "Everything Checks Out!", MBB_OK);
				}
				else {
					MessageBox(&Me, "Invalid Password", "You have entered an incorrect password for the selected user", MBB_OK);
				}
			}
			return true;
		}
		break;
		}
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////
void GenerateSubs()
{
	PRegKey a, b;
	PRegKey p = ResolveKey("/USERS/");
	l_text Path;

	if (!p) return;

	EmptySubTreeItems(t->Items);

	if (!p->Last) return;

	a = b = p->Last->Next;
	do {
		Path = FileNameToPath("/USERS/", a->Name);
		AddTreeItem(t->Items, a->Name, Path, UserIcon, NULL);
		free(Path);
		a = a->Next;
	} while (a != b);

	WidgetDrawAll(WIDGET(t));
}

////////////////////////////////////////////////////////////////////////////////
void GenerateSubMenu(PMenuItem itm, void* Args)
{
	l_text arg = (l_text)Args;

	if (TextCompare(arg, "File") == 0) {
		itm->SubMenu = NewMenu(
			NewMenuItem("Exit", NULL, WM_CLOSE, 0, NULL, NULL)
		);
	}
	else if (TextCompare(arg, "Help") == 0) {
		itm->SubMenu = NewMenu(
			NewMenuItem("About", NULL, WM_ABOUT, 0, NULL, NULL)
		);
	}
}

l_int Main(int argc, l_text* argv)
{
	TRect r, wr;
	PButton b;
	PLabel l;

	//UserIcon = NewIcon(LoadImage("SYSTEM/ICONS/user.bmp"), NULL, NULL);

	RectAssign(&wr, 0, 0, 300, 700);
	w = CreateWindow(&Me, wr, "Login", WF_NORMAL | WF_CENTERED);
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	InsertWidget(WIDGET(DeskTop), WIDGET(w));

	PMenu Menu = NewMenu(
		NewMenuItemEx("File", NULL, 0, 0, NULL, &GenerateSubMenu, "File", NULL)
	);
	AddMenuItemEx(Menu, "Help", NULL, 0, 0, NULL, &GenerateSubMenu, "Help");

	RectAssign(&r, 0, 0, 300, 20);
	PMenuView mv = NewMenuView(&Me, r, Menu, MenuViewStyleHorizontal, 0);
	InsertWidget(WIDGET(w), WIDGET(mv));

	RectAssign(&r, 5, 25, 295, 35);
	l = CreateLabel(&Me, r, "Username");
	InsertWidget(WIDGET(w), WIDGET(l));

	RectAssign(&r, 10, 40, 290, 365);
	t = CreateTreeView(&Me, r, MSG_TREESEL);
	InsertWidget(WIDGET(w), WIDGET(t));

	AddTreeItem(t->Items, "Users", "/USERS/", UserIcon, &GenerateSubs);

	GenerateSubs();

	RectAssign(&r, 5, 370, 295, 380);
	l = CreateLabel(&Me, r, "Password");
	InsertWidget(WIDGET(w), WIDGET(l));

	RectAssign(&r, 10, 385, 290, 395);
	ext = CreateTextbox(&Me, r, TBF_EDITABLE);
	InsertWidget(WIDGET(w), WIDGET(ext));

	RectAssign(&r, 10, 400, 290, 410);
	sext = CreateTextbox(&Me, r, NULL);
	InsertWidget(WIDGET(w), WIDGET(sext));

	RectAssign(&r, 150, 415, 250, 435);
	b = CreateButton(&Me, r, "Login", MSG_LOGIN);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetDrawAll(WIDGET(w));

	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close(void)
{
	WidgetDispose(WIDGET(w));
}
////////////////////////////////////////////////////////////////////////////////
