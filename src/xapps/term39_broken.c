////////////////////////////////////////////////////////////////////////////////
//
//
// Terminal
//
// Copyright (c) Aura GUI m6 Terminal.
//
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

// Avoid including this if you're including <allegro.h> because they seem to conflict
// #include <keyboard.h>
#define clear allegro_clear
#define hline allegro_hline
#define vline allegro_vline

#include "awrap.h"
#include "prap.h"

l_ulong AppVersion = ULONG_ID(0,0,3,9);
char AppName[] = "Terminal";
l_uid NeededLibs[] = { "conlib", "" };

l_text _ResolveFileName(l_text File) {
    // No changes here...
    return File;
}


////////////////////////////////////////////////////////////////////////////////

l_text ResolveFileName(l_text File) {
    return _ResolveFileName(TextDup(File));
}

////////////////////////////////////////////////////////////////////////////////

l_int Main(int arc, l_text *arv) {
    
    l_text a;
    l_text CurrentDir = TextDup("/system");
    l_text *argv;
    l_int argc;
    int ch;
    int pos = 0;
    l_text buffer = (l_text)malloc(256); // or any other appropriate max length
    memset(buffer, 0, 256);

    // PDCURSES
    #define MAX_HISTORY 50 // Store last 50 commands
    l_int HistoryIndex = 0; // Points to where the next command will be stored
    l_int CurrentHistory = -1; // For the user's current position in history
    l_text History[MAX_HISTORY];

    // pdcurses
    initscr();
    cbreak();
    noecho();            // Don't echo while we get chars
    keypad(stdscr, TRUE);
    move(5,5); 
    pdc_hline(ACS_HLINE, 10);

    move(5,5);
    pdc_vline(ACS_VLINE, 10);

    getch(); // Wait for user input
    endwin(); // End PDCurses session

        do {
        Printf(&Me, "%s>", CurrentDir);

        while (1) {
            ch = getch();
            if (ch == KEY_UP) {
                if (CurrentHistory > 0 || (CurrentHistory == -1 && HistoryIndex > 0)) {
                    if (CurrentHistory == -1) CurrentHistory = HistoryIndex - 1;
                    else CurrentHistory--;
                    strncpy(buffer, History[CurrentHistory], 255);
                }
            } else if (ch == KEY_DOWN) {
                if (CurrentHistory != -1 && CurrentHistory < HistoryIndex - 1) {
                    CurrentHistory++;
                    strncpy(buffer, History[CurrentHistory], 255);
                } else {
                    CurrentHistory = -1;
                    memset(buffer, 0, 256);
                }
            } else if (ch == '\n' || ch == '\r') {
                break;
            } else {
                // Append to buffer and print character
                buffer[pos++] = ch;
                addch(ch);
            }
        }

        a = TextDup(buffer);

        _ParseArgs(a, NULL, &argc, &argv);
        Printf(&Me, "\n");



        if (argc) {
            if (!TextCaseCompare(argv[0], "exit")) {
                return false;
            } else if (!TextCaseCompare(argv[0], "cd")) {
                // ... Handling for "cd" ...
                   if (argc != 2) {
                    Printf(&Me, "Syntax : CD <path>\n");
                } else {
                    l_text np, r;
                    PFileInfo i;

                    if (*argv[1] == '/') {
                        r = TextDup(argv[1]);
                    } else {
                        np = FileNameToPath(CurrentDir, argv[1]);
                        r = ResolveFileName(np);
                        free(np);
                    }

                    i = FileGetInfo(r);

                    if (i) {
                        free(CurrentDir);
                        CurrentDir = TextDup(r);
                        FreeFileInfo(i);
                    } else {
                        Printf(&Me, "\nUnknown directory : %s\n\n", r);
                    }

                    free(r);
                }



//pdcurses

                if (HistoryIndex == MAX_HISTORY) {
                   free(History[0]);
                int i;
                for (i = 1; i < MAX_HISTORY; i++) {
                    History[i-1] = History[i];
                                                    }
                    History[MAX_HISTORY - 1] = TextDup(a);
                } else {
    History[HistoryIndex++] = TextDup(a);
                }
                CurrentHistory = -1;  // Reset the history position


if (install_keyboard() != 0) {
    allegro_message("Error initializing keyboard: %s", allegro_error);
    exit(1);
}


            } else if (!TextCaseCompare(argv[0], "dir") || !TextCaseCompare(argv[0], "ls")) {
                // ... Handling for "dir" and "ls" ...
  PList l = ListDir(CurrentDir, FA_DIREC | FA_LABEL);
                PListItem a, b;

                Printf(&Me, "\nListing of %s\n\n", CurrentDir);

                if (l->Last) {
                    a = b = l->Last->Next;
                    l_ulong n = 0;
                    do {
                        if (FILEINFO(a->Data)->Attributes & FA_DIREC) {
                            Printf(&Me, "[dir]");
                        } else {
                            Printf(&Me, "     ");
                        }

                        Printf(&Me, "%s\n", FILEINFO(a->Data)->Name);
                        n++;
                        a = a->Next;
                    } while (a != b);
                    Printf(&Me, "\nTotal : %d files and directories\n\n", n);
                } else {
                    Printf(&Me, "no files\n\n");
                }








            } else if (!TextCaseCompare(argv[0], "mkdir")) {
                // ... Handling for "mkdir" ...
                            if (argc != 2) {
                    Printf(&Me, "Syntax : MKDIR <directory_name>\n");
                } else {
                    l_text np, r;

                    if (*argv[1] == '/') {  // Adjusted for Unix-like paths
                        r = TextDup(argv[1]);
                    } else {
                        np = FileNameToPath(CurrentDir, argv[1]);
                        r = ResolveFileName(np);
                        free(np);
                    }

                    if (mkdir(r) != 0) {
                        Printf(&Me, "Error: Unable to create directory '%s'. Reason: %s\n", r, strerror(errno));
                    } else {
                        Printf(&Me, "Directory created: %s\n", r);
                    }

                    free(r);
                }




            } else if (!TextCaseCompare(argv[0], "copy") || !TextCaseCompare(argv[0], "cp")) {
                // ... Handling for "copy" and "cp" ...
                 if (argc != 3) {
                    Printf(&Me, "Syntax : COPY/CP <source_file> <destination_file>\n");
                } else {
                    FILE *sourceFile, *destFile;
                    int ch;

                    sourceFile = fopen(argv[1], "rb");
                    if (sourceFile == NULL) {
                        Printf(&Me, "Error: Unable to open source file '%s'. Reason: %s\n", argv[1], strerror(errno));
                        continue;  // Adjusted to continue rather than return
                    }

                    destFile = fopen(argv[2], "wb");
                    if (destFile == NULL) {
                        Printf(&Me, "Error: Unable to open destination file '%s'. Reason: %s\n", argv[2], strerror(errno));
                        fclose(sourceFile);
                        continue;  // Adjusted to continue
                    }

                    while ((ch = fgetc(sourceFile)) != EOF) {
                        fputc(ch, destFile);
                    }

                    fclose(sourceFile);
                    fclose(destFile);
                    Printf(&Me, "File copied successfully.\n");
                }





            } else if (!TextCaseCompare(argv[0], "echo")) {
                // ... Handling for "echo" ...
                    int i;
                    if (argc < 2) {
                        Printf(&Me, "\n");
                } else {
                    for (i = 1; i < argc; i++) {
                    if (i > 1) Printf(&Me, " ");
                        Printf(&Me, "%s", argv[i]);
                }
                         Printf(&Me, "\n");
             }

// this works but doesnt execute the files yet
             // add ability to only red the file line by line

            } else if (!TextCaseCompare(argv[0], "run")) {
                // ... Handling for "runscript" ...
                    if (argc != 2) {
                Printf(&Me, "Syntax : RUN <script_filename.bat>\n");
                } else {
                int len = strlen(argv[1]);
                if (len < 4 || strcmp(argv[1] + len - 4, ".bat") != 0) { 
                    Printf(&Me, "Error: The script file must have a '.bat' extension.\n");
                    continue;
                }

                FILE *scriptFile = fopen(argv[1], "r");
                if (scriptFile == NULL) {
                    Printf(&Me, "Error: Unable to open script file '%s'. Reason: %s\n", argv[1], strerror(errno));
                    return;
                }

                // Get the file size
                fseek(scriptFile, 0, SEEK_END);
                long fsize = ftell(scriptFile);
                fseek(scriptFile, 0, SEEK_SET);

                // Read entire file into buffer
                char *buffer = (char*)malloc(fsize + 1);
                fread(buffer, fsize, 1, scriptFile);
                buffer[fsize] = 0; // Null-terminate the buffer
                fclose(scriptFile);

                // Tokenize commands line-by-line
                char *line = strtok(buffer, "\n");
                while (line) {
                    // Handling the command here
                    l_text scriptCommand = TextDup(line);
                    l_text *scriptArgv;
                    l_int scriptArgc;
                    
                    _ParseArgs(scriptCommand, NULL, &scriptArgc, &scriptArgv);
                    // Handle the command here or recursively call your command interpreter function
                    
                    FreeArgs(scriptArgc, scriptArgv);
                    free(scriptCommand);

                    // Get the next line
                    line = strtok(NULL, "\n");
                }

                free(buffer); // Release the buffer memory
            }







            } else if (!TextCaseCompare(argv[0], "del") || !TextCaseCompare(argv[0], "rm")) {
                // ... Handling for "del" and "rm" ...
                if (argc != 2) {
                    Printf(&Me, "Syntax : DEL/RM <filename_or_directory>\n");
                } else {
                    l_text np, r;

                    if (*argv[1] == '/') {
                        r = TextDup(argv[1]);
                    } else {
                        np = FileNameToPath(CurrentDir, argv[1]);
                        r = ResolveFileName(np);
                        free(np);
                    }

                    if (remove(r) != 0) {
                        Printf(&Me, "Error: Unable to delete '%s'. Reason: %s\n", r, strerror(errno));
                    } else {
                        Printf(&Me, "File or directory deleted: %s\n", r);
                    }

                    free(r);


}



            } else {
                Printf(&Me, "Unknown command : %s\n", argv[0]);
            }

            FreeArgs(argc, argv);
            free(a);
        }
   } while (1);

    endwin();       // end PDCurses session
remove_keyboard();

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

void Close(void) {
    l_text History[MAX_HISTORY];

    int i;
    for (i = 0; i < MAX_HISTORY; i++) {
        if (History[i]) {
            free(History[i]);
        }
    }
}