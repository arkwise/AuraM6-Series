#include <allegro.h>
#include <algif.h>

void cleanup_and_exit(int error_code) {
    allegro_exit();
    exit(error_code);
}

int main(int argc, char **argv) {
    char const *name = "e_gifs/alex.gif";
    if (argc == 2)
        name = argv[1];

    // Initialize Allegro
    if (allegro_init() != 0) {
        allegro_message("Error initializing Allegro: %s\n", allegro_error);
        return 1;
    }

    // Set color depth and graphics mode
    set_color_depth(16); // Set color depth to 16-bit
    if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) != 0) {
        allegro_message("Error setting graphics mode: %s\n", allegro_error);
        cleanup_and_exit(1);
    }

    // Clear the screen
    clear_to_color(screen, makecol(255, 0, 255));

    // Install the keyboard
    if (install_keyboard() != 0) {
        allegro_message("Error installing keyboard: %s\n", allegro_error);
        cleanup_and_exit(1);
    }

    // Initialize ALGIF
    algif_init();

    // Load the bitmap
    BITMAP *bmp = load_bitmap(name, NULL);
    if (!bmp) {
        // Display an error message
        allegro_message("Error: Could not open %s.\n", name);
        cleanup_and_exit(1);
    }

    // Draw the bitmap
    draw_sprite(screen, bmp, 0, 0);
    textout_centre_ex(screen, font, "Press any key to exit", SCREEN_W / 2, SCREEN_H - text_height(font), makecol(255, 255, 255), -1);

    // Wait for key press
    readkey();

    // Clean up
    destroy_bitmap(bmp);
    cleanup_and_exit(0);
}
END_OF_MAIN()
