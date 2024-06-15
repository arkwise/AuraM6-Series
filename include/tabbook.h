////////////////////////////////////////////////////////////////////////////////
//
//  Tabbook - Header File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _TABBOOK_H_INCLUDED_
#define _TABBOOK_H_INCLUDED_

#include "widget.h"


typedef struct TTab *PTab;
typedef struct TTab
{
	struct TWidget o;

	l_text		Title;
	p_bitmap	Icon;

	TRect		BtCache;
} TTab;

typedef struct TTabBook *PTabBook;
typedef struct TTabBook
{
	struct TWidget o;

	PTab	Current;

	l_ulong	Max;
	l_ulong	Pos;

	l_ulong	Flags;
} TTabBook;


#define TAB(o) ((PTab)(o))
#define TABBOOK(o) ((PTabBook)(o))

#define TBF_LDWN 0x01
#define TBF_RDWN 0x02

PTabBook CreateTabBook ( PApplication App, TRect r );
PTab TabBookAddTab ( PTabBook o, l_text Title, p_bitmap Icon );
void TabBookDisposeTab ( PTabBook o, PTab t );
void TabBookRemoveTab ( PTabBook o, PTab t );
void TabBookSelectTab ( PTabBook o, PTab t );

#endif
