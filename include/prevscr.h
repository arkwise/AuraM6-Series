////////////////////////////////////////////////////////////////////////////////
//
//  Screen Preview - Header File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _PREVSCR_H_INCLUDED_
#define _PREVSCR_H_INCLUDED_

typedef struct TPrevScr {
	
	struct		TWidget o;

	p_bitmap  Scr;	
	
} TPrevScr, *PPrevScr;

#define PREVSCR(o) ((PPrevScr)(o))

void  PrevScrSetScr ( PPrevScr o, p_bitmap Scr );
PPrevScr  CreatePrevScr ( PApplication App, TRect r, p_bitmap Scr );


#endif /* _PREVSCR_H_INCLUDED_ */
