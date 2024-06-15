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
#include <dpmi.h>
#include <go32.h>

// internal applications here



l_ulong AppVersion = ULONG_ID(0,0,4,2);
char AppName[] = "Terminal";
l_uid NeededLibs[] = { "conlib", "" };

l_text _ResolveFileName(l_text File) {
    // ... (No changes here) ...
    return File;
}
void display_memory_info() {
    _go32_dpmi_meminfo meminfo;
    
    if (_go32_dpmi_get_free_memory_information(&meminfo)) {
        Printf(&Me, "Failed to retrieve memory information.\n");
        return;
    }
    
    Printf(&Me, "Total available memory: %lu bytes\n", meminfo.total_physical_pages * 4096);
    Printf(&Me, "Largest available block: %lu bytes\n", meminfo.available_lockable_pages);
    Printf(&Me, "Free available memory: %lu bytes\n", meminfo.available_physical_pages * 4096);
}

#define CPUID_FEAT_ECX_SSE3        0x00000001 /* Streaming SIMD Extensions 3 */
/* ... You can add more CPUID feature flags as required ... */

typedef struct {
    unsigned eax, ebx, ecx, edx;
} CPUID_Result;

/* This function will execute the CPUID instruction */
void cpuid(int code, CPUID_Result *result) {
    asm volatile(
        "mov %%ebx, %%edi;" /* Preserve EBX (for PIC) */
        "cpuid;"
        "mov %%ebx, %%esi;" /* Move result from EBX to ESI */
        "mov %%edi, %%ebx;" /* Restore original EBX */
        : "=a" (result->eax), "=S" (result->ebx),
          "=c" (result->ecx), "=d" (result->edx)
        : "0" (code)
        : "edi"
    );
}

void list_cpu_info() {
    char vendor[13];
    CPUID_Result result;

    /* Get vendor string */
    cpuid(0, &result);
    *((int *)vendor) = result.ebx;
    *((int *)(vendor + 4)) = result.edx;
    *((int *)(vendor + 8)) = result.ecx;
    vendor[12] = '\0';

    Printf(&Me, "CPU Vendor: %s\n", vendor);

    /* Get feature information */
    cpuid(1, &result);
    
    Printf(&Me, "CPU Signature: EAX=0x%08x\n", result.eax);
    
    if (result.ecx & CPUID_FEAT_ECX_SSE3) {
        Printf(&Me, "Supports: SSE3\n");
    }

    /* ... You can add more checks for CPU features here ... */
}
#define PCI_CONFIG_ADDRESS  0x0CF8
#define PCI_CONFIG_DATA     0x0CFC

unsigned long pci_read_config_dword(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset) {
    // ... Same as before ...
}

void list_pci_devices() {
    int bus;
    int slot;
    unsigned long vendor_device;
    unsigned short vendorID;
    unsigned short deviceID;

    for(bus = 0; bus < 256; bus++) {
        for(slot = 0; slot < 32; slot++) {
            vendor_device = pci_read_config_dword(bus, slot, 0, 0);
            vendorID = vendor_device & 0xFFFF;
            deviceID = (vendor_device >> 16) & 0xFFFF;

            if(vendorID != 0xFFFF) {
                Printf(&Me, "PCI Device found at Bus %d Slot %d: VendorID=0x%X, DeviceID=0x%X\n", bus, slot, vendorID, deviceID);
            }
        }
    }
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

    // ... (No changes at the start) ...

    do {
        Printf(&Me, "%s>", CurrentDir);
        a = Prompt(&Me);
        _ParseArgs(a, NULL, &argc, &argv);
        Printf(&Me, "\n");

        if (argc) {
            if (!TextCaseCompare(argv[0], "exit")) {
                return false;
       } else if (!TextCaseCompare(argv[0], "lspci")) {
            list_pci_devices();
                        
            } else if (!TextCaseCompare(argv[0], "cpuinfo")) {
                list_cpu_info();
                        
            } else if (!TextCaseCompare(argv[0], "meminfo")) {
                display_memory_info();
            
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
#define TRUE  1
#define FALSE 0

// ... rest of the code ...

} else if (!TextCaseCompare(argv[0], "dir") || !TextCaseCompare(argv[0], "ls")) {
    // By default, show both directories and files.
    int attributes = FA_DIREC | FA_LABEL;
    int onlyDirectories = FALSE;  // Indicates if we should only show directories

    // If the second argument is /d, show only directories.
    if (argc == 2 && !TextCaseCompare(argv[1], "/d")) {
        onlyDirectories = TRUE;
        attributes = FA_DIREC;
    }

    PList l = ListDir(CurrentDir, attributes);
    PListItem a, b;

    Printf(&Me, "\nListing of %s\n\n", CurrentDir);

    if (l->Last) {
        a = b = l->Last->Next;
        l_ulong n = 0;
        do {
            if (FILEINFO(a->Data)->Attributes & FA_DIREC) {
                Printf(&Me, "[dir] %s\n", FILEINFO(a->Data)->Name);
                n++;
            } else if (!onlyDirectories) {  // If not only listing directories, show the file.
                Printf(&Me, "     %s\n", FILEINFO(a->Data)->Name);
                n++;
            }

            a = a->Next;
        } while (a != b);

        if (onlyDirectories) {
            Printf(&Me, "\nTotal : %d directories\n\n", n);
        } else {
            Printf(&Me, "\nTotal : %d files and directories\n\n", n);
        }

    } else {
        Printf(&Me, "no files\n\n");
    }

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
    Printf(&Me, "help      - Display this help menu\n");





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

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

void Close(void) {
    // Currently nothing to close.
}
