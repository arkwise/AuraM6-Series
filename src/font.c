/*
*       (c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
*       FILE:                   font.c
*
*       PROJECT:                Phoenix engine - Core
*
*       DESCRIPTION:    Exetended font manipulation
*
*       CONTRIBUTORS:
*                                       Lukas Lipka
*
*       TODO:                   @
*
*       BUG:                    @
*
*       MISTAKE:                @
*
*       IDEA:                   @
*/

#include "kernel.h"
//#include "alfont.h"
#include "allegttf.h"

/** @defgroup font Font loader for allegro based text output
        @{
*/
/**     @file
        Font core file
*/

/**
        Global font variable, which is used as the default system font.
*/
FONT* default_font = 0;

/**
        Loads a font
        @param szFilename Path to the font to load
        @return The loaded font, or default font on failure.
*/
FONT*  FontLoad ( l_text szFilename )
{
        l_text ext = TextRChr(szFilename, '.');
        PALETTE palette;

        if (!ext) return default_font;

        if (!fopen(szFilename, "rb")) return default_font;

        if (!stricmp(ext, ".FNT"))
	  return load_font(szFilename, palette, NULL);
        else if (!stricmp(ext, ".TTF"))
	  return load_ttf_font (szFilename, 9, ALLEGTTF_REALSMOOTH); // FIXME: looks nice but slow (FX)
        else
	  return default_font;
}

#ifdef NEWFONTSYS

/*

        New font system

        Work In Progress, !! not working !!

*/


FT_Library FreeTypeLibrary;

PList FontFaces;
PFontFace DefaultFontFace;
PFontSize DefaultFontSize;

PFontFace LoadFontFace ( l_text File ) {
        l_int Error;
        PFontFace o = malloc(sizeof(TFontFace));

        if ( !o ) return NULL;

        Error = FT_New_Face( FreeTypeLibrary, File, 0, &o->face );

        if ( Error ) {

                free(o);
                return NULL;

        }


        return o;
}


void InitFontSystem ( void ) {

        l_int Error;

        FontFaces = NewList();

        Error = FT_Init_FreeType( &FreeTypeLibrary );

        if ( Error ) return;

}

void UnInitFontSystem ( void ) {

        if ( FontFaces ) FreeList(FontFaces);


}

#endif

/**
@}
*/
