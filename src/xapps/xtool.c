////////////////////////////////////////////////////////////////////////////////
//
//  Textbox to Label Text Move Application
//
//  Aura Milestone 6
//
//  (c) doscore 2007-2024
//
////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "tbox.h"
#include "label.h"
#include "button.h"
#include "clipboard.h"
#include "menu.h"

#define BTN_MOVE_TEXT 1001

void MoveTextToLabel(PWidget o, PEvent Ev) {
    if (Ev->Message == MSG_CLICK) {
        PTextbox tb = (PTextbox)o->UserData;
        l_text text = ClipboardGet(TYPE_TEXT);
        PLabel lbl = (PLabel)o->UserData2;
        LabelSetText(lbl, text);
        WidgetDraw(WIDGET(lbl), NULL);
    }
}

l_bool AppEventHandler(PWidget o, PEvent Ev) {
    switch (Ev->Message) {
        case MSG_CLICK:
            if (Ev->Command == BTN_MOVE_TEXT) {
                MoveTextToLabel(o, Ev);
                return true;
            }
            break;
    }
    return false;
}

l_int Main(int argc, l_text *argv) {
    TRect r;
    PTextbox tb;
    PLabel lbl;
    PButton btn;

    // Create an editable textbox
    RectAssign(&r, 10, 10, 200, 30);
    tb = CreateTextbox(&Me, r, TBF_EDITABLE);
    InsertWidget(Desktop, WIDGET(tb));

    // Create a label
    RectAssign(&r, 10, 50, 200, 30);
    lbl = CreateLabel(&Me, r, "Label");
    InsertWidget(Desktop, WIDGET(lbl));

    // Create a button to move text from the textbox to the label
    RectAssign(&r, 10, 90, 100, 30);
    btn = CreateButton(&Me, r, "Move Text", BTN_MOVE_TEXT);
    btn->UserData = tb;
    btn->UserData2 = lbl;
    InsertWidget(Desktop, WIDGET(btn));

    // Set the application event handler
    WIDGET(btn)->AppEvHdl = &AppEventHandler;

    // Draw all widgets
    WidgetDrawAll(Desktop);
    return true;
}

void Close(void) {
    // Add any cleanup code here if necessary
}
