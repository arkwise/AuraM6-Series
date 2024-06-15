////////////////////////////////////////////////////////////////////////////////
//
//  Console
//
//  Copyright (c) Doscore Aura GUI M Series.
//
////////////////////////////////////////////////////////////////////////////////

/*
this version is for adding commands and tools to the terminal to make sure they work.
*/
#include "kernel.h"
#include "console.h"
#include "debug.h"
#include <stdlib.h> // Include this header for the mkdir function
#include <stdio.h>  // for copying?
#include <string.h>
#include <io.h> // Include this for CopyFile function
#include <errno.h> // error handling of files IO

#include <dos.h> // for DOS interrupts

l_ulong AppVersion = ULONG_ID(0,0,3,6);
char AppName[] = "Terminal";
l_uid NeededLibs[] = { "conlib", "" };

l_text _ResolveFileName ( l_text File ) {
    // ... (No changes here) ...
    return File;
}

////////////////////////////////////////////////////////////////////////////////

l_text ResolveFileName ( l_text File ) {
    return _ResolveFileName(TextDup(File));
}

////////////////////////////////////////////////////////////////////////////////

void CommandDf() {
    struct dfree diskfree;
    int drive = getdisk() + 1; // get the current drive number (1=A, 2=B, etc.)

    if (_dos_getdiskfree(drive, &diskfree) == 0) {
        long total_clusters = diskfree.df_total;
        long free_clusters = diskfree.df_avail;
        long sectors_per_cluster = diskfree.df_sclus;
        long bytes_per_sector = diskfree.df_bsec;
        long total_bytes = total_clusters * sectors_per_cluster * bytes_per_sector;
        long free_bytes = free_clusters * sectors_per_cluster * bytes_per_sector;

        Printf(&Me, "Filesystem information for drive %c:\n", 'A' + drive - 1);
        Printf(&Me, "Total space: %ld bytes (%.2f MB)\n", total_bytes, total_bytes / (1024.0 * 1024.0));
        Printf(&Me, "Free space: %ld bytes (%.2f MB)\n", free_bytes, free_bytes / (1024.0 * 1024.0));
    } else {
        Printf(&Me, "Error: Unable to retrieve disk information for drive %c\n", 'A' + drive - 1);
    }
}

l_int Main( int arc, l_text *arv ) {
    l_text a;
    l_text CurrentDir =  TextDup("/system");
    l_text  *argv;
    l_int   argc;

    do {
        Printf(&Me,"%s>",CurrentDir);
        a = Prompt(&Me);
        _ParseArgs(a,NULL,&argc,&argv);
        Printf(&Me,"\n");

        if ( argc ) {
            if ( !TextCaseCompare(argv[0],"exit") ) {
                return false;
            } else if ( !TextCaseCompare(argv[0],"cd") ) {
                if ( argc != 2 ) {
                    Printf(&Me,"Syntax : CD <path>\n");
                } else {
                    l_text np,r;
                    PFileInfo i;

                    if ( *argv[1] == '/' )
                        r = TextDup(argv[1]);
                    else {
                        np = FileNameToPath(CurrentDir,argv[1]);
                        r = ResolveFileName(np);
                        free(np);
                    }

                    i = FileGetInfo(r);

                    if ( i ) {
                        free(CurrentDir);
                        CurrentDir = TextDup(r);
                        FreeFileInfo(i);
                    } else {
                        Printf(&Me,"\nUnknown directory : %s\n\n",r);
                    }

                    free(r);
                }
            } else if ( !TextCaseCompare(argv[0],"dir") ) {
                PList l = ListDir(CurrentDir,FA_DIREC|FA_LABEL);
                PListItem a, b;

                Printf(&Me,"\nListing of %s\n\n",CurrentDir);

                if ( l->Last ) {
                    a = b = l->Last->Next;
                    l_ulong n = 0;
                    do
                    {
                        if ( FILEINFO(a->Data)->Attributes & FA_DIREC )
                            Printf(&Me,"[dir]");
                        else
                            Printf(&Me,"     ");

                        Printf(&Me,"%s\n",FILEINFO(a->Data)->Name);
                        n++;
                        a = a->Next;
                    }
                    while ( a != b );
                    Printf(&Me,"\nTotal : %d files and directories\n\n",n);

                } else {
                    Printf(&Me,"no files\n\n");
                }
            } else if ( !TextCaseCompare(argv[0],"ls") ) {
                PList l = ListDir(CurrentDir,FA_DIREC|FA_LABEL);
                PListItem a, b;

                Printf(&Me,"\nListing of %s\n\n",CurrentDir);

                if ( l->Last ) {
                    a = b = l->Last->Next;
                    l_ulong n = 0;
                    do
                    {
                        if ( FILEINFO(a->Data)->Attributes & FA_DIREC )
                            Printf(&Me,"[ListDir]");
                        else
                            Printf(&Me,"     ");

                        Printf(&Me,"%s\n",FILEINFO(a->Data)->Name);
                        n++;
                        a = a->Next;
                    }
                    while ( a != b );
                    Printf(&Me,"\nTotal : %d files and directories\n\n",n);

                } else {
                    Printf(&Me,"no files\n\n");
                }
            } else if (!TextCaseCompare(argv[0], "mkdir")) {
                if (argc != 2) {
                    Printf(&Me, "Syntax : MKDIR <directory_name>\n");
                } else {
                    l_text np, r;

                    if (*argv[1] == '\\') {  // considering backslash for DOS or Windows
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
            } else if (!TextCaseCompare(argv[0], "copy")) {
                if (argc != 3) {
                    Printf(&Me, "Syntax : COPY <source_file> <destination_file>\n");
                } else {
                    FILE *sourceFile, *destFile;
                    int ch;

                    sourceFile = fopen(argv[1], "rb");
                    if (sourceFile == NULL) {
                        Printf(&Me, "Error: Unable to open source file '%s'. Reason: %s\n", argv[1], strerror(errno));
                        return;
                    }

                    destFile = fopen(argv[2], "wb");
                    if (destFile == NULL) {
                        Printf(&Me, "Error: Unable to open destination file '%s'. Reason: %s\n", argv[2], strerror(errno));
                        fclose(sourceFile);
                        return;
                    }

                    while ((ch = fgetc(sourceFile)) != EOF) {
                        fputc(ch, destFile);
                    }

                    fclose(sourceFile);
                    fclose(destFile);
                    Printf(&Me, "File copied successfully.\n");
                }
            } else if (!TextCaseCompare(argv[0], "del")) {
                if (argc != 2) {
                    Printf(&Me, "Syntax : DEL <filename>\n");
                } else {
                    l_text np, r;

                    if (*argv[1] == '/')
                        r = TextDup(argv[1]);
                    else {
                        np = FileNameToPath(CurrentDir, argv[1]);
                        r = ResolveFileName(np);
                        free(np);
                    }

                    if (remove(r) != 0) {
                        Printf(&Me, "Error: Unable to delete file: %s\n", r);
                    } else {
                        Printf(&Me, "File deleted: %s\n", r);
                    }
                }
            } else if (!TextCaseCompare(argv[0], "rm")) {
                if (argc != 2) {
                    Printf(&Me, "Syntax : rm <filename_or_directory>\n");
                } else {
                    l_text np, r;

                    if (*argv[1] == '\\')
                        r = TextDup(argv[1]);
                    else {
                        np = FileNameToPath(CurrentDir, argv[1]);
                        r = ResolveFileName(np);
                        free(np);
                    }

                    if (remove(r) == 0) {
                        Printf(&Me, "File removed: %s\n", r);
                    } else if (rmdir(r) == 0) {
                        Printf(&Me, "Directory removed: %s\n", r);
                    } else {
                        Printf(&Me, "Error: Unable to remove %s\n", r);
                    }

                    free(r);
                }
            } else if (!TextCaseCompare(argv[0], "format")) {
                if (argc != 2) {
                    Printf(&Me, "Syntax : FORMAT <directory>\n");
                } else {
                    l_text np, r;

                    if (*argv[1] == '\\')
                        r = TextDup(argv[1]);
                    else {
                        np = FileNameToPath(CurrentDir, argv[1]);
                        r = ResolveFileName(np);
                        free(np);
                    }

                    CommandFormat(r);
                    free(r);
                }
            } else if (!TextCaseCompare(argv[0], "df")) {
                CommandDf();
            } else {
                l_text np,r;
                PFileInfo i;

                if ( *argv[0] == '/' )
                    r = TextDup(argv[0]);
                else {
                    np = FileNameToPath(CurrentDir,argv[0]);
                    r = ResolveFileName(np);
                    free(np);
                }

                i = FileGetInfo(r);

                if ( i ) {
                    RunFile3(i,argc-1,&argv[1],&Me,NULL);
                    FreeFileInfo(i);
                } else {
                    Printf(&Me,"\n'%s' : Unknown command\n\n",argv[0]);
                }
                free(r);
            }

            FreeArgs(argc,argv);
            free(a);

        } // end of if (argc)
    } while ( 1 );

    return false;
}

////////////////////////////////////////////////////////////////////////////////

void Close(void) {
    // Anything that needs to be closed or cleaned up.

}
