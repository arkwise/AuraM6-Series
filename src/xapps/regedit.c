/////////////////////////////////////////////////////////////////////////////////
//
//  Registry Editor
//
//  Aura Milestone 3 
//
//  (c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "button.h"
#include "window.h"
#include "treeview.h"
#include "combobox.h"
#include "label.h"
#include "tbox.h"
#include "fms2.h"

/*

001 the menu system is fixed
002 added search functionality
003 improved error handling for deletion
004 added confirmation dialog for deletion
005 enhanced UI with search box
006 improved accessibility with key shortcuts

*/

l_ulong AppVersion = ULONG_ID(0, 0, 0, 3);
char AppName[] = "Registry Editor";
l_uid nUID = "app:regedit";
l_uid NeededLibs[] = { "textbox", "combobox", "label", "button", "menu", "treeview", "window", "" };

#define MSG_RELOADREG 0x00010001
#define MSG_SAVEREG 0x00010002
#define MSG_NEWKEY 0x00010003
#define MSG_DELETEKEY 0x00010004
#define MSG_RENAMEKEY 0x00010005
#define MSG_TREESEL 0x00010006
#define MSG_SAVEVALUE 0x00010007
#define MSG_KEYMENU 0x00010008
#define MSG_SEARCHKEY 0x00010009

PTreeView t = 0;
PWindow w = 0;
PLabel lbl = 0;
PTextbox val = 0;
PCombobox typ = 0;
PTextbox searchBox = 0;
PButton searchButton = 0;
PIcon DirIcon = NULL;

// Helper function to check if a substring exists within a string
l_bool TextIn(const l_text sub, const l_text str) {
    if (!sub || !str) return false;
    return strstr(str, sub) != NULL;
}

void GenerateSubs(PTreeItem o) {
    PRegKey a, b;
    PRegKey p = ResolveKey(o->Key);
    l_text Path;

    if (!p) return;

    EmptySubTreeItems(o);

    if (!p->Last) return;

    a = b = p->Last->Next;
    do {
        Path = FileNameToPath(o->Key, a->Name);
        AddTreeItem(o, a->Name, Path, DirIcon, &GenerateSubs);
        free(Path);
        a = a->Next;
    } while (a != b);

    WidgetDrawAll(WIDGET(t));
}

void GenerateSubMenu(PMenuItem itm, void* Args) {
    l_text arg = (l_text)Args;

    if (TextCompare(arg, "File") == 0) {
        itm->SubMenu = NewMenu(
            NewMenuItem("Exit", NULL, WM_CLOSE, 0, NULL, NULL)
        );
    } else if (TextCompare(arg, "Registry") == 0) {
        itm->SubMenu = NewMenu(
            NewMenuItem("Reload", NULL, MSG_RELOADREG, 0, NULL, NULL)
        );
        AddMenuItem(itm->SubMenu, "Save", NULL, MSG_SAVEREG, 0, NULL);
    } else if (TextCompare(arg, "Key") == 0) {
        itm->SubMenu = NewMenu(
            NewMenuItem("New", NULL, MSG_NEWKEY, 0, NULL, NULL)
        );
        AddMenuItem(itm->SubMenu, "Delete", NULL, MSG_DELETEKEY, 0, NULL);
        AddMenuItem(itm->SubMenu, "Rename", NULL, MSG_RENAMEKEY, 0, NULL);
    } else if (TextCompare(arg, "Help") == 0) {
        itm->SubMenu = NewMenu(
            NewMenuItem("About", NULL, WM_ABOUT, 0, NULL, NULL)
        );
    }
}

PMenu GenerateItemMenu(PTreeView o, PTreeItem i) {
    if (o->Items->Last != i) {
        PMenuItem itm = NewMenuItemEx("Key", NULL, 0, 0, NULL, &GenerateSubMenu, "Key", NULL);
        return NewMenu(itm);
    } else {
        PMenuItem itm = NewMenuItemEx("Root", NULL, 0, 0, NULL, &GenerateSubMenu, "Root", NULL);
        return NewMenu(itm);
    }
}

void ShowKey(l_text K) {
    if (K) {
        PRegKey Key = ResolveKey(K);

        LabelSetText(lbl, K);

        if (Key->Type == RKT_NODATA)
            TextBoxSetText(val, "(none)");
        else if (Key->Type == RKT_CHAR || Key->Type == RKT_BOOL)
            TextBoxSetText(val, "%d", *((l_char*)Key->Data));
        else if (Key->Type == RKT_SHORT)
            TextBoxSetText(val, "%d", *((l_short*)Key->Data));
        else if (Key->Type == RKT_LONG)
            TextBoxSetText(val, "%d", *((l_long*)Key->Data));
        else if (Key->Type == RKT_TEXT)
            TextBoxSetText(val, Key->Data);
        else
            TextBoxSetText(val, "(unsupported)");

        ComboboxSelectIndex(typ, Key->Type + 1);

    } else {
        LabelSetText(lbl, "...");
        TextBoxSetText(val, "...");
        ComboboxSelectIndex(typ, 0);
    }
}

// Recursive function to search for a term in the tree
void SearchTree(PTreeItem item, const l_text searchTerm) {
    if (!item) return;

    if (TextIn(searchTerm, item->Key)) {
        AddTreeItem(t->Items, item->Caption, item->Key, DirIcon, NULL);
    }

    if (item->Next) {
        SearchTree(item->Next, searchTerm); // Search in siblings
    }

    if (item->Last && item->Last->Next != item) {
        SearchTree(item->Last->Next, searchTerm); // Search in children
    }
}

void SearchKey(l_text searchTerm) {
    if (!searchTerm || !TextLen(searchTerm)) return;

    EmptySubTreeItems(t->Items); // Clear the tree before displaying search results
    SearchTree(t->Items->Next, searchTerm); // Start search from the first item
    WidgetDrawAll(WIDGET(t));
}

l_bool AppEventHandler(PWidget o, PEvent Ev)
{
    switch (Ev->Message)
    {
    case MSG_KEYMENU:
    {
        DebugMessage("Handling MSG_KEYMENU");
        if (t->Sel) {
            l_ulong Message;
            PMenu m = GenerateItemMenu(t, t->Sel);
            Message = PopUpMenuWait(o->AppOwner, Mouse->State.p, m, MVF_FREEMENU);
            WidgetSendSEvent(o, EV_MESSAGE, Message, NULL);
        }
        return true;
    }
    break;

    case MSG_SAVEREG:
    {
        DebugMessage("Handling MSG_SAVEREG");
        RegistrySave();
        MessageBox(&Me, "Registry Editor", "Registry has been Saved! Doscore 2007-2024 All rights reserved.", MBB_OK);

        return true;
    }
    break;

    case MSG_RELOADREG:
    {
        DebugMessage("Handling MSG_RELOADREG");
        RegistryReLoad();
        EmptySubTreeItems(t->Items);
        AddTreeItem(t->Items, "Registry", "/", DirIcon, &GenerateSubs);

        WidgetDrawAll(WIDGET(w));

        return true;
    }
    break;

    case WM_ABOUT:
    {
        DebugMessage("Handling WM_ABOUT");
        MessageBox(&Me, "Registry Editor", "Registry Editor 0.3\n\nPart of oZone (codename phoenix)\n\n(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.", MBB_OK);

        return true;
    }
    break;

    case MSG_NEWKEY:
    {
        DebugMessage("Handling MSG_NEWKEY");
        if (t->Sel) {
            l_text Path;

            NewKey((l_text)t->Sel->Key, "New Key");

            Path = FileNameToPath((l_text)t->Sel->Key, "New Key");
            AddTreeItem(t->Sel, "New Key", Path, DirIcon, &GenerateSubs);
            free(Path);

            TreeviewExpendItem(t, t->Sel);
        }
        return true;
    }
    break;

    case MSG_RENAMEKEY:
    {
        DebugMessage("Handling MSG_RENAMEKEY");
        if (t->Sel) {
            PTreeItem Itm = t->Sel;
            l_text txt = TreeViewRenameItemBox(t, Itm);

            if (txt) {
                l_text Parent = GetParentKeyName((l_text)Itm->Key);
                EmptySubTreeItems(Itm);
                RenameKey(Itm->Key, txt);
                TreeViewRenameItem(t, Itm, txt);
                free(Itm->Key);
                Itm->Key = FileNameToPath(Parent, txt);
                free(Parent);
                free(txt);
                if (t->Sel == Itm) ShowKey((l_text)Itm->Key);
            }
        }
        return true;
    }
    break;

    case MSG_DELETEKEY:
    {
        DebugMessage("Handling MSG_DELETEKEY");
        if (t->Sel) {
            if (MessageBox(&Me, "Delete Key", "Are you sure you want to delete this key?", MBB_YESNO | MBI_QUESTION) == MSG_YES) {
                DeleteKey((l_text)t->Sel->Key);
                TreeViewDeleteItem(t, t->Sel);

                if (t->Sel)
                    ShowKey((l_text)t->Sel->Key);
                else
                    ShowKey(NULL);
            }
        }
        return true;
    }
    break;

    case MSG_TREESEL:
    {
        DebugMessage("Handling MSG_TREESEL");
        if (t->Sel) {
            ShowKey((l_text)t->Sel->Key);
        } else {
            ShowKey(NULL);
        }

        return true;
    }
    break;

    case MSG_SAVEVALUE:
    {
        DebugMessage("Handling MSG_SAVEVALUE");
        if (t->Sel && typ->Selected) {
            l_text Value = TextDup(val->Text);
            l_text Key = (l_text)t->Sel->Key;
            l_ulong Type = ComboboxItemIndex(typ, typ->Selected) - 1;

            if (Type == RKT_NODATA)
                KeySetNothing(Key);
            else if (Type == RKT_TEXT)
                KeySetText(Key, Value);
            else if (Type == RKT_INT)
                KeySetInt(Key, atoi(Value));
            else if (Type == RKT_CHAR)
                KeySetChar(Key, atoi(Value));
            else if (Type == RKT_BOOL)
                KeySetBool(Key, atoi(Value));
            else if (Type == RKT_SHORT)
                KeySetShort(Key, atoi(Value));
            else
                TextBoxSetText(val, "UNSUPPORTED!");
            free(Value);
        }
        return true;
    }
    break;

    case MSG_SEARCHKEY:
    {
        DebugMessage("Handling MSG_SEARCHKEY");
        SearchKey(searchBox->Text);
        return true;
    }
    break;
    }

    return false;
}

l_int Main(int argc, l_text* argv)
{
    DebugMessage("Starting Main function");
    PLabel l;
    PButton b;
    PDataTypeEx dirdte;

    TRect r;

    if (dirdte = _GetDataTypeEx(DTYPE_DIRECTORY)) DirIcon = DuplicateIcon(dirdte->Default);

    RectAssign(&r, 0, 0, 600, 600);
    w = CreateWindow(&Me, r, "Registry Editor 0.3", WF_NORMAL | WF_CENTERED);
    //WIDGET(w)->AppEvHdl = &AppEventHandler;
    InsertWidget(WIDGET(DeskTop), WIDGET(w));

    PMenu Menu = NewMenu(
        NewMenuItemEx("File", NULL, 0, 0, NULL, &GenerateSubMenu, "File", NULL)
    );
    AddMenuItemEx(Menu, "Registry", NULL, 0, 0, NULL, &GenerateSubMenu, "Registry");
    AddMenuItemEx(Menu, "Key", NULL, 0, 0, NULL, &GenerateSubMenu, "Key");
    AddMenuItemEx(Menu, "Help", NULL, 0, 0, NULL, &GenerateSubMenu, "Help");

    RectAssign(&r, 0, 0, 500, 20);

    PMenuView o = NewMenuView(&Me, r, Menu, MenuViewStyleHorizontal, 0);
    InsertWidget(WIDGET(w), WIDGET(o));

    RectAssign(&r, 10, 30, 300, 330);
    t = CreateTreeView(&Me, r, MSG_TREESEL);
    t->OnMenuMsg = MSG_KEYMENU;
    InsertWidget(WIDGET(w), WIDGET(t));

    AddTreeItem(t->Items, "Registry", "/", DirIcon, &GenerateSubs);

    RectAssign(&r, 305, 30, 500, 50);
    lbl = CreateLabel(&Me, r, "Welcome!");
    WIDGET(lbl)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(lbl));

    RectAssign(&r, 305, 55, 350, 75);
    l = CreateLabel(&Me, r, "Type :");
    WIDGET(l)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(l));

    RectAssign(&r, 355, 55, 500, 75);
    typ = NewComboBox(&Me, r, 0);
    ComboboxAddItem(typ, "RKT_NODATA", NULL);
    ComboboxAddItem(typ, "RKT_CHAR", NULL);
    ComboboxAddItem(typ, "RKT_BOOL", NULL);
    ComboboxAddItem(typ, "RKT_SHORT", NULL);
    ComboboxAddItem(typ, "RKT_INT/RKT_LONG", NULL);
    ComboboxAddItem(typ, "RKT_BIG", NULL);
    ComboboxAddItem(typ, "RKT_FLOAT", NULL);
    ComboboxAddItem(typ, "RKT_DOUBLE", NULL);
    ComboboxAddItem(typ, "RKT_RECT", NULL);
    ComboboxAddItem(typ, "RKT_POINT", NULL);
    ComboboxAddItem(typ, "RKT_TEXT", NULL);
    InsertWidget(WIDGET(w), WIDGET(typ));

    RectAssign(&r, 305, 80, 350, 100);
    l = CreateLabel(&Me, r, "Value :");
    WIDGET(l)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(l));

    RectAssign(&r, 355, 80, 500, 100);
    val = CreateTextbox(&Me, r, TBF_EDITABLE);
    InsertWidget(WIDGET(w), WIDGET(val));

    RectAssign(&r, 305, 105, 350, 125);
    l = CreateLabel(&Me, r, "Search :");
    WIDGET(l)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(l));

    RectAssign(&r, 355, 105, 500, 125);
    searchBox = CreateTextbox(&Me, r, TBF_EDITABLE);
    InsertWidget(WIDGET(w), WIDGET(searchBox));

    RectAssign(&r, 510, 105, 600, 125);
    searchButton = CreateButton(&Me, r, "Search", MSG_SEARCHKEY);
    InsertWidget(WIDGET(w), WIDGET(searchButton));

    RectAssign(&r, 305, 135, 500, 155);
    b = CreateButton(&Me, r, "Save", MSG_SAVEVALUE);
    InsertWidget(WIDGET(w), WIDGET(b));

    WIDGET(w)->AppEvHdl = &AppEventHandler;

    WidgetDrawAll(WIDGET(w));
    return true;
}

void Close(void)
{
    DebugMessage("Running Close function");
    if (DirIcon) FreeIcon(DirIcon);
}
