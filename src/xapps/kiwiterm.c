////////////////////////////////////////////////////////////////////////////////
//
//  Terminal
//
//  Copyright (c) Aura GUI m6 Terminal. v2
//
////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "console.h"
#include "debug.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include "command.h"

// Added for copy to directory support
#include <sys/stat.h>

#ifndef S_IFDIR
#define S_IFDIR 0040000  // This is a common UNIX value, but check DJGPP's documentation
#endif

int mkdir(const char *pathname, mode_t mode); // <-- This was originally in your code.

l_ulong AppVersion = ULONG_ID(0,0,5,4);
char AppName[] = "Terminal";
l_uid NeededLibs[] = { "conlib", "" };

/* UPDATES

0039 cd path logic added
0400 dir + ls added
0046 tidy up + cp (everything working good)
0048 added echo 
0049 added runscript
0050 runscript without c99
30 aug

0051 copy to folder support added?
unless you specify the full path of the file and folder destination it wasnt working.

dir /w & /d (for directories only) fixed 

0052 moved the commands to a new header and c file.
0053 added meminfo via sys.c working fine
0054 added more information to the meminfo function
0055 trying to add cpu load via cpux tsr @ meminfo in sys.c

l_text _ResolveFileName(l_text File) {
    // ... (No changes here) ...
    return File;
}

////////////////////////////////////////////////////////////////////////////////

l_text ResolveFileName(l_text File) {
    return _ResolveFileName(TextDup(File));
}

////////////////////////////////////////////////////////////////////////////////

PApplication Me; // Added to use the console library

l_int Main(int argc, l_text *argv) {
    l_text a;
    l_text CurrentDir = TextDup("/system");
    l_text *new_argv;
    l_int new_argc;

    // Initialize the console
    Me = CreateApplication(AppName, AppVersion, NeededLibs);
    if (!Me) {
        printf("Failed to create application.\n");
        return 1;
    }

    // Create a new console window for the application
    if (!Me->Cons) {
        Me->Cons = NewConsole(Me);
        if (!Me->Cons) {
            printf("Failed to create console.\n");
            return 1;
        }
    }

    do {
        Printf(Me, "%s>", CurrentDir);
        a = Prompt(Me);
        _ParseArgs(a, NULL, &new_argc, &new_argv);
        Printf(Me, "\n");

        if (new_argc) {
            if (!TextCaseCompare(new_argv[0], "exit")) {
                CommandExit();
            } else if (!TextCaseCompare(new_argv[0], "cd")) {
                CommandCd(new_argc, new_argv, CurrentDir);
            } else if (!TextCaseCompare(new_argv[0], "dir") || !TextCaseCompare(new_argv[0], "ls")) {
                CommandDir(new_argc, new_argv, CurrentDir);
            } else if (!TextCaseCompare(new_argv[0], "mkdir")) {
                CommandMkdir(new_argc, new_argv, CurrentDir);
            } else if (!TextCaseCompare(new_argv[0], "copy") || !TextCaseCompare(new_argv[0], "cp")) {
                CommandCopy(new_argc, new_argv);
            } else if (!TextCaseCompare(new_argv[0], "help")) {
                CommandHelp();
            } else if (!TextCaseCompare(new_argv[0], "lspci")) {
                CommandLsPci();
            } else if (!TextCaseCompare(new_argv[0], "echo")) {
                CommandEcho(new_argc, new_argv);
            } else if (!TextCaseCompare(new_argv[0], "runscript")) {
                CommandRunScript(new_argc, new_argv);
            } else if (!TextCaseCompare(new_argv[0], "meminfo")) {
                display_memory_info();
                 
            } else {
                // The rest of your commands can follow this pattern. You'd replace the body
                // of each command's logic with a call to the appropriate function from command.h.
                Printf(Me, "Unknown command : %s\n", new_argv[0]);
            }

            FreeArgs(new_argc, new_argv);
            free(a);
        }

    } while (1);
    
    return false;
}

////////////////////////////////////////////////////////////////////////////////

void Close(void) {
    // Clean up the console and application
    if (Me) {
        if (Me->Cons) {
            CloseConsole(Me);
        }
        DestroyApplication(Me);
    }
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//  command.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef COMMAND_H
#define COMMAND_H

void CommandExit(void);
void CommandCd(int argc, char **argv, char *currentDir);
void CommandDir(int argc, char **argv, char *currentDir);
void CommandMkdir(int argc, char **argv, char *currentDir);
void CommandCopy(int argc, char **argv);
void CommandHelp(void);
void CommandLsPci(void);
void CommandEcho(int argc, char **argv);
void CommandRunScript(int argc, char **argv);
void display_memory_info(void);

#endif // COMMAND_H

////////////////////////////////////////////////////////////////////////////////
//
//  command.c
//
////////////////////////////////////////////////////////////////////////////////

#include "command.h"
#include "kernel.h"
#include "console.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include <sys/stat.h>

// Added for copy to directory support
#ifndef S_IFDIR
#define S_IFDIR 0040000  // This is a common UNIX value, but check DJGPP's documentation
#endif

int mkdir(const char *pathname, mode_t mode);

extern PApplication Me;

void CommandExit(void) {
    Close();
    exit(0);
}

void CommandCd(int argc, char **argv, char *currentDir) {
    if (argc != 2) {
        Printf(Me, "Syntax : CD <path>\n");
        return;
    }
    l_text np, r;
    PFileInfo i;

    if (*argv[1] == '/') {
        r = TextDup(argv[1]);
    } else {
        np = FileNameToPath(currentDir, argv[1]);
        r = ResolveFileName(np);
        free(np);
    }

    i = FileGetInfo(r);

    if (i) {
        free(currentDir);
        currentDir = TextDup(r);
        FreeFileInfo(i);
    } else {
        Printf(Me, "Unknown directory : %s\n", r);
    }

    free(r);
}

void CommandDir(int argc, char **argv, char *currentDir) {
    PList l = ListDir(currentDir, FA_DIREC | FA_LABEL);
    PListItem a, b;

    Printf(Me, "Listing of %s\n\n", currentDir);

    if (l->Last) {
        a = b = l->Last->Next;
        l_ulong n = 0;
        do {
            if (FILEINFO(a->Data)->Attributes & FA_DIREC)
                Printf(Me, "[dir]");
            else
                Printf(Me, "     ");

            Printf(Me, "%s\n", FILEINFO(a->Data)->Name);
            n++;
            a = a->Next;
        } while (a != b);
        Printf(Me, "Total : %d files and directories\n\n", n);
    } else {
        Printf(Me, "No files\n\n");
    }
}

void CommandMkdir(int argc, char **argv, char *currentDir) {
    if (argc != 2) {
        Printf(Me, "Syntax : MKDIR <directory_name>\n");
        return;
    }
    l_text np, r;

    if (*argv[1] == '\\') {
        r = TextDup(argv[1]);
    } else {
        np = FileNameToPath(currentDir, argv[1]);
        r = ResolveFileName(np);
        free(np);
    }

    if (mkdir(r) != 0) {
        Printf(Me, "Error: Unable to create directory '%s'. Reason: %s\n", r, strerror(errno));
    } else {
        Printf(Me, "Directory created: %s\n", r);
    }

    free(r);
}

void CommandCopy(int argc, char **argv) {
    if (argc != 3) {
        Printf(Me, "Syntax : COPY <source_file> <destination_file>\n");
        return;
    }
    FILE *sourceFile, *destFile;
    int ch;

    sourceFile = fopen(argv[1], "rb");
    if (sourceFile == NULL) {
        Printf(Me, "Error: Unable to open source file '%s'. Reason: %s\n", argv[1], strerror(errno));
        return;
    }

    destFile = fopen(argv[2], "wb");
    if (destFile == NULL) {
        Printf(Me, "Error: Unable to open destination file '%s'. Reason: %s\n", argv[2], strerror(errno));
        fclose(sourceFile);
        return;
    }

    while ((ch = fgetc(sourceFile)) != EOF) {
        fputc(ch, destFile);
    }

    fclose(sourceFile);
    fclose(destFile);
    Printf(Me, "File copied successfully.\n");
}

void CommandHelp(void) {
    Printf(Me, "Available commands:\n");
    Printf(Me, "  exit\n");
    Printf(Me, "  cd <path>\n");
    Printf(Me, "  dir / ls\n");
    Printf(Me, "  mkdir <directory_name>\n");
    Printf(Me, "  copy / cp <source_file> <destination_file>\n");
    Printf(Me, "  help\n");
    Printf(Me, "  lspci\n");
    Printf(Me, "  echo <text>\n");
    Printf(Me, "  runscript <file>\n");
    Printf(Me, "  meminfo\n");
}

void CommandLsPci(void) {
    // Implement the logic for lspci here
    Printf(Me, "lspci command is not implemented yet.\n");
}

void CommandEcho(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        Printf(Me, "%s ", argv[i]);
    }
    Printf(Me, "\n");
}

void CommandRunScript(int argc, char **argv) {
    if (argc != 2) {
        Printf(Me, "Syntax : RUNSCRIPT <script_file>\n");
        return;
    }
    FILE *scriptFile;
    l_text line = NULL;
    size_t len = 0;
    ssize_t read;

    scriptFile = fopen(argv[1], "r");
    if (scriptFile == NULL) {
        Printf(Me, "Error: Unable to open script file '%s'. Reason: %s\n", argv[1], strerror(errno));
        return;
    }

    while ((read = getline(&line, &len, scriptFile)) != -1) {
        Printf(Me, "Executing: %s", line);
        _ParseArgs(line, NULL, &argc, &argv);
        if (argc) {
            if (!TextCaseCompare(argv[0], "exit")) {
                CommandExit();
            } else if (!TextCaseCompare(argv[0], "cd")) {
                CommandCd(argc, argv, "/system");
            } else if (!TextCaseCompare(argv[0], "dir") || !TextCaseCompare(argv[0], "ls")) {
                CommandDir(argc, argv, "/system");
            } else if (!TextCaseCompare(argv[0], "mkdir")) {
                CommandMkdir(argc, argv, "/system");
            } else if (!TextCaseCompare(argv[0], "copy") || !TextCaseCompare(argv[0], "cp")) {
                CommandCopy(argc, argv);
            } else if (!TextCaseCompare(argv[0], "help")) {
                CommandHelp();
            } else if (!TextCaseCompare(argv[0], "lspci")) {
                CommandLsPci();
            } else if (!TextCaseCompare(argv[0], "echo")) {
                CommandEcho(argc, argv);
            } else if (!TextCaseCompare(argv[0], "runscript")) {
                CommandRunScript(argc, argv);
            } else if (!TextCaseCompare(argv[0], "meminfo")) {
                display_memory_info();
            } else {
                Printf(Me, "Unknown command : %s\n", argv[0]);
            }
            FreeArgs(argc, argv);
        }
    }

    free(line);
    fclose(scriptFile);
}

void display_memory_info(void) {
    // Implement the logic to display memory info here
    Printf(Me, "Memory info command is not implemented yet.\n");
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//  console.c (Console Library Integration)
//
////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "window.h"
#include "label.h"
#include "internal.h"
#include "console.h"
#include "allegro/internal/aintern.h"

l_ulong AppVersion	= ULONG_ID(0,1,0,0);
char AppName[]		= "Console Library";
l_uid nUID			= "conlib";
l_uid NeededLibs[] = { "window","widget","label","" };

l_color Palette[16];
////////////////////////////////////////////////////////////////////////////////
l_bool AppEventHandler ( PWidget o, PEvent Ev )
{
	if ( Ev->Type == EV_MESSAGE )
	{
		if ( Ev->Message == WM_CLOSE )
		{
			BreakApp(o->AppOwner);
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
void TimerPoll(void* A) {
	WidgetDraw(WIDGET(CONSOLE(A)->box), NULL);
}
////////////////////////////////////////////////////////////////////////////////
void TimerPoll1(void* A) {
	CONSOLE(A)->flags ^= 1;
}
////////////////////////////////////////////////////////////////////////////////
void ConsoleDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	FONT *f = font;
	PConsole Cons = CONWDG(o)->o;
	l_text c = *Cons->VideoMem; l_text col = *Cons->VideoMem1;
	l_long sx = o->Absolute.a.x, sy = o->Absolute.a.y,x=0,y=0;

	while ( y < 24 ) {
		sx = o->Absolute.a.x;
		x=0;
		while ( x < 80 ) {
			rectfill(buffer, sx, sy, sx+8, sy+8, Palette[ (*col >> 4 ) & 0x7 ]);
			f->vtable->render_char( f,*c,Palette[*col & 0xF],-1,buffer,sx,sy);
			if ( (c == Cons->cur) && (Cons->flags & 1) && Cons->cursor ) {
	   			xor_mode(TRUE);
	   			if ( Cons->cursor == 1 )
	    			rectfill(buffer, sx, sy+6, sx+8, sy+8, Palette[Cons->Color & 0xF] );
	    		else if ( Cons->cursor == 2 )
	    			rectfill(buffer, sx, sy+4, sx+8, sy+8, Palette[Cons->Color & 0xF] );
	    		else
	    			rectfill(buffer, sx, sy, sx+8, sy+8, Palette[Cons->Color & 0xF] );
		  		xor_mode(FALSE);
			}
			c++;
			col++;
			sx+=8;
			x++;
		}
		sy+=8;
		y++;
	}
}
////////////////////////////////////////////////////////////////////////////////
l_bool ConsoleEventHandler ( PWidget o, PEvent Ev )
{
	if ( Ev->Type == EV_KEYBOARD )
	{
		PConsole Cons = CONWDG(o)->o;
		Cons->KeyBuffer[Cons->KeyNb] = Ev->Keyb.Key;

		if (SCANCODE(Ev, KEY_ENTER))
			Cons->KeyBuffer[Cons->KeyNb] = ((Cons->KeyBuffer[Cons->KeyNb]) & ~0xFF )| '\n';

		Cons->KeyNb++;
		return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
void ClearConsole ( PConsole Cons ) {
	memset(*Cons->VideoMem,' ',80*24);
	memset(*Cons->VideoMem1,Cons->Color,80*24);
}
////////////////////////////////////////////////////////////////////////////////
void SetPos ( PConsole Cons, l_long x, l_long y ) {
	x = min(max(x,0),80);
	y = min(max(y,0),24);
	Cons->cur  = (*Cons->VideoMem)+x+(y*80);
	Cons->cur1 = (*Cons->VideoMem1)+x+(y*80);
}
////////////////////////////////////////////////////////////////////////////////
void CursorMoveRight ( PConsole o ) {
	o->cur++;
	o->cur1++;
	if ( o->cur >= (*o->VideoMem)+(80*24) ) {
		o->cur -= 80;
		o->cur1 -= 80;
		memcpy((*o->VideoMem),(*o->VideoMem)+80,80*23);
		memcpy((*o->VideoMem1),(*o->VideoMem1)+80,80*23);
		memset((*o->VideoMem)+(80*23),' ',80);
		memset((*o->VideoMem1)+(80*23),o->Color,80);
	}
}
////////////////////////////////////////////////////////////////////////////////
void CursorMoveLeft ( PConsole o ) {
	if ( o->cur == (*o->VideoMem) ) return;
	o->cur--;
	o->cur1--;
}
////////////////////////////////////////////////////////////////////////////////
void PutChar ( PConsole o, l_char c ) {
	if ( c == '\n' ) {
		o->cur += 80-((o->cur-(*o->VideoMem))%80);
		o->cur1 += 80-((o->cur1-(*o->VideoMem1))%80);
	} else {
		*o->cur = c;
		*o->cur1 = o->Color;
		o->cur++;
		o->cur1++;
 	}
	if ( o->cur >= (*o->VideoMem)+(80*24) ) {
		o->cur -= 80;
		o->cur1 -= 80;
		memcpy((*o->VideoMem),(*o->VideoMem)+80,80*23);
		memcpy((*o->VideoMem1),(*o->VideoMem1)+80,80*23);
		memset((*o->VideoMem)+(80*23),' ',80);
		memset((*o->VideoMem1)+(80*23),o->Color,80);
	}
}

////////////////////////////////////////////////////////////////////////////////
PConsole NewConsole ( PApplication App ) {
	TRect r;
	PConsole Cons;

	if ( App->Cons ) return App->Cons;
	if ( App->Host ) return NewConsole(App->Host);

	Cons = malloc(sizeof(TConsole));
	memset(Cons,0,sizeof(TConsole));
	Cons->cursor = 1;
	Cons->Color = 0x07;
	ClearConsole(Cons);
	RectAssign(&r,0, 0, 80*8, 24*8);
	Cons->w = CreateWindow(App, r, App->Name, WF_FRAME|WF_CAPTION|WF_MINIMIZE|WF_CENTERED);
	WIDGET(Cons->w)->AppEvHdl = &AppEventHandler;
	InsertWidget(WIDGET(DeskTop), WIDGET(Cons->w));
	RectAssign(&r,0, 0, 80*8, 24*8);
	Cons->box = malloc(sizeof(TConWdg));
	memset(Cons->box,0,sizeof(TConWdg));
	IntialiseWidget(App,StdWidgetClass,WIDGET(Cons->box),r);
	WIDGET(Cons->box)->Draw = &ConsoleDraw;
	WIDGET(Cons->box)->EventHandler = &ConsoleEventHandler;
	WIDGET(Cons->box)->Flags |= WFForceBuffer|WFFocusable;
	Cons->box->o = Cons;
	InsertWidget(WIDGET(Cons->w), WIDGET(Cons->box));
	WidgetDrawAll(WIDGET(Cons->w));
	Cons->t = NewTimer(App,20, &TimerPoll,Cons);
	Cons->t1 = NewTimer(App,500, &TimerPoll1,Cons);
	Cons->cur = *Cons->VideoMem;
	Cons->cur1 = *Cons->VideoMem1;

	return Cons;
}
////////////////////////////////////////////////////////////////////////////////
l_int   ConsoleStdOutFileWrite ( PFile f, void *d, l_ulong s ) {
	l_ulong p = s;
        int i=0;

	if ( !APPLICATION(f->Ex1)->Cons ) {
		APPLICATION(f->Ex1)->Cons = NewConsole(APPLICATION(f->Ex1));
		if ( !APPLICATION(f->Ex1)->Cons ) return 0;
	}

	while ( p-- ) {
		PutChar(APPLICATION(f->Ex1)->Cons,((l_text)d)[i]);
                i++;
	}
	return s;
}
////////////////////////////////////////////////////////////////////////////////
int GetKey ( PApplication App ) {
	PConsole Cons =	App->Cons;
	int v;
	if ( !Cons ) return 0;
	while ( !Cons->KeyNb ) { SysPoll(); }
	v = Cons->KeyBuffer[0];
	Cons->KeyNb--;
	if ( Cons->KeyNb )
		memcpy(Cons->KeyBuffer,(void*)((l_ulong)Cons->KeyBuffer)+sizeof(int),Cons->KeyNb*sizeof(int));
	return v;
}
////////////////////////////////////////////////////////////////////////////////
l_int ConsoleStdInFileRead ( PFile f, void *d, l_ulong s ) {
	PApplication App = APPLICATION(f->Ex1);
        int i=0;
	while ( s ) {
		*((l_text)d++) = GetKey(App) & 0xFF;
		( (l_text)d)[i] = GetKey(App) & 0xFF;
		s--;
                i++;
	}
	return s;
}

////////////////////////////////////////////////////////////////////////////////
int Printf(PApplication App, l_text Format, ... ) {
	l_char Buffer[16384];
	va_list argptr;
	va_start(argptr, Format);
	vsprintf(Buffer, Format, argptr);
	va_end(argptr);

	if ( !App->Cons ) {
		App->Cons = NewConsole(App);
		if ( !App->Cons ) return 0;
	}

    int buflen = sizeof(Buffer) / sizeof(l_char);
    int i = 0;

	while ( Buffer[i] )
	{
		DebugMessage("%dth char: %c\n", i, Buffer[i]);
		PutChar(App->Cons, Buffer[i]);
		i++;
	}

	return buflen - i;
}
////////////////////////////////////////////////////////////////////////////////
l_text Prompt ( PApplication App ) {
	l_text t = TextDup("");
	l_text p = t;
	l_int c;

	if ( !App->Cons ) {
		App->Cons = NewConsole(App);
		if ( !App->Cons ) return 0;
	}

	while ( ((c = GetKey(App)) & 0xFF) != '\n' ) {

		switch ( c >> 8 ) {

			case KEY_LEFT : {
				if ( p != t ) {
					CursorMoveLeft(App->Cons);
					p--;
				}
			} break;

			case KEY_RIGHT : {
				if ( p != t+TextLen(t) ) {
					CursorMoveRight(App->Cons);
					p++;
				}
			} break;

			case KEY_BACKSPACE : {

				if ( p != t ) {

					l_text tmp = TextDup(p);
					l_text ex = realloc(t,TextLen(t));
					TextCopy(ex+(p-t-1),tmp);
					free(tmp);
					p = ex+(p-t-1);
					t = ex;

					if ( p != t+TextLen(t) ) {
						l_text a = p, b = p;
						CursorMoveLeft(App->Cons);
						while ( *a ) {
							PutChar(App->Cons,*a);
							a++;
						}
						PutChar(App->Cons,' ');
						while ( *(b++) ) CursorMoveLeft(App->Cons);
						CursorMoveLeft(App->Cons);
					} else {
						CursorMoveLeft(App->Cons);
						PutChar(App->Cons,' ');
						CursorMoveLeft(App->Cons);
					}

				}
			} break;

			case KEY_DEL : {

				if ( p != t ) {
					l_text tmp = TextDup(p+1);
					l_text ex = realloc(t,TextLen(t));
					TextCopy(ex+(p-t),tmp);
					free(tmp);
					p = ex+(p-t);
					t = ex;
					if ( p != t+TextLen(t) ) {
						l_text a = p, b = p;
						while ( *a ) {
							PutChar(App->Cons,*a);
							a++;
						}
						PutChar(App->Cons,' ');
						while ( *(b++) ) CursorMoveLeft(App->Cons);
						CursorMoveLeft(App->Cons);
					} else {
						PutChar(App->Cons,' ');
						CursorMoveLeft(App->Cons);
					}
				}
			} break;

			default : if ( c & 0xFF ) {
				l_text tmp = TextDup(p);
				l_text ex = realloc(t,TextLen(t)+2);
				TextCopy(ex+(p-t+1),tmp);
				free(tmp);
				PutChar(App->Cons,c & 0xFF);
				*(ex+(p-t)) = c & 0xFF;
				p = ex+(p-t+1);
				t = ex;
				if ( p != t+TextLen(t) ) {
					l_text a = p, b = p;
					while ( *a ) {
						PutChar(App->Cons,*a);
						a++;
					}
					while ( *(b++) ) CursorMoveLeft(App->Cons);
				} else
					*p = 0;


			} break;


		}
	}

	return t;
}
////////////////////////////////////////////////////////////////////////////////
void _CloseConsole ( PConsole Cons ) {
	DebugMessage("_CloseConsole(%x)",Cons);
	KillTimer(Cons->t);
	KillTimer(Cons->t1);
	WidgetDispose(WIDGET(Cons->w));
	free(Cons);
	DebugMessage("_CloseConsole return;");
}
////////////////////////////////////////////////////////////////////////////////
void CloseConsole ( PApplication App ) {

	if ( App->Cons ) _CloseConsole(App->Cons);
	App->Cons = NULL;

}
////////////////////////////////////////////////////////////////////////////////
TFileDriver ConsoleStdOut = { "Console stdout",
	NULL,NULL,NULL,
	&ConsoleStdOutFileWrite,
	NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL
