#include "kernel.h"
#include "screen.h"

// Define a list of refresh rates to try
int refresh_rates[] = {60, 75, 120, 144, 240, 0}; // 0 at the end as a sentinel value

void screen_init(void) {
    int i = 0;
    bool success = false;

    // Loop through the list of refresh rates
    while (refresh_rates[i] != 0) {
        // Try to initialize the screen with the current refresh rate
        if (allegro_set_refresh_rate(refresh_rates[i])) {
            success = true;
            break;
        }
        i++;
    }

    if (!success) {
        // Handle the error (e.g., show an error message or log the error)
        fprintf(stderr, "Failed to initialize the screen with any of the tried refresh rates.\n");
        exit(1); // Or any other error handling mechanism you prefer
    }
}
