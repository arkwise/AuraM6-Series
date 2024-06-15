////////////////////////////////////////////////////////////////////////////////
//
//  Console - Header File
//
//  Copyright (c) Aura GUI m6 Terminal.
//
////////////////////////////////////////////////////////////////////////////////

#include"window.h"

#ifndef _CONSOLE_H_INCLUDED_
#define _CONSOLE_H_INCLUDED_

//#ifndef _text_h_included_
//#define _text_h_included_

//typedef struct TConsole *PConsole; // Defined in dynld.h

typedef struct TConWdg
{
	struct TWidget w;
	
	PConsole o;
} TConWdg, *PConWdg;

#define CONWDG(o) ((PConWdg)(o))

typedef struct TConsole
{
	l_uchar Color;
	l_long   flags;
	l_long   cursor;
//	l_text TextDup			( l_text t );
	PWindow w;
	PConWdg box;
	PTimer t;
	PTimer t1;
	char VideoMem[24][80];
	char VideoMem1[24][80];
	char *cur;
	char *cur1;
	
	int KeyBuffer[256];
	l_long KeyNb;
	
	PApplication App; // An application can only open one console.
	
} TConsole;

#define CONSOLE(o) ((PConsole)(o))

int GetKey ( PApplication App );
int Printf(PApplication App, l_text Format, ... );


/*
l_text txt =  Prompt ( &Me ) ;

It will prompt user for a string allowing right and left moves, and use of del and backspace keys.
When user press [enter] it will return string (no '\n' at end).

*/
l_text Prompt ( PApplication App );

#endif
