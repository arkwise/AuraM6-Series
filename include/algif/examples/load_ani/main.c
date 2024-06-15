#include <allegro.h>
#include <algif.h>

// load animated gifs

static volatile int ticks = 0;

static void ticker(void)
{
    ticks++;
}

int main(int argc, char **argv)
{
    BITMAP **frames;
    int *durations;
    int i, n;
    char const *name = "e_gifs/allefant.gif";

    if (argc == 2)
        name = argv[1];

    // Initialize Allegro
    if (allegro_init() != 0) {
        allegro_message("Error initializing Allegro: %s\n", allegro_error);
        return -1;
    }

    // Install timer
    install_timer();

    // Set color depth and graphics mode
    set_color_depth(16); // Set color depth to 16-bit
    if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) != 0) {
        allegro_message("Error setting graphics mode: %s\n", allegro_error);
        //cleanup_and_exit(1);
    }

    // Clear the screen
    clear_to_color(screen, makecol(255, 0, 255));

    // Install the keyboard
    if (install_keyboard() != 0) {
        allegro_message("Error installing keyboard: %s\n", allegro_error);
        return -1;
    }

    // Initialize ALGIF
    algif_init();

    // Load the GIF animation
    n = algif_load_animation(name, &frames, &durations);
    if (n <= 0) {
        allegro_message("Error loading GIF animation: %s\n", name);
        return -1;
    }

    // Install ticker function
    install_int_ex(ticker, BPS_TO_TIMER(100));

    // Display the animation
    while (!keypressed()) {
        for (i = 0; i < n; i++) {
            int e;
            draw_sprite(screen, frames[i], 0, 0);
            e = ticks + durations[i];
            while (ticks < e && !keypressed()) {
                rest(1);
                textprintf_ex(screen, font, 0, frames[i]->h + 4,
                              makecol(255, 255, 255), makecol(255, 0, 255),
                              "%-4d: %-4d", i, e - ticks);
            }
            if (keypressed())
                break;
        }
    }

    // Clean up
    for (i = 0; i < n; i++)
        destroy_bitmap(frames[i]);
    free(frames);
    free(durations);
    return 0;
}
END_OF_MAIN()
