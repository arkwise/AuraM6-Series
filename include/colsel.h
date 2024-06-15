////////////////////////////////////////////////////////////////////////////////
//
//  Color selector - Header File
//
//  (c) Copyright 2004 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include"widget.h"
#include "label.h"
#include "tbox.h"
#include "window.h"
#include "listview.h"

#ifndef _COLSEL_H_INCLUDED_
#define _COLSEL_H_INCLUDED_


typedef struct TColSelEl *PColSelEl;


typedef struct TColSelDat
{
	PColSelEl 	hue,sallgh,sr,sg,sb,pal;
	PTextbox 	tr,tg,tb,th,ts,tv,thtml,tfav;
	PWindow 	w;
	PWidget   	col;
	PLabel 		l;
	PListview 	lv;
	
	int 		r, g, b;
	float 		h,s,v;
	
} TColSelDat, *PColSelDat;


typedef struct TColSelEl
{
	struct		TWidget o;

	PColSelDat a;
	
} TColSelEl;

typedef struct TColFavItem *PColFavItem;
typedef struct TColFavItem
{
	struct	TListviewItem o;

	int r, g, b;

} TColFavItem;


#define COLSELEL(o)	((PColSelEl)(o))


l_bool DialogColor ( RGB *rgb, l_color *col );



#endif /* _COLSEL_H_INCLUDED_ */
