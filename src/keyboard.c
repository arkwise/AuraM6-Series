/**
*	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
*	FILE:			keyboard.c
*
*	PROJECT:		Phoenix engine - Core
*
*	DESCRIPTION:	The keyboard event handler
*
*	CONTRIBUTORS:
*					Lukas Lipka
*					Julien Etelain
*
*	TODO:			@
*
*	BUG:			@
*
*	MISTAKE:		@
*
*	IDEA:			@
*/
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "allegro/internal/aintern.h"

#ifndef _OZONE__LINUX_

unsigned short *std_key_ascii_table;
unsigned short *std_key_capslock_table;
unsigned short *std_key_shift_table;
unsigned short *std_key_control_table;
unsigned short *std_key_empty_table;

#endif

TKeybState KState;

PKeyboardLayout CurrentLayout = 0;
////////////////////////////////////////////////////////////////////////////////
void KeyboardSetLayout ( PKeyboardLayout l ) {
	
#ifndef _OZONE__LINUX_
	
	if ( CurrentLayout ) free(CurrentLayout);
	CurrentLayout = malloc(sizeof(TKeyboardLayout));
	if ( !CurrentLayout ) return;
	memcpy(CurrentLayout,l,sizeof(TKeyboardLayout));
	
  _key_ascii_table         = CurrentLayout->ascii_table;
  _key_capslock_table      = CurrentLayout->capslock_table;
  _key_shift_table         = CurrentLayout->shift_table;
  _key_control_table       = CurrentLayout->control_table;
  _key_altgr_lower_table   = CurrentLayout->altgr_lower_table;
  _key_altgr_upper_table   = CurrentLayout->altgr_upper_table;
  _key_accent1_lower_table = CurrentLayout->accent1_lower_table;
  _key_accent1_upper_table = CurrentLayout->accent1_upper_table;
  _key_accent2_lower_table = CurrentLayout->accent2_lower_table;
  _key_accent2_upper_table = CurrentLayout->accent2_upper_table;
  _key_accent3_lower_table = CurrentLayout->accent3_lower_table;
  _key_accent3_upper_table = CurrentLayout->accent3_upper_table;
  _key_accent4_lower_table = CurrentLayout->accent4_lower_table;
  _key_accent4_upper_table = CurrentLayout->accent4_upper_table;
	_key_accent1 = CurrentLayout->accent1;
	_key_accent2 = CurrentLayout->accent2;
	_key_accent3 = CurrentLayout->accent3;
	_key_accent4 = CurrentLayout->accent4;
	_key_accent1_flag = CurrentLayout->accent1_flag;
	_key_accent2_flag = CurrentLayout->accent2_flag;
	_key_accent3_flag = CurrentLayout->accent3_flag;
	_key_accent4_flag = CurrentLayout->accent4_flag;
  _key_standard_kb = FALSE;
  
#endif

}
////////////////////////////////////////////////////////////////////////////////
PKeyboardLayout KeyboardGetLayout ( void ) {
	return CurrentLayout;
}
////////////////////////////////////////////////////////////////////////////////
void KeyboardEventGenerator ( PEvent Event )
{
	Event->Type = EV_NOTHING;

	KState.Shift = key_shifts;

	if (keypressed()) {

		Event->Type = EV_KEYBOARD;
		KState.Key = readkey();

		Event->Keyb = KState;

	} else
		KState.Key = 0;

}
////////////////////////////////////////////////////////////////////////////////
void KeyboardDefaultLayout ( void ) {
	
	#ifndef _OZONE__LINUX_
	_key_ascii_table 		 = std_key_ascii_table;
	_key_capslock_table      = std_key_capslock_table;
	_key_shift_table         = std_key_shift_table;
	_key_control_table       = std_key_control_table;
	_key_altgr_lower_table   = std_key_empty_table;
	_key_altgr_upper_table   = std_key_empty_table;
	_key_accent1_lower_table = std_key_empty_table;
	_key_accent1_upper_table = std_key_empty_table;
	_key_accent2_lower_table = std_key_empty_table;
	_key_accent2_upper_table = std_key_empty_table;
	_key_accent3_lower_table = std_key_empty_table;
	_key_accent3_upper_table = std_key_empty_table;
	_key_accent4_lower_table = std_key_empty_table;
	_key_accent4_upper_table = std_key_empty_table;
	_key_accent1 = 0;
	_key_accent2 = 0;
	_key_accent3 = 0;
	_key_accent4 = 0;
	_key_accent1_flag = 0;
	_key_accent2_flag = 0;
	_key_accent3_flag = 0;
	_key_accent4_flag = 0;
	_key_standard_kb = TRUE;
	#endif
}
////////////////////////////////////////////////////////////////////////////////
void KeyboardInstallLayout ( void ) {
	
	#ifndef _OZONE__LINUX_
	
	l_text LayoutFile = KeyGetText("/SYSTEM/KEYBOARD/LAYOUT", "");

	if ( TextCompare(LayoutFile,"" ) ) {
		PFile f = FileOpen(LayoutFile,"rb");
		if ( f ) {
			PKeyboardLayout CL = malloc(sizeof(TKeyboardLayout));			
			if ( CL ) {
				FileRead(CL,1,sizeof(TKeyboardLayout),f);
				KeyboardSetLayout(CL);
				free(CL);
			}
			FileClose(f);
		}
	}	else if ( CurrentLayout ) {
		free(	CurrentLayout);
		CurrentLayout = 0;
		KeyboardDefaultLayout();
	}
	
	#endif
	
}
////////////////////////////////////////////////////////////////////////////////
void GSSystemKeyboardInstall ( void )
{
	#ifndef _OZONE__LINUX_
	std_key_ascii_table 		= _key_ascii_table;
	std_key_capslock_table 	= _key_capslock_table;
	std_key_shift_table 		= _key_shift_table;
	std_key_control_table 	= _key_control_table;
	std_key_empty_table 		= _key_altgr_lower_table;
	#endif
	
	install_keyboard();
	InstallEventGenerator(KeyboardEventGenerator);
	
	#ifndef _OZONE__LINUX_
	KeyboardInstallLayout();
	#endif
	
}
////////////////////////////////////////////////////////////////////////////////
void GSSystemKeyboardUnInstall ( void )
{
	if ( CurrentLayout ) free(CurrentLayout);
	CurrentLayout = NULL;
	#ifndef _OZONE__LINUX_
	KeyboardDefaultLayout();
	#endif
}
////////////////////////////////////////////////////////////////////////////////
l_bool __KeyboardStop ( void ) {
	remove_keyboard();
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool __KeyboardRestart ( void ) {
	install_keyboard();
	KeyboardInstallLayout();
	return true;
}
////////////////////////////////////////////////////////////////////////////////


