////////////////////////////////////////////////////////////////////////////////
//
//  Console
//
//  Copyright (c) Aura GUI m6 Terminal.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "console.h"
#include "debug.h"
#include <stdlib.h> // Include this header for the mkdir function
#include <io.h> // Include this for CopyFile function
#include <errno.h> // error handling of files IO

l_ulong AppVersion = ULONG_ID(0,0,3,5);
char AppName[] = "console";
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

l_int Main( int arc, l_text *arv ) {
    l_text a;
    l_text CurrentDir =  TextDup("/system");
    l_text  *argv;
    l_int   argc;
    
    // ... (No changes at the start) ...

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
                        
                        Printf(&Me,"\nUnknow directory : %s\n\n",r);
                        
                    }
                    
                    
                    free(r);
                }

//
// file directory commands
//
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
//
// making directories commands
//
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
    



//
// Deltion commands
//
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

        // Try to remove as file first
        if (remove(r) == 0) {
            Printf(&Me, "File removed: %s\n", r);
        }
        // If unsuccessful, try to remove as directory
        else if (rmdir(r) == 0) {
            Printf(&Me, "Directory removed: %s\n", r);
        } else {
            Printf(&Me, "Error: Unable to remove %s\n", r);
        }

        free(r);
    }
}


//
// Deltion commands
//

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

         // end of if (argc)
    } while ( 1 );
    
    return false;


////////////////////////////////////////////////////////////////////////////////

void Close(void) {
}
}

////////////////////////////////////////////////////////////////////////////////
