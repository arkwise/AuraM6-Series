////////////////////////////////////////////////////////////////////////////////
//
//  TextBox Widget - Header File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include"widget.h"
#include"sbar.h"

#ifndef _TEXTBOX_H_INCLUDED_
#define _TEXTBOX_H_INCLUDED_


#define TEXTBOX(w)	((PTextbox)(w))

typedef struct TTextbox *PTextbox;
typedef struct TTextbox
{
	struct	TScrollWidget o;

	FONT*	Font;

	l_ulong Flags;

	l_text Text;

	l_text SelPos;
	l_text SelPosEnd;

	l_text DragPos;

	l_ulong ValidMsg;

	PTimer Tmr;

} TTextbox;

#define TBF_EDITABLE		0x0001
#define TBF_MULTILINE		0x0002
#define TBF_TIMERHIDECURSOR	0x0004
#define TBF_PASSWORD		0x0008
#define TBF_DRAGING			0x0010
#define TBF_PREDRAG			0x0020

PTextbox CreateTextbox(PApplication App, TRect r, l_ulong Flags);
void TextBoxSetTextEx ( PTextbox o, l_text Txt );
void TextBoxSetText ( PTextbox o, l_text Format, ... );

#endif /* _TEXTBOX_H_INCLUDED_ */
