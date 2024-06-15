////////////////////////////////////////////////////////////////////////////////
//
//  Label Widget - Header File
//
//  (c) Copyright 2003 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _LABEL_H_INCLUDED_
#define _LABEL_H_INCLUDED_

#include "widget.h"

typedef struct TLabel *PLabel;
typedef struct TLabel
{
	struct		TWidget o;

	l_text		Caption;
	l_color		FontColor;

} TLabel;

#define LABEL(w) ((PLabel)(w))


typedef struct TImageLabel *PImageLabel;
typedef struct TImageLabel
{
	struct		TWidget o;

	BITMAP		*Ico;
	l_ulong		Flags;

} TImageLabel;

#define IMAGELABEL(w)	((PImageLabel)(w))

#define ILF_FRAME	0x00000001

PLabel CreateLabel( PApplication App, TRect r, l_text Caption );
void LabelSetText ( PLabel l, l_text Format, ... );
void LabelSetTextEx ( PLabel l, l_text Caption );

PImageLabel CreateImageLabel( PApplication App, TRect r, BITMAP *Ico );

#endif /* _LABEL_H_INCLUDED_ */
