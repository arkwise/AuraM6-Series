////////////////////////////////////////////////////////////////////////////////
//
//    Aloha Chat 0.1
//      
//      developed for Aura GUI 0.9.8.0 milestone 6
//
////////////////////////////////////////////////////////////////////////////////

// the flags for compiling are : (to be added)




#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "button.h"
#include "window.h"
#include "tbox.h"
#include "iodlg.h"
#include "toolbar.h"
#include "wattcp.h" // Include the Watt TCP library header

#define MAX_BUFFER_SIZE 256

char AppName[] = "Alohachat";
l_uid nUID = "app:alohahat";
l_uid NeededLibs[] = { "menu","button","iodlg","window","textbox","toolbar","" };

#define MSG_FILENEW     0xF0020001
#define MSG_FILEOPEN    0xF0020002
#define MSG_FILESAVE    0xF0020003
#define MSG_FILESAVEAS  0xF0020004
#define MSG_GO          0xF0020005
#define MSG_IP          0xF0020006

PTextbox tb = 0;
PButton b = 0;
PTextbox addtb = 0;
PTextbox addip = 0;
PWindow w = 0;
PToolbar bar = 0;
PFileTypes Filter = 0;
l_text OpenedFile = 0;

// Server socket and client socket
SOCKET serverSocket;
SOCKET clientSocket;

l_bool AppEventHandler(PWidget o, PEvent Event)
{
    if (Event->Type == EV_MESSAGE)
    {
        switch (Event->Message)
        {
            case WM_CLOSE:
            {
                CloseApp(&Me);
                return true;
            }
            break;

            case MSG_FILENEW:
            {
                TextBoxSetTextEx(tb, "");
                WindowSetCaptionEx(w, "Aloha Chat");
                return true;
            }
            break;

            case MSG_GO:
            {
                l_text tmp = OpenedFile;
                OpenedFile = TextDup(addtb->Text);

                if (OpenedFile)
                {
                    l_text txt = LoadData2(OpenedFile, TYPE_TEXT);

                    if (txt)
                    {
                        TextBoxSetTextEx(tb, txt);
                        free(txt);
                    }

                    free(tmp);
                }
                return true;
            }
            break;

            case MSG_IP:
            {
                l_text serverIP = TextDup(addip->Text);
                unsigned short serverPort = 1234;

                if (serverSocket == INVALID_SOCKET)
                {
                    // Create the server socket
                    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
                    if (serverSocket != INVALID_SOCKET)
                    {
                        struct sockaddr_in serverAddr;
                        memset(&serverAddr, 0, sizeof(serverAddr));
                        serverAddr.sin_family = AF_INET;
                        serverAddr.sin_port = htons(serverPort);
                        serverAddr.sin_addr.s_addr = inet_addr(serverIP);

                        // Bind the server socket to the specified IP and port
                        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == 0)
                        {
                            // Listen for incoming connections
                            if (listen(serverSocket, SOMAXCONN) == 0)
                            {
                                printf("Server started\n");
                            }
                            else
                            {
                                printf("Error listening on the server socket\n");
                                closesocket(serverSocket);
                                serverSocket = INVALID_SOCKET;
                            }
                        }
                        else
                        {
                            printf("Error binding the server socket\n");
                            closesocket(serverSocket);
                            serverSocket = INVALID_SOCKET;
                        }
                    }
                    else
                    {
                        printf("Error creating the server socket\n");
                    }
                }
                else
                {
                    printf("Server already started\n");
                }

                free(serverIP);
                return true;
            }
            break;

            case MSG_FILEOPEN:
            {
                l_text tmp = OpenedFile;

                OpenedFile = IOBox("Open text file (UTF-8)", IOBOX_OPEN, NULL, Filter, true);

                if (OpenedFile)
                {
                    l_text txt = LoadData2(OpenedFile, TYPE_TEXT);

                    if (txt)
                    {
                        TextBoxSetTextEx(addtb, OpenedFile);
                        TextBoxSetTextEx(tb, txt);
                        free(txt);
                    }

                    free(tmp);
                }
                return true;
            }
            break;

            case MSG_FILESAVE:
            case MSG_FILESAVEAS:
            {
                l_text File;

                if (Event->Message == MSG_FILESAVEAS)
                {
                    free(OpenedFile);
                    OpenedFile = IOBox("Save text file (UTF-8)", IOBOX_SAVE, NULL, Filter, false);
                }

                if (OpenedFile)
                {
                    l_text txt = TextDup(tb->Text);

                    if (SaveData2(OpenedFile, txt, TYPE_TEXT))
                    {
                        WindowSetCaption(w, "Type Writer");
                    }

                    free(txt);
                }
                return true;
            }
            break;

            case WM_ABOUT:
            {
                MessageBox(&Me, "Aloha Chat", "the ip chat 1.1\nThe Aloha Chat Processer\n\nCopyright (c) 2023. All rights reserved.", MBB_OK);
                return true;
            }
            break;
        }
    }

    return false;
}

void GenerateEditSubMenu(PMenuItem o, void* Args) {
    o->SubMenu = NewMenu(
        NewMenuItem("Copy", NULL, MSG_COPY, (TEXTBOX(tb)->SelPos != TEXTBOX(tb)->SelPosEnd) ? 0 : MI_DISABLE, NULL,
            NewMenuItem("Cut", NULL, MSG_CUT, (TEXTBOX(tb)->SelPos != TEXTBOX(tb)->SelPosEnd) && (TEXTBOX(tb)->Flags & TBF_EDITABLE) ? 0 : MI_DISABLE, NULL,
                NewMenuItem("Paste", NULL, MSG_PASTE, ((TEXTBOX(tb)->Flags & TBF_EDITABLE) && ClipboardGet(TYPE_TEXT, NULL)) ? 0 : MI_DISABLE, NULL,
                    NewMenuItemSeparator(
                        NewMenuItem("Delete", NULL, MSG_DELETE, (TEXTBOX(tb)->SelPos != TEXTBOX(tb)->SelPosEnd) && (TEXTBOX(tb)->Flags & TBF_EDITABLE) ? 0 : MI_DISABLE, NULL,
                            NULL))))));
}

l_int Main(int argc, l_text* argv)
{

    TRect r;
    PToolbarItem ti;

    RectAssign(&r, 0, 0, 540, 380);
    w = CreateWindow(&Me, r, "Aloha Chat", WF_NORMAL | WF_MAXIMIZE | WF_CENTERED | WF_RESIZE);
    WIDGET(w)->AppEvHdl = &AppEventHandler;
    InsertWidget(WIDGET(DeskTop), WIDGET(w));

    Filter = NewFileTypes("HTML files", "htm,html",
        NewFileTypes("Text files", "txt",
            NewFileTypes("All files", NULL,
                NULL)));

    PMenu Menu = NewMenu(
        NewMenuItem("File", NULL, NULL, NULL,
            NewMenu(
                NewMenuItem("New", NULL, MSG_FILENEW, NULL, NULL,
                    NewMenuItem("Open", NULL, MSG_FILEOPEN, NULL, NULL,
                        NewMenuItem("Save", NULL, MSG_FILESAVE, NULL, NULL,
                            NewMenuItem("Save as...", NULL, MSG_FILESAVEAS, NULL, NULL,
                                NewMenuItem("Exit", NULL, WM_CLOSE, NULL, NULL, NULL))))))),
        NewMenuItemEx("Edit", NULL, 0, 0, NULL, &GenerateEditSubMenu, NULL,
            NewMenuItem("View", NULL, NULL, NULL, NULL,
                NewMenuItem("Help", NULL, NULL, NULL,
                    NewMenu(
                        NewMenuItem("About", NULL, WM_ABOUT, NULL, NULL, NULL)),
                    NULL))));

    RectAssign(&r, 0, 0, 540, 20);
    PMenuView o = NewMenuView(&Me, r, Menu, MenuViewStyleHorizontal, 0);
    InsertWidget(WIDGET(w), WIDGET(o));

    RectAssign(&r, 0, 25, 505, 40);
    addip = CreateTextbox(&Me, r, TBF_EDITABLE);
    WIDGET(addip)->Flags |= WF_AUTORESIZE | WF_MAGNETRIGHT;
    InsertWidget(WIDGET(w), WIDGET(addip));

    RectAssign(&r, 510, 25, 540, 40);
    b = CreateButton(&Me, r, "Connect", MSG_IP);
    InsertWidget(WIDGET(w), WIDGET(b));

    RectAssign(&r, 0, 25, 505, 40);
    addtb = CreateTextbox(&Me, r, TBF_EDITABLE);
    WIDGET(addtb)->Flags |= WF_AUTORESIZE | WF_MAGNETRIGHT;
    InsertWidget(WIDGET(w), WIDGET(addtb));

    RectAssign(&r, 510, 25, 540, 40);
    b = CreateButton(&Me, r, "Go", MSG_GO);
    InsertWidget(WIDGET(w), WIDGET(b));

    RectAssign(&r, 10, 45, 530, 370);
    tb = CreateTextbox(&Me, r, TBF_EDITABLE | TBF_MULTILINE);
    WIDGET(tb)->Flags |= WF_AUTORESIZE | WF_MAGNETRIGHT | WF_MAGNETBOTTOM;
    InsertWidget(WIDGET(w), WIDGET(tb));

    if (argc > 1)
    {
        l_text txt = LoadData2(argv[1], TYPE_TEXT);
        OpenedFile = TextDup(argv[1]);

        if (txt)
        {
            TextBoxSetTextEx(addtb, argv[1]);
            TextBoxSetTextEx(tb, txt);
            free(txt);
        }
    }

    // Initialize the Watt TCP library
    if (sock_init() == -1)
    {
        printf("Error initializing Watt TCP library\n");
        return false;
    }

    // Initialize the server socket
    serverSocket = INVALID_SOCKET;

    WidgetDrawAll(WIDGET(w));
    return true;
}

void Close(void)
{
    // Close the client socket if connected
    if (clientSocket != INVALID_SOCKET)
    {
        closesocket(clientSocket);
    }

    // Close the server socket if created
    if (serverSocket != INVALID_SOCKET)
    {
        closesocket(serverSocket);
    }

    // Cleanup the Watt TCP library
    sock_exit();

    if (OpenedFile) free(OpenedFile);

    WidgetDispose(WIDGET(w));
    FreeFileTypes(Filter);
}
