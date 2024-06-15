////////////////////////////////////////////////////////////////////////////////
//
// Codename: Illkirch B
//
// (c) Copyright 2004 Point Mad. All rights reserved.
//
//      Contributors : Julien Etelain (Point Mad)
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "window.h"
#include "label.h"
#include "iodlg.h"
#include "tbox.h"
#include "button.h"
#include "menu.h"
#include "listview.h"
#include "illkirch.h"
#include "memfile.h"
#include <string.h>
////////////////////////////////////////////////////////////////////////////////
#define MSG_SEL 0x33FF0001
#define MSG_NEWSUB 0x33FF0002
#define MSG_REMOVE 0x33FF0003
#define MSG_SAVE   0x33FF0004

PMsgInfo MsgInfoAdd(PList l, l_text Name, l_ulong Value, l_text Description);
l_ulong MessageCount = 0;
////////////////////////////////////////////////////////////////////////////////
PBinData MsgInfoGenerateStore(void)
{

        DebugMessage("MsgInfoGenerateStore");

        PFile f = MemFileNewEmpty();
        l_ulong n = 0;

        DebugMessage("FileWrite");
        FileWrite(&n, sizeof(n), 1, f);

        if (MsgColl)
                if (MsgColl->Last)
                {
                        PListItem a, b;
                        a = b = MsgColl->Last->Next;
                        do
                        {
                                DebugMessage("FileWriteBinString");
                                FileWriteBinString(f, MSGINFO(a->Data)->Name);
                                FileWriteBinString(f, MSGINFO(a->Data)->Description);
                                FileWrite(&MSGINFO(a->Data)->Value, sizeof(l_ulong), 1, f);
                                DebugMessage("n++");
                                n++;
                                a = a->Next;
                        } while (a != b);
                }

        DebugMessage("FileSeek");
        FileSeek(f, 0, SEEK_SET);

        DebugMessage("FileWrite");
        FileWrite(&n, sizeof(n), 1, f);

        DebugMessage("/MsgInfoGenerateStore");

        return MemFileCloseToBinData(f);
}
////////////////////////////////////////////////////////////////////////////////
void MsgInfoInitFromStore(PBinData o)
{

        PFile f = MemFileFromBinData(o);
        l_ulong n = 0, Msg;
        l_text Name, Description;

        if (MsgColl)
                FreeList(MsgColl);
        MsgColl = NewList();

        FileRead(&n, sizeof(n), 1, f);

        while (n)
        {
                Name = FileReadBinString(f);
                Description = FileReadBinString(f);
                FileRead(&Msg, sizeof(l_ulong), 1, f);
                MsgInfoAdd(MsgColl, Name, Msg, Description);
                n--;
        }

        FileClose(f);
}
////////////////////////////////////////////////////////////////////////////////
void ResetMsgInfo(void)
{
        if (MsgColl)
                FreeList(MsgColl);
        MsgColl = NewList();
}
////////////////////////////////////////////////////////////////////////////////
void FreeMsgInfo(PMsgInfo o)
{
        if (o->Name)
                free(o->Name);
        if (o->Description)
                free(o->Description);
        free(o);
}
////////////////////////////////////////////////////////////////////////////////
PMsgInfo MsgInfoAdd(PList l, l_text Name, l_ulong Value, l_text Description)
{
        PMsgInfo o = malloc(sizeof(TMsgInfo));
        if (!o)
                return NULL;
        o->Name = TextDup(Name);
        o->Value = Value;
        o->Description = TextDup(Description);
        ListAdd(l, NULL, o, (void *)&FreeMsgInfo);
        return o;
}
////////////////////////////////////////////////////////////////////////////////
void MsgMan(void)
{
        l_ulong Msg;
        PLabel l;
        PButton b;
        PListview t = 0;
        PWindow w = 0;
        PTextbox iName = 0;
        PTextbox iDescription = 0;
        PTextbox iMessage = 0;
        PListviewItem Current = NULL;
        TRect r;

        RectAssign(&r, 0, 0, 500, 300);
        w = CreateWindow(&Me, r, "codename illkirch : Message manager", WF_NORMAL | WF_CENTERED);
        InsertWidget(WIDGET(DeskTop), WIDGET(w));

        RectAssign(&r, 0, 0, 295, 300);
        t = CreateListview(&Me, r, "Name", LVS_LIST, LVF_COLUMS | LVF_COLRESIZE);
        t->OnSelMsg = MSG_SEL;
        InsertWidget(WIDGET(w), WIDGET(t));

        RectAssign(&r, 300, 0, 375, 20);
        l = CreateLabel(&Me, r, "Name :");
        InsertWidget(WIDGET(w), WIDGET(l));

        RectAssign(&r, r.b.x + 5, r.a.y, 500, r.b.y);
        iName = CreateTextbox(&Me, r, TBF_EDITABLE);
        InsertWidget(WIDGET(w), WIDGET(iName));

        RectAssign(&r, 300, r.b.y + 5, 375, r.b.y + 25);
        l = CreateLabel(&Me, r, "Comment :");
        InsertWidget(WIDGET(w), WIDGET(l));

        RectAssign(&r, r.b.x + 5, r.a.y, 500, r.b.y);
        iDescription = CreateTextbox(&Me, r, TBF_EDITABLE);
        InsertWidget(WIDGET(w), WIDGET(iDescription));

        RectAssign(&r, 300, r.b.y + 5, 375, r.b.y + 25);
        l = CreateLabel(&Me, r, "Message :");
        InsertWidget(WIDGET(w), WIDGET(l));

        RectAssign(&r, r.b.x + 5, r.a.y, 500, r.b.y);
        iMessage = CreateTextbox(&Me, r, TBF_EDITABLE);
        InsertWidget(WIDGET(w), WIDGET(iMessage));

        RectAssign(&r, 300, r.b.y + 10, 360, r.b.y + 30);
        b = CreateButton(&Me, r, "Add New", MSG_NEWSUB);
        InsertWidget(WIDGET(w), WIDGET(b));

        RectAssign(&r, 370, r.a.y, 430, r.b.y);
        b = CreateButton(&Me, r, "Save", MSG_SAVE);
        InsertWidget(WIDGET(w), WIDGET(b));

        RectAssign(&r, 440, r.a.y, 500, r.b.y);
        b = CreateButton(&Me, r, "Remove", MSG_REMOVE);
        InsertWidget(WIDGET(w), WIDGET(b));

        RectAssign(&r, 300, 280, 400, 300);
        b = CreateButton(&Me, r, "OK", MSG_OK);
        InsertWidget(WIDGET(w), WIDGET(b));

        RectAssign(&r, 405, r.a.y, 500, r.b.y);
        b = CreateButton(&Me, r, "Cancel", WM_CLOSE);
        InsertWidget(WIDGET(w), WIDGET(b));

        ListviewSetColSize(t, 0, 125);
        ListviewAddColum(t, "Message", NULL, 55, 0);
        ListviewAddColum(t, "Description", NULL, 120, 0);

        // show saved messages
        if (MsgColl)
                if (MsgColl->Last)
                {
                        PListItem a, b;
                        a = b = MsgColl->Last->Next;
                        do
                        {
                                PListviewItem n;
                                l_text Message;

                                Message = TextArgs("%x", MSGINFO(a->Data)->Value);

                                n = ListviewAddItem(t, MSGINFO(a->Data)->Name, NULL);
                                if (n)
                                {
                                        ListviewItemPushCol(n, Message);
                                        ListviewItemPushCol(n, MSGINFO(a->Data)->Description);
                                }
                                free(Message);

                                a = a->Next;
                        } while (a != b);
                }

        WidgetDrawAll(WIDGET(w));

        // message handler
        do
        {
                Msg = WidgetExecute(WIDGET(w));

                if (Msg == MSG_REMOVE)
                {
                        if (Current)
                                ListviewItemRemoveItem(t, Current);
                        ListviewUpdateScrollbars(t);
                        WidgetDraw(WIDGET(t), NULL);

                        Current = NULL;
                }
                if (Msg == MSG_SAVE)
                {
                        if (Current)
                        {
                                l_text Name, Message;

                                Message = TextArgs("%x", strtoul(iMessage->Text, NULL, 16));

                                Name = TextDup(iName->Text);

                                ListviewItemSetColCaption(Current, 0, Name);
                                ListviewItemSetColCaption(Current, 1, Message);
                                ListviewItemSetColCaption(Current, 2, iDescription->Text);

                                free(Name);
                                free(Message);

                                WidgetDraw(WIDGET(t), &Current->BufAbs);
                        }
                }

                if (Msg == MSG_NEWSUB)
                {
                        if (TextLen(iName->Text))
                        {
                                PListviewItem n;
                                l_text Name, Description;
                                l_text MessageValue;

                                Name = iName->Text;
                                Description = iDescription->Text;

                                MessageValue = TextArgs("%x", 0x00010000 + MessageCount);

                                n = ListviewAddItem(t, Name, NULL);

                                if (n)
                                {
                                        ListviewItemPushCol(n, MessageValue);
                                        ListviewItemPushCol(n, Description);
                                }

                                ListviewUpdateScrollbars(t);
                                WidgetDraw(WIDGET(t), NULL);
                                MessageCount++;

                                free(MessageValue);
                        }
                }

                if (Msg == MSG_SEL)
                {
                        Current = GetSelectedItem(t);
                        if (Current)
                        {
                                TextBoxSetText(iName, ListAt(Current->ColumsData, 1));
                                TextBoxSetText(iDescription, ListAt(Current->ColumsData, 3));
                                TextBoxSetText(iMessage, ListAt(Current->ColumsData, 2));
                        }
                        else
                        {
                                TextBoxSetText(iName, "");
                                TextBoxSetText(iDescription, "");
                                TextBoxSetText(iMessage, "");
                        }
                }

        } while ((Msg != WM_CLOSE) && (Msg != MSG_OK));

        if (Msg == MSG_OK)
        {

                if (MsgColl)
                        FreeList(MsgColl);
                MsgColl = NewList();

                if (t->Items->Last)
                {
                        PListItem a = t->Items->Last->Next, b = a;
                        PListviewItem p;
                        l_text Message, Name, Description;
                        do
                        {
                                p = LISTVIEWITEM(a->Data);
                                Name = ListAt(p->ColumsData, 1);
                                Description = ListAt(p->ColumsData, 3);
                                Message = ListAt(p->ColumsData, 2);
                                MsgInfoAdd(MsgColl, Name, strtoul(Message, NULL, 16), Description);
                                a = a->Next;
                        } while (a != b);
                }
        }
        WidgetDispose(WIDGET(w));
}
////////////////////////////////////////////////////////////////////////////////
l_ulong MsgSelect(l_ulong Message)
{

        l_ulong Msg;
        PListview t = 0;
        PWindow w = 0;
        PButton b;
        TRect r;
        PListviewItem p = NULL;

        RectAssign(&r, 0, 0, 300, 300);
        w = CreateWindow(&Me, r, "codename illkirch : Message Selector", WF_NORMAL | WF_CENTERED);
        InsertWidget(WIDGET(DeskTop), WIDGET(w));

        RectAssign(&r, 0, 0, 300, 275);
        t = CreateListview(&Me, r, "Name", LVS_LIST, LVF_COLUMS | LVF_COLRESIZE);
        InsertWidget(WIDGET(w), WIDGET(t));

        RectAssign(&r, 0, 280, 100, 300);
        b = CreateButton(&Me, r, "OK", MSG_OK);
        InsertWidget(WIDGET(w), WIDGET(b));

        RectAssign(&r, 105, r.a.y, 200, r.b.y);
        b = CreateButton(&Me, r, "Cancel", WM_CLOSE);
        InsertWidget(WIDGET(w), WIDGET(b));

        ListviewSetColSize(t, 0, 125);
        ListviewAddColum(t, "Message", NULL, 55, 0);
        ListviewAddColum(t, "Description", NULL, 120, 0);

        if (MsgColl)
                if (MsgColl->Last)
                {
                        PListItem a, b;
                        a = b = MsgColl->Last->Next;
                        do
                        {

                                PListviewItem n;
                                l_text textMessage;

                                textMessage = TextArgs("%x", MSGINFO(a->Data)->Value);

                                n = ListviewAddItem(t, MSGINFO(a->Data)->Name, NULL);
                                if (n)
                                {
                                        ListviewItemPushCol(n, textMessage);
                                        ListviewItemPushCol(n, MSGINFO(a->Data)->Description);
                                }

                                if (MSGINFO(a->Data)->Value == Message)
                                        (p = n)->Flags |= LVI_SELECTED;

                                free(textMessage);

                                a = a->Next;
                        } while (a != b);
                }

        ListviewCalculateMax(WIDGET(t));

        if (p)
                ListviewScrollTo(t, p);

        WidgetDrawAll(WIDGET(w));

        do
        {
                Msg = WidgetExecute(WIDGET(w));

        } while ((Msg != WM_CLOSE) && (Msg != MSG_OK));

        if (Msg == MSG_OK)
        {
                p = GetSelectedItem(t);
                if (p)
                {
                        l_text textMessage = ListAt(p->ColumsData, 2);
                        if (textMessage)
                                Message = strtoul(textMessage, NULL, 16);
                }
        }
        WidgetDispose(WIDGET(w));

        return Message;
}
////////////////////////////////////////////////////////////////////////////////
