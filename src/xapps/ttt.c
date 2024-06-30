////////////////////////////////////////////////////////////////////////////////
//
//  Tic Tac Toe Game
//
//  (c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "button.h"
#include "window.h"
#include "label.h"
#include "tbox.h"

l_ulong AppVersion = ULONG_ID(0, 0, 0, 1);
char AppName[] = "Tic Tac Toe";
l_uid nUID = "app:tictactoe";
l_uid NeededLibs[] = { "button", "window", "menu", "label", "" };

#define MSG_NEWGAME 0x00010001
#define MSG_ABOUT 0x00010002
#define MSG_EXIT 0x00010003
#define MSG_MOVE 0x00010004

PWindow w = 0;
PLabel lbl = 0;
PButton board[3][3];
char currentPlayer = 'X';

void GenerateSubMenu(PMenuItem itm, void* Args) {
    l_text arg = (l_text)Args;

    if (TextCompare(arg, "File") == 0) {
        PMenuItem items = NewMenuItem("New Game", NULL, MSG_NEWGAME, 0, NULL, NULL);
        AddMenuItem(items->SubMenu, "Exit", NULL, MSG_EXIT, 0, NULL);
        itm->SubMenu = items->SubMenu;
    } else if (TextCompare(arg, "Help") == 0) {
        PMenuItem items = NewMenuItem("About", NULL, MSG_ABOUT, 0, NULL, NULL);
        itm->SubMenu = items->SubMenu;
    }
}

void ResetBoard() {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            ButtonSetText(board[i][j], "");
        }
    }
    currentPlayer = 'X';
    LabelSetText(lbl, "Player X's turn");
}

l_bool CheckWin() {
    int i;
    const char *text1, *text2, *text3;
    for (i = 0; i < 3; i++) {
        text1 = ButtonText(board[i][0]);
        text2 = ButtonText(board[i][1]);
        text3 = ButtonText(board[i][2]);
        if (text1 != NULL && text1[0] != '\0' && text1[0] == text2[0] && text2[0] == text3[0]) {
            return true;
        }
        text1 = ButtonText(board[0][i]);
        text2 = ButtonText(board[1][i]);
        text3 = ButtonText(board[2][i]);
        if (text1 != NULL && text1[0] != '\0' && text1[0] == text2[0] && text2[0] == text3[0]) {
            return true;
        }
    }
    text1 = ButtonText(board[0][0]);
    text2 = ButtonText(board[1][1]);
    text3 = ButtonText(board[2][2]);
    if (text1 != NULL && text1[0] != '\0' && text1[0] == text2[0] && text2[0] == text3[0]) {
        return true;
    }
    text1 = ButtonText(board[0][2]);
    text2 = ButtonText(board[1][1]);
    text3 = ButtonText(board[2][0]);
    if (text1 != NULL && text1[0] != '\0' && text1[0] == text2[0] && text2[0] == text3[0]) {
        return true;
    }
    return false;
}

l_bool BoardFull() {
    int i, j;
    const char *text;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            text = ButtonText(board[i][j]);
            if (text == NULL || text[0] == '\0') {
                return false;
            }
        }
    }
    return true;
}

l_bool AppEventHandler(PWidget o, PEvent Ev)
{
    char msg[50];
    const char *buttonText;
    switch (Ev->Message)
    {
    case MSG_NEWGAME:
        ResetBoard();
        return true;
    case MSG_ABOUT:
        MessageBox(&Me, "About Tic Tac Toe", "Tic Tac Toe Game\n\n(c) 2012 Finn Technologies and Chase Finn. All rights reserved.", MBB_OK);
        return true;
    case MSG_EXIT:
        CloseApp(&Me);
        return true;
    case MSG_MOVE:
        buttonText = ButtonText((PButton)o);
        if (buttonText == NULL || buttonText[0] == '\0') {
            ButtonSetText((PButton)o, "%c", currentPlayer);
            if (CheckWin()) {
                sprintf(msg, "Player %c wins!", currentPlayer);
                MessageBox(&Me, "Game Over", msg, MBB_OK);
                ResetBoard();
            } else if (BoardFull()) {
                MessageBox(&Me, "Game Over", "It's a draw!", MBB_OK);
                ResetBoard();
            } else {
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                sprintf(msg, "Player %c's turn", currentPlayer);
                LabelSetText(lbl, msg);
            }
        }
        return true;
    }

    return false;
}

l_int Main(int argc, l_text* argv)
{
    PLabel l;
    TRect r;
    int i, j;

    RectAssign(&r, 0, 0, 400, 400);
    w = CreateWindow(&Me, r, "Tic Tac Toe", WF_NORMAL | WF_CENTERED);
    InsertWidget(WIDGET(DeskTop), WIDGET(w));

    PMenuItem fileMenu = NewMenuItemEx("File", NULL, 0, 0, NULL, &GenerateSubMenu, "File", NULL);
    PMenu Menu = NewMenu(fileMenu);
    AddMenuItemEx(Menu, "Help", NULL, 0, 0, NULL, &GenerateSubMenu, "Help");

    RectAssign(&r, 0, 0, 400, 20);
    PMenuView o = NewMenuView(&Me, r, Menu, MenuViewStyleHorizontal, 0);
    InsertWidget(WIDGET(w), WIDGET(o));

    RectAssign(&r, 10, 30, 380, 50);
    lbl = CreateLabel(&Me, r, "Player X's turn");
    WIDGET(lbl)->BackgroundColor = COL_3DFACE;
    InsertWidget(WIDGET(w), WIDGET(lbl));

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            RectAssign(&r, 10 + j * 60, 60 + i * 60, 70 + j * 60, 120 + i * 60);
            board[i][j] = CreateButton(&Me, r, "", MSG_MOVE);
            InsertWidget(WIDGET(w), WIDGET(board[i][j]));
        }
    }

    WIDGET(w)->AppEvHdl = &AppEventHandler;

    WidgetDrawAll(WIDGET(w));
    return true;
}

void Close(void)
{
    DebugMessage("Running Close function");
}
