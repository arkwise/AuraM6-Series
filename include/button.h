#include "widget.h"

#ifndef _BUTTON_H_INCLUDED_
#define _BUTTON_H_INCLUDED_

/**	
	@addtogroup button
	@{
*/
/**
	@file
	Button widget header file.
*/

#define BUTTON(w) ((PButton)(w))

#define BS_OVER 0x1
#define BS_DOWN 0x2

typedef struct TButton *PButton;
/**
	Button structure
	@see CreateButton
*/
typedef struct TButton
{
	struct TWidget o;

	l_text Caption; ///< Caption

	l_ulong Message; ///< Message send when clicked

	void *MessageExtra; ///< Extra value send with message when clicked

	/**
		Facultative bitmap drawn on button
		@note By default, icon is on the left of the text, whole content is center
		@see buttonflags
	*/

	p_bitmap Icon;

	l_ushort State;

	/**
		Button flags
		@see buttonflags
	*/
	l_ushort Flags;

} TButton;

PButton CreateButton(PApplication App, TRect r, l_text Caption, l_ulong Message);
void ButtonSetCaption(PButton b, l_text Caption);
void ButtonSetCaptionEx(PButton b, l_text Format, ...);

/** @defgroup buttonflags Buttons flags
	Values that can be set into TButton Flags field.
	Use 'OR' to associate those flags.
	@see CreateButton
	@see TButton
	@{
*/

#define BF_TEXTUNDERICON 0x00000001 ///< If icon is set, draw the text under the icon and center both of them
#define BF_DISABLED 0x00000002			///< Disable the button, and if an icon is set, draw icon in grey
#define BF_ALIGNLEFT 0x00000004			///< If icon is set, draw icon on left and draw text on the right of the icon

/**
	@}
*/

#endif /* _BUTTON_H_INCLUDED_ */
