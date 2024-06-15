////////////////////////////////////////////////////////////////////////////////
//
//  Keyboard - Header File
//
//  (c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _KEYBOARD_H_INCLUDED_
#define _KEYBOARD_H_INCLUDED_


/* Keyboard support */
#define KB_ALT          KB_ALT_FLAG
#define KB_CTRL         KB_CTRL_FLAG
#define KB_SHIFT        KB_SHIFT_FLAG

#define ASCII(e)        (e->Keyb.Key & 0xFF)
#define KEY(e,k)        (ASCII(e) == k)
#define SCANCODE(e,k)   ((e->Keyb.Key >> 8) == k)
#define ALT(e,k)        (((e->Keyb.Key >> 8) == k) && (e->Keyb.Shift & KB_ALT))
#define CTRL(e,k)       (((e->Keyb.Key >> 8) == k) && (e->Keyb.Shift & KB_CTRL))
#define SHIFT(e,k)      (((e->Keyb.Key >> 8) == k) && (e->Keyb.Shift & KB_SHIFT))
#define COMBINE(e,k,c)  (((e->Keyb.Key >> 8) == k) && (e->Keyb.Shift & (c)))

/* event generator for keyboard events */
void    KeyboardEventGenerator(PEvent Event);

/* installs the keyboard system */
_PUBLIC void  GSSystemKeyboardInstall();
void GSSystemKeyboardUnInstall ( void );

extern TKeybState KState;

typedef struct TKeyboardLayout {
	
	l_int accent1;
	l_int accent2;
	l_int accent3;
	l_int accent4;
	
	l_int accent1_flag;
	l_int accent2_flag;
	l_int accent3_flag;
	l_int accent4_flag;
	
	l_ushort ascii_table[KEY_MAX];
	l_ushort capslock_table[KEY_MAX];
	l_ushort shift_table[KEY_MAX];
	l_ushort control_table[KEY_MAX];
	l_ushort altgr_lower_table[KEY_MAX];
	l_ushort altgr_upper_table[KEY_MAX];
	l_ushort accent1_lower_table[KEY_MAX];
	l_ushort accent1_upper_table[KEY_MAX];
	l_ushort accent2_lower_table[KEY_MAX];
	l_ushort accent2_upper_table[KEY_MAX];
	l_ushort accent3_lower_table[KEY_MAX];
	l_ushort accent3_upper_table[KEY_MAX];
	l_ushort accent4_lower_table[KEY_MAX];
	l_ushort accent4_upper_table[KEY_MAX];
	
	l_char Name[32];
	l_char Code[5];
	
} TKeyboardLayout, *PKeyboardLayout;

void KeyboardSetLayout ( PKeyboardLayout l );
PKeyboardLayout KeyboardGetLayout ( void );
void KeyboardInstallLayout ( void );

l_bool __KeyboardStop ( void );
l_bool __KeyboardRestart ( void );

#endif
