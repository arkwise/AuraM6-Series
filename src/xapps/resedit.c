/////////////////////////////////////////////////////////////////////////////////
//
//  Resource Editor
//
//  Aura Milestone 6 
//
//  (c) Doscore Aura GUI. All rights reserved.
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
#include "iodlg.h"

/*
001 fixed menu generation errors




*/



l_ulong AppVersion = ULONG_ID(0,0,0,1);
char AppName[] = "Resource file editor";
l_uid nUID = "app:resedit";
l_uid NeededLibs[] = { "textbox","combobox","label","button","menu","treeview","window","iodlg","" };

#define MSG_NEWRES    0x00010100
#define MSG_LOADFILE  0x00010101
#define MSG_SAVEFILE  0x00010102

#define MSG_RENAMEITM 0x00010103
#define MSG_REMOVEITM 0x00010104
#define MSG_ADDIMGFRM 0x00010105
#define MSG_ADDOIWFRM 0x00010108

#define MSG_SELITEM   0x00010106

#define MSG_SAVEASFILE 0x00010107

PTreeView t = 0;
PImageLabel ImgLbl = 0;

PList ActualRes = NULL;
PWindow w = NULL;

PFileTypes Filter1 = 0;
PFileTypes Filter2 = 0;
PFileTypes Filter3 = 0;

l_text File = NULL;

////////////////////////////////////////////////////////////////////////////////
l_bool AppEventHandler(PWidget o, PEvent Ev)
{
    switch (Ev->Message)
    {
        case WM_CLOSE:
        {
            ImgLbl->Ico = 0;
            FreeList(ActualRes);
            ActualRes = NULL;
            WidgetDispose(WIDGET(o));    
            CloseApp(&Me);
            return true;
        }
        break;

        case WM_ABOUT:
        {
            MessageBox(&Me, "Resource Editor", "Resource File Editor 0.1\n\nPart of oZone (codename phoenix)\n\nCopyright (c) 2003 Point Mad, Lukas Lipka. All rights reserved.", MBB_OK | MBI_INFORMATION);
            return true;
        }
        break;
        case MSG_NEWRES:
        {
            ImgLbl->Ico = 0;
            if (ActualRes) FreeList(ActualRes);
            ActualRes = NewList();
            EmptySubTreeItems(t->Items);
            WidgetDraw(WIDGET(t), NULL);
            WindowSetCaption(w, "Resource file editor - New");
            if (File) free(File);
            File = NULL;
            return true;
        }
        break;
        case MSG_LOADFILE:
        {
            l_text file = IOBox("Open resource file", IOBOX_OPEN, NULL, Filter1, true);
            if (file) {
                PListItem a, b;
                ImgLbl->Ico = 0;
                if (ActualRes) FreeList(ActualRes);
                EmptySubTreeItems(t->Items);

                ActualRes = LoadResourceFile(file);
                if (ActualRes && ActualRes->Last) {
                    a = b = ActualRes->Last->Next;
                    do {
                        AddTreeItem(t->Items, a->Key, NULL, NULL, NULL);
                        a = a->Next;
                    } while (a != b);
                }
                TreeviewScrollBarRecalculate(t);
                WindowSetCaption(w, "Resource file editor - %s", file);
                WidgetDraw(WIDGET(t), NULL);
                ScrollBarRedraw(SLLWDG(t));
                if (File) free(File);
                File = TextDup(file);
                free(file);
            }
            return true;
        }
        break;
        case MSG_SAVEFILE:
        {
            if (ActualRes) {
                if (File) {
                    SaveResourceFile(File, ActualRes);
                    return true;
                }
            }
        }
        break;
        case MSG_SAVEASFILE:
        {
            if (ActualRes) {
                l_text file = IOBox("Save resource file", IOBOX_SAVE, NULL, Filter1, false);
                if (file) {
                    SaveResourceFile(file, ActualRes);
                    WindowSetCaption(w, "Resource file editor - %s", file);
                    if (File) free(File);
                    File = TextDup(file);
                    free(file);
                }
            }
            return true;
        }
        break;
        case MSG_RENAMEITM:
        {
            if (t->Sel && ActualRes) {
                PTreeItem Itm = t->Sel;
                l_text txt = TreeViewRenameItemBox(t, Itm);
                if (txt) {
                    PListItem i = ListKeyItem(ActualRes, Itm->Caption);
                    free(i->Key);
                    i->Key = TextDup(txt);
                    TreeViewRenameItem(t, Itm, txt);
                    WidgetDraw(WIDGET(t), NULL);
                }
            }
            return true;
        }
        break;
        case MSG_REMOVEITM:
        {
            if (t->Sel && ActualRes) {
                PListItem i = ListKeyItem(ActualRes, t->Sel->Caption);
                ImgLbl->Ico = 0;
                WidgetDraw(WIDGET(ImgLbl), NULL);
                ListRemoveItem(ActualRes, i);
                TreeViewDeleteItem(t, t->Sel);
                WidgetDraw(WIDGET(t), NULL);
            }
            return true;
        }
        break;
        case MSG_ADDIMGFRM:
        {
            if (ActualRes) {
                l_text file = IOBox("Add image from file", IOBOX_OPEN, NULL, Filter2, true);
                if (file) {
                    PListItem i; 
                    l_text name, a, b;
                    a = TextRChr(file, '/');
                    b = TextRChr(file, '.');

                    name = a ? a + 1 : file;

                    if (b)
                        name = TextNDup(name, b - name);
                    else
                        name = TextDup(name);

                    TextToUpper(name);

                    i = NewListItemEx(name, LoadData2(file, TYPE_IMAGE), (void*)&destroy_bitmap, TYPE_IMAGE, 0);
                    ListAddItem(ActualRes, i);
                    AddTreeItem(t->Items, name, NULL, NULL, NULL);
                    TreeviewScrollBarRecalculate(t);
                    WidgetDraw(WIDGET(t), NULL);
                    ScrollBarRedraw(SLLWDG(t));
                    free(name);
                    free(file);
                }
            }
            return true;
        }
        break;
        case MSG_ADDOIWFRM:
        {
            if (ActualRes) {
                l_text file = IOBox("Add iWidget from file", IOBOX_OPEN, NULL, Filter3, true);
                if (file) {
                    PListItem i; 
                    l_text name, a, b;
                    a = TextRChr(file, '/');
                    b = TextRChr(file, '.');

                    name = a ? a + 1 : file;

                    if (b)
                        name = TextNDup(name, b - name);
                    else
                        name = TextDup(name);

                    TextToUpper(name);

                    i = NewListItemEx(name, LoadData2(file, DTYPE_IWIDGET), (void*)&FreeiWidget, DTYPE_IWIDGET, 0);
                    ListAddItem(ActualRes, i);
                    AddTreeItem(t->Items, name, NULL, NULL, NULL);
                    TreeviewScrollBarRecalculate(t);
                    WidgetDraw(WIDGET(t), NULL);
                    ScrollBarRedraw(SLLWDG(t));
                    free(name);
                    free(file);
                }
            }
            return true;
        }
        break;

        case MSG_SELITEM:
        {
            if (t->Sel && ActualRes) {
                PListItem i = ListKeyItem(ActualRes, t->Sel->Caption);
                if (i->DataType == TYPE_IMAGE)
                    ImgLbl->Ico = i->Data;
                else
                    ImgLbl->Ico = 0;
                WidgetDraw(WIDGET(ImgLbl), NULL);
            }
            return true;
        }
    }

    return false;    
}

////////////////////////////////////////////////////////////////////////////////
void GenerateSubMenu(PMenuItem itm, void* Args)
{
    // Convert Args to string
    l_text arg = (l_text)Args;

    if (TextCompare(arg, "File") == 0)
    {
        itm->SubMenu = NewMenu(
            NewMenuItem("New", NULL, MSG_NEWRES, 0, NULL, NULL)
        );
        AddMenuItem(itm->SubMenu, "Open...", NULL, MSG_LOADFILE, 0, NULL);
        AddMenuItem(itm->SubMenu, "Save", NULL, MSG_SAVEFILE, 0, NULL);
        AddMenuItem(itm->SubMenu, "Save as...", NULL, MSG_SAVEASFILE, 0, NULL);
        AddMenuItemSeparator(itm->SubMenu);
        AddMenuItem(itm->SubMenu, "Exit", NULL, WM_CLOSE, 0, NULL);
    }
    else if (TextCompare(arg, "Resource Item") == 0)
    {
        itm->SubMenu = NewMenu(
            NewMenuItem("Rename", NULL, MSG_RENAMEITM, 0, NULL, NULL)
        );
        AddMenuItem(itm->SubMenu, "Remove", NULL, MSG_REMOVEITM, 0, NULL);
    }
    else if (TextCompare(arg, "Add") == 0)
    {
        itm->SubMenu = NewMenu(
            NewMenuItem("Image from file", NULL, MSG_ADDIMGFRM, 0, NULL, NULL)
        );
        AddMenuItem(itm->SubMenu, "iWidget from file", NULL, MSG_ADDOIWFRM, 0, NULL);
    }
    else if (TextCompare(arg, "Help") == 0)
    {
        itm->SubMenu = NewMenu(
            NewMenuItem("About", NULL, WM_ABOUT, 0, NULL, NULL)
        );
    }
}



l_int Main(int argc, l_text *argv)
{
    TRect r; 
    PListItem a, b;
    PMenu Menu = NULL;

    // Define file filters
    Filter1 = NewFileTypes("Resource files (*.res)", "res",
                        NewFileTypes("DynLD binary executable (*.app,*.dl)", "app,dl",
                        NewFileTypes("All files", NULL,
                        NULL)));    

    Filter2 = NewFileTypes("Supported images files (*.png;*.bmp)", "png,bmp",
                        NewFileTypes("All files", NULL,
                        NULL));    

    Filter3 = NewFileTypes("oZone iWidget (*.oiw)", "oiw",
                        NewFileTypes("All files", NULL,
                        NULL));    

    ActualRes = NewList();  // Initialize the resource list

    // Create the main application window
    RectAssign(&r, 0, 0, 510, 360);
    w = CreateWindow(&Me, r, "Resource file editor - New", WF_NORMAL | WF_CENTERED);
    WIDGET(w)->AppEvHdl = &AppEventHandler;
    InsertWidget(WIDGET(DeskTop), WIDGET(w));

    // Create the menu
    Menu = NewMenu(
        NewMenuItemEx("File", NULL, 0, 0, NULL, &GenerateSubMenu, "File", NULL)
    );
    AddMenuItemEx(Menu, "Resource Item", NULL, 0, 0, NULL, &GenerateSubMenu, "Resource Item");
    AddMenuItemEx(Menu, "Add", NULL, 0, 0, NULL, &GenerateSubMenu, "Add");
    AddMenuItemEx(Menu, "Help", NULL, 0, 0, NULL, &GenerateSubMenu, "Help");

    // Add menu to the window
    RectAssign(&r, 0, 0, 500, 20);
    PMenuView o = CreateMenuView(&Me, r, Menu, MenuViewStyleHorizontal, 0);
    InsertWidget(WIDGET(w), WIDGET(o));

    // Create and insert the tree view
    RectAssign(&r, 10, 30, 300, 330);
    t = CreateTreeView(&Me, r, 0);
    t->OnSelMsg = MSG_SELITEM;
    InsertWidget(WIDGET(w), WIDGET(t));

    // Create and insert the image label
    RectAssign(&r, 305, 30, 500, 330);
    ImgLbl = CreateImageLabel(&Me, r, 0);
    WIDGET(ImgLbl)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(ImgLbl));

    // Load resource file if provided as an argument
    if (argc > 1) {
        if (ActualRes) FreeList(ActualRes);  // Ensure to free any existing resources
        ActualRes = LoadResourceFile(argv[1]); //still an error here?
        if (ActualRes && ActualRes->Last) {
            a = b = ActualRes->Last->Next;
            do {
                AddTreeItem(t->Items, a->Key, NULL, NULL, NULL);
                a = a->Next;
            } while (a != b);
            TreeviewScrollBarRecalculate(t);
        }
        File = TextDup(argv[1]);
        WindowSetCaption(w, "Resource file editor - %s", argv[1]);
    }

    WidgetDrawAll(WIDGET(w));

    return true;
}

////////////////////////////////////////////////////////////////////////////////
void Close(void)
{
    if (File) free(File);
    if (Filter1) FreeFileTypes(Filter1);
    if (Filter2) FreeFileTypes(Filter2);
    if (Filter3) FreeFileTypes(Filter3);
}
////////////////////////////////////////////////////////////////////////////////

