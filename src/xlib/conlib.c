////////////////////////////////////////////////////////////////////////////////
//
//  Console (Library)
//
//  Copyright (c) 2024 Doscore. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "window.h"
#include "label.h"
#include "internal.h"
#include "console.h"
#include "allegro/internal/aintern.h"

l_ulong AppVersion = ULONG_ID(0, 1, 0, 0);
char AppName[] = "Console Library";
l_uid nUID = "conlib";
l_uid NeededLibs[] = { "window", "widget", "label", "" };

l_color Palette[16];

////////////////////////////////////////////////////////////////////////////////
l_bool AppEventHandler(PWidget o, PEvent Ev) {
    if (Ev->Type == EV_MESSAGE) {
        if (Ev->Message == WM_CLOSE) {
            // Handle window close event by breaking the application loop
            BreakApp(o->AppOwner);
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void TimerPoll(void* A) {
    // Redraw the widget associated with the console box
    WidgetDraw(WIDGET(CONSOLE(A)->box), NULL);
}

////////////////////////////////////////////////////////////////////////////////
void TimerPoll1(void* A) {
    // Toggle the console cursor visibility flag
    CONSOLE(A)->flags ^= 1;
}

////////////////////////////////////////////////////////////////////////////////
void ConsoleDraw(PWidget o, p_bitmap buffer, PRect w) {
    FONT* f = font;
    PConsole Cons = CONWDG(o)->o;
    l_text c = *Cons->VideoMem; 
    l_text col = *Cons->VideoMem1;
    l_long sx = o->Absolute.a.x, sy = o->Absolute.a.y, x = 0, y = 0;

    while (y < 24) {
        sx = o->Absolute.a.x;
        x = 0;
        while (x < 80) {
            // Draw the background color
            rectfill(buffer, sx, sy, sx + 8, sy + 8, Palette[(*col >> 4) & 0xF]);
            // Render the character with the foreground color
            f->vtable->render_char(f, *c, Palette[*col & 0xF], -1, buffer, sx, sy);
            if ((c == Cons->cur) && (Cons->flags & 1) && Cons->cursor) {
                xor_mode(TRUE);
                if (Cons->cursor == 1)
                    rectfill(buffer, sx, sy + 6, sx + 8, sy + 8, Palette[Cons->Color & 0xF]);
                else if (Cons->cursor == 2)
                    rectfill(buffer, sx, sy + 4, sx + 8, sy + 8, Palette[Cons->Color & 0xF]);
                else
                    rectfill(buffer, sx, sy, sx + 8, sy + 8, Palette[Cons->Color & 0xF]);
                xor_mode(FALSE);
            }
            c++;
            col++;
            sx += 8;
            x++;
        }
        sy += 8;
        y++;
    }
}

////////////////////////////////////////////////////////////////////////////////
l_bool ConsoleEventHandler(PWidget o, PEvent Ev) {
    if (Ev->Type == EV_KEYBOARD) {
        PConsole Cons = CONWDG(o)->o;

        // Store the key pressed in the key buffer
        Cons->KeyBuffer[Cons->KeyNb] = Ev->Keyb.Key;

        // Handle the Enter key separately by replacing its scancode with a newline character
        if (SCANCODE(Ev, KEY_ENTER))
            Cons->KeyBuffer[Cons->KeyNb] = ((Cons->KeyBuffer[Cons->KeyNb]) & ~0xFF) | '\n';

        Cons->KeyNb++;

        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void ClearConsole(PConsole Cons) {
    // Clear the console's video memory and set the default color
    memset(*Cons->VideoMem, ' ', 80 * 24);
    memset(*Cons->VideoMem1, Cons->Color, 80 * 24);
}

////////////////////////////////////////////////////////////////////////////////
void SetPos(PConsole Cons, l_long x, l_long y) {
    // Set the cursor position within the console bounds
    x = min(max(x, 0), 80);
    y = min(max(y, 0), 24);
    Cons->cur = (*Cons->VideoMem) + x + (y * 80);
    Cons->cur1 = (*Cons->VideoMem1) + x + (y * 80);
}

////////////////////////////////////////////////////////////////////////////////
void CursorMoveRight(PConsole o) {
    // Move the cursor to the right
    o->cur++;
    o->cur1++;

    if (o->cur >= (*o->VideoMem) + (80 * 24)) {
        // Handle scrolling when the cursor reaches the end of the screen
        o->cur -= 80;
        o->cur1 -= 80;
        memcpy((*o->VideoMem), (*o->VideoMem) + 80, 80 * 23);
        memcpy((*o->VideoMem1), (*o->VideoMem1) + 80, 80 * 23);
        memset((*o->VideoMem) + (80 * 23), ' ', 80);
        memset((*o->VideoMem1) + (80 * 23), o->Color, 80);
    }
}

////////////////////////////////////////////////////////////////////////////////
void CursorMoveLeft(PConsole o) {
    // Move the cursor to the left
    if (o->cur == (*o->VideoMem)) return;

    o->cur--;
    o->cur1--;
}

////////////////////////////////////////////////////////////////////////////////
void PutChar(PConsole o, l_char c) {
    // Place a character at the current cursor position
    if (c == '\n') {
        o->cur += 80 - ((o->cur - (*o->VideoMem)) % 80);
        o->cur1 += 80 - ((o->cur1 - (*o->VideoMem1)) % 80);
    } else {
        *o->cur = c;
        *o->cur1 = o->Color;
        o->cur++;
        o->cur1++;
    }

    if (o->cur >= (*o->VideoMem) + (80 * 24)) {
        // Handle scrolling when the cursor reaches the end of the screen
        o->cur -= 80;
        o->cur1 -= 80;
        memcpy((*o->VideoMem), (*o->VideoMem) + 80, 80 * 23);
        memcpy((*o->VideoMem1), (*o->VideoMem1) + 80, 80 * 23);
        memset((*o->VideoMem) + (80 * 23), ' ', 80);
        memset((*o->VideoMem1) + (80 * 23), o->Color, 80);
    }
}

////////////////////////////////////////////////////////////////////////////////
PConsole NewConsole(PApplication App) {
    // Create a new console window
    TRect r;
    PConsole Cons;

    if (App->Cons) return App->Cons;
    if (App->Host) return NewConsole(App->Host);

    Cons = malloc(sizeof(TConsole));
    memset(Cons, 0, sizeof(TConsole));
    Cons->cursor = 1;
    Cons->Color = 0x07;
    ClearConsole(Cons);
    RectAssign(&r, 0, 0, 80 * 8, 24 * 8);
    Cons->w = CreateWindow(App, r, App->Name, WF_FRAME | WF_CAPTION | WF_MAXIMIZE| WF_MINIMIZE | WF_RESIZE | WF_CENTERED);
    WIDGET(Cons->w)->AppEvHdl = &AppEventHandler;
    InsertWidget(WIDGET(DeskTop), WIDGET(Cons->w));
    RectAssign(&r, 0, 0, 80 * 8, 24 * 8);
    Cons->box = malloc(sizeof(TConWdg));
    memset(Cons->box, 0, sizeof(TConWdg));
    IntialiseWidget(App, StdWidgetClass, WIDGET(Cons->box), r);
    WIDGET(Cons->box)->Draw = &ConsoleDraw;
    WIDGET(Cons->box)->EventHandler = &ConsoleEventHandler;
    WIDGET(Cons->box)->Flags |= WFForceBuffer | WFFocusable;
    Cons->box->o = Cons;
    InsertWidget(WIDGET(Cons->w), WIDGET(Cons->box));
    WidgetDrawAll(WIDGET(Cons->w));
    Cons->t = NewTimer(App, 20, &TimerPoll, Cons);
    Cons->t1 = NewTimer(App, 500, &TimerPoll1, Cons);
    Cons->cur = *Cons->VideoMem;
    Cons->cur1 = *Cons->VideoMem1;

    return Cons;
}

////////////////////////////////////////////////////////////////////////////////
l_int ConsoleStdOutFileWrite(PFile f, void* d, l_ulong s) {
    // Write to the console's standard output
    l_ulong p = s;
    int i = 0;

    if (!APPLICATION(f->Ex1)->Cons) {
        APPLICATION(f->Ex1)->Cons = NewConsole(APPLICATION(f->Ex1));
        if (!APPLICATION(f->Ex1)->Cons) return 0;
    }

    while (p--) {
        PutChar(APPLICATION(f->Ex1)->Cons, ((l_text)d)[i]);
        i++;
    }
    return s;
}

////////////////////////////////////////////////////////////////////////////////
int GetKey(PApplication App) {
    // Get a key from the console's input buffer
    PConsole Cons = App->Cons;
    int v;
    if (!Cons) return 0;
    while (!Cons->KeyNb) { SysPoll(); }
    v = Cons->KeyBuffer[0];
    Cons->KeyNb--;
    if (Cons->KeyNb)
        memcpy(Cons->KeyBuffer, (void*)((l_ulong)Cons->KeyBuffer) + sizeof(int), Cons->KeyNb * sizeof(int));
    return v;
}

////////////////////////////////////////////////////////////////////////////////
l_int ConsoleStdInFileRead(PFile f, void* d, l_ulong s) {
    // Read from the console's standard input
    PApplication App = APPLICATION(f->Ex1);
    int i = 0;
    while (s) {
        *((l_text)d++) = GetKey(App) & 0xFF;
        ((l_text)d)[i] = GetKey(App) & 0xFF;
        s--;
        i++;
    }
    return s;
}

////////////////////////////////////////////////////////////////////////////////
int Printf(PApplication App, l_text Format, ...) {
    // Print formatted text to the console
    l_char Buffer[16384];
    va_list argptr;
    va_start(argptr, Format);
    vsprintf(Buffer, Format, argptr);
    va_end(argptr);

    if (!App->Cons) {
        App->Cons = NewConsole(App);
        if (!App->Cons) return 0;
    }

    int buflen = sizeof(Buffer) / sizeof(l_char);
    int i = 0;

    while (Buffer[i]) {
        DebugMessage("%dth char: %c\n", i, Buffer[i]);
        PutChar(App->Cons, Buffer[i]);
        i++;
    }

    return buflen - i;
}

////////////////////////////////////////////////////////////////////////////////
l_text Prompt(PApplication App) {
    // Prompt the user for input
    l_text t = TextDup("");
    l_text p = t;
    l_int c;

    if (!App->Cons) {
        App->Cons = NewConsole(App);
        if (!App->Cons) return 0;
    }

    while (((c = GetKey(App)) & 0xFF) != '\n') {

        switch (c >> 8) {

        case KEY_LEFT: {
            if (p != t) {
                CursorMoveLeft(App->Cons);
                p--;
            }
        } break;

        case KEY_RIGHT: {
            if (p != t + TextLen(t)) {
                CursorMoveRight(App->Cons);
                p++;
            }
        } break;

        case KEY_BACKSPACE: {
            if (p != t) {
                l_text tmp = TextDup(p);
                l_text ex = realloc(t, TextLen(t));
                TextCopy(ex + (p - t - 1), tmp);
                free(tmp);
                p = ex + (p - t - 1);
                t = ex;

                if (p != t + TextLen(t)) {
                    l_text a = p, b = p;
                    CursorMoveLeft(App->Cons);
                    while (*a) {
                        PutChar(App->Cons, *a);
                        a++;
                    }
                    PutChar(App->Cons, ' ');
                    while (*(b++)) CursorMoveLeft(App->Cons);
                    CursorMoveLeft(App->Cons);
                } else {
                    CursorMoveLeft(App->Cons);
                    PutChar(App->Cons, ' ');
                    CursorMoveLeft(App->Cons);
                }
            }
        } break;

        case KEY_DEL: {
            if (p != t) {
                l_text tmp = TextDup(p + 1);
                l_text ex = realloc(t, TextLen(t));
                TextCopy(ex + (p - t), tmp);
                free(tmp);
                p = ex + (p - t);
                t = ex;
                if (p != t + TextLen(t)) {
                    l_text a = p, b = p;
                    while (*a) {
                        PutChar(App->Cons, *a);
                        a++;
                    }
                    PutChar(App->Cons, ' ');
                    while (*(b++)) CursorMoveLeft(App->Cons);
                    CursorMoveLeft(App->Cons);
                } else {
                    PutChar(App->Cons, ' ');
                    CursorMoveLeft(App->Cons);
                }
            }
        } break;

        default: if (c & 0xFF) {
            l_text tmp = TextDup(p);
            l_text ex = realloc(t, TextLen(t) + 2);
            TextCopy(ex + (p - t + 1), tmp);
            free(tmp);
            PutChar(App->Cons, c & 0xFF);
            *(ex + (p - t)) = c & 0xFF;
            p = ex + (p - t + 1);
            t = ex;
            if (p != t + TextLen(t)) {
                l_text a = p, b = p;
                while (*a) {
                    PutChar(App->Cons, *a);
                    a++;
                }
                while (*(b++)) CursorMoveLeft(App->Cons);
            } else
                *p = 0;
        } break;
        }
    }

    return t;
}

////////////////////////////////////////////////////////////////////////////////
void _CloseConsole(PConsole Cons) {
    // Close the console and release resources
    DebugMessage("_CloseConsole(%x)", Cons);
    KillTimer(Cons->t);
    KillTimer(Cons->t1);
    WidgetDispose(WIDGET(Cons->w));
    free(Cons);
    DebugMessage("_CloseConsole return;");
}

////////////////////////////////////////////////////////////////////////////////
void CloseConsole(PApplication App) {
    // Close the console associated with the application
    if (App->Cons) _CloseConsole(App->Cons);
    App->Cons = NULL;
}

////////////////////////////////////////////////////////////////////////////////
TFileDriver ConsoleStdOut = { "Console stdout",
    NULL,NULL,NULL,
    &ConsoleStdOutFileWrite,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };

////////////////////////////////////////////////////////////////////////////////
TDrive ConsoleStrOutDrv = {
    "Console stdout",
    0,0,0,NULL,NULL,
    &ConsoleStdOut
};

////////////////////////////////////////////////////////////////////////////////
TFileDriver ConsoleStdIn = { "Console stdin",
    NULL,NULL,
    &ConsoleStdInFileRead,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };

////////////////////////////////////////////////////////////////////////////////
TDrive ConsoleStdInDrv = {
    "Console stdin",
    0,0,0,NULL,NULL,
    &ConsoleStdIn
};

////////////////////////////////////////////////////////////////////////////////
void _ExternConsolePreInit(PApplication App) {
    // Pre-initialize the console for the application
    DebugMessage("_ExternConsolePreInit %x;", App);
    if (App->Host) {
        App->Cons = App->Host->Cons;
    }
    App->StdOut = malloc(sizeof(TFile));
    memset(App->StdOut, 0, sizeof(TFile));
    App->StdOut->Drive = &ConsoleStrOutDrv;
    App->StdOut->Ex1 = App;

    App->StdIn = malloc(sizeof(TFile));
    memset(App->StdIn, 0, sizeof(TFile));
    App->StdIn->Drive = &ConsoleStdInDrv;
    App->StdIn->Ex1 = App;
}

////////////////////////////////////////////////////////////////////////////////
void _ExternConsolePreUnInit(PApplication App) {
    // Uninitialize the console for the application
    DebugMessage("_ExternConsolePreUnInit %x;", App);
    if (!App->Host) {
        if (App->Cons) _CloseConsole(App->Cons);
        App->Cons = NULL;
    }

    free(App->StdOut);
    free(App->StdIn);
    App->StdOut = NULL;
    App->StdIn = NULL;
}

////////////////////////////////////////////////////////////////////////////////
PAppExtender ConsoleExtender = NULL;

l_int LibMain(int argc, l_text* argv) {
    // Initialize the console library
    Palette[0] = makecol(0, 0, 0);
    Palette[1] = makecol(0, 0, 128);
    Palette[2] = makecol(0, 128, 0);
    Palette[3] = makecol(0, 128, 128);
    Palette[4] = makecol(128, 0, 0);
    Palette[5] = makecol(128, 0, 128);
    Palette[6] = makecol(128, 128, 0);
    Palette[7] = makecol(192, 192, 192);
    Palette[8] = makecol(128, 128, 128);
    Palette[9] = makecol(0, 0, 255);
    Palette[10] = makecol(0, 255, 0);
    Palette[11] = makecol(0, 255, 255);
    Palette[12] = makecol(255, 0, 0);
    Palette[13] = makecol(255, 0, 255);
    Palette[14] = makecol(255, 255, 0);
    Palette[15] = makecol(255, 255, 255);

    ConsoleExtender = InstallAppExtender(&_ExternConsolePreInit, &_ExternConsolePreUnInit);

    SYSEXPORT(GetKey);
    SYSEXPORT(Printf);
    SYSEXPORT(Prompt);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void Close(void) {
    // Clean up and remove the console extender
    RemoveAppExtender(ConsoleExtender);
}
////////////////////////////////////////////////////////////////////////////////
