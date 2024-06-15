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

// added for sopy to directory support
#include <sys/stat.h>
#ifndef S_IFDIR
#define S_IFDIR 0040000  // This is a common UNIX value, but check DJGPP's documentation
#endif

int mkdir(const char *pathname, mode_t mode);   // <-- Add this line here



l_ulong AppVersion = ULONG_ID(0,0,5,1);
char AppName[] = "Terminal";
l_uid NeededLibs[] = { "conlib", "" };

l_text _ResolveFileName(l_text File) {
    // ... (No changes here) ...
    return File;
}

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

0052
*/

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

    do {
        Printf(&Me, "%s>", CurrentDir);
        a = Prompt(&Me);
        _ParseArgs(a, NULL, &argc, &argv);
        Printf(&Me, "\n");

        if (argc) {
            if (!TextCaseCompare(argv[0], "exit")) {
                return false;


            ////////////////////////////////////////////////////////////////////////////////


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
                    }

                    free(r);
                }



                ////////////////////////////////////////////////////////////////////////////////



            } else if (!TextCaseCompare(argv[0], "dir") || !TextCaseCompare(argv[0], "ls")) {
                // By default, show both directories and files.
                int attributes = FA_DIREC | FA_LABEL;
                int onlyDirectories = FALSE;  // Indicates if we should only show directories

                // Add the flag to determine if you want to list horizontally
                int listHorizontally = FALSE;

                // If the second argument is /d, show only directories.
                if (argc == 2 && !TextCaseCompare(argv[1], "/d")) {
                    onlyDirectories = TRUE;
                    attributes = FA_DIREC;
                }
                
                // Detect the "/w" argument
                if (argc == 2 && !TextCaseCompare(argv[1], "/w")) {
                    listHorizontally = TRUE;
                }

                PList l = ListDir(CurrentDir, attributes);
                PListItem a, b;

                Printf(&Me, "\nListing of %s\n\n", CurrentDir);

                if (l->Last) {
                    a = b = l->Last->Next;
                    l_ulong n = 0;

                    // Add the counter for horizontal listing
                    int countPerLine = 0;
                    const int maxItemsPerLine = 5; // Adjust this number for your desired output

                   do {
                if (FILEINFO(a->Data)->Attributes & FA_DIREC) {
                    textcolor(2); // Set the color to green for directories
                    
                    if (listHorizontally) {
                        Printf(&Me, "[dir] %s  ", FILEINFO(a->Data)->Name);
                    } else {
                        Printf(&Me, "[dir] %s\n", FILEINFO(a->Data)->Name);
                    }
                    
                    textcolor(7); // Reset color to light gray (or whatever the default is) after printing the directory name
                    n++;
                } else if (!onlyDirectories) {
                    // Since this section is for non-directory items, there's no color change here.
                    if (listHorizontally) {
                        Printf(&Me, "%s  ", FILEINFO(a->Data)->Name);
                    } else {
                        Printf(&Me, "     %s\n", FILEINFO(a->Data)->Name);
                    }
                    n++;
                }

                countPerLine++;
                if (listHorizontally && countPerLine >= maxItemsPerLine) {
                    Printf(&Me, "\n");
                    countPerLine = 0;
                }

                a = a->Next;
            } while (a != b);

            if (listHorizontally && countPerLine > 0) {
                Printf(&Me, "\n");


                    }

                    if (onlyDirectories) {
                        Printf(&Me, "\nTotal : %d directories\n\n", n);
                    } else {
                        Printf(&Me, "\nTotal : %d files and directories\n\n", n);
                    }

                } else {
                    Printf(&Me, "no files\n\n");
                }




                ////////////////////////////////////////////////////////////////////////////////



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

                        if (mkdir(r, 0777) != 0) {  // Add the mode argument here
                            Printf(&Me, "Error: Unable to create directory '%s'. Reason: %s\n", r, strerror(errno));
                        } else {
                            Printf(&Me, "Directory created: %s\n", r);
                        }

                        free(r);
                    }





                ////////////////////////////////////////////////////////////////////////////////







} else if (TextCaseCompare(argv[0], "copy") == 0 || TextCaseCompare(argv[0], "cp") == 0) {
    if (argc != 3) {
        Printf(&Me, "Syntax : COPY/CP <source_file> <destination_file/directory>\n");
    } else {
        FILE *sourceFile, *destFile;
        int ch;
        char destPath[512]; // Buffer for the destination path

        sourceFile = fopen(argv[1], "rb");
        if (sourceFile == NULL) {
            Printf(&Me, "Error: Unable to open source file '%s'. Reason: %s\n", argv[1], strerror(errno));
            continue;
        }

        // Check if destination is a directory
        struct stat s;
        if (stat(argv[2], &s) == 0 && (s.st_mode & S_IFDIR)) {
            // Concatenate the paths manually since we can't use snprintf
            strcpy(destPath, argv[2]);
            strcat(destPath, "\\");
            strcat(destPath, strrchr(argv[1], '\\') + 1);
        } else {
            strcpy(destPath, argv[2]);
        }

        destFile = fopen(destPath, "wb");
        if (destFile == NULL) {
            Printf(&Me, "Error: Unable to open destination file '%s'. Reason: %s\n", destPath, strerror(errno));
            fclose(sourceFile);
            continue;
        }

        while ((ch = fgetc(sourceFile)) != EOF) {
            fputc(ch, destFile);
        }

        fclose(sourceFile);
        fclose(destFile);
        Printf(&Me, "File copied successfully to '%s'.\n", destPath);
    }






                ////////////////////////////////////////////////////////////////////////////////



            } else if (!TextCaseCompare(argv[0], "help")) {
    Printf(&Me, "List of available commands:\n\n");
    Printf(&Me, "exit      - Exit the terminal\n");
    Printf(&Me, "cd <path> - Change current directory\n");
    Printf(&Me, "dir       - List all files and directories in the current directory\n");
    Printf(&Me, "ls        - Alias for dir\n");
    Printf(&Me, "ls /d     - List only directories in the current directory\n");
    Printf(&Me, "mkdir     - Create a new directory\n");
    Printf(&Me, "copy      - Copy a file\n");
    Printf(&Me, "cp        - Alias for copy\n");
    Printf(&Me, "del       - Delete a file or directory\n");
    Printf(&Me, "rm        - Alias for del\n");
    Printf(&Me, "set <var=value> - Set a variable for use in the terminal\n");
    Printf(&Me, "lspci     - Display all the PCI Devices\n");
    Printf(&Me, "meminfo     - Display system memory usage\n");
    Printf(&Me, "cpuinfo     - Display system CPU\n");
    Printf(&Me, "echo     - Echo text for shell scripts\n");
    Printf(&Me, "help      - Display this help menu\n");

    

                ////////////////////////////////////////////////////////////////////////////////




            } else if (!TextCaseCompare(argv[0], "lspci")) {
                // Logic for 'lspci' command


                ////////////////////////////////////////////////////////////////////////////////


            } else if (!TextCaseCompare(argv[0], "echo")) {
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


                ////////////////////////////////////////////////////////////////////////////////

            } else if (!TextCaseCompare(argv[0], "runscript")) {
                    if (argc != 2) {
                        Printf(&Me, "Syntax : RUNSCRIPT <script_filename.bat>\n");
                    } else {
                        int len = strlen(argv[1]);
                        if (len < 4 || strcmp(argv[1] + len - 4, ".bat") != 0) { 
                            Printf(&Me, "Error: The script file must have a '.bat' extension.\n");
                            return;
                        }

                        FILE *scriptFile = fopen(argv[1], "r");
                        if (scriptFile == NULL) {
                            Printf(&Me, "Error: Unable to open script file '%s'. Reason: %s\n", argv[1], strerror(errno));
                            return;
                        }

                        char line[256];
                        while (fgets(line, sizeof(line), scriptFile) != NULL) {
                            // Remove newline characters from the line
                            line[strcspn(line, "\r\n")] = '\0';

                            l_text scriptCommand = TextDup(line);
                            l_text *scriptArgv;
                            l_int scriptArgc;

                            _ParseArgs(scriptCommand, NULL, &scriptArgc, &scriptArgv);
                            
                            if (scriptArgc) {
                                l_int orig_argc = argc;
                                l_text *orig_argv = argv;
                                argc = scriptArgc;
                                argv = scriptArgv;
                                Main(argc, argv); // Call the main command interpreter function
                                argc = orig_argc;
                                argv = orig_argv;
                            }

                            FreeArgs(scriptArgc, scriptArgv);
                            free(scriptCommand);
                        }

                        fclose(scriptFile);
                    }




                ////////////////////////////////////////////////////////////////////////////////

    
            } else if (!TextCaseCompare(argv[0], "cpuinfo")) {
                // Logic for 'cpuinfo' command
            } else if (!TextCaseCompare(argv[0], "cpubench")) {
                // Logic for 'cpubench' command
            } else if (!TextCaseCompare(argv[0], "meminfo")) {
                // Logic for 'meminfo' command
            } else if (!TextCaseCompare(argv[0], "login")) {
                // Logic for 'login' command
            } else if (!TextCaseCompare(argv[0], "clock")) {
                // Logic for 'clock' command
            } else {
                Printf(&Me, "Unknown command : %s\n", argv[0]);
            }

            
            FreeArgs(argc, argv);
            free(a);
        }

    } while (1);
    
    return false;
}

////////////////////////////////////////////////////////////////////////////////

void Close(void) {
    // Anything that needs to be closed or cleaned up.
}

////////////////////////////////////////////////////////////////////////////////
