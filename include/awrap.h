#ifndef ALLEGRO_WRAPPER_H
#define ALLEGRO_WRAPPER_H
#define clear allegro_clear
#define hline allegro_hline
#define vline allegro_vline

#include <allegro.h>
#undef KEY_END
#undef KEY_MAX
#undef KEY_F
#undef KEY_COMMAND
#undef KEY_ENTER
#undef KEY_BACKSPACE
// This is a subset of commonly used macros/definitions from curses.h

// Keyboard Key Codes
#undef KEY_BREAK
#undef KEY_DOWN
#undef KEY_UP
#undef KEY_LEFT
#undef KEY_RIGHT
#undef KEY_HOME
#undef KEY_END
#undef KEY_BACKSPACE
#undef KEY_F
#undef KEY_DL
#undef KEY_IL
#undef KEY_DC
#undef KEY_IC
#undef KEY_EIC
#undef KEY_CLEAR
#undef KEY_EOS
#undef KEY_EOL
#undef KEY_SF
#undef KEY_SR
#undef KEY_NPAGE
#undef KEY_PPAGE
#undef KEY_STAB
#undef KEY_CTAB
#undef KEY_CATAB
#undef KEY_ENTER
#undef KEY_SRESET
#undef KEY_RESET
#undef KEY_PRINT
#undef KEY_LL
#undef KEY_A1
#undef KEY_A3
#undef KEY_B2
#undef KEY_C1
#undef KEY_C3
#undef KEY_BTAB
#undef KEY_BEG
#undef KEY_CANCEL
#undef KEY_CLOSE
#undef KEY_COMMAND
#undef KEY_COPY
#undef KEY_CREATE
#undef KEY_EXIT
#undef KEY_FIND
#undef KEY_HELP
#undef KEY_MARK
#undef KEY_MESSAGE
#undef KEY_MOVE
#undef KEY_NEXT
#undef KEY_OPEN
#undef KEY_OPTIONS
#undef KEY_PREVIOUS
#undef KEY_REDO
#undef KEY_REFERENCE
#undef KEY_REFRESH
#undef KEY_REPLACE
#undef KEY_RESTART
#undef KEY_RESUME
#undef KEY_SAVE
#undef KEY_SBEG
#undef KEY_SCANCEL
#undef KEY_SCOMMAND
#undef KEY_SCOPY
#undef KEY_SCREATE
#undef KEY_SDC
#undef KEY_SDL
#undef KEY_SELECT
#undef KEY_SEND
#undef KEY_SEOL
#undef KEY_SEXIT
#undef KEY_SFIND
#undef KEY_SHELP
#undef KEY_SHOME
#undef KEY_SIC
#undef KEY_SLEFT
#undef KEY_SMESSAGE
#undef KEY_SMOVE
#undef KEY_SNEXT
#undef KEY_SOPTIONS
#undef KEY_SPREVIOUS
#undef KEY_SPRINT
#undef KEY_SREDO
#undef KEY_SREPLACE
#undef KEY_SRIGHT
#undef KEY_SRSUME
#undef KEY_SSAVE
#undef KEY_SSUSPEND
#undef KEY_SUNDO
#undef KEY_SUSPEND
#undef KEY_UNDO
#undef KEY_MOUSE
#undef KEY_RESIZE
#undef KEY_EVENT

// Color Codes
#undef COLOR_BLACK
#undef COLOR_RED
#undef COLOR_GREEN
#undef COLOR_YELLOW
#undef COLOR_BLUE
#undef COLOR_MAGENTA
#undef COLOR_CYAN
#undef COLOR_WHITE

// Other common constants
#undef TRUE
#undef FALSE
#undef ERR
#undef OK

// Functions
#undef clear
#undef hline
#undef vline
#undef getch

#define ALLEGRO_KEY_END KEY_END
#define ALLEGRO_KEY_MAX KEY_MAX
#define ALLEGRO_KEY_F(n) KEY_F(n)
#define ALLEGRO_GETCH() readkey()

#endif // ALLEGRO_WRAPPER_H
