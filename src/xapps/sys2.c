////////////////////////////////////////////////////////////////////////////////
//
//  Terminal
//
//  Copyright (c) Aura GUI m6 Terminal. System Tools v2
//
////////////////////////////////////////////////////////////////////////////////

#include <dpmi.h>
#include <go32.h>
#include <dos.h>     // Required for union REGS and int86()
#include "command.h"  // Including the header so the Printf function is recognized
#include "kernel.h"
#include "console.h"
#include "debug.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include <fcntl.h>

/*updates

002 basic dpmi information
003 added more calls to functions in dpmi. this is for additional information
004 it works but the output crashes the terminal for some reason but shows it fine in debug.

*/

// Define FP_SEG and FP_OFF macros if not defined
#ifndef FP_SEG
#define FP_SEG(fp)   (((unsigned long)(fp)) >> 16)
#endif
#ifndef FP_OFF
#define FP_OFF(fp)   (((unsigned long)(fp)) & 0xFFFF)
#endif


void display_memory_info() {
    _go32_dpmi_meminfo meminfo;

    if (_go32_dpmi_get_free_memory_information(&meminfo)) {
        Printf(&Me, "Failed to retrieve memory information.\n");
        return;
    }

    Printf(&Me, "=========================================\n");
    Printf(&Me, "=  meminfo 0.0.3 doscore 2007-2023.     =\n");
    Printf(&Me, "=========================================\n\n");

    Printf(&Me, "-----------------------------------------\n");
    Printf(&Me, "| Available memory:            %.2f MB |\n", meminfo.available_memory / 1048576.0);
    Printf(&Me, "| Largest available block:     %.2f MB |\n", (meminfo.available_lockable_pages * 4096) / 1048576.0);
    Printf(&Me, "| Total linear space by CPU:   %.2f MB |\n", (meminfo.linear_space * 4096) / 1048576.0);
    Printf(&Me, "| Free linear space:           %.2f MB |\n", (meminfo.free_linear_space * 4096) / 1048576.0);
    Printf(&Me, "-----------------------------------------\n\n");

    Printf(&Me, "-----------------------------------------\n");
    Printf(&Me, "| Available pages:            %7lu   |\n", meminfo.available_pages);
    Printf(&Me, "| Total unlocked pages:       %7lu   |\n", meminfo.unlocked_pages);
    Printf(&Me, "| Available physical pages:   %7lu   |\n", meminfo.available_physical_pages);
    Printf(&Me, "| Total physical pages:       %7lu   |\n", meminfo.total_physical_pages);
    Printf(&Me, "| Max pages in paging file:   %7lu   |\n", meminfo.max_pages_in_paging_file);
    Printf(&Me, "-----------------------------------------\n\n");

    // Uncomment below if you wish to display the reserved values:
    /*
    Printf(&Me, "-----------------------------------------\n");
    Printf(&Me, "| Reserved value 1:           %7lu   |\n", meminfo.reserved[0]);
    Printf(&Me, "| Reserved value 2:           %7lu   |\n", meminfo.reserved[1]);
    Printf(&Me, "| Reserved value 3:           %7lu   |\n", meminfo.reserved[2]);
    Printf(&Me, "-----------------------------------------\n\n");
    */

    // Retrieve TSR data
    union REGS regs;     // Define a union for registers
    int86(0x1C, &regs, &regs);  // Call interrupt 0x1C
    int tsr_data = regs.x.ax;   // TSR returned value in ax

    Printf(&Me, "TSR Data: %d\n", tsr_data);  // Display the value from the TSR

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
    unsigned long address;
    unsigned long lbus  = (unsigned long)bus;
    unsigned long lslot = (unsigned long)slot;
    unsigned long lfunc = (unsigned long)func;
    unsigned long tmp = 0;

    address = (unsigned long)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((unsigned long)0x80000000));

    outportl(PCI_CONFIG_ADDRESS, address);
    tmp = inportl(PCI_CONFIG_DATA);
    return (tmp);
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

unsigned _get_volume_info(const char *path, int *max_file_len, int *max_path_len, char *fsystype) {
    union REGS regs;
    struct SREGS sregs;

    // Initialize fsystype to an empty string
    if (fsystype) {
        memset(fsystype, 0, 32);
    }

    // Prepare registers for the interrupt call
    regs.x.ax = 0x7303;  // Function 0x7303 (Get Extended Drive Parameters)
    regs.h.dl = (path[0] - 'A') + 1; // Drive number (1=A:, 2=B:, etc.)
    sregs.ds = FP_SEG(fsystype);
    regs.x.si = FP_OFF(fsystype);

    // Call the interrupt
    intdosx(&regs, &regs, &sregs);

    // Check for errors
    if (regs.x.cflag) {
        errno = EINVAL;  // Set appropriate errno value
        return _FILESYS_UNKNOWN; // Indicate error
    }

    // Extract additional volume info
    if (max_file_len) {
        *max_file_len = regs.x.cx;
    }
    if (max_path_len) {
        *max_path_len = regs.x.dx;
    }

    return regs.x.bx;  // Return filesystem flags
}

void Commanddf() {
    struct diskfree_t dfree;
    unsigned long total, free, used;
    char drive_letter;
    int drive;
    int drive_count = 0;
    char fs_type[32]; // Buffer to hold filesystem type
    int max_file_len, max_path_len;
    unsigned volume_info;

    Printf(&Me, "Filesystem     Type     Size     Used     Avail    Use%% Mounted on\n");

    for (drive = 3; drive <= 26; drive++) { // Iterate through drives C to Z
        drive_letter = 'A' + drive - 1;
        char path[4] = { drive_letter, ':', '\\', '\0' }; // Create path string

        DebugMessage("Checking drive: %c\n", drive_letter);

        if (_dos_getdiskfree(drive, &dfree) == 0) {
            total = (unsigned long)dfree.total_clusters * dfree.sectors_per_cluster * dfree.bytes_per_sector;
            free = (unsigned long)dfree.avail_clusters * dfree.sectors_per_cluster * dfree.bytes_per_sector;
            used = total - free;

            DebugMessage("Drive %c: total clusters = %u, available clusters = %u, sectors per cluster = %u, bytes per sector = %u\n",
                drive_letter, dfree.total_clusters, dfree.avail_clusters, dfree.sectors_per_cluster, dfree.bytes_per_sector);

            // Convert bytes to human-readable format (GB)
            double total_gb = total / (1024.0 * 1024 * 1024);
            double used_gb = used / (1024.0 * 1024 * 1024);
            double free_gb = free / (1024.0 * 1024 * 1024);

            // Retrieve filesystem type using _get_volume_info
            volume_info = _get_volume_info(path, &max_file_len, &max_path_len, fs_type);
            if (volume_info == _FILESYS_UNKNOWN) {
                DebugMessage("Error retrieving volume information for drive %c\n", drive_letter);
                strcpy(fs_type, "Unknown");
            } else {
                DebugMessage("Drive %c: max file length = %d, max path length = %d, filesystem type = %s\n",
                    drive_letter, max_file_len, max_path_len, fs_type);
            }

            Printf(&Me, "%-13s %-8s %-8.2f %-8.2f %-8.2f %4.0f%% /drive%c\n",
                   path, fs_type, total_gb, used_gb, free_gb, (double)used / total * 100, drive_letter);
            
            drive_count++;
        } else {
            DebugMessage("Error retrieving disk free space for drive %c\n", drive_letter);
        }
    }

    if (drive_count == 0) {
        Printf(&Me, "No valid drives found.\n");
    }
}
\