////////////////////////////////////////////////////////////////////////////////
//
//  O3 Pad
//      Codename Phoenix text editor
//
//  Copyright (c) 2003 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "button.h"
#include "window.h"
#include "tbox.h"
#include "iodlg.h"
#include "toolbar.h"

char AppName[] = "O3 Pad";
l_uid nUID = "app:O3pad";
l_uid NeededLibs[] = { "menu", "button", "iodlg", "window", "textbox", "toolbar", "" };

#define MSG_FILENEW 0xF0020001
#define MSG_FILEOPEN 0xF0020002
#define MSG_FILESAVE 0xF0020003
#define MSG_FILESAVEAS 0xF0020004

PTextbox tb = 0;
PWindow w = 0;
PToolbar bar = 0;
PFileTypes Filter = 0;
l_text OpenedFile = 0;

l_bool AppEventHandler(PWidget o, PEvent Event) {
    DebugMessage("AppEventHandler: Event Type = %d, Message = 0x%08X", Event->Type, Event->Message);

    if (Event->Type == EV_MESSAGE) {
        switch (Event->Message) {
            case WM_CLOSE:
            {
                DebugMessage("AppEventHandler: WM_CLOSE received, closing app.");
                CloseApp(&Me);
                return true;
            }
            break;

            case MSG_FILENEW:
            {
                DebugMessage("AppEventHandler: MSG_FILENEW received, creating new file.");
                TextBoxSetTextEx(tb, "");
                WindowSetCaptionEx(w, "O3 Pad");
                return true;
            }
            break;

            case MSG_FILEOPEN:
            {
                DebugMessage("AppEventHandler: MSG_FILEOPEN received, opening file.");
                l_text tmp = OpenedFile;

                OpenedFile = IOBox("Open text file (UTF-8)", IOBOX_OPEN, NULL, Filter, true);

                if (OpenedFile) {
                    l_text txt = LoadData2(OpenedFile, TYPE_TEXT);

                    if (txt) {
                        TextBoxSetTextEx(tb, txt);
                        free(txt);
                        WindowSetCaption(w, "O3 Pad - %s", OpenedFile);
                    } else {
                        DebugMessage("AppEventHandler: Failed to load text from %s", OpenedFile);
                    }

                    free(tmp);
                } else {
                    DebugMessage("AppEventHandler: No file selected.");
                }
                return true;
            }
            break;

            case MSG_FILESAVE:
            case MSG_FILESAVEAS:
            {
                DebugMessage("AppEventHandler: MSG_FILESAVE or MSG_FILESAVEAS received, saving file.");
                l_text File;

                if (Event->Message == MSG_FILESAVEAS) {
                    free(OpenedFile);
                    OpenedFile = IOBox("Save text file (UTF-8)", IOBOX_SAVE, NULL, Filter, false);
                    DebugMessage("AppEventHandler: MSG_FILESAVEAS, new file name: %s", OpenedFile);
                }

                if (OpenedFile) {
                    l_text txt = TextDup(tb->Text);

                    if (SaveData2(OpenedFile, txt, TYPE_TEXT)) {
                        WindowSetCaption(w, "O3 Pad - %s", OpenedFile);
                        DebugMessage("AppEventHandler: File saved successfully.");
                    } else {
                        DebugMessage("AppEventHandler: Failed to save file.");
                    }

                    free(txt);
                } else {
                    DebugMessage("AppEventHandler: No file selected.");
                }
                return true;
            }
            break;

            case WM_ABOUT:
            {
                DebugMessage("AppEventHandler: WM_ABOUT received, showing about box.");
                MessageBox(&Me, "About O3 Pad", "O3pad 0.1\nThe ultimate oZone text editor\n\nCopyright (c) 2003 Point Mad, Lukas Lipka. All rights reserved.", MBB_OK);
                return true;
            }
            break;
        }
    }

    return false;
}

void GenerateEditSubMenu(PMenuItem itm, void* Args) {
    DebugMessage("GenerateEditSubMenu: Generating Edit submenu.");

    PMenuItem subItems[5];

    subItems[0] = NewMenuItem("Copy", NULL, MSG_COPY, (TEXTBOX(tb)->SelPos != TEXTBOX(tb)->SelPosEnd) ? 0 : MI_DISABLE, NULL, NULL);
    subItems[1] = NewMenuItem("Cut", NULL, MSG_CUT, (TEXTBOX(tb)->SelPos != TEXTBOX(tb)->SelPosEnd) && (TEXTBOX(tb)->Flags & TBF_EDITABLE) ? 0 : MI_DISABLE, NULL, NULL);
    subItems[2] = NewMenuItem("Paste", NULL, MSG_PASTE, ((TEXTBOX(tb)->Flags & TBF_EDITABLE) && ClipboardGet(TYPE_TEXT, NULL)) ? 0 : MI_DISABLE, NULL, NULL);
    subItems[3] = NewMenuItemSeparator(NULL);
    subItems[4] = NewMenuItem("Delete", NULL, MSG_DELETE, (TEXTBOX(tb)->SelPos != TEXTBOX(tb)->SelPosEnd) && (TEXTBOX(tb)->Flags & TBF_EDITABLE) ? 0 : MI_DISABLE, NULL, NULL);

    itm->SubMenu = NewMenu(subItems[0]);
    subItems[0]->Next = subItems[1];
    subItems[1]->Next = subItems[2];
    subItems[2]->Next = subItems[3];
    subItems[3]->Next = subItems[4];
}

void GenerateSubMenu(PMenuItem itm, void* Args) {
    l_text arg = (l_text)Args;
    DebugMessage("GenerateSubMenu: Generating submenu for %s", arg);

    if (TextCompare(arg, "File") == 0) {
        PMenuItem subItems[6];

        subItems[0] = NewMenuItem("New", NULL, MSG_FILENEW, 0, NULL, NULL);
        subItems[1] = NewMenuItem("Open", NULL, MSG_FILEOPEN, 0, NULL, NULL);
        subItems[2] = NewMenuItem("Save", NULL, MSG_FILESAVE, 0, NULL, NULL);
        subItems[3] = NewMenuItem("Save as...", NULL, MSG_FILESAVEAS, 0, NULL, NULL);
        subItems[4] = NewMenuItem("Exit", NULL, WM_CLOSE, 0, NULL, NULL);
        subItems[5] = NULL;

        itm->SubMenu = NewMenu(subItems[0]);
        subItems[0]->Next = subItems[1];
        subItems[1]->Next = subItems[2];
        subItems[2]->Next = subItems[3];
        subItems[3]->Next = subItems[4];
        subItems[4]->Next = subItems[5];
    } else if (TextCompare(arg, "Edit") == 0) {
        GenerateEditSubMenu(itm, Args);
    } else if (TextCompare(arg, "Help") == 0) {
        PMenuItem subItems[2];

        subItems[0] = NewMenuItem("About", NULL, WM_ABOUT, 0, NULL, NULL);
        subItems[1] = NULL;

        itm->SubMenu = NewMenu(subItems[0]);
        subItems[0]->Next = subItems[1];
    }
}

l_int Main(int argc, l_text *argv) {
    TRect r;
    PToolbarItem ti;

    DebugMessage("Main: Starting O3 Pad application");

    RectAssign(&r, 0, 0, 540, 380);
    w = CreateWindow(&Me, r, "O3 Pad", WF_NORMAL | WF_CENTERED | WF_RESIZE);
    WIDGET(w)->AppEvHdl = &AppEventHandler;
    InsertWidget(WIDGET(DeskTop), WIDGET(w));

    Filter = NewFileTypes("Text files", "txt",
                          NewFileTypes("HTML files", "htm,html",
                                       NewFileTypes("C/C++ files", "c,cpp,cxx,h",
                                                    NewFileTypes("All files", NULL,
                                                                 NULL))));

    PMenuItem fileMenu = NewMenuItemEx("File", NULL, 0, 0, NULL, &GenerateSubMenu, "File", NULL);
    PMenu Menu = NewMenu(fileMenu);
    AddMenuItemEx(Menu, "Edit", NULL, 0, 0, NULL, &GenerateSubMenu, "Edit");
    AddMenuItemEx(Menu, "Help", NULL, 0, 0, NULL, &GenerateSubMenu, "Help");

    RectAssign(&r, 0, 0, 540, 20);
    PMenuView o = NewMenuView(&Me, r, Menu, MenuViewStyleHorizontal, 0);
    InsertWidget(WIDGET(w), WIDGET(o));

    RectAssign(&r, 10, 30, 530, 75);
    bar = CreateToolbar(&Me, r);
    InsertWidget(WIDGET(w), WIDGET(bar));

    ti = CreateToolbarItem("Open", 0, MSG_FILEOPEN, 0);
    ToolbarAddItem(bar, ti);
    ti = CreateToolbarItem("Save", 0, MSG_FILESAVE, 0);
    ToolbarAddItem(bar, ti);
    ti = CreateToolbarItem("Save as", 0, MSG_FILESAVEAS, 0);
    ToolbarAddItem(bar, ti);
    ti = CreateToolbarItem("New", 0, MSG_FILENEW, 0);
    ToolbarAddItem(bar, ti);

    RectAssign(&r, 10, 85, 530, 370);
    tb = CreateTextbox(&Me, r, TBF_EDITABLE | TBF_MULTILINE);

    WIDGET(tb)->Flags |= WF_AUTORESIZE | WF_MAGNETRIGHT | WF_MAGNETBOTTOM;

    InsertWidget(WIDGET(w), WIDGET(tb));

    if (argc > 1) {
        l_text txt = LoadData2(argv[1], TYPE_TEXT);
        OpenedFile = TextDup(argv[1]);

        if (txt) {
            TextBoxSetTextEx(tb, txt);
            WindowSetCaption(w, "O3 Pad - %s", argv[1]);
            free(txt);
        } else {
            DebugMessage("Main: Failed to load initial file %s", argv[1]);
        }
    }

    WidgetDrawAll(WIDGET(w));
    DebugMessage("Main: Initialization complete");
    return true;
}

void Close(void) {
    DebugMessage("Close: Cleaning up and closing application");

    if (OpenedFile) free(OpenedFile);

    WidgetDispose(WIDGET(w));
    FreeFileTypes(Filter);

    DebugMessage("Close: Cleanup complete");
}
