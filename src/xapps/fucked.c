////////////////////////////////////////////////////////////////////////////////

//
//  Terminal
//
//  Copyright (c) Aura GUI m6 Terminal.
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
#include <allegro.h>  // You'll need to include this for Allegro functionality

#define HISTORY_SIZE 10

char *commandHistory[HISTORY_SIZE];
int currentHistoryPos = 0;
int currentViewPos = 0;


l_ulong AppVersion = ULONG_ID(0,0,3,9);
char AppName[] = "Terminal";
l_uid NeededLibs[] = { "conlib", "" };

l_text _ResolveFileName(l_text File) {
    // ... (No changes here) ...
    return File;
}

////////////////////////////////////////////////////////////////////////////////

l_text ResolveFileName(l_text File) {
    return _ResolveFileName(TextDup(File));
}

////////////////////////////////////////////////////////////////////////////////


void addToHistory(const char *command) {
    int i;   // Moved to top
    if (commandHistory[currentHistoryPos]) {
        free(commandHistory[currentHistoryPos]);
    }
    commandHistory[currentHistoryPos] = strdup(command);
    currentHistoryPos = (currentHistoryPos + 1) % HISTORY_SIZE;

    // Reset the view position
    currentViewPos = currentHistoryPos;
}


l_text EnhancedPrompt(l_text CurrentDir) {
    if (!CurrentDir) {
        fprintf(stderr, "Error: CurrentDir is null in EnhancedPrompt.\n");
        return NULL;
    }

    char input[256] = {0};
    l_text cmd = NULL;
    int key;
    int pos = currentViewPos; // Initialized pos to prevent undefined behavior

    while (1) {
        key = readkey();

        if (key >> 8 == KEY_UP) {
            if (pos < 0 || pos >= HISTORY_SIZE) {
                fprintf(stderr, "Error: pos value out of bounds in EnhancedPrompt.\n");
                continue;
            }

            if (commandHistory[pos]) {
                strncpy(input, commandHistory[pos], sizeof(input) - 1);
                input[sizeof(input) - 1] = '\0'; // Ensure null termination
                Printf(&Me, "\r%s>%s", CurrentDir, input);

                pos = (pos - 1 + HISTORY_SIZE) % HISTORY_SIZE;
            }
        } else if (key >> 8 == KEY_ENTER) {
            return strdup(input);
        
        
         } else {
            // ... handle other key inputs, like backspace, characters, etc ...
            // You can extend this to handle more complex input editing.
        }
    }

}



l_int Main(int arc, l_text *arv) {
    l_text a;
    l_text CurrentDir = TextDup("/system");
    l_text *argv;
    l_int argc;

    // ... (No changes at the start) ...

    do {
        Printf(&Me, "%s>", CurrentDir);
       a = EnhancedPrompt(CurrentDir);
       addToHistory(a);

        _ParseArgs(a, NULL, &argc, &argv);
        Printf(&Me, "\n");

        if (argc) {
            if (!TextCaseCompare(argv[0], "exit")) {
                return false;
            } else if (!TextCaseCompare(argv[0], "cd")) {
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
                        free(r);
                    }

                    free(r);
                }
            } else if (!TextCaseCompare(argv[0], "dir") || !TextCaseCompare(argv[0], "ls")) {
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
            } else if (!TextCaseCompare(argv[0], "del") || !TextCaseCompare(argv[0], "rm")) {  // Combined 'del' and 'rm' commands
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
        }
        FreeArgs(argc, argv);
        free(a);
    } while (1);
int i;  // Declare at the start of the block
for (i = 0; i < HISTORY_SIZE; i++) {
    if (commandHistory[i]) {
        free(commandHistory[i]);
        free(CurrentDir);
    }

    
}
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

void Close(void) {
    // Currently nothing to close.
}
