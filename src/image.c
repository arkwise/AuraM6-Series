/**
*	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
*	FILE:			image.c
*
*	PROJECT:		Phoenix engine - Core
*
*	DESCRIPTION:	Functions for loading images
*
*	CONTRIBUTORS:
*					Lukas Lipka
*
*	TODO:			@
*
*	BUG:			@
*
*	MISTAKE:		@
*
*	IDEA:			@
*/
#define DMS2
#include "kernel.h"

/* TODO: */
p_bitmap ImageInvert ( p_bitmap bmp );
p_bitmap ImageFlip ( p_bitmap bmp, l_char angle );
p_bitmap ImageResize ( p_bitmap bmp, l_int w, l_int h );

/**
*	NAME: LoadImage
*	DESCRIPTION: Loads a bitmap from the specified path
*	RETURN: Pointer to the bitmap
*/
p_bitmap LoadImage ( l_text szFileName )
{
	/* IF DMS DOESNT WORK :)
	PALLETE Pal;
	return load_bitmap(szFileName, (RGB*)&Pal);
	*/

	return LoadData2(szFileName,TYPE_IMAGE);
}

/**
*	NAME: DestroyImage
*	DESCRIPTION: Loads a bitmap from the specified path
*/
void DestroyImage ( p_bitmap bmp )
{
	if ( bmp )
		destroy_bitmap(bmp);
}
