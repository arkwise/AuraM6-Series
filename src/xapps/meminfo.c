#include <dpmi.h>
#include <go32.h>


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
