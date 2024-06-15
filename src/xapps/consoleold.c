////////////////////////////////////////////////////////////////////////////////
//
//	Console
//
//	Copyright (c) Aura GUI m6 Terminal.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "console.h"
#include "debug.h"
#include <stdlib.h> // Include this header for the mkdir function
#include <io.h> // Include this for CopyFile function

l_ulong AppVersion = ULONG_ID(0,0,3,0);
char AppName[] = "console";
l_uid NeededLibs[] = { "conlib", "" };

l_text _ResolveFileName(l_text File) {
    l_text t = File, // Current char 
           l = NULL, // Last token
           c = File; // Current token

    do {
        if ((*t == '/') || !*t) {
            if (!TextSqNCaseCompare(c,"..",t-c)) {
                if (l) {
                    l_text n = TextDup(File);
                    TextCopy(n+(l-File),n+(t+(*t?1:0)-File));
                    free(File);
                    return _ResolveFileName(n);
                } else {
                    l_text n = TextDup(File);
                    TextCopy(n+(c-File),n+(t+(*t?1:0)-File));
                    free(File);
                    return _ResolveFileName(n);
                }
            } else if (!TextSqNCaseCompare(c,".",t-c)) {
                l_text n = TextDup(File);
                TextCopy(n+(c-File),n+(t+(*t?1:0)-File));
                free(File);
                return _ResolveFileName(n);
            }
            
            if (t != File) l = c;
            c = t + 1;
        }
    } while (*(t++));	
    return File;
}

l_text ResolveFileName(l_text File) {
    return _ResolveFileName(TextDup(File));
}

l_int Main(int argc, l_text *argv) {
    l_text a;
    l_text CurrentDir = TextDup("/system");
    l_text *argv_local;
    l_int argc_local;
    
    DebugMessage("starting printf.....\n");
    Printf(&Me, "%s %d.%d.%d.%d\n%s\n\nConsole %d.%d.%d.%d\n",
           SystemApp.Name,
           ULID_A(SystemApp.Version), ULID_B(SystemApp.Version),
           ULID_C(SystemApp.Version), ULID_D(SystemApp.Version),
           KernelCopyright,
           ULID_A(Me.Version), ULID_B(Me.Version),
           ULID_C(Me.Version), ULID_D(Me.Version));

    Printf(&Me,"\n");
    do {
        Printf(&Me,"%s>",CurrentDir);
        a = Prompt(&Me);
        _ParseArgs(a, NULL, &argc_local, &argv_local);
        Printf(&Me, "\n");
        if (argc_local) {
            if (!TextCaseCompare(argv_local[0], "exit")) {
                return false;	
            } else if (!TextCaseCompare(argv_local[0], "cd")) {
                if (argc_local != 2) {
                    Printf(&Me,"Syntax : CD <path>\n");
                } else {
                    l_text np, r;
                    PFileInfo i;

                    if (*argv_local[1] == '/')
                        r = TextDup(argv_local[1]);
                    else {
                        np = FileNameToPath(CurrentDir, argv_local[1]);
                        r = ResolveFileName(np);
                        free(np);
                    }

                    i = FileGetInfo(r);

                    if (i) {
                        free(CurrentDir);
                        CurrentDir = TextDup(r);
                        FreeFileInfo(i);
                    } else {
                        Printf(&Me,"\nUnknown directory : %s\n\n", r);
                    }

                    free(r);
                }
            } else if (!TextCaseCompare(argv_local[0], "dir")) {
                PList l = ListDir(CurrentDir, FA_DIREC | FA_LABEL);
                PListItem a_item, b_item;

                Printf(&Me,"\nListing of %s\n\n", CurrentDir);

                if (l->Last) {
                    a_item = b_item = l->Last->Next;
                    l_ulong n = 0;
                    do {
                        if (FILEINFO(a_item->Data)->Attributes & FA_DIREC)
                            Printf(&Me,"[dir]");
                        else
                            Printf(&Me,"     ");
                        Printf(&Me,"%s\n",FILEINFO(a_item->Data)->Name);
                        n++;
                        a_item = a_item->Next;
                    }
                    while (a_item != b_item);
                    Printf(&Me,"\nTotal : %d files and directories\n\n",n);
                } else {
                    Printf(&Me,"no files\n\n");
                }
            } else if (!TextCaseCompare(argv_local[0], "ls")) {
                PList l = ListDir(CurrentDir,FA_DIREC|FA_LABEL);
                PListItem a_item, b_item;

                Printf(&Me,"\nListing of %s\n\n",CurrentDir);

                if (l->Last) {
                    a_item = b_item = l->Last->Next;
                    l_ulong n = 0;
                    do {
                        if (FILEINFO(a_item->Data)->Attributes & FA_DIREC)
                            Printf(&Me,"[ls]");
                        else
                            Printf(&Me,"     ");
                        Printf(&Me,"%s\n",FILEINFO(a_item->Data)->Name);
                        n++;
                        a_item = a_item->Next;
                    }
                    while (a_item != b_item);
                    Printf(&Me,"\nTotal : %d files and directories\n\n",n);
                } else {
                    Printf(&Me,"no files\n\n");
                }
            } else if (!TextCaseCompare(argv_local[0], "mkdir")) {
                if (argc_local != 2) {
                    Printf(&Me,"Syntax : MKDIR <directory_name>\n");
                } else {
                    l_text np, r;

                    if (*argv_local[1] == '/')
                        r = TextDup(argv_local[1]);
                    else {
                        np = FileNameToPath(CurrentDir, argv_local[1]);
                        r = ResolveFileName(np);
                        free(np);
                    }

                    if (mkdir(r) != 0) {
                        Printf(&Me,"\nUnable to create directory : %s\n\n", r);
                    } else {
                        Printf(&Me,"\nDirectory created : %s\n\n", r);
                    }

                    free(r);
                }
            } else if (!TextCaseCompare(argv_local[0], "copy")) {
                if (argc_local != 3) {
                    Printf(&Me,"Syntax : COPY <source> <destination>\n");
                } else {
                    l_text np1, r1, np2, r2;

                    if (*argv_local[1] == '/')
                        r1 = TextDup(argv_local[1]);
                    else {
                        np1 = FileNameToPath(CurrentDir, argv_local[1]);
                        r1 = ResolveFileName(np1);
                        free(np1);
                    }

                    if (*argv_local[2] == '/')
                        r2 = TextDup(argv_local[2]);
                    else {
                        np2 = FileNameToPath(CurrentDir, argv_local[2]);
                        r2 = ResolveFileName(np2);
                        free(np2);
                    }

                    if (CopyFile(r1, r2) == 0) {
                        Printf(&Me,"\nFile copied successfully.\n\n");
                    } else {
                        Printf(&Me,"\nUnable to copy file.\n\n");
                    }

                    free(r1);
                    free(r2);
                }
            } else {
                Printf(&Me,"Unknown command: %s\n\n", argv_local[0]);
            }
            for (int i = 0; i < argc_local; i++) {
                free(argv_local[i]);
            }
            free(argv_local);
        }

        free(a);
    } while (1);
    return true;
}
