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
0060 added the rest of the commands to sys.c 14/06/24

l_text _ResolveFileName(l_text File) {
    // ... (No changes here) ...
    return File;
}

////////////////////////////////////////////////////////////////////////////////

l_text ResolveFileName(l_text File) {
    return _ResolveFileName(TextDup(File));
}

////////////////////////////////////////////////////////////////////////////////



*/


l_int Main(int argc, l_text *argv) {
    l_text a;
    l_text CurrentDir = TextDup("/system");
    l_text *new_argv;
    l_int new_argc;

    do {
        Printf(&Me, "%s>", CurrentDir);
        a = Prompt(&Me);
        _ParseArgs(a, NULL, &new_argc, &new_argv);
        Printf(&Me, "\n");

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
                list_pci_devices();
            } else if (!TextCaseCompare(new_argv[0], "echo")) {
                CommandEcho(new_argc, new_argv);
            } else if (!TextCaseCompare(new_argv[0], "runscript")) {
                CommandRunScript(new_argc, new_argv);
            } else if (!TextCaseCompare(new_argv[0], "meminfo")) {
                display_memory_info();
            } else if (!TextCaseCompare(new_argv[0], "cpuinfo")) {
                list_cpu_info();                

                 
            } else {
                // The rest of your commands can follow this pattern. You'd replace the body
                // of each command's logic with a call to the appropriate function from command.h.
                Printf(&Me, "Unknown command : %s\n", new_argv[0]);
            }

            FreeArgs(new_argc, new_argv);
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
