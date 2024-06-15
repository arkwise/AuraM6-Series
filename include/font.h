/**
*	Font manipulation extension
*/

#ifndef _FONT_H_INCLUDED_
#define _FONT_H_INCLUDED_

_PUBLIC FONT*  FontLoad ( l_text szFilename );

/**
*	Global main system font
*/
extern FONT* default_font;

#ifdef NEWFONTSYS

#include <ft2build.h>  
#include FT_FREETYPE_H


typedef struct TFontFace *PFontFace;
typedef struct TFontSize *PFontSize;
typedef struct TFontGlyph *PFontGlyph;

typedef struct TFontFace {
	
	l_text Name;
	
	FT_Face face; 
	
	PList FontSizesCache;
	
} TFontFace;

typedef struct TFontGlyph {
	
	l_uchar Type;
	
	l_ushort Width;
	l_ushort Height;
	
	l_ushort DeltaX;
	
	void *Data;
	
} TFontGlyph;

typedef struct TFontSize {
	
	PFontFace Face;
	
	l_ushort PixelSize;
	l_ushort DeltaY;
	
	PList Glyphs;
	
} TFontSize;

extern PList FontFaces;
extern PFontFace DefaultFontFace;
extern PFontSize DefaultFontSize;



#endif


#endif /* _FONT_H_INCLUDED_ */
